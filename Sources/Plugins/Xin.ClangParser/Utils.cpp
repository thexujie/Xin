#include "PCH.h"
#include "Utils.h"

namespace Xin::Utils
{
	clang::QualType GetSpecializationT0QType(clang::FunctionDecl * SpecialFD)
	{
		clang::FunctionTemplateSpecializationInfo * TemplateSpecInfo = SpecialFD->getTemplateSpecializationInfo();
		for (const clang::TemplateArgument & TA : TemplateSpecInfo->TemplateArguments->asArray())
		{
			clang::TemplateArgument::ArgKind TAKind = TA.getKind();
			if (TAKind == clang::TemplateArgument::Type)
			{
				clang::QualType Type = TA.getAsType();
				return Type;
			}
		}
		AssertFailed(u8"invalid SpecialFD.");
		return clang::QualType { };
	}

	FString GetNamespaceName(const clang::DeclContext * DC)
	{
		TList<const clang::NamespaceDecl *> Namespaces;
		while (DC && DC->isNamespace())
		{
			const clang::NamespaceDecl * ND = dyn_cast<clang::NamespaceDecl>(DC);
			Namespaces.Add(ND);
			DC = DC->getParent();
		}

		FString Result; 
		for (sizet Index = Namespaces.Size; Index > 0; --Index)
		{
			if (Result.Length)
				Result.Append(u8"::"V);
			Result.Append(Namespaces[Index - 1]->getName());
		}
		return Result;
	}

	FString GetRelativeDeclContext(const clang::DeclContext * DC, const clang::DeclContext * ReferenceDC)
	{
		TList<const clang::DeclContext *> DCs;
		while (DC)
		{
			if (DC->isNamespace())
				DCs.PushFront(dyn_cast<clang::NamespaceDecl>(DC)->getFirstDecl());
			else if (DC->isRecord())
				DCs.PushFront(dyn_cast<clang::RecordDecl>(DC)->getFirstDecl());
			else
				DCs.PushFront(DC);
			DC = DC->getParent();
		}

		TList<const clang::DeclContext *> ReferenceDCs;
		while (ReferenceDC)
		{
			if (ReferenceDC->isNamespace())
				ReferenceDCs.PushFront(dyn_cast<clang::NamespaceDecl>(ReferenceDC)->getFirstDecl());
			else if (ReferenceDC->isRecord())
				ReferenceDCs.PushFront(dyn_cast<clang::RecordDecl>(ReferenceDC)->getFirstDecl());
			else
				ReferenceDCs.PushFront(ReferenceDC);
			ReferenceDC = ReferenceDC->getParent();
		}

		uintx RelativeDeclContentIndex = 0;
		while (RelativeDeclContentIndex < Min(DCs.Size, ReferenceDCs.Size))
		{
			if (DCs[RelativeDeclContentIndex] != ReferenceDCs[RelativeDeclContentIndex])
				break;
			++RelativeDeclContentIndex;
		}

		if (RelativeDeclContentIndex >= DCs.Size)
			return FString::None;

		FString RelativeDeclContextName;
		bool NeedTypename = false;
		for (uintx DeclContextIndex = RelativeDeclContentIndex; DeclContextIndex < DCs.Size; ++DeclContextIndex)
		{
			if (DeclContextIndex > RelativeDeclContentIndex)
				RelativeDeclContextName.Append(u8"::"V);

			const clang::DeclContext * TempDC = DCs[DeclContextIndex];
			if (TempDC->isNamespace())
				RelativeDeclContextName.Append(dyn_cast<clang::NamespaceDecl>(TempDC)->getName());
			else if (TempDC->isRecord())
			{
				const clang::CXXRecordDecl * CXXRD = dyn_cast<clang::CXXRecordDecl>(TempDC);
				if (CXXRD->getDescribedClassTemplate() != nullptr)
					NeedTypename = true;
				RelativeDeclContextName.Append(CXXRD->getName());
			}
		}
		return NeedTypename ? FString(u8"typename "V + RelativeDeclContextName) : RelativeDeclContextName;
	}

