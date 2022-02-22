#include "PCH.h"
#include "Visitor.h"

#include "LLVMIncludes.h"
#include "Generator.h"
#include "Utils.h"

namespace Xin
{
	static bool IsReflectedCXXRDDecl(clang::CXXRecordDecl * CXXRD)
	{
		for (clang::CXXMethodDecl * CXXMD : CXXRD->methods())
		{
			if (!CXXMD->isStatic())
				continue;

			// maybe operator ...
			if (CXXMD->getIdentifier() == nullptr)
				continue;

			if (CXXMD->getName() != "StaticType")
				continue;

			// check for static FType & StaticType();
			clang::QualType QType = CXXMD->getReturnType();
			if (QType->getTypeClass() == clang::Type::TypeClass::LValueReference)
			{
				const clang::LValueReferenceType * LVRefType = QType->getAs<clang::LValueReferenceType>();
				clang::QualType QPointeeType = LVRefType->getPointeeType();
				if (QPointeeType->getTypeClass() == clang::Type::TypeClass::Elaborated)
				{
					const clang::ElaboratedType * ELBType = QPointeeType->getAs<clang::ElaboratedType>();
					clang::QualType NamedType = ELBType->getNamedType();
					clang::CXXRecordDecl * TypeCXXRD = NamedType->getAsCXXRecordDecl();
					if (TypeCXXRD->getName() == "FType")
						return true;
				}
			}
		}
		return false;
	}

	static const clang::NamespaceDecl * GetRootNamespace(const clang::Decl & RD)
	{
		const clang::NamespaceDecl * ND = nullptr;
		const clang::DeclContext * DC = RD.getDeclContext();
		while (DC && DC->isNamespace())
		{
			ND = dyn_cast<clang::NamespaceDecl>(DC);
			DC = DC->getParent();
		}
		return ND;
	}

	class XinFileManager : public clang::FileManager
	{
	public:
		XinFileManager() : clang::FileManager(clang::FileSystemOptions(), llvm::vfs::getRealFileSystem())
		{
		}

		llvm::ErrorOr<const clang::FileEntry *>
			getFile(llvm::StringRef Filename, bool OpenFile = false, bool CacheFailure = true)
		{
			return nullptr;
		}
	};

	class XinFilterFileSystem : public llvm::vfs::FileSystem
	{
	public:
		XinFilterFileSystem() {}

		llvm::ErrorOr<llvm::vfs::Status> status(const llvm::Twine & Path) override
		{
			llvm::StringRef PathRef = Path.getSingleStringRef();
			if (PathRef.ends_with(".meta.inl"))
				return std::errc::no_such_file_or_directory;
			return RealFileSystem->status(Path);
		}

		llvm::ErrorOr<std::unique_ptr<llvm::vfs::File>> openFileForRead(const llvm::Twine & Path) override
		{
			llvm::StringRef PathRef = Path.getSingleStringRef();
			if (PathRef.ends_with(".meta.inl"))
				return std::errc::no_such_file_or_directory;
			return RealFileSystem->openFileForRead(Path);
			
		}
		llvm::vfs::directory_iterator dir_begin(const llvm::Twine & Dir, std::error_code & EC) override
		{
			return RealFileSystem->dir_begin(Dir, EC);
		}
		std::error_code setCurrentWorkingDirectory(const llvm::Twine & Path) override
		{
			return RealFileSystem->setCurrentWorkingDirectory(Path);
			
		}
		llvm::ErrorOr<std::string> getCurrentWorkingDirectory() const override
		{
			return RealFileSystem->getCurrentWorkingDirectory();
		}

	private:
		llvm::IntrusiveRefCntPtr<FileSystem> RealFileSystem = llvm::vfs::getRealFileSystem();
	};

	static void InitializeCompiler(clang::CompilerInstance & Compiler, llvm::ArrayRef<const char *> Arguments)
	{
		// 0 -- FileManager
		//llvm::IntrusiveRefCntPtr OverlayFileSystem(new llvm::vfs::OverlayFileSystem(llvm::vfs::getRealFileSystem()));
		//llvm::IntrusiveRefCntPtr InMemoryFileSystem(new llvm::vfs::InMemoryFileSystem);
		llvm::IntrusiveRefCntPtr XinFilterFileSystem(new Xin::XinFilterFileSystem());

		//OverlayFileSystem->pushOverlay(InMemoryFileSystem);

		llvm::IntrusiveRefCntPtr<clang::FileManager> FileMgr(new clang::FileManager(clang::FileSystemOptions(), XinFilterFileSystem));
		Compiler.setFileManager(FileMgr.get());

		// 1 -- Diagnostics
		llvm::IntrusiveRefCntPtr<clang::DiagnosticOptions> ParsedDiagOpts = clang::CreateAndPopulateDiagOpts(Arguments);
		llvm::IntrusiveRefCntPtr<clang::DiagnosticsEngine> Diagnostics = clang::CompilerInstance::createDiagnostics(Compiler.getVirtualFileSystem(), ParsedDiagOpts.get(), nullptr);
		Compiler.setDiagnostics(Diagnostics.get());

		// 2 -- SourceManager
		llvm::IntrusiveRefCntPtr<clang::SourceManager> SourceMgr(new clang::SourceManager(*Diagnostics, Compiler.getFileManager()));
		Compiler.setSourceManager(SourceMgr.get());

		// 3 -- Miscs
		Compiler.getFrontendOpts().DisableFree = false;
	}

	clang::TypeAliasDecl * IActionSink::FindTypeAlias(clang::CXXRecordDecl * CXXRD)
	{
		for (clang::TypeAliasDecl * TAD  : TADs)
		{
			clang::QualType TADQType = TAD->getUnderlyingType();
			if (TADQType->getTypeClass() != clang::Type::Elaborated)
				continue;

			const clang::ElaboratedType * ELBType = TADQType->getAs<clang::ElaboratedType>();
			clang::QualType NamedQType = ELBType->getNamedType();
			if (NamedQType->getTypeClass() != clang::Type::TemplateSpecialization)
				continue;

			clang::CXXRecordDecl * TADCXXRD = NamedQType->getAsCXXRecordDecl();
			if (TADCXXRD == CXXRD)
				return TAD;
		}
		return nullptr;
	}

	class FXinASTVisitor : public clang::RecursiveASTVisitor<FXinASTVisitor>
	{
	public:
		explicit FXinASTVisitor(clang::CompilerInstance & Compiler, clang::ASTContext & Context, IActionSink & ParserSink)
			: Compiler { Compiler }
			, Context { Context }
			, ParserSink { ParserSink }
		{ }

		TList<clang::Decl *> Decls;
		bool VisitDecl(clang::Decl * D)
		{
			clang::SourceManager & SM = Context.getSourceManager();
			clang::FileID FileID = SM.getFileID(D->getLocation());
			if (FileID != SM.getMainFileID())
				return true;

			Decls.Add(D);
			return true;
		}

		bool VisitNamespaceDecl(clang::NamespaceDecl * ND)
		{
			if (ND->getName() == "Xin")
			{
				if (ND->getDeclContext()->isTranslationUnit())
					ParserSink.XinNamespace = ND->getFirstDecl();
			}
			return true;
		}

		bool VisitTypeAliasDecl(clang::TypeAliasDecl * TAD)
		{
			clang::SourceManager & SM = Context.getSourceManager();
			clang::FileID FileID = SM.getFileID(TAD->getLocation());
			if (FileID != SM.getMainFileID())
				return true;

			if (not TAD->isFirstDecl())
				return true;

			//if (const clang::NamespaceDecl * ND = GetRootNamespace(*TAD); ND && ND->getName() == "Xin")
			{
				if (TAD->getPreviousDecl() == nullptr)
				{
					clang::QualType TADQType = TAD->getUnderlyingType();
					if (TADQType->getTypeClass() != clang::Type::Elaborated)
						return true;

					const clang::ElaboratedType * ELBType = TADQType->getAs<clang::ElaboratedType>();
					clang::QualType NamedQType = ELBType->getNamedType();
					if (NamedQType->getTypeClass() != clang::Type::TemplateSpecialization)
						return true;

					ParserSink.TADs.AddUnique(TAD);
				}
			}

			return true;
		}

		TList<clang::TypeAliasTemplateDecl *> TATDs;
		bool VisitTypeAliasTemplateDecl(clang::TypeAliasTemplateDecl * TATD)
		{
			if (const clang::NamespaceDecl * ND = GetRootNamespace(*TATD); ND && ND->getName() == "Xin")
				TATDs.AddUnique(TATD);
			return true;
		}