	FString GetRelativeQualifiedPrefix(const clang::DeclContext * DC, const clang::DeclContext * ReferenceDC)
	{
		//TList<const clang::NamespaceDecl *> NDs;
		//while (DC && DC->isNamespace())
		//{
		//	const clang::NamespaceDecl * ND = dyn_cast<clang::NamespaceDecl>(DC);
		//	NDs.PushFront(ND->getFirstDecl());
		//	DC = DC->getParent();
		//}

		//TList<const clang::NamespaceDecl *> ReferenceNDs;
		//while (ReferenceDC && ReferenceDC->isNamespace())
		//{
		//	const clang::NamespaceDecl * ND = dyn_cast<clang::NamespaceDecl>(ReferenceDC);
		//	ReferenceNDs.PushFront(ND->getFirstDecl());
		//	ReferenceDC = ReferenceDC->getParent();
		//}

		//uintx RelativeNamespaceIndex = 0;
		//while (RelativeNamespaceIndex < Min(NDs.Size, ReferenceNDs.Size))
		//{
		//	if (NDs[RelativeNamespaceIndex] != ReferenceNDs[RelativeNamespaceIndex])
		//		break;
		//	++RelativeNamespaceIndex;
		//}

		//if (RelativeNamespaceIndex >= NDs.Size)
		//	return FString::None;

		//FString RelativeNamespaceName;
		//for (uintx NamespaceIndex = RelativeNamespaceIndex; NamespaceIndex < NDs.Size; ++NamespaceIndex)
		//{
		//	if (NamespaceIndex > RelativeNamespaceIndex)
		//		RelativeNamespaceName.Append(u8"::"V);
		//	RelativeNamespaceName.Append(NDs[NamespaceIndex]->getName());
		//}
		//return RelativeNamespaceName;


		TList<const clang::DeclContext *> DCs;
		while (DC)
		{
			if (DC->isNamespace())
				DCs.PushFront(dyn_cast<clang::NamespaceDecl>(DC)->getFirstDecl());
			else if (DC->isRecord())
				DCs.PushFront(dyn_cast<clang::RecordDecl>(DC)->getFirstDecl());
			else
				DCs.PushFront(DC);
			DC = DC->getParent();
		}

		TList<const clang::DeclContext *> ReferenceDCs;
		while (ReferenceDC)
		{
			if (ReferenceDC->isNamespace())
				ReferenceDCs.PushFront(dyn_cast<clang::NamespaceDecl>(ReferenceDC)->getFirstDecl());
			else if (ReferenceDC->isRecord())
				ReferenceDCs.PushFront(dyn_cast<clang::RecordDecl>(ReferenceDC)->getFirstDecl());
			else
				ReferenceDCs.PushFront(ReferenceDC);
			ReferenceDC = ReferenceDC->getParent();
		}

		uintx RelativeDeclContentIndex = 0;
		while (RelativeDeclContentIndex < Min(DCs.Size, ReferenceDCs.Size))
		{
			if (DCs[RelativeDeclContentIndex] != ReferenceDCs[RelativeDeclContentIndex])
				break;
			++RelativeDeclContentIndex;
		}

		if (RelativeDeclContentIndex >= DCs.Size)
			return FString::None;

		FString RelativeDeclContextName;
		bool NeedTypename = false;
		for (uintx DeclContextIndex = RelativeDeclContentIndex; DeclContextIndex < DCs.Size; ++DeclContextIndex)
		{
			if (DeclContextIndex > RelativeDeclContentIndex)
				RelativeDeclContextName.Append(u8"::"V);

			const clang::DeclContext * TempDC = DCs[DeclContextIndex];
			if (TempDC->isNamespace())
				RelativeDeclContextName.Append(dyn_cast<clang::NamespaceDecl>(TempDC)->getName());
			else if (TempDC->isRecord())
			{
				const clang::CXXRecordDecl * CXXRD = dyn_cast<clang::CXXRecordDecl>(TempDC);
				if (CXXRD->getDescribedClassTemplate() != nullptr)
					NeedTypename = true;
				RelativeDeclContextName.Append(CXXRD->getName());
			}
		}
		return NeedTypename ? FString(u8"typename "V + RelativeDeclContextName) : RelativeDeclContextName;
	}

	FString GetDeclNamespaceName(const clang::Decl & RD)
	{
		return GetNamespaceName(RD.getDeclContext());
	}