		bool VisitCXXRecordDecl(clang::CXXRecordDecl * CXXRD)
		{
			clang::SourceManager & SM = Context.getSourceManager();
			clang::FileID FileID = SM.getFileID(CXXRD->getLocation());
			if (FileID != SM.getMainFileID())
				return true;

			if (CXXRD->getTemplateSpecializationKind() == clang::TSK_Undeclared)
			{
				if (IsReflectedCXXRDDecl(CXXRD))
				{
					ParserSink.CXXRecordDecls.AddUnique(CXXRD);
					ParserSink.StaticTypeCXXRDs.AddUnique(CXXRD);
				}
				else if (Utils::FindXinAttr(CXXRD) != nullptr)
				{
					ParserSink.CXXRecordDecls.AddUnique(CXXRD);
				}
				else { }
			}
			return true;
		}

		bool VisitVarDecl(clang::VarDecl * VD)
		{
			return true;
		}

		bool VisitFunctionTemplateDecl(clang::FunctionTemplateDecl * FTD)
		{
			clang::Decl::Kind FTDKind = FTD->getKind();
			if (FTDKind != clang::Decl::Kind::FunctionTemplate)
				return true;

			//clang::SourceManager & SM = Context.getSourceManager();
			//clang::FileID FileID = SM.getFileID(FTD->getLocation());
			//if (FileID != SM.getMainFileID())
			//	return true;

			clang::FunctionDecl * FD = FTD->getTemplatedDecl();
			clang::Decl::Kind FDKind = FD->getKind();
			if (FDKind != clang::Decl::Kind::Function)
				return true;

			if (not FD->isGlobal())
				return true;

			if (FD->isCXXClassMember())
				return true;

			if (not FD->isFunctionOrFunctionTemplate())
				return true;

			if (FD->isOverloadedOperator())
				return true;

			if (FD->getName() != "Typeof")
				return true;

			clang::DeclContext * DC = FD->getDeclContext();
			if (not DC->isNamespace())
				return true;

			clang::NamespaceDecl * ND = dyn_cast<clang::NamespaceDecl>(DC);
			if (ND->getName() == "Xin" && FTD->isFirstDecl())
			{
				clang::SourceManager & SM = Context.getSourceManager();
				clang::SourceLocation Loc = FD->getLocation();
				auto [FileID, FileOffset] = SM.getDecomposedLoc(Loc);
				llvm::StringRef Filename = SM.getFilename(Loc);
				unsigned Line = SM.getLineNumber(FileID, FileOffset);
				LogInfo(u8"Typeof<T> found: \"{0}\":{1}"V, Filename, Line);
				ParserSink.TypeofFTD = FTD;
			}
			return true;
		}

		bool VisitFunctionDecl(clang::FunctionDecl * FD)
		{
			return true;
		}

		bool VisitClassTemplateSpecializationDecl(clang::ClassTemplateSpecializationDecl * CTSD)
		{
			clang::SourceManager & SM = Context.getSourceManager();
			clang::FileID FileID = SM.getFileID(CTSD->getLocation());
			if (FileID != SM.getMainFileID())
				return true;

			return true;
		}

		bool VisitRecordDecl(clang::RecordDecl * Declaration)
		{
			return true;
		}

		bool VisitDLLExportAttr(clang::DLLExportAttr * DEA)
		{
			return true;
		}

		bool VisitTypeAliasDecl(const clang::TypeAliasDecl * Declaration)
		{
			FStringV Name = Declaration->getName();
			return true;
		}

		bool VisitTypedefDecl(clang::TypedefDecl * Declaration)
		{
			FStringV Name = Declaration->getName();
			return true;
		}

		bool VisitEnumDecl(clang::EnumDecl * EnumDecl)
		{
			clang::SourceManager & SM = Context.getSourceManager();
			clang::FileID FileID = SM.getFileID(EnumDecl->getLocation());
			if (FileID != SM.getMainFileID())
				return true;

			if (Utils::FindXinAttr(EnumDecl))
				ParserSink.EnumDecls.AddUnique(EnumDecl);
			return true;
		}

		TList<clang::VarTemplateDecl *> VTDs;
		bool VisitVarTemplateDecl(clang::VarTemplateDecl * VTD)
		{
			clang::SourceManager & SM = Context.getSourceManager();
			clang::FileID FileID = SM.getFileID(VTD->getLocation());
			if (FileID != SM.getMainFileID())
				return true;
			return true;
		}

		bool VisitClassTemplateDecl(clang::ClassTemplateDecl * CTD)
		{
			return true;
		}

		void Finish()
		{
			if (ParserSink.TypeofFTD)
			{
				clang::SourceManager & SM = Context.getSourceManager();
				for (clang::FunctionDecl * SpecialFD : ParserSink.TypeofFTD->specializations())
				{
					clang::FunctionTemplateSpecializationInfo * TemplateSpecInfo = SpecialFD->getTemplateSpecializationInfo();
					const clang::TemplateArgument & TA = TemplateSpecInfo->TemplateArguments->get(0);
					clang::TemplateArgument::ArgKind TAKind = TA.getKind();
					if (TAKind == clang::TemplateArgument::Type)
					{
						clang::QualType TArgQType = TA.getAsType();
						clang::Type::TypeClass TArgTC = TArgQType->getTypeClass();
						if (TArgTC == clang::Type::Builtin)
						{
							clang::SourceLocation Loc = SpecialFD->getLocation();
							auto [FileID, FileOffset] = SM.getDecomposedLoc(Loc);
							llvm::StringRef Filename = SM.getFilename(Loc);
							unsigned Line = SM.getLineNumber(FileID, FileOffset);
							if (FileID != SM.getMainFileID())
								continue;

							const clang::BuiltinType * BType = TArgQType->getAs<clang::BuiltinType>();
							ParserSink.BuiltinTypes.Add(BType);
							ParserSink.TypeofSpecialFDs.Add(SpecialFD);
						}
						else if (TArgTC == clang::Type::Enum)
						{
							clang::EnumDecl * EnumDecl = static_cast<clang::EnumDecl *>(TArgQType->getAsTagDecl());
							clang::SourceLocation Loc = EnumDecl->getLocation();
							auto [FileID, FileOffset] = SM.getDecomposedLoc(Loc);
							llvm::StringRef Filename = SM.getFilename(Loc);
							unsigned Line = SM.getLineNumber(FileID, FileOffset);
							if (FileID != SM.getMainFileID())
								continue;

							ParserSink.EnumDecls.AddUnique(EnumDecl);
							ParserSink.TypeofSpecialFDs.Add(SpecialFD);
						}
						else if (TArgTC == clang::Type::Record)
						{
							clang::CXXRecordDecl * CXXRD = TArgQType->getAsCXXRecordDecl();
							if (CXXRD)
							{
								clang::Decl::Kind CXXRDKind = CXXRD->getKind();
								if (CXXRDKind == clang::Decl::CXXRecord)
								{
									clang::SourceLocation Loc = CXXRD->getLocation();
									auto [FileID, FileOffset] = SM.getDecomposedLoc(Loc);
									llvm::StringRef Filename = SM.getFilename(Loc);
									unsigned Line = SM.getLineNumber(FileID, FileOffset);
									if (FileID == SM.getMainFileID())
									{
										ParserSink.CXXRecordDecls.AddUnique(CXXRD);
										ParserSink.TypeofSpecialFDs.Add(SpecialFD);
										ParserSink.TypeofCXXRDs.AddUnique(CXXRD);
									}
								}
								else if (CXXRDKind == clang::Decl::ClassTemplateSpecialization)
								{
									{
										clang::ClassTemplateSpecializationDecl * CTSD = dyn_cast<clang::ClassTemplateSpecializationDecl>(CXXRD);
										clang::ClassTemplateDecl * CTD = CTSD->getSpecializedTemplate();
										clang::CXXRecordDecl * TempateCXXRD = CTD->getTemplatedDecl();
										clang::SourceLocation Loc = TempateCXXRD->getLocation();
										auto [FileID, FileOffset] = SM.getDecomposedLoc(Loc);
										llvm::StringRef Filename = SM.getFilename(Loc);
										unsigned Line = SM.getLineNumber(FileID, FileOffset);
										if (FileID == SM.getMainFileID())
											ParserSink.CXXRecordDecls.AddUnique(TempateCXXRD);
									}
									{
										clang::SourceLocation Loc = CXXRD->getLocation();
										auto [FileID, FileOffset] = SM.getDecomposedLoc(Loc);
										llvm::StringRef Filename = SM.getFilename(Loc);
										unsigned Line = SM.getLineNumber(FileID, FileOffset);
										if (FileID == SM.getMainFileID())
										{
											ParserSink.TypeofCXXRDs.AddUnique(CXXRD);
											ParserSink.TypeofSpecialFDs.Add(SpecialFD);
										}
									}
								}
								else { }
							}
						}
						else {}
					}
				}
			}
			else
			{
				LogInfo(u8"Can't find defination of template funtion Typeof<T>."V);
			}
			ParserSink.OnParsed();
		}