	static TPair<clang::BuiltinType::Kind, FStringV> BuiltinTypes[] =
	{
		{ clang::BuiltinType::NullPtr, u8"nullp"V },
		{ clang::BuiltinType::Void, u8"void"V },
		{ clang::BuiltinType::Bool, u8"bool"V },
		{ clang::BuiltinType::Char_S, u8"chara"V },
		{ clang::BuiltinType::Char_U, u8"chara"V },
		{ clang::BuiltinType::WChar_S, u8"charw"V },
		{ clang::BuiltinType::WChar_U, u8"charw"V },
		{ clang::BuiltinType::Char8, u8"char8"V },
		{ clang::BuiltinType::Char16, u8"char16"V },
		{ clang::BuiltinType::Char32, u8"char32"V },
		{ clang::BuiltinType::SChar, u8"int8"V },
		{ clang::BuiltinType::UChar, u8"uint8"V },
		{ clang::BuiltinType::Short, u8"int16"V },
		{ clang::BuiltinType::UShort, u8"uint16"V },
		{ clang::BuiltinType::Int, u8"int32"V },
		{ clang::BuiltinType::UInt, u8"uint32"V },
		{ clang::BuiltinType::LongLong, u8"int64"V },
		{ clang::BuiltinType::ULongLong, u8"uint64"V },
		{ clang::BuiltinType::Float, u8"float32"V },
		{ clang::BuiltinType::Double, u8"float64"V },
		{ clang::BuiltinType::LongDouble, u8"float64"V },
	};

	bool IsAdapterBuiltinType(clang::TypedefNameDecl * TDND)
	{
		FStringV TDName = TDND->getName();
		if (TDName == u8"int8"V || TDName == u8"uint8"V ||
			TDName == u8"int16"V || TDName == u8"uint16"V ||
			TDName == u8"int32"V || TDName == u8"uint32"V ||
			TDName == u8"int64"V || TDName == u8"uint64"V ||
			TDName == u8"intx"V || TDName == u8"uintx"V ||
			TDName == u8"float32"V || TDName == u8"float64"V ||
			TDName == u8"chara"V || TDName == u8"charw"V || TDName == u8"char8"V || TDName == u8"char16"V || TDName == u8"char32"V ||
			TDName == u8"sizet"V || TDName == u8"voidp"V || TDName == u8"bytep"V)
			return true;
		return false;
	}

	FStringV GetCXXBuiltinTypeName(clang::BuiltinType::Kind Kind)
	{
		for (auto & Pair : BuiltinTypes)
		{
			if (Pair.First == Kind)
				return Pair.Second;
		}
		return FStringV::None;
	}

	bool SaveFileWithMD5(FReflectSettings & ReflectSettings, FStringV FilePath, FStringV FileConent)
	{
		FString MD5Header = u8"// A123456789B123456789C123456789D1 2000-00-00 00:00:00"V + ReflectSettings.LineEnd;
		MD5Encrypt(FileConent, MD5Header.Slice(3, 32));
		FStringV MD5 = MD5Header.View(3, 32);

		if (FFile::Exists(FilePath))
		{
			FFileStream File { FilePath, EFileModes::Input };
			FString ExistsHeader = File.ReadLine();
			FStringV ExistsMD5 = ExistsHeader.View(3, 32);
			if (StringEqualIC<char8>(ExistsMD5, MD5))
			{
				//FConsole::WriteLineF(u8"skiped, there's no change(s)."V);
				return false;
			}
		}

		{
			FString TimeStamp = FDateTime::Current().Format(u8"yy-MM-dd HH:mm:ss"V);
			MD5Header.SetAt(3 + 32 + 1, TimeStamp);
			FFileStream FileStream { FilePath, EFileModes::Output };
			FileStream.WriteBytes({ 0xEF, 0xBB, 0xBF });
			FileStream.WriteData(MD5Header.Data, MD5Header.Length);
			FileStream.WriteData(FileConent.Data, FileConent.Size);
			return true;
		}
	}

	clang::XinMetaAttr * FindXinAttr(clang::Decl * D)
	{
		for (clang::Attr * Attr : D->attrs())
		{
			if (Attr->getKind() == clang::attr::XinMeta)
				return dyn_cast<clang::XinMetaAttr>(Attr);
		}
		return  nullptr;
	}

	clang::UnresolvedLookupExpr * FindAttrArg(clang::XinMetaAttr * XinAttr, FStringV ArgName)
	{
		for (clang::Expr * Arg : XinAttr->args())
		{
			if (Arg->getStmtClass() == clang::Stmt::UnresolvedLookupExprClass)
			{
				clang::UnresolvedLookupExpr * UnresolvedLookupExpr = dyn_cast<clang::UnresolvedLookupExpr>(Arg);
				if (UnresolvedLookupExpr->getName().getAsString() == std::string_view((const chara *)ArgName.Data, ArgName.Size))
					return UnresolvedLookupExpr;
			}
		}
		return nullptr;
	}