	private:
		clang::ASTContext & Context;
		clang::CompilerInstance & Compiler;
		IActionSink & ParserSink;
	};

	class FXinPPCallback : public clang::PPCallbacks
	{
	public:
		FXinPPCallback(clang::ASTContext & Context, IActionSink & ParserSink) : Context(Context), ParserSink(ParserSink) {}

		clang::ASTContext & Context;
		IActionSink & ParserSink;

		std::vector<clang::SourceRange> Ranges;
		std::vector<FString> Strings;

		bool FileNotFound(llvm::StringRef FileName) override
		{
			if (FileName.ends_with(".meta.inl"))
				return true;
			return false;
		}

		void HasInclude(clang::SourceLocation Loc, llvm::StringRef FileName, bool IsAngled, clang::OptionalFileEntryRef File, clang::SrcMgr::CharacteristicKind FileType) override
		{
			return clang::PPCallbacks::HasInclude(Loc, FileName, IsAngled, File, FileType);
		}

		void InclusionDirective(clang::SourceLocation HashLoc,
			const clang::Token &IncludeTok, llvm::StringRef FileName,
			bool IsAngled, clang::CharSourceRange FilenameRange,
			clang::OptionalFileEntryRef File,
			llvm::StringRef SearchPath, llvm::StringRef RelativePath,
			const clang::Module *SuggestedModule,
			bool ModuleImported,
			clang::SrcMgr::CharacteristicKind FileType) override
		{
			return clang::PPCallbacks::InclusionDirective(HashLoc, IncludeTok, FileName, IsAngled, FilenameRange, File, SearchPath, RelativePath, SuggestedModule, ModuleImported, FileType);
		}

		void MacroExpands(const clang::Token & MacroNameTok, const clang::MacroDefinition &, const clang::SourceRange Range, const clang::MacroArgs * const Args) override
		{
			auto & SM = Context.getSourceManager();
			clang::FileID FID = SM.getFileID(Range.getBegin());
			clang::FileID MainFileID = SM.getMainFileID();
			if (FID != MainFileID)
				return;

			auto & LangOpts = Context.getLangOpts();
			FStringV MacroName = MacroNameTok.getIdentifierInfo()->getName();
			if (MacroName == u8"XinType"V || MacroName == u8"XinAttributes"V)
			{
#if 0
				FReflectionMacro & ReflectionMacro = ParserSink.ReflectionMacros.AddZeroed();
				ReflectionMacro.FID = MainFileID;
				ReflectionMacro.LineNumber = SM.getSpellingLineNumber(Range.getBegin());
				ReflectionMacro.SourceRange = Range;
				ReflectionMacro.FilePath = SM.getFilename(Range.getBegin());
				ReflectionMacro.SourceCode = clang::Lexer::getSourceText(clang::CharSourceRange::getCharRange(Range), SM, LangOpts);

				const clang::Token * T = Args->getUnexpArgument(0u);
				while (T->isNot(clang::tok::eof))
				{
					auto TokenLocation = T->getLocation();
					clang::tok::TokenKind AK = T->getKind();
					if (AK == clang::tok::identifier)
					{
						auto II = T->getIdentifierInfo();
						ReflectionMacro.Attributes.Add({ FStringV(II->getName()) });
					}
					else
					{
						switch (AK)
						{
						case clang::tok::kw_int:
						case clang::tok::kw_long:
						case clang::tok::kw_signed:
						case clang::tok::kw_unsigned:
						case clang::tok::kw_float:
						case clang::tok::kw_double:
						case clang::tok::kw_short:
						case clang::tok::kw_bool:
						case clang::tok::numeric_constant:
						case clang::tok::char_constant:
						case clang::tok::wide_char_constant:
						case clang::tok::utf8_char_constant:
						case clang::tok::utf16_char_constant:
						case clang::tok::utf32_char_constant:
						case clang::tok::string_literal:
						{
							FReflectionMacroAttribute & ReflectionMacroAttribute = ReflectionMacro.Attributes.Back();
							clang::CharSourceRange CharRange = clang::CharSourceRange::getCharRange(TokenLocation, T->getEndLoc());
							FStringV String = clang::Lexer::getSourceText(CharRange, SM, LangOpts);
							ReflectionMacroAttribute.Args.push_back(String);
							ReflectionMacroAttribute.ArgKinds.push_back(AK);
							break;
						}
						default:
							break;
						}
					}
					++T;
				}
				return;
				//// Save Range into a queue.
				//Ranges.push_back(Range);
#endif
			}
			else
			{
				return;
			}
		}
	};

	class FXinASTConsumer : public clang::ASTConsumer
	{
	public:
		explicit FXinASTConsumer(clang::CompilerInstance & Compiler, clang::ASTContext & Context, IActionSink & ParserSink)
			: Compiler(Compiler), Context(Context), ParserSink(ParserSink)
		{
			clang::Preprocessor & PP = Compiler.getPreprocessor();
			PP.addPPCallbacks(std::make_unique<FXinPPCallback>(Context, ParserSink));
		}

		void HandleTranslationUnit(clang::ASTContext & Context) override
		{
			clang::TranslationUnitDecl * TUD = Context.getTranslationUnitDecl();
			FXinASTVisitor Visitor { Compiler, Context, ParserSink };
			Visitor.TraverseDecl(TUD);
			Visitor.Finish();
		}

	private:
		clang::CompilerInstance & Compiler;
		clang::ASTContext & Context;
		IActionSink & ParserSink;
	};

	class FXinPCHFrontendAction : public clang::GeneratePCHAction
	{
	public:
		FXinPCHFrontendAction() = default;

		clang::TranslationUnitKind getTranslationUnitKind() override { return clang::TU_Complete; }

		bool hasASTFileSupport() const override { return false; }

		bool shouldEraseOutputFiles() override
		{
			if (getCompilerInstance().getPreprocessorOpts().AllowPCHWithCompilerErrors)
				return false;
			return ASTFrontendAction::shouldEraseOutputFiles();
		}
	};

	class FXinASTFrontendAction : public clang::ASTFrontendAction
	{
	public:
		FXinASTFrontendAction(IActionSink & ParserSink) : ParserSink(ParserSink) {}

		clang::TranslationUnitKind getTranslationUnitKind() override { return clang::TU_Complete; }

		std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance & CI, llvm::StringRef InFile) override
		{
			CI.getFrontendOpts().SkipFunctionBodies = true;
			return std::unique_ptr<clang::ASTConsumer> { new FXinASTConsumer { CI, CI.getASTContext(), ParserSink } };
		}