	clang::UnresolvedLookupExpr * FindXinAttribute(clang::Decl * D, FStringV Name)
	{
		if (auto * XinAttr = FindXinAttr(D))
			return FindAttrArg(XinAttr, Name);
		return nullptr;
	}

	bool IsCXXRecordCopyConstructable(clang::CXXRecordDecl * CXXRD)
	{
		for (clang::CXXConstructorDecl * Ctor : CXXRD->ctors())
		{
			if (Ctor->isCopyConstructor())
				return not Ctor->isDeleted();
		}
		return CXXRD->hasSimpleCopyConstructor();
	}

	bool IsCXXRecordCopyAssignable(clang::CXXRecordDecl * CXXRD)
	{
		for (clang::CXXMethodDecl * CXXMD : CXXRD->methods())
		{
			if (CXXMD->isCopyAssignmentOperator())
				return not CXXMD->isDeleted();
		}
		return CXXRD->hasSimpleCopyAssignment();
	}

	clang::NamedDecl * FindNamedDecl(clang::DeclContext * DC, FStringV Name)
	{
		for (clang::Decl * D : DC->decls())
		{
			if (not llvm::isa<clang::NamedDecl>(D))
				continue;

			clang::NamedDecl * ND = dyn_cast<clang::NamedDecl>(D);
			if (not ND->getDeclName().isIdentifier())
				continue;

			if (FStringV(ND->getName()) == Name)
				return ND;
		}


		if (DC->getDeclKind() == clang::Decl::CXXRecord)
		{
			clang::CXXRecordDecl * CXXRecordDecl = dyn_cast<clang::CXXRecordDecl>(DC);
			for (clang::CXXBaseSpecifier & BaseDecl : CXXRecordDecl->bases())
			{
				if (BaseDecl.getAccessSpecifier() != clang::AS_public)
					continue;

				clang::QualType BaseQType = BaseDecl.getType();
				clang::Type::TypeClass BaseQTypeClass = BaseQType->getTypeClass();
				if (BaseQTypeClass == clang::Type::Elaborated)
				{
					const clang::ElaboratedType * ELBType = BaseQType->getAs<clang::ElaboratedType>();
					clang::QualType NamedType = ELBType->getNamedType();
					clang::Type::TypeClass NamedTypeClass = NamedType->getTypeClass();
					if (NamedTypeClass == clang::Type::Record)
					{
						clang::CXXRecordDecl * CXXRD = NamedType->getAsCXXRecordDecl();
						if (auto FoundInBase = FindNamedDecl(CXXRD, Name))
							return FoundInBase;
					}
				}
			}
		}
		return nullptr;
	}

	clang::NamedDecl * FindNamedDecl(clang::DeclContext * DC, FStringV Name, clang::Decl::Kind Kind, bool FindInBaseDecl)
	{
		for (clang::Decl * D : DC->decls())
		{
			if (not llvm::isa<clang::NamedDecl>(D))
				continue;

			if (D->getKind() != Kind)
				continue;

			clang::NamedDecl * ND = dyn_cast<clang::NamedDecl>(D);
			if (not ND->getDeclName().isIdentifier())
				continue;

			if (FStringV(ND->getName()) == Name)
				return ND;
		}

		if (FindInBaseDecl && DC->getDeclKind() == clang::Decl::CXXRecord)
		{
			clang::CXXRecordDecl * CXXRecordDecl = dyn_cast<clang::CXXRecordDecl>(DC);
			for (clang::CXXBaseSpecifier & BaseDecl : CXXRecordDecl->bases())
			{
				if (BaseDecl.getAccessSpecifier() != clang::AS_public)
					continue;

				clang::QualType BaseQType = BaseDecl.getType();
				clang::Type::TypeClass BaseQTypeClass = BaseQType->getTypeClass();
				if (BaseQTypeClass == clang::Type::Elaborated)
				{
					const clang::ElaboratedType * ELBType = BaseQType->getAs<clang::ElaboratedType>();
					clang::QualType NamedType = ELBType->getNamedType();
					clang::Type::TypeClass NamedTypeClass = NamedType->getTypeClass();
					if (NamedTypeClass == clang::Type::Record)
					{
						clang::CXXRecordDecl * CXXRD = NamedType->getAsCXXRecordDecl();
						if (auto FoundInBase = FindNamedDecl(CXXRD, Name, Kind))
							return FoundInBase;
					}
				}
			}
		}
		return nullptr;
	}
}