	public:
		IActionSink & ParserSink;
	};

	const llvm::opt::ArgStringList & getCC1Arguments(clang::DiagnosticsEngine & Diagnostics, clang::driver::Compilation * Compilation)
	{
		const clang::driver::JobList & Jobs = Compilation->getJobs();

		auto IsCC1Command = [](const clang::driver::Command & Cmd)
		{
			return llvm::StringRef(Cmd.getCreator().getName()) == "clang";
		};

		auto IsSrcFile = [](const clang::driver::InputInfo & II)
		{
			return isSrcFile(II.getType());
		};

		llvm::SmallVector<const clang::driver::Command *, 1> CC1Jobs;
		for (const clang::driver::Command & Job : Jobs)
		{
			if (IsCC1Command(Job) && llvm::all_of(Job.getInputInfos(), IsSrcFile))
			{
				return Job.getArguments();
			}
		}

		//if (CC1Jobs.empty() ||
		//	(CC1Jobs.size() > 1 && !ignoreExtraCC1Commands(Compilation))) {
		//	SmallString<256> error_msg;
		//	llvm::raw_svector_ostream error_stream(error_msg);
		//	Jobs.Print(error_stream, "; ", true);
		//	Diagnostics->Report(diag::err_fe_expected_compiler_job)
		//		<< error_stream.str();
		//	return nullptr;
		//}

		static llvm::opt::ArgStringList EmptyArgs;
		return EmptyArgs;
	}

	static void GenerateDefaultArguments(std::vector<std::string> & ParseArgs)
	{
		std::vector<std::string> CommonParseArgs =
		{
			"xin-reflector", // custom tool name, only for raw frontend action
			"-DXIN_REFLECT=1",
			"-D_UNICODE=1",
			"-D_WIN32",
			"-DXIN_WINDOWS=1",
			"-std=c++23",
			//"-fsyntax-only",
			"-fms-compatibility",
			"-fms-extensions",
			//"-fexceptions",
			//"-fsigned-char",
			//"-fdelayed-template-parsing",
			//"-fshow-source-location",
			// Open warnings
			"-Wall",
			"-Wextra",
			"-Wpadded",
			// Ignore specific.
			"-Wdefaulted-function-deleted",
			"-Wno-class-conversion",
			"-Wno-user-defined-literals",
			//"-Wno-reserved-user-defined-literal",
			//"-Wno-nonportable-include-path",
			//"-Wno-macro-redefined",
			"-Wno-defaulted-function-deleted",

			"-Wno-unused-function",
			"-Wno-unused-const-variable",
			"-Wno-null-dereference",
			"-Wno-unused-private-field",
			"-Wno-unused-local-typedef",
			"-Wno-undefined-bool-conversion",
		};

		ParseArgs.append_range(CommonParseArgs);

		FString EngineRoot { u8R"(H:\Projs\XinSamples\Xin\)"V };
		FString EngineSourcesDir = FPath::Combine(EngineRoot, u8"Sources"V);

		ParseArgs.push_back("-I" + StdString(EngineSourcesDir));
		ParseArgs.push_back("-I" + StdString(FPath::Combine(EngineRoot, u8"Sources"V, u8"Externals"V)));
		ParseArgs.push_back("-I" + StdString(FPath::Combine(EngineRoot, u8"Sources"V, u8"Externals"V, u8"cpuid-for-metaparser\\include"V)));
		ParseArgs.push_back("-I" + StdString(FPath::Combine(EngineRoot, u8"Sources"V, u8"Depends"V)));
		ParseArgs.push_back("-I" + StdString(FPath::Combine(EngineRoot, u8"Sources"V, u8"Runtimes"V)));
		ParseArgs.push_back("-I" + StdString(FPath::Combine(EngineRoot, u8"Sources"V, u8"Plugins"V)));
		ParseArgs.push_back("-I" + StdString(FPath::Combine(EngineRoot, u8"Sources"V, u8"Runtimes"V, u8"Xin.Core"V)));
		ParseArgs.push_back("-I" + StdString(FPath::Combine(EngineRoot, u8"Sources"V, u8"Runtimes"V, u8"Xin.UI"V)));
		ParseArgs.push_back("-I" + StdString(FPath::Combine(EngineRoot, u8"Sources"V, u8"Plugins"V, u8"Xin.Direct2D"V)));
	}

	bool RunPCHAction(FStringV FilePath, FStringV PCHFilePath, FReflectSettings & ReflectSettings)
	{
		std::vector<std::string> CompileArguments;
		GenerateDefaultArguments(CompileArguments);
		CompileArguments.push_back("-I" + StdString(ReflectSettings.ModuleRoot));

		CompileArguments.push_back("-xc++-header");
		CompileArguments.push_back("-o" + StdString(PCHFilePath));
		CompileArguments.push_back(StdString(FilePath));

		llvm::opt::ArgStringList CommandLineArguments(CompileArguments.size(), nullptr);
		llvm::transform(CompileArguments, CommandLineArguments.begin(), [](const std::string & Arg) { return Arg.c_str(); });

		if (FFile::Exists(PCHFilePath))
		{
			clang::CompilerInstance Compiler { };
			InitializeCompiler(Compiler, CommandLineArguments);

			llvm::IntrusiveRefCntPtr<clang::DiagnosticIDs> DiagIDs(Compiler.getDiagnostics().getDiagnosticIDs());
			llvm::IntrusiveRefCntPtr<clang::DiagnosticsEngine> DiagEngine(
				new clang::DiagnosticsEngine(DiagIDs, &Compiler.getDiagnosticOpts(), 
					new clang::ForwardingDiagnosticConsumer(*Compiler.getDiagnostics().getClient()), /*ShouldOwnClient=*/true));
			std::unique_ptr<clang::ASTUnit> LoadedAST = clang::ASTUnit::LoadFromASTFile(StdString(PCHFilePath), Compiler.getPCHContainerReader(), clang::ASTUnit::LoadEverything,
				DiagEngine.get(), Compiler.getFileSystemOpts(), Compiler.getHeaderSearchOptsPtr());
			if (LoadedAST != nullptr)
				return true;
		}

		clang::CompilerInstance Compiler { };
		InitializeCompiler(Compiler, CommandLineArguments);

		const std::unique_ptr<clang::driver::Driver> Driver(new clang::driver::Driver("xin-reflector", llvm::sys::getDefaultTargetTriple(),
			Compiler.getDiagnostics(), "Xin Reflector", &Compiler.getVirtualFileSystem()));

		const std::unique_ptr<clang::driver::Compilation> Compilation(Driver->BuildCompilation(CommandLineArguments));
		if (!Compilation)
			return false;

		const clang::driver::ToolChain & ToolTC = Compilation->getDefaultToolChain();
		FString EffectiveTripleName = FEncoding::AsUtf8(llvm::Triple(ToolTC.ComputeEffectiveClangTriple(Compilation->getArgs())).getTriple());
		FConsole::WriteLine(u8"Rebuilding pch: \"{}\" with \"{}\""V, FilePath, EffectiveTripleName);

		const llvm::opt::ArgStringList & CC1Args = getCC1Arguments(Compiler.getDiagnostics(), Compilation.get());
		if (CC1Args.empty())
			return false;

		std::shared_ptr<clang::CompilerInvocation> Invocation = std::make_shared<clang::CompilerInvocation>();
		clang::CompilerInvocation::CreateFromArgs(*Invocation, CC1Args, Compiler.getDiagnostics(), "xin-reflector");
		//Invocation->getDiagnosticOpts().ShowCarets = false;
		//Invocation->getFrontendOpts().DisableFree = false;
		Compiler.setInvocation(Invocation);

		FDirectory::EnsureExists(FPath::GetDirectory(PCHFilePath));
		FXinPCHFrontendAction XinPCHAction;
		Compiler.ExecuteAction(XinPCHAction);
		return not Compiler.getDiagnostics().hasErrorOccurred();
	}

	bool RunASTAction(FStringV FilePath, FStringV PCHFilePath, IActionSink & ActionSink)
	{
		std::vector<std::string> CompileArguments;
		GenerateDefaultArguments(CompileArguments);
		CompileArguments.push_back("-I" + StdString(ActionSink.ReflectSettings.ModuleRoot));

		CompileArguments.push_back("-xc++-header");
		//CompileArguments.push_back("-xc++");
		if (PCHFilePath.Size)
		{
			CompileArguments.push_back("-include-pch");
			CompileArguments.push_back(StdString(PCHFilePath));
		}
		CompileArguments.push_back(StdString(FilePath));

		llvm::opt::ArgStringList CommandLineArguments(CompileArguments.size(), nullptr);
		llvm::transform(CompileArguments, CommandLineArguments.begin(), [](const std::string & Arg) { return Arg.c_str(); });

		clang::CompilerInstance Compiler { };
		InitializeCompiler(Compiler, CommandLineArguments);

		const std::unique_ptr<clang::driver::Driver> Driver(new clang::driver::Driver("xin-reflector", llvm::sys::getDefaultTargetTriple(),
			Compiler.getDiagnostics(), "Xin Reflector", &Compiler.getVirtualFileSystem()));

		const std::unique_ptr<clang::driver::Compilation> Compilation(Driver->BuildCompilation(CommandLineArguments));
		if (!Compilation)
			return false;

		//const clang::driver::ToolChain & ToolTC = Compilation->getDefaultToolChain();
		//FString EffectiveTriple = Strings::AnsiAsUTF8(llvm::Triple(ToolTC.ComputeEffectiveClangTriple(Compilation->getArgs())).getTriple());

		//FConsole::WriteLineF(u8"\tEffectiveTriple=\"{}\""V, EffectiveTriple);

		const llvm::opt::ArgStringList & CC1Args = getCC1Arguments(Compiler.getDiagnostics(), Compilation.get());
		if (CC1Args.empty())
			return false;

		std::shared_ptr<clang::CompilerInvocation> Invocation = std::make_shared<clang::CompilerInvocation>();
		clang::CompilerInvocation::CreateFromArgs(*Invocation, CC1Args, Compiler.getDiagnostics(), "xin-reflector");
		Invocation->getDiagnosticOpts().ShowCarets = false;
		Invocation->getFrontendOpts().DisableFree = false;
		//Invocation->getFrontendOpts().Inputs.emplace_back(StdString(FilePath), clang::InputKind(clang::Language::CXX, clang::InputKind::Source));

		Compiler.setInvocation(Invocation);

		FXinASTFrontendAction XinASTAction { ActionSink };
		Compiler.ExecuteAction(XinASTAction);
		return not Compiler.getDiagnostics().hasErrorOccurred();
	}
}
