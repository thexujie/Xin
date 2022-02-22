#include "PCH.h"
#include "Generator.h"
#include "Utils.h"

namespace Xin
{
	FReflectBuilder::FReflectBuilder(FStringV LineEnd, FStringV Tab) : LineEnd(LineEnd), Tab(Tab)
	{
		for (auto Index : IRange(16))
		{
			LineEndx16.Append(LineEnd);
			Tabx16.Append(Tab);
		}
	}

	void FReflectBuilder::AppendLine(uintx LineCount)
	{
		if (LineCount > 16)
			return;

		Result.Append(LineEndx16.View(0, LineCount * LineEnd.Length));
	}

	void FReflectBuilder::AppendLine(TView<FStringV> Contents)
	{
		if (TabLevel > 16 || !Contents.Size)
			return;

		bool HasContent = false;
		for (uintx Index = 0; Index < Contents.Size; ++Index)
		{
			if (Contents[Index].Size)
			{
				HasContent = true;
				break;
			}
		}

		if (!HasContent)
			return;

		Result.Append(Tabx16.View(0, TabLevel * Tab.Length));
		if (Contents.Size)
		{
			for (uintx Index = 0; Index < Contents.Size; ++Index)
				Result.Append(Contents[Index]);
		}
		Result.Append(LineEnd);
		
	}

	void FReflectBuilder::AppendLine(FStringV Content)
	{
		if (TabLevel > 16 || !Content.Size)
			return;

		if (Content.Size)
		{
			Result.Append(Tabx16.View(0, TabLevel * Tab.Length));
			Result.Append(Content);
		}
		Result.Append(LineEnd);
	}


	void FReflectBuilder::BeginNamespace(FStringV Namespace)
	{
		AppendLine(u8"namespace {0}"V, Namespace);
		AppendLine(u8"{"V);
		++TabLevel;
	}

	void FReflectBuilder::EndNamespace()
	{
		++TabLevel;
		AppendLine(u8"{"V);
	}

	FReflectionScope::FReflectionScope(FReflectBuilder & Builder, EReflectionScope ReflectionScope) : Builder(Builder), ReflectionScopeType(ReflectionScope)
	{
		switch (ReflectionScopeType)
		{
		case EReflectionScope::Scope:
		case EReflectionScope::Declaration:
			Builder.AppendLine(u8"{"V);
			break;
		default:
			break;
		}
	}

	FReflectionScope::~FReflectionScope()
	{
		switch (ReflectionScopeType)
		{
		case EReflectionScope::Scope:
			Builder.AppendLine(u8"}"V);
			break;
		case EReflectionScope::Declaration:
			Builder.AppendLine(u8"};"V);
			break;
		default:
			break;
		}
	}

	FReflectionTab::FReflectionTab(FReflectBuilder & Builder) : Builder(Builder)
	{
		++Builder.TabLevel;
	}

	FReflectionTab::~FReflectionTab()
	{
		--Builder.TabLevel;
	}

	FReflectionTabScope::FReflectionTabScope(FReflectBuilder & Builder, EReflectionScope ReflectionScope) : Builder(Builder), ReflectionScope(ReflectionScope)
	{
		switch (ReflectionScope)
		{
		case EReflectionScope::Scope:
		case EReflectionScope::Declaration:
			Builder.AppendLine(u8"{"V);
			++Builder.TabLevel;
			break;
		default:
			break;
		}
	}

	FReflectionTabScope::~FReflectionTabScope()
	{
		switch (ReflectionScope)
		{
		case EReflectionScope::Scope:
			--Builder.TabLevel;
			Builder.AppendLine(u8"}"V);
			break;
		case EReflectionScope::Declaration:
			--Builder.TabLevel;
			Builder.AppendLine(u8"};"V);
			break;
		default:
			break;
		}
	}

	static clang::CXXRecordDecl * GetBaseClassRecord(clang::CXXRecordDecl * CXXRD)
	{
		if (!CXXRD)
			return nullptr;

		if (CXXRD->getNumBases() == 0 || Utils::FindXinAttribute(CXXRD, u8"SystemType"V))
			return nullptr;

		clang::QualType BaseQType = CXXRD->bases_begin()->getType();
		clang::Type::TypeClass BaseTypeClass = BaseQType->getTypeClass();
		if (BaseTypeClass == clang::Type::Elaborated)
		{
			const clang::ElaboratedType * ELBType = BaseQType->getAs<clang::ElaboratedType>();
			clang::QualType NamedQType = ELBType->getNamedType();
			clang::Type::TypeClass TC = NamedQType->getTypeClass();
			if (TC == clang::Type::Record)
				return NamedQType->getAsCXXRecordDecl();
			else
				return nullptr;
		}
		else if (BaseTypeClass == clang::Type::Record)
		{
			clang::RecordDecl * RD = BaseQType->getAsRecordDecl();
			return dyn_cast<clang::CXXRecordDecl>(RD);
		}
		else
			return nullptr;
	}

	static bool IsIObjectRecord(clang::CXXRecordDecl & RD)
	{
		clang::CXXRecordDecl * ParentRD  = GetBaseClassRecord(&RD);
		while(ParentRD)
		{
			if (ParentRD->getName() == "IObject")
			{
				clang::DeclContext * ParentNamespaceDC = ParentRD->getDeclContext();
				if (ParentNamespaceDC->getDeclKind() != clang::Decl::Namespace)
					return false;

				clang::NamespaceDecl * ND = llvm::dyn_cast<clang::NamespaceDecl>(ParentNamespaceDC);
				return ND->getName() == "Xin";
			}
			ParentRD = GetBaseClassRecord(ParentRD);
		}
		return false;
	}

	TList<TPair<FString, FString>> FNameContext::GenerateMetadata(clang::Decl & D)
	{
		clang::XinMetaAttr * MetaAttr = nullptr;
		for (clang::Attr * A : D.attrs())
		{
			if (A->getKind() == clang::attr::XinMeta)
			{
				MetaAttr = static_cast<clang::XinMetaAttr *>(A);
				break;
			}
		}
		if (!MetaAttr)
			return { };

		TList<TPair<FString, FString>> Metadata;

		const auto GetArgExprString = [&](clang::Expr * E) -> FStringV
			{
				//ClassSource = clang::Lexer::getSourceText(clang::CharSourceRange::getTokenRange(SR), Context.getSourceManager(), Context.getLangOpts());
				clang::Stmt::StmtClass StmtClass = E->getStmtClass();
				switch (StmtClass)
				{
				case clang::Stmt::StringLiteralClass:
				{
					clang::StringLiteral * SL = static_cast<clang::StringLiteral *>(E);
					return u8"u8\""V + FStringV(SL->getString()) + u8"\"V"V;
				}
				case clang::Stmt::IntegerLiteralClass:
				{
					clang::IntegerLiteral * IL = static_cast<clang::IntegerLiteral *>(E);
					return Format(u8"{}"V, IL->getValue().getSExtValue());
				}
				case clang::Stmt::FloatingLiteralClass:
				{
					clang::FloatingLiteral * FL = static_cast<clang::FloatingLiteral *>(E);
					return Format(u8"{}"V, FL->getValue().convertToDouble());
				}
				default:
					AssertFailed(u8"Invalid StmtClass"V);
					return FStringV::None;
				}
			};

		const auto AppendPropertyMember = [&](clang::UnresolvedLookupExpr * IdentifierExpr, clang::InitListExpr * IdentifierArgExpr)
		{
			if (!IdentifierExpr)
				return;

			clang::IdentifierInfo * II = IdentifierExpr->getName().getAsIdentifierInfo();
			FStringV Name = II->getName();

			if (!IdentifierArgExpr)
				Metadata.Add({ Name, u8"true"V });
			else if (IdentifierArgExpr->getNumInits() == 0)
				Metadata.Add({ Name, u8"{}"V });
			else if (IdentifierArgExpr->getNumInits() == 1)
				Metadata.Add({ Name, GetArgExprString(IdentifierArgExpr->getInit(0)) });
			else
			{
				FString ArgsString;
				for (unsigned ArgIndex = 0; ArgIndex < IdentifierArgExpr->getNumInits(); ++ArgIndex)
					Metadata.Add({ Name, GetArgExprString(IdentifierArgExpr->getInit(ArgIndex)) });
			}
		};

		clang::UnresolvedLookupExpr * IdentifierExpr = nullptr;
		clang::InitListExpr * IdentifierArgExpr = nullptr;

		for (clang::Expr * ArgExpr : MetaAttr->args())
		{
			clang::Stmt::StmtClass StmtClass = ArgExpr->getStmtClass();
			switch (StmtClass)
			{
			case clang::Stmt::InitListExprClass:
			{
				IdentifierArgExpr = static_cast<clang::InitListExpr *>(ArgExpr);
				break;
			}
			case clang::Stmt::UnresolvedLookupExprClass:
			{
				AppendPropertyMember(IdentifierExpr, IdentifierArgExpr);
				IdentifierExpr = static_cast<clang::UnresolvedLookupExpr *>(ArgExpr);
				IdentifierArgExpr = nullptr;
				break;
			}
			default:
				break;
			}
		}
		AppendPropertyMember(IdentifierExpr, IdentifierArgExpr);
		return Metadata;
	}

	TPair<FString, FString> FNameContext::GenerateTemplateDescriptorsAndParameters(clang::TemplateDecl * TD)
	{
		FString TemplateDescriptors;
		FString TemplateParameters;

		TemplateDecls.PushBack(TD);
		clang::TemplateParameterList * TPL = TD->getTemplateParameters();
		for (uint32 TPIndex = 0; TPIndex < TPL->size(); ++TPIndex)
		{
			clang::NamedDecl * TPND = TPL->getParam(TPIndex);

			// TemplateArguments
			if (TPIndex > 0)
				TemplateParameters.Append(u8", "V);
			TemplateParameters.Append(TPND->getName());

			// TemplateParameters
			if (TPIndex > 0)
				TemplateDescriptors.Append(u8", "V);

			clang::Decl::Kind TPNDKind = TPND->getKind();
			if (TPNDKind == clang::Decl::Kind::TemplateTypeParm)
			{
				FStringV ParameterTypeName = u8"typename"V;
				clang::TemplateTypeParmDecl * TTPD = dyn_cast<clang::TemplateTypeParmDecl>(TPND);
				if (const clang::TypeConstraint * TypeConstraint = TTPD->getTypeConstraint())
				{
					if (clang::ConceptDecl * TypeConstraintConcept = TypeConstraint->getNamedConcept())
					{
						ParameterTypeName = TypeConstraintConcept->getName();
					}
				}
				TemplateDescriptors.Append(ParameterTypeName + u8" "V);
				TemplateDescriptors.Append(TPND->getName());
				// !!! 不在这里赋值默认参数了
				//if (TTPD->hasDefaultArgument())
				//{
				//	clang::QualType DefaultArgumentQType = TTPD->getDefaultArgument();
				//	TemplateParameters.Append(u8" = "V);
				//	TemplateParameters.Append(GenerateQualTypeName(DefaultArgumentQType));
				//}
			}
			else
			{
				TemplateDescriptors.Append(u8"typename "V);
				TemplateDescriptors.Append(TPND->getName());
			}
		}
		TemplateDecls.PopBack();
		return { TemplateDescriptors , TemplateParameters };
	}

	FString FNameContext::GenerateCXXRecordName(clang::NamedDecl * Decl)
	{
		FString Result;
		Result.Append(Decl->getName());
		if (Decl->getKind() == clang::Decl::ClassTemplateSpecialization)
		{
			const clang::ClassTemplateSpecializationDecl * CTSD = dyn_cast<clang::ClassTemplateSpecializationDecl>(Decl);
			const clang::TemplateArgumentList & TAList = CTSD->getTemplateArgs();
			Result.Append(u8"<"V);
			for (uint32 TAIndex = 0; TAIndex < TAList.size(); ++TAIndex)
			{
				clang::TemplateArgument TA = TAList[TAIndex];
				if (TA.getIsDefaulted())
					break;

				if (TAIndex > 0)
					Result.Append(u8", "V);
				Result.Append(GenerateQualTypeName(TA.getAsType()));
			}
			Result.Append(u8">"V);
		}

		AssertExpr(ReferenceDC);
		if (FString RelativeNamespace = Utils::GetRelativeQualifiedPrefix(Decl->getDeclContext(), ReferenceDC))
			return RelativeNamespace + u8"::"V + Result;
		return Result;
	}

	FString FNameContext::GenerateCXXRecordIdentifier(clang::CXXRecordDecl * CXXRD)
	{
		FString Result;
		Result.Append(CXXRD->getName());
		if (CXXRD->getKind() == clang::Decl::ClassTemplateSpecialization)
		{
			const clang::ClassTemplateSpecializationDecl * CTSD = dyn_cast<clang::ClassTemplateSpecializationDecl>(CXXRD);
			const clang::TemplateArgumentList & TAList = CTSD->getTemplateArgs();
			Result.Append(u8"__"V);
			for (uint32 TAIndex = 0; TAIndex < TAList.size(); ++TAIndex)
			{
				clang::TemplateArgument TA = TAList[TAIndex];
				if (TA.getIsDefaulted())
					break;

				if (TAIndex > 0)
					Result.Append(u8"_"V);
				Result.Append(GenerateQualTypeIdentifier(TA.getAsType()));
			}
		}
		return Result;
	}

	FString FNameContext::GenerateQualTypeName(clang::QualType QType)
	{
		FString Qualifiers;
		uint32 LocalCVR = QType.getLocalCVRQualifiers();
		if (LocalCVR & clang::Qualifiers::Const)
			Qualifiers.Append(u8" const"V);
		if (LocalCVR & clang::Qualifiers::Volatile)
			Qualifiers.Append(u8" volatile"V);

		clang::Type::TypeClass TypeClass = QType->getTypeClass();
		if (QType->isNullPtrType())
		{
			return Utils::GetCXXBuiltinTypeName(clang::BuiltinType::Kind::NullPtr);
		}
		else if (TypeClass == clang::Type::Builtin)
		{
			const clang::BuiltinType * BType = QType->getAs<clang::BuiltinType>();
			return Utils::GetCXXBuiltinTypeName(BType->getKind()) + Qualifiers;
		}
		else if (TypeClass == clang::Type::Enum)
		{
			const clang::EnumType * EType = QType->getAs<clang::EnumType>();
			clang::EnumDecl * ED = EType->getDecl();
			if (FString QialifiedPrefix = Utils::GetRelativeDeclContext(ED->getDeclContext(), ReferenceDC))
				return QialifiedPrefix + u8"::"V + FString(ED->getName()) + Qualifiers;
			else
				return FString(ED->getName()) + Qualifiers;
		}
		else if (TypeClass == clang::Type::Elaborated)
		{
			const clang::ElaboratedType * ELBType = QType->getAs<clang::ElaboratedType>();
			return GenerateQualTypeName(ELBType->getNamedType()) + Qualifiers;
		}
		else if (TypeClass == clang::Type::Typedef)
		{
			const clang::TypedefType * TDType = QType->getAs<clang::TypedefType>();
			clang::TypedefNameDecl * TDND = TDType->getDecl();
			if (FString RelativeDeclContext = Utils::GetRelativeDeclContext(TDND->getDeclContext(), ReferenceDC))
				return RelativeDeclContext + u8"::"V + FStringV(TDND->getName());
			else
				return FStringV(TDND->getName());

			//clang::QualType UnderlyingQType = TDND->getUnderlyingType();
			//if (Utils::IsAdapterBuiltinType(TDND))
			//	Result.Append(TDND->getName());
			//else
			//	Result.Append(GenerateQualTypeName(UnderlyingQType));
		}
		else if (TypeClass == clang::Type::Decltype)
		{
			const clang::DecltypeType * DeclT = QType->getAs<clang::DecltypeType>();
			clang::QualType UnderlyingQType = DeclT->getUnderlyingType();
			return GenerateQualTypeName(UnderlyingQType);
		}
		else if (TypeClass == clang::Type::InjectedClassName)
		{
			const clang::InjectedClassNameType * ICNT = QType->getAs<clang::InjectedClassNameType>();
			return FStringV(ICNT->getDecl()->getName()) + Qualifiers;
		}
		else if (TypeClass == clang::Type::Record)
		{
			clang::CXXRecordDecl * CXXRD = QType->getAsCXXRecordDecl();
			return GenerateCXXRecordName(CXXRD) + Qualifiers;
		}
		else if (TypeClass == clang::Type::Pointer)
		{
			const clang::PointerType * PType = QType->getAs<clang::PointerType>();
			return GenerateQualTypeName(PType->getPointeeType()) + Qualifiers + u8" *"V;
		}
		else if (TypeClass == clang::Type::LValueReference)
		{
			const clang::LValueReferenceType * LVRefType = QType->getAs<clang::LValueReferenceType>();
			clang::QualType PointeeQType = LVRefType->getPointeeType();
			if (PointeeQType->getTypeClass() == clang::Type::Paren)
			{
				const clang::ParenType * PType = PointeeQType->getAs<clang::ParenType>();
				clang::QualType ParenInnerQType = PType->getInnerType();
				if (ParenInnerQType->getTypeClass() == clang::Type::ConstantArray)
				{
					const clang::ConstantArrayType * CAType = dyn_cast<clang::ConstantArrayType>(ParenInnerQType->getAsArrayTypeUnsafe());
					clang::QualType ElementQType = CAType->getElementType();
					uint32 ArraySize = uint32(CAType->getSize().getZExtValue());
					return GenerateQualTypeName(ElementQType) + Format(u8"(&)[{}]"V, ArraySize);
				}
				else { }
			}
			else { }
			return GenerateQualTypeName(PointeeQType) + u8" &"V;
		}
		else if (TypeClass == clang::Type::RValueReference)
		{
			const clang::RValueReferenceType * RVRefType = QType->getAs<clang::RValueReferenceType>();
			return GenerateQualTypeName(RVRefType->getPointeeType()) + u8" &&"V;
		}
		else if (TypeClass == clang::Type::TemplateTypeParm)
		{
			const clang::TemplateTypeParmType * TTPType = QType->getAs<clang::TemplateTypeParmType>();
			clang::NamedDecl * ParamND = GetTemplateParam(TTPType->getDepth(), TTPType->getIndex());
			return FStringV(ParamND->getName()) + Qualifiers;
		}
		else if (TypeClass == clang::Type::Paren)
		{
			const clang::ParenType * PType = QType->getAs<clang::ParenType>();
			clang::QualType ParenInnerQType = PType->getInnerType();
			return u8"("V + GenerateQualTypeName(ParenInnerQType) + u8")"V;
		}
		else if (TypeClass == clang::Type::ConstantArray)
		{
			//const clang::ConstantArrayType * CAType = QType->getAs<clang::ConstantArrayType>();
			const clang::ConstantArrayType * CAType = dyn_cast<clang::ConstantArrayType>(QType->getAsArrayTypeUnsafe());
			clang::QualType ElementQType = CAType->getElementType();
			uint32 ArraySize = uint32(CAType->getSize().getZExtValue());
			return GenerateQualTypeName(ElementQType) + Format(u8"[{}]"V, ArraySize);
		}
		else if (TypeClass == clang::Type::FunctionProto)
		{
			const clang::FunctionProtoType * FPType = QType->getAs<clang::FunctionProtoType>();

			FString FunctionProtoName;
			FunctionProtoName.Append(GenerateQualTypeName(FPType->getReturnType()));
			FunctionProtoName.Append(u8'(');
			for (unsigned ParamTypeIndex = 0; ParamTypeIndex < FPType->getNumParams(); ++ParamTypeIndex)
			{
				clang::QualType ParameterQType = FPType->getParamType(ParamTypeIndex);
				if (ParamTypeIndex > 0)
					FunctionProtoName.Append(u8", "V);
				FunctionProtoName.Append(GenerateQualTypeName(ParameterQType));
			}
			FunctionProtoName.Append(u8')');
			return FunctionProtoName + Qualifiers;
		}
		else if (TypeClass == clang::Type::TemplateSpecialization)
		{
			FString TemplateSpecializationName;
			const clang::TemplateSpecializationType * TSType = QType->getAs<clang::TemplateSpecializationType>();
			const clang::TemplateName & TN = TSType->getTemplateName();
			clang::TemplateName::NameKind TNKind = TN.getKind();
			switch (TNKind)
			{
			case clang::TemplateName::NameKind::Template:
			case clang::TemplateName::NameKind::QualifiedTemplate:
			{
				clang::TemplateDecl * TD = TN.getAsTemplateDecl();
				TemplateDecls.PushBack(TD);
				TemplateSpecializationName.Append(TD->getName());
				TemplateSpecializationName.Append(u8"<"V);

				llvm::ArrayRef<clang::TemplateArgument> TAs = TSType->template_arguments();
				for (uint32 TPIndex = 0; TPIndex < TAs.size(); ++TPIndex)
				{
					const clang::TemplateArgument & TA = TAs[TPIndex];
					if (TPIndex > 0)
						TemplateSpecializationName.Append(u8", "V);

					clang::TemplateArgument::ArgKind ArgKind = TA.getKind();
					switch (ArgKind)
					{
					case clang::TemplateArgument::Type:
					{
						clang::QualType TArgQType = TA.getAsType();
						TemplateSpecializationName.Append(GenerateQualTypeName(TA.getAsType()));
						break;
					}
					case clang::TemplateArgument::Expression:
					{
						clang::Expr * ArgExpr = TA.getAsExpr();
						clang::Stmt::StmtClass ArgStmtClass = ArgExpr->getStmtClass();
						if (ArgStmtClass == clang::Stmt::ImplicitCastExprClass)
						{
							clang::ImplicitCastExpr * CastExpr = dyn_cast<clang::ImplicitCastExpr>(ArgExpr);
							clang::QualType ArgQType = CastExpr->getType();
							TemplateSpecializationName.Append(GenerateQualTypeName(ArgQType));
						}
						break;
					}
					default:
						LogInfo(u8"Invalid ArgKind ({})"V, TA.getKind());
						break;
					}
				}
				TemplateDecls.PopBack();
				TemplateSpecializationName.Append(u8">"V);
				return TemplateSpecializationName;
			}
			default:
				LogInfo(u8"Invalid clang::TemplateName::NameKind ({})"V, TNKind);
				return FString::None;
			}
		}
		else
		{
			FConsole::WriteLine(u8"unkonwn TypeClass {}"V, FStringV(QType->getTypeClassName()));
			return FString::None;
		}
	}

	FString FNameContext::GenerateQualTypeIdentifier(clang::QualType QType)
	{
		FString Qualifiers;
		uint32 LocalCVR = QType.getLocalCVRQualifiers();
		if (LocalCVR & clang::Qualifiers::Const)
			Qualifiers.Append(u8"_const"V);
		if (LocalCVR & clang::Qualifiers::Volatile)
			Qualifiers.Append(u8"_volatile"V);

		clang::Type::TypeClass TypeClass = QType->getTypeClass();
		if (TypeClass == clang::Type::Builtin)
		{
			const clang::BuiltinType * BType = QType->getAs<clang::BuiltinType>();
			return Utils::GetCXXBuiltinTypeName(BType->getKind()) + Qualifiers;
		}
		else if (TypeClass == clang::Type::Enum)
		{
			const clang::EnumType * EType = QType->getAs<clang::EnumType>();
			clang::EnumDecl * ED = EType->getDecl();
			return FStringV(ED->getName()) + Qualifiers;
		}
		else if (TypeClass == clang::Type::Elaborated)
		{
			const clang::ElaboratedType * ELBType = QType->getAs<clang::ElaboratedType>();
			return GenerateQualTypeIdentifier(ELBType->getNamedType()) + Qualifiers;
		}
		else if (TypeClass == clang::Type::Typedef)
		{
			const clang::TypedefType * TDType = QType->getAs<clang::TypedefType>();
			clang::TypedefNameDecl * TDND = TDType->getDecl();
			if (Utils::IsAdapterBuiltinType(TDND))
				return FStringV(TDND->getName());
			else
				return GenerateQualTypeIdentifier(TDND->getUnderlyingType());
		}
		else if (TypeClass == clang::Type::Decltype)
		{
			const clang::DecltypeType * DeclT = QType->getAs<clang::DecltypeType>();
			clang::QualType UnderlyingQType = DeclT->getUnderlyingType();
			return GenerateQualTypeIdentifier(UnderlyingQType);
		}
		else if (TypeClass == clang::Type::InjectedClassName)
		{
			const clang::InjectedClassNameType * ICNT = QType->getAs<clang::InjectedClassNameType>();
			return FStringV(ICNT->getDecl()->getName());
		}
		else if (TypeClass == clang::Type::Record)
		{
			clang::CXXRecordDecl * CXXRD = QType->getAsCXXRecordDecl();
			return GenerateCXXRecordIdentifier(CXXRD);
		}
		else if (TypeClass == clang::Type::Pointer)
		{
			const clang::PointerType * PType = QType->getAs<clang::PointerType>();
			return GenerateQualTypeIdentifier(PType->getPointeeType()) + Qualifiers + u8"_ptr"V;;
		}
		else if (TypeClass == clang::Type::LValueReference)
		{
			const clang::LValueReferenceType * LVRefType = QType->getAs<clang::LValueReferenceType>();
			clang::QualType PointeeQType = LVRefType->getPointeeType();
			if (PointeeQType->getTypeClass() == clang::Type::Paren)
			{
				const clang::ParenType * PType = PointeeQType->getAs<clang::ParenType>();
				clang::QualType ParenInnerQType = PType->getInnerType();
				if (ParenInnerQType->getTypeClass() == clang::Type::ConstantArray)
				{
					const clang::ConstantArrayType * CAType = dyn_cast<clang::ConstantArrayType>(ParenInnerQType->getAsArrayTypeUnsafe());
					clang::QualType ElementQType = CAType->getElementType();
					uint32 ArraySize = uint32(CAType->getSize().getZExtValue());
					return GenerateQualTypeIdentifier(ElementQType) + Qualifiers + Format(u8"_lref_{}"V, ArraySize);
				}
				else { }
			}
			else { }
			return GenerateQualTypeIdentifier(LVRefType->getPointeeType()) + Qualifiers + u8"_lref"V;
		}
		else if (TypeClass == clang::Type::RValueReference)
		{
			const clang::RValueReferenceType * RVRefType = QType->getAs<clang::RValueReferenceType>();
			return GenerateQualTypeIdentifier(RVRefType->getPointeeType()) + Qualifiers + u8"_ref"V;
		}
		else if (TypeClass == clang::Type::Paren)
		{
			const clang::ParenType * PType = QType->getAs<clang::ParenType>();
			return u8"_lpar_"V + GenerateQualTypeIdentifier(PType->getInnerType()) + u8"_rpar_"V;
		}
		else if (TypeClass == clang::Type::ConstantArray)
		{
			//const clang::ConstantArrayType * CAType = QType->getAs<clang::ConstantArrayType>();
			const clang::ConstantArrayType * CAType = dyn_cast<clang::ConstantArrayType>(QType->getAsArrayTypeUnsafe());
			clang::QualType ElementQType = CAType->getElementType();
			uint32 ArraySize = uint32(CAType->getSize().getZExtValue());
			return GenerateQualTypeIdentifier(ElementQType) + u8"_{}_"V.Format(ArraySize);
		}
		else if (TypeClass == clang::Type::FunctionProto)
		{
			FString FunctionProtoName;
			const clang::FunctionProtoType * FPType = QType->getAs<clang::FunctionProtoType>();
			FunctionProtoName.Append(GenerateQualTypeIdentifier(FPType->getReturnType()));
			FunctionProtoName.Append(u8"___"V);
			for (unsigned ParamTypeIndex = 0; ParamTypeIndex < FPType->getNumParams(); ++ParamTypeIndex)
			{
				clang::QualType ParameterQType = FPType->getParamType(ParamTypeIndex);
				if (ParamTypeIndex > 0)
					FunctionProtoName.Append(u8"__"V);
				FunctionProtoName.Append(GenerateQualTypeIdentifier(ParameterQType));
			}
			FunctionProtoName.Append(u8"___"V);
			return FunctionProtoName + Qualifiers;
		}
		else if (TypeClass == clang::Type::TemplateTypeParm)
		{
			const clang::TemplateTypeParmType * TTPType = QType->getAs<clang::TemplateTypeParmType>();
			clang::NamedDecl * ParamND = GetTemplateParam(TTPType->getDepth(), TTPType->getIndex());
			return FStringV(ParamND->getName()) + Qualifiers;
		}
		else if (TypeClass == clang::Type::TemplateSpecialization)
		{
			FString TemplateSpecializationName;
			const clang::TemplateSpecializationType * TSType = QType->getAs<clang::TemplateSpecializationType>();
			const clang::TemplateName & TN = TSType->getTemplateName();
			clang::TemplateName::NameKind TNKind = TN.getKind();
			switch (TNKind)
			{
			case clang::TemplateName::NameKind::QualifiedTemplate:
			case clang::TemplateName::NameKind::Template:
			{
				clang::TemplateDecl * TD = TN.getAsTemplateDecl();
				TemplateDecls.PushBack(TD);
				TemplateSpecializationName.Append(TD->getName());
				TemplateSpecializationName.Append(u8"__"V);

				llvm::ArrayRef<clang::TemplateArgument> TAs = TSType->template_arguments();
				for (uint32 TPIndex = 0; TPIndex < TAs.size(); ++TPIndex)
				{
					const clang::TemplateArgument & TA = TAs[TPIndex];
					if (TPIndex > 0)
						TemplateSpecializationName.Append(u8"_"V);
					TemplateSpecializationName.Append(GenerateQualTypeIdentifier(TA.getAsType()));
				}
				TemplateDecls.PopBack();
				TemplateSpecializationName.Append(u8"__"V);
				return TemplateSpecializationName + Qualifiers;
			}
			default:
				AssertExpr(TNKind == clang::TemplateName::NameKind::Template);
				return FString::None;
			}
			}
		else
		{
			return FString::None;
		}
	}

	FString FNameContext::GenerateCXXMethodParametersIdentifier(const clang::CXXMethodDecl & CXXMD)
	{
		FString Result;
		TView<clang::ParmVarDecl *> ParamVarDecls = CXXMD.parameters();
		for (sizet PVDIndex = 0; PVDIndex < ParamVarDecls.Size; ++PVDIndex)
		{
			const clang::ParmVarDecl & PVD = *ParamVarDecls[PVDIndex];
			if (PVDIndex > 0)
				Result += u8"__"V;
			Result += GenerateQualTypeIdentifier(PVD.getType());
		}
		return Result;
	}

	FString FNameContext::GenerateCXXMethodParametersSignature(const clang::CXXMethodDecl & CXXMD)
	{
		FString Result;
		TView<clang::ParmVarDecl *> ParamVarDecls = CXXMD.parameters();
		for (sizet PVDIndex = 0; PVDIndex < ParamVarDecls.Size; ++PVDIndex)
		{
			const clang::ParmVarDecl & PVD = *ParamVarDecls[PVDIndex];
			if (PVDIndex > 0)
				Result += u8", "V;
			Result += GenerateQualTypeName(PVD.getType());
		}
		return Result;
	}

	FString FNameContext::GenerateCXXMethodSignature(const clang::CXXMethodDecl & CXXMD)
	{
		std::string QualifiedName = CXXMD.getQualifiedNameAsString();

		FString Result;
		Result += GenerateQualTypeName(CXXMD.getReturnType());
		Result += u8"("V;
		TView<clang::ParmVarDecl *> ParamVarDecls = CXXMD.parameters();
		for (sizet PVDIndex = 0; PVDIndex < ParamVarDecls.Size; ++PVDIndex)
		{
			const clang::ParmVarDecl & PVD = *ParamVarDecls[PVDIndex];
			if (PVDIndex > 0)
				Result += u8", "V;
			Result += GenerateQualTypeName(PVD.getType());
		}
		Result += u8")"V;

		clang::Qualifiers MethodQualifiers = CXXMD.getMethodQualifiers();
		if (MethodQualifiers)
		{
			if (MethodQualifiers.hasConst())
				Result += u8" const"V;
			if (MethodQualifiers.hasVolatile())
				Result += u8" volatile"V;
		}
		return Result;
	}

	FString FNameContext::GenerateCXXFunctorSignature(const clang::CXXMethodDecl & CXXMD)
	{
		std::string QualifiedName = CXXMD.getQualifiedNameAsString();

		FString Result;
		Result += GenerateQualTypeName(CXXMD.getReturnType());
		Result += u8"("V;
		TView<clang::ParmVarDecl *> ParamVarDecls = CXXMD.parameters();
		for (sizet PVDIndex = 0; PVDIndex < ParamVarDecls.Size; ++PVDIndex)
		{
			const clang::ParmVarDecl & PVD = *ParamVarDecls[PVDIndex];
			if (PVDIndex > 0)
				Result += u8", "V;
			Result += GenerateQualTypeName(PVD.getType());
		}
		Result += u8")"V;
		return Result;
	}

	FString FNameContext::GenerateCXXMethodIdentifier(const clang::CXXMethodDecl & CXXMD)
	{
		std::string QualifiedName = CXXMD.getQualifiedNameAsString();

		FString Result;
		Result += GenerateQualTypeIdentifier(CXXMD.getReturnType());
		Result += u8"___"V;
		Result += CXXMD.getName();
		TView<clang::ParmVarDecl *> ParamVarDecls = CXXMD.parameters();
		if (ParamVarDecls.Size)
		{
			Result += u8"__"V;
			for (sizet PVDIndex = 0; PVDIndex < ParamVarDecls.Size; ++PVDIndex)
			{
				const clang::ParmVarDecl & PVD = *ParamVarDecls[PVDIndex];
				if (PVDIndex > 0)
					Result += u8"__"V;

				Result += GenerateQualTypeIdentifier(PVD.getType());
			}
		}
		clang::Qualifiers MethodQualifiers = CXXMD.getMethodQualifiers();
		if (MethodQualifiers)
		{
			Result += u8"___"V;
			if (MethodQualifiers.hasConst())
				Result += u8"_const"V;
			if (MethodQualifiers.hasVolatile())
				Result += u8"_volatile"V;
		}
		return Result;
	}

	//#pragma optimize("", off)
	void FCXXRDGenerator::GenerateCXXRD(FReflectBuilder & Builder, FNameContext & NameContext, clang::CXXRecordDecl & CXXRD)
	{
		clang::ASTContext & Context = CXXRD.getASTContext();
		std::string CXXRDQualName = CXXRD.getQualifiedNameAsString();

		//FString ClassSource;
		//{
		//	clang::SourceRange SR = CXXRD.getSourceRange();
		//	ClassSource = clang::Lexer::getSourceText(clang::CharSourceRange::getTokenRange(SR), Context.getSourceManager(), Context.getLangOpts());
		//}

		//if (APIAttribute.Empty())
		//{
		//	auto & LangOpts = CXXRD.getASTContext().getLangOpts();

		//	for (clang::Attr * A : CXXRD.attrs())
		//	{
		//		if (A->getKind() == clang::attr::Kind::DLLExport)
		//		{
		//			clang::Token T;
		//			if (not clang::Lexer::getRawToken(A->getLocation(), T, Context.getSourceManager(), LangOpts))
		//			{
		//				clang::CharSourceRange CharRange = clang::CharSourceRange::getTokenRange(T.getLocation());
		//				APIAttribute = clang::Lexer::getSourceText(CharRange, Context.getSourceManager(), LangOpts);
		//			}
		//		}
		//		else { }
		//	}

		//	if (not APIAttribute.Size)
		//		APIAttribute = u8"NONE_API"V;
		//}

		FStringV TypeName = CXXRD.getName();
		clang::NamedDecl * MetadataDecl = Utils::FindNamedDecl(&CXXRD, u8"FMetadata"V, clang::Decl::CXXRecord);
		clang::NamedDecl * PropertyMetadataDecl = MetadataDecl ? MetadataDecl : Utils::FindNamedDecl(&CXXRD, u8"FPropertyMetadata"V, clang::Decl::CXXRecord);
		clang::NamedDecl * MethodMetadataDecl = MetadataDecl ? MetadataDecl : Utils::FindNamedDecl(&CXXRD, u8"FMethodMetadata"V, clang::Decl::CXXRecord);

		FString QualifiedPrefix = Utils::GetRelativeQualifiedPrefix(CXXRD.getDeclContext(), NameContext.ReferenceDC);
		FString QualifiedTypeName = QualifiedPrefix ? (QualifiedPrefix + u8"::"V + TypeName) : TypeName;

		FString ReflectTypeIdentifier = u8"{0}_Type"V.Format(CXXRD.getName());
		FString BaseType0Expr;
		FString BaseTypeExprs;

		clang::XinMetaAttr * XinAttr = Utils::FindXinAttr(&CXXRD);
		bool IsAncestorType = XinAttr ? !!Utils::FindAttrArg(XinAttr, u8"SystemType"V) : false;

		TList<clang::QualType> PublicBaseQTypes;
		if (!IsAncestorType && CXXRD.getNumBases() > 0)
		{
			for (clang::CXXBaseSpecifier BaseSpecifier : CXXRD.bases())
			{
				clang::AccessSpecifier AccessSpecifier = BaseSpecifier.getAccessSpecifier();
				if (AccessSpecifier != clang::AS_public)
					continue;

				clang::QualType BaseQType = BaseSpecifier.getType();
				FString BaseTypeExpr = NameContext.GenerateQualTypeName(BaseQType);
				//if (BaseTypeExpr == u8"IReferable"V || BaseTypeExpr == u8"IRefer"V)
				//	continue;
				PublicBaseQTypes.Add(BaseQType);

				if (BaseTypeExprs.Length)
					BaseTypeExprs.Append(u8", "V);
				BaseTypeExprs.Append(BaseTypeExpr);
				if (not BaseType0Expr.Length)
					BaseType0Expr = u8"Classof<{}>()"V.Format(BaseTypeExpr);
			}
		}
		if (BaseType0Expr.Empty())
			BaseType0Expr = u8"FClass::None"V;

		FString TemplateDescriptors;
		FString TemplateArguments;
		clang::ClassTemplateDecl * CTD = CXXRD.getDescribedClassTemplate();

		// class TClass<T>
		if (CTD)
		{
			auto [Descriptors, Arguments] = NameContext.GenerateTemplateDescriptorsAndParameters(CTD);
			TemplateDescriptors = Descriptors;
			TemplateArguments = Arguments;
			Builder.AppendLine(u8"template<{0}>"V, TemplateDescriptors);
			Builder.AppendLine(u8"class {0}_Type : public TClass<{1}<{2}>>"V, TypeName, QualifiedTypeName, TemplateArguments);

			NameContext.TemplateDecls.PushBack(CTD);
			// pop at the ending of the generator
			//TemplateDecls.PopBack();
		}
		else
		{
			Builder.AppendLine(u8"class {0}_Type : public TClass<{1}>"V, TypeName, QualifiedTypeName);
		}

		TList<clang::MSPropertyDecl *> ProxyMSPDs;
		{
			FReflectionScope Scope_Class { Builder, EReflectionScope::Declaration };
			Builder.AppendLine(u8"public:"V);
			FReflectionTab Tab_Class_Public { Builder };
			if (CTD)
			{
				Builder.AppendLine(u8"using {0} = {1}<{2}>;"V, TypeName, QualifiedTypeName, TemplateArguments);
				Builder.AppendLine();
			}
			else if (QualifiedPrefix)
			{
				Builder.AppendLine(u8"using {0} = {1};"V, TypeName, QualifiedTypeName);
				Builder.AppendLine();
			}
			else { }

			// Constructors --------------------------------------------------------------------
			Builder.AppendLine(u8"// --- 0 Constructors"V);
			TList<FString> Constructors;
			if (!CXXRD.isAbstract() && !CXXRD.ctors().empty())
			{
				for (clang::CXXConstructorDecl * CXXCD : CXXRD.ctors())
				{
					if (CXXCD->getAccess() != clang::AccessSpecifier::AS_public)
						continue;

					if (CXXCD->isDeleted())
						continue;

					if (CXXCD->getNumParams() > 0)
					{
						FString ParametersSignature = NameContext.GenerateCXXMethodParametersSignature(*CXXCD);
						FString ParametersIdentifier = NameContext.GenerateCXXMethodParametersIdentifier(*CXXCD);
						FString ConstructorIdentifier = u8"Constructor___"V + ParametersIdentifier;
						Builder.AppendLine(u8R"(static inline const TConstructor<{0}, {1}> Constructor___{2} {{ u8"!Constructor___{2}"N, FMetadata::None }};)"V, TypeName, ParametersSignature, ParametersIdentifier);
						Constructors.Add(ConstructorIdentifier);
					}
					else
					{
						FString ConstructorIdentifier = u8"Constructor"V;
						Builder.AppendLine(u8R"(static inline const TConstructor<{0}> Constructor {{ u8"!Constructor"N, FMetadata::None }};)"V, TypeName);
						Constructors.Add(ConstructorIdentifier);
					}
				}

				if (Constructors.Size)
				{
					Builder.AppendLine(u8"static inline const TReferenceWrapper<const FConstructor> ReflectedConstructors[] ="V);
					{
						FReflectionTabScope TabScope_ReflectedConstructors { Builder, EReflectionScope::Declaration };

						for (sizet ConstructorIndex = 0; ConstructorIndex < Constructors.Size; ++ConstructorIndex)
						{
							Builder.AppendLine({ Constructors[ConstructorIndex], u8","V });
						}
					}
				}
			}

			// Methods--------------------------------------------------------------------
			TList<FString> Methods;
			Builder.AppendLine();
			Builder.AppendLine(u8"// --- 1 Methods"V);
			if (not CXXRD.methods().empty())
			{
				for (clang::CXXMethodDecl * CXXMD : CXXRD.methods())
				{
					if (CXXMD->isStatic() || CXXMD->isDeleted())
						continue;

					clang::Decl::Kind MethodKind = CXXMD->getKind();
					if (MethodKind != clang::Decl::CXXMethod)
						continue;

					if (CXXMD->getAccess() != clang::AccessSpecifier::AS_public)
						continue;

					if (not CXXMD->getDeclName().isIdentifier())
						continue;

					if (CXXMD->getName().starts_with("__"))
						continue;

					// we need explicit signature for method override.
					FString MethodStructName = u8"Method___"V + NameContext.GenerateCXXMethodIdentifier(*CXXMD);
					FString FunctorSignature = NameContext.GenerateCXXFunctorSignature(*CXXMD);
					FStringV MethodName = CXXMD->getName();
					FString MethodMember = u8"&{0}::{1}"V.Format(TypeName, MethodName);

					Builder.AppendLine(u8"struct {0}"V, MethodStructName);
					{
						FReflectionTabScope TabScope_Function { Builder, EReflectionScope::Declaration };

						if (CXXMD->size_overridden_methods() || true)
							Builder.AppendLine(u8R"(static inline const TMethod<{0}, {3}> Method {{ u8"{1}"N, {2}, FMetadata::None }};)"V, TypeName, MethodName, MethodMember, FunctorSignature);
						else
							Builder.AppendLine(u8R"(static inline const TMethod Method {{ u8"{1}"N, {2}, FMetadata::None }};)"V, TypeName, MethodName, MethodMember, FunctorSignature);
					}
					//Builder.AppendLine();

					Methods.Add(MethodStructName);
				}

				// ReflectedMethods
				if (Methods.Size)
				{
					Builder.AppendLine(u8"static inline const TReferenceWrapper<const FMethod> ReflectedMethods[] ="V);
					{
						FReflectionTabScope TabScope_ReflectedFunctions { Builder, EReflectionScope::Declaration };

						for (sizet FunctionIndex = 0; FunctionIndex < Methods.Size; ++FunctionIndex)
						{
							Builder.AppendLine({ Methods[FunctionIndex], u8"::Method,"V });
						}
					}
				}
			}

			// Fields--------------------------------------------------------------------
			TList<FString> Fields;
			Builder.AppendLine();
			Builder.AppendLine(u8"// --- 2 Fields"V);
			if constexpr (true)
			{
				// --- field
				auto GenerateFieldLambda = [&](this auto && Self, clang::FieldDecl * FD)
				{
					if (FD->getAccess() != clang::AccessSpecifier::AS_public)
						return;

					clang::QualType FieldQType = FD->getType();
					if (FD->isBitField() || FieldQType->isReferenceType())
						return;

					if (FD->getName().starts_with("__"))
						return;

					clang::QualType DesugaredType = FieldQType.getDesugaredType(FD->getASTContext());
					clang::Type::TypeClass FieldTypeClass = DesugaredType->getTypeClass();
					if (FieldTypeClass == clang::Type::ConstantArray)
						return;

					if (FD->isAnonymousStructOrUnion())
					{
						AssertExpr(FieldTypeClass == clang::Type::Record);
						clang::CXXRecordDecl * FieldCXXRD = DesugaredType->getAsCXXRecordDecl();
						for (clang::FieldDecl * AnonymousFD : FieldCXXRD->fields())
							Self(AnonymousFD);
						return;
					}

					if (FieldTypeClass == clang::Type::Record)
					{
						clang::CXXRecordDecl * FieldCXXRD = DesugaredType->getAsCXXRecordDecl();
						if (not Utils::IsCXXRecordCopyConstructable(FieldCXXRD))
							return;

						if (not Utils::IsCXXRecordCopyAssignable(FieldCXXRD))
							return;
					}

					FStringV FieldName = FD->getName();
					FString FieldType = NameContext.GenerateQualTypeName(FieldQType);

					FString FieldStructName = u8"Field___{0}"V.Format(FieldName);
					FString FieldMember = u8"&{0}::{1}"V.Format(TypeName, FieldName);

					Builder.AppendLine(u8"struct {0}"V, FieldStructName);
					{
						FReflectionTabScope TabScope_Field { Builder, EReflectionScope::Declaration };

						//Builder.AppendLine(u8"// {0} {1}"V, FieldType, FieldName);
						Builder.AppendLine(u8"static inline const TField Field {{ u8\"{0}\"N, {1}, FMetadata::None }};"V, FieldName, FieldMember);
					}
					//Builder.AppendLine();
					Fields.Add(FieldStructName);
				};

				for (clang::FieldDecl * FD : CXXRD.fields())
				{
					GenerateFieldLambda(FD);
				}

				// ReflectedFields
				if (Fields.Size)
				{
					Builder.AppendLine(u8"static inline const TReferenceWrapper<const FField> ReflectedFields[] ="V);
					{
						FReflectionTabScope TabScope_ReflectedFields { Builder, EReflectionScope::Declaration };

						for (sizet FieldIndex = 0; FieldIndex < Fields.Size; ++FieldIndex)
						{
							Builder.AppendLine({ Fields[FieldIndex], u8"::Field,"V });
						}
					}
				}
			}

			// Properties--------------------------------------------------------------------
			TList<FString> Properties;
			Builder.AppendLine();
			Builder.AppendLine(u8"// --- 3 Properties"V);
			if constexpr (true)
			{
				for (clang::Decl * MSPropertyDecl : CXXRD.decls())
				{
					if (MSPropertyDecl->getAccess() != clang::AccessSpecifier::AS_public)
						continue;

					if (MSPropertyDecl->getKind() != clang::Decl::Kind::MSProperty)
						continue;

					clang::SourceRange SR = MSPropertyDecl->getSourceRange();

					clang::MSPropertyDecl * MSPD = (clang::MSPropertyDecl *)MSPropertyDecl;
					clang::IdentifierInfo * Getter = MSPD->getGetterId();
					clang::IdentifierInfo * Setter = MSPD->getSetterId();
					clang::CXXMethodDecl * GetterMethod = Getter ? Utils::FindNamedDeclT<clang::CXXMethodDecl>(&CXXRD, Getter->getName(), clang::Decl::CXXMethod) : nullptr;
					clang::CXXMethodDecl * SetterMethod = Setter ? Utils::FindNamedDeclT<clang::CXXMethodDecl>(&CXXRD, Setter->getName(), clang::Decl::CXXMethod) : nullptr;
					if (!GetterMethod && !SetterMethod)
						continue;

					clang::CXXMethodDecl * SetValueMethod = Setter ? Utils::FindNamedDeclT<clang::CXXMethodDecl>(&CXXRD, FStringV(Setter->getName()) + u8"Value"V, clang::Decl::CXXMethod) : nullptr;
					SetterMethod = SetValueMethod ? SetValueMethod : SetterMethod;

					TList<TPair<FString, FString>> MetadataItems = NameContext.GenerateMetadata(*MSPropertyDecl);
					FStringV PropertyName = MSPD->getName();
					FString PropertyTypeSignature = NameContext.GenerateQualTypeName(MSPD->getType());
					FString PropertyIdentifier = u8"Property___{0}"V.Format(PropertyName);
					FString MetadataIdentifier;
					//fmt::formatter<std::basic_string<char8>, char8>();
					//fmt::formatter<llvm::StringRef, char8>();
					//fmt::formatter<clang::TemplateArgument::ArgKind, char8>();
					//static_assert(fmt::has_formatter<llvm::StringRef, fmt::buffered_context<char8>>::value);
					//using chart_1 = fmt::v10::char_t<llvm::StringRef>;
					//using chart_2 = decltype(fmt::detail::to_string_view(std::declval<llvm::StringRef>()))::value_type;
					//static_assert(std::is_same_v<chart_1, chart_2>);

					//using mapped_type = decltype(fmt::detail::arg_mapper<fmt::buffered_context<char8>>().map<llvm::StringRef>(std::declval<llvm::StringRef &>()))::value;
					static_assert(fmt::is_formattable<llvm::StringRef, char8>::value);

					static_assert(IsAdapterFormatAbleV<llvm::StringRef>);
					if (PropertyMetadataDecl)
						MetadataIdentifier = MetadataItems.Size ? u8"Metadata"V : Format(u8"{0}::{1}::None"V, TypeName, PropertyMetadataDecl->getName());
					else
						MetadataIdentifier = u8"FMetadata::None"V;
					FStringV MSPDSource = clang::Lexer::getImmediateMacroName(SR.getBegin(), Context.getSourceManager(), Context.getLangOpts());
					bool IsDependencyProperty = MSPDSource.Contains(u8"DependencyProperty"V);
					bool IsProxyProperty = MSPDSource.Contains(u8"ProxyProperty"V);
					bool IsAttachedProperty = MSPDSource.Contains(u8"AttachedProperty"V);
					if (IsDependencyProperty || IsProxyProperty || IsAttachedProperty)
						ProxyMSPDs.Add(MSPD);

					//FString OnChangedMethodName = u8"On{}Changed"V.Format(PropertyName);
					//clang::CXXMethodDecl * OnChangedMethod = Utils::FindNamedDeclT<clang::CXXMethodDecl>(&CXXRD, OnChangedMethodName, clang::Decl::CXXMethod, false);
					//FString OnChangedMethodMemberName = OnChangedMethod ? Format(u8"&{0}::{1}"V, TypeName, OnChangedMethodName) : u8"nullptr"V;
					//FStringV OnChangedMethodMemberName = u8"nullptr"V;

					Builder.AppendLine(u8"struct {0}"V, PropertyIdentifier);
					FReflectionTabScope TabScope_Field { Builder, EReflectionScope::Declaration };

					if (PropertyMetadataDecl && MetadataItems.Size)
					{
						Builder.AppendLine(u8"struct FPropertyMetadata : {0}::{1}"V, TypeName, PropertyMetadataDecl->getName());
						{
							FReflectionTabScope TabScope_Metadata { Builder, EReflectionScope::Declaration };
							{
								Builder.AppendLine(u8"FPropertyMetadata()"V);
								{
									FReflectionTabScope TabScope_MetadataConstructor { Builder, EReflectionScope::Scope };
									{
										for (const TPair<FString, FString> & MetadataItem : MetadataItems)
											Builder.AppendLine(u8"{0} = {1};"V, MetadataItem.First, MetadataItem.Second);
									}
								}
							}
						}
						Builder.AppendLine(u8"static inline const FPropertyMetadata Metadata;"V);
					}

					if (IsDependencyProperty)
					{
						FString LocalValue = u8"&{0}::__{1}"V.Format(TypeName, PropertyName);
						FString DefaultValue = u8"{0}::__{1}__"V.Format(TypeName, PropertyName);
						FString GetterMember = GetterMethod ? (u8"&{0}::{1}"V.Format(TypeName, GetterMethod->getName())) : u8"nullptr"V;
						FString SetterMember = SetterMethod ? (u8"&{0}::{1}"V.Format(TypeName, SetterMethod->getName())) : u8"nullptr"V;

						Builder.AppendLine(u8R"(static inline const TDependencyProperty Property {{ u8"{0}"N, {1}, {2}, {3}, {4}, {5} }};)"V
							, PropertyName, LocalValue, GetterMember, SetterMember, DefaultValue, MetadataIdentifier);
					}
					else if (IsAttachedProperty)
					{
						FString AttachedValue = u8"{0}::__{1}__"V.Format(TypeName, PropertyName);

						Builder.AppendLine(u8R"(static inline const TAttachedProperty<{0}, {1}> Property {{ u8"{2}"N, {3}, {4} }};)"V,
							TypeName, PropertyTypeSignature,
							PropertyName, AttachedValue, MetadataIdentifier);
					}
					else if (IsProxyProperty)
					{
						FString FieldMember = u8"&{0}::__{1}"V.Format(TypeName, PropertyName);
						FString GetterMember = GetterMethod ? (u8"&{0}::{1}"V.Format(TypeName, GetterMethod->getName())) : u8"nullptr"V;
						FString SetterMember = SetterMethod ? (u8"&{0}::{1}"V.Format(TypeName, SetterMethod->getName())) : u8"nullptr"V;

						Builder.AppendLine(u8R"(static inline const TProxyProperty Property {{ u8"{0}"N, {1}, {2}, {3}, {4} }};)"V
							, PropertyName, FieldMember, GetterMember, SetterMember, MetadataIdentifier);
					}
					else
					{
						FString GetterMember = GetterMethod ? (u8"&{0}::{1}"V.Format(TypeName, GetterMethod->getName())) : u8"nullptr"V;
						FString SetterMember = SetterMethod ? (u8"&{0}::{1}"V.Format(TypeName, SetterMethod->getName())) : u8"nullptr"V;

						Builder.AppendLine(u8R"(static inline const TFunctorProperty Property {{ u8"{0}"N, {1}, {2}, {3} }};)"V
							,PropertyName, GetterMember, SetterMember, MetadataIdentifier);
					}
					//Builder.AppendLine();
					Properties.Add(PropertyIdentifier);
				}

				// ReflectedProperties
				if (Properties.Size)
				{
					Builder.AppendLine(u8"static inline const TReferenceWrapper<const FProperty> ReflectedProperties[] ="V);
					{
						FReflectionTabScope TabScope_ReflectedProperties { Builder, EReflectionScope::Declaration };

						for (sizet PropertyIndex = 0; PropertyIndex < Properties.Size; ++PropertyIndex)
						{
							Builder.AppendLine({ Properties[PropertyIndex], u8"::Property,"V });
						}
					}
				}
			}

			// Base Types --------------------------------------------------------------------
			Builder.AppendLine();
			Builder.AppendLine(u8"// --- 4 Base Types"V);
			if (PublicBaseQTypes.Size)
			{
				Builder.AppendLine(u8"static inline const TReferenceWrapper<const FClass> ReflectedBaseTypes[] ="V);
				{
					FReflectionTabScope TabScope_ReflectedBaseTypes { Builder, EReflectionScope::Declaration };
					for (uintx BaseTypeIndex = 0; BaseTypeIndex < PublicBaseQTypes.Size; ++BaseTypeIndex)
					{
						clang::QualType BaseQType = PublicBaseQTypes[BaseTypeIndex];
						FString BaseTypeName = NameContext.GenerateQualTypeName(BaseQType);
						Builder.AppendLine({ u8"Classof<"V, BaseTypeName, u8">(),"V });
					}
				}
			}

			// Constructor--------------------------------------------------------------------
			Builder.AppendLine();
			Builder.AppendLine(u8"// --- 5 Constructor"V);
			if (CXXRD.getDescribedClassTemplate())
				Builder.AppendLine(u8R"({0}_Type(FName Name = u8"{0}"N) : TClass<{0}>(Name, {1}))"V, TypeName, BaseType0Expr);
			else
				Builder.AppendLine(u8R"({0}_Type(FName Name = u8"{0}"N) : TClass(Name, {1}))"V, TypeName, BaseType0Expr);
			{
				FReflectionTabScope TabScope_Constructor { Builder, EReflectionScope::Scope };
				if (Constructors.Size)
					Builder.AppendLine(u8"FClass::Constructors = ReflectedConstructors;"V);
				if (Fields.Size)
					Builder.AppendLine(u8"FClass::Fields = ReflectedFields;"V);
				if (Methods.Size)
					Builder.AppendLine(u8"FClass::Methods = ReflectedMethods;"V);
				if (Properties.Size)
					Builder.AppendLine(u8"FClass::Properties = ReflectedProperties;"V);
				if (PublicBaseQTypes.Size)
					Builder.AppendLine(u8"FClass::BaseTypes = ReflectedBaseTypes;"V);
			}
		}

		// Dependency Properties
		if (ProxyMSPDs.Size)
		{
			if (CTD)
			{
				for (uintx ProxyMSPDIndex = 0; ProxyMSPDIndex < ProxyMSPDs.Size; ++ProxyMSPDIndex)
				{
					clang::MSPropertyDecl * MSPD = ProxyMSPDs[ProxyMSPDIndex];
					FStringV PropertyName = MSPD->getName();
					FString PropertyTypeName = NameContext.GenerateQualTypeName(MSPD->getType());

					Builder.AppendLine();
					Builder.AppendLine(u8"template<{}>"V, TemplateDescriptors);
					//const TProperty<T> & TDoubleAnimation<T>::FromProperty() { return TDoubleAnimation_Type<T>::Property___From::Property; }
					Builder.AppendLine(u8"inline const TProperty<{3}> & {1}<{4}>::{2}Property() {{ return {0}_Type<{3}>::Property___{2}::Property; }}"V, TypeName, QualifiedTypeName, PropertyName, PropertyTypeName, TemplateArguments);
				}
			}
			else
			{
				for (clang::MSPropertyDecl * MSPD : ProxyMSPDs)
				{
					FStringV PropertyName = MSPD->getName();
					FString PropertyTypeName = NameContext.GenerateQualTypeName(MSPD->getType());

					Builder.AppendLine();
					//inline const TProperty<uint32> & FTimeline::StartProperty() { return FTimeline_Type::Property___Start::Property; }
					Builder.AppendLine(u8"const TProperty<{3}> & {1}::{2}Property() {{ return {0}_Type::Property___{2}::Property; }}"V, TypeName, QualifiedTypeName, PropertyName, PropertyTypeName);
				}
			}
		}

		if (CTD)
			NameContext.TemplateDecls.PopBack();
	}

	void FEDGenerator::GenerateED(FReflectBuilder & Builder, FNameContext & NameContext, clang::EnumDecl & ED)
	{
		clang::ASTContext & Context = ED.getASTContext();
		std::string CXXRDQualName = ED.getQualifiedNameAsString();

		FStringV TypeName = ED.getName();
		FString QualifiedPrefix = Utils::GetRelativeQualifiedPrefix(ED.getDeclContext(), NameContext.ReferenceDC);
		FString QualifiedTypeName = QualifiedPrefix ? (QualifiedPrefix + u8"::"V + TypeName) : TypeName;

		clang::XinMetaAttr * XinAttr = Utils::FindXinAttr(&ED);

		Builder.AppendLine(u8"class {0}_Type : public TEnum<{1}>"V, TypeName, QualifiedTypeName);
		{
			FReflectionScope Scope_Class { Builder, EReflectionScope::Declaration };
			Builder.AppendLine(u8"public:"V);
			FReflectionTab Tab_Class_Public { Builder };

			// Fields--------------------------------------------------------------------
			TList<FString> Items;
			Builder.AppendLine(u8"// --- 1 Items"V);
			if constexpr (true)
			{
				// --- field
				auto GenerateItemLambda = [&, this](this auto & Self, clang::EnumConstantDecl * ECD)
					{
						if (ECD->getName().starts_with("__"))
							return;

						FStringV ItemName = ECD->getName();
						FString ItemStructName = u8"Item___{0}"V.Format(ItemName);

						Builder.AppendLine(u8"struct {0}"V, ItemStructName);
						{
							FReflectionTabScope TabScope_Field { Builder, EReflectionScope::Declaration };

							//Builder.AppendLine(u8"// {0}"V, ItemName);
							Builder.AppendLine(u8"static inline const TEnumItem Item {{ u8\"{0}\"N, {1}::{0}, FMetadata::None }};"V, ItemName, QualifiedTypeName);
						}
						//Builder.AppendLine();
						Items.Add(ItemStructName);
					};

				for (clang::EnumConstantDecl * ECD : ED.enumerators())
				{
					GenerateItemLambda(ECD);
				}

				// ReflectedFields
				if (Items.Size)
				{
					Builder.AppendLine(u8"static inline const TReferenceWrapper<const FEnumItem> ReflectedItems[] ="V);
					{
						FReflectionTabScope TabScope_ReflectedFields { Builder, EReflectionScope::Declaration };

						for (sizet FieldIndex = 0; FieldIndex < Items.Size; ++FieldIndex)
						{
							Builder.AppendLine({ Items[FieldIndex], u8"::Item,"V });
						}
					}
				}
			}

			// Constructor--------------------------------------------------------------------
			Builder.AppendLine();
			Builder.AppendLine(u8"// --- 2 Constructor"V);
			Builder.AppendLine(u8R"({0}_Type(FName Name = u8"{0}"N) : TEnum(Name, FMetadata::None))"V, TypeName);
			{
				FReflectionTabScope TabScope_Constructor { Builder, EReflectionScope::Scope };
				if (Items.Size)
					Builder.AppendLine(u8"FEnum::Items = ReflectedItems;"V);
			}
		}
	}

	void FActionSink::OnParsed()
	{
		if (BuiltinTypes.Empty() && EnumDecls.Empty() && CXXRecordDecls.Empty())
		{
			//FConsole::WriteLine(u8"nothing reflected"V);
			if (FFile::Exists(ReflectedFilePath))
				FFile::Delete(ReflectedFilePath);
			return;
		}

		Sinked = true;
		FDirectory::EnsureExists(FPath::GetDirectory(ReflectedFilePath));

		clang::NamespaceDecl * Namespace;
		if (CXXRecordDecls.Size)
		{
			clang::DeclContext * DC = CXXRecordDecls[0]->getDeclContext();
			assert(DC->getDeclKind() == clang::Decl::Namespace);
			Namespace = dyn_cast<clang::NamespaceDecl>(DC);
		}
		else if (EnumDecls.Size)
		{
			clang::DeclContext * DC = EnumDecls[0]->getDeclContext();
			assert(DC->getDeclKind() == clang::Decl::Namespace);
			Namespace = dyn_cast<clang::NamespaceDecl>(DC);
		}
		else if (BuiltinTypes.Size)
		{
			AssertExpr(TypeofSpecialFDs.Size > 0);
			clang::DeclContext * DC = TypeofSpecialFDs[0]->getDeclContext();
			assert(DC->getDeclKind() == clang::Decl::Namespace);
			Namespace = dyn_cast<clang::NamespaceDecl>(DC);
		}
		else
			return;

		if (!Namespace)
		{
			LogWarning(u8"Reflect type need a namespace."V);
			return;
		}

		FReflectBuilder Builder { ReflectSettings.LineEnd, ReflectSettings.TabChar };
		Builder.AppendLine(u8"// Generated code begin."V);
		Builder.AppendLine();
		Builder.AppendLine(u8"#pragma once"V);
		if (ReflectSettings.PCHIncludeFile.Length)
			Builder.AppendLine(u8R"(#include "{0}")"V, ReflectSettings.PCHIncludeFile);
		Builder.AppendLine(u8R"(#include "{0}")"V, IncludeFilePath);
		Builder.AppendLine();

		FString NamespaceName = Utils::GetNamespaceName(Namespace);
		Builder.AppendLine(u8"namespace {0}"V, FStringV(XinNamespace->getName()));
		{
			FReflectionTabScope TabScope_Namespace(Builder, EReflectionScope::Scope);

			sizet ReflectedIndex = 0;
			FNameContext NameContext;
			NameContext.ReferenceDC = XinNamespace;

			// 0 -------------------- BuiltinTypes
			for (sizet BuiltinTypeIndex = 0; BuiltinTypeIndex < BuiltinTypes.Size; ++BuiltinTypeIndex, ++ReflectedIndex)
			{
				const clang::BuiltinType * BType = BuiltinTypes[BuiltinTypeIndex];
				clang::BuiltinType::Kind BuiltinKind = BType->getKind();
				FStringV TypeName = Utils::GetCXXBuiltinTypeName(BuiltinKind);

				if (ReflectedIndex > 0)
					Builder.AppendLine();

				Builder.AppendLine(u8"// -------------------------------- {0} --------------------------------"V, TypeName);
				// class FType_bool
				Builder.AppendLine(u8"class {0}_Type : public TBuiltinType<{0}>"V, TypeName);
				{
					FReflectionScope Scope_Type { Builder, EReflectionScope::Declaration };
					Builder.AppendLine(u8"public:"V);
					{
						FReflectionTab Tab_Class_Public { Builder };
						Builder.AppendLine(u8R"({0}_Type() : TBuiltinType<{0}>(u8"{0}"N) {{ }})"V, TypeName);
					}
				}

				// TStaticType<T>::Type()
				Builder.AppendLine();
				Builder.AppendLine(u8"template<> struct XIN_EXPORT TStaticType<{0}>"V, TypeName);
				{
					FReflectionTabScope TabScope_TStaticType { Builder, EReflectionScope::Declaration };
					Builder.AppendLine(u8"static FType & Type()"V);
					{
						FReflectionTabScope TabScope_StaticType { Builder, EReflectionScope::Scope };
						Builder.AppendLine(u8"static {0}_Type {0}Type;"V, TypeName);
						Builder.AppendLine(u8"return {0}Type;", TypeName);
					}
				}

				// FType & Typeof<T>()
				Builder.AppendLine();
				Builder.AppendLine(u8"template<> XIN_EXPORT inline FType & Typeof<{0}>()"V, TypeName);
				{
					FReflectionTabScope TabScope_StaticType { Builder, EReflectionScope::Scope };
					Builder.AppendLine(u8"return TStaticType<{0}>().Type();"V, TypeName);
				}

				//static TTypeRegister<T> TypeRegister;
				Builder.AppendLine();
				Builder.AppendLine(u8"static TTypeRegister<{0}> {0}TypeRegister {{ TStaticType<{0}>().Type() }};"V, TypeName);
			}

			// 1 -------------------- EnumDecls
			for (sizet ReflectedEDIndex = 0; ReflectedEDIndex < EnumDecls.Size; ++ReflectedEDIndex, ++ReflectedIndex)
			{
				if (ReflectedIndex > 0)
					Builder.AppendLine();

				clang::EnumDecl * ED = EnumDecls[ReflectedEDIndex];
				FStringV TypeName = ED->getName();
				FString QualifiedPrefix = Utils::GetRelativeQualifiedPrefix(ED->getDeclContext(), XinNamespace);
				FString QualifiedTypeName = QualifiedPrefix ? (QualifiedPrefix + u8"::"V + TypeName) : TypeName;

				FEDGenerator Generator { *this };
				Generator.GenerateED(Builder, NameContext, *ED);

				// TStaticType<T>::Type()
				Builder.AppendLine();
				Builder.AppendLine(u8"template<> struct XIN_EXPORT TStaticType<{0}>"V, QualifiedTypeName);
				{
					FReflectionTabScope TabScope_TStaticType { Builder, EReflectionScope::Declaration };

					Builder.AppendLine(u8"static inline {0}_Type {0}Type {{ u8\"{0}\"N }};"V, TypeName);
					Builder.AppendLine();

					Builder.AppendLine(u8"static FType & Type()"V);
					{
						FReflectionTabScope TabScope_StaticType { Builder, EReflectionScope::Scope };
						Builder.AppendLine(u8"return {0}Type;", TypeName);
					}
				}

				// FType & Typeof<T>()
				Builder.AppendLine();
				Builder.AppendLine(u8"template<> XIN_EXPORT inline FType & Typeof<{0}>()"V, QualifiedTypeName);
				{
					FReflectionTabScope TabScope_StaticType { Builder, EReflectionScope::Scope };
					Builder.AppendLine(u8"return TStaticType<{0}>().Type();"V, QualifiedTypeName);
				}

				//static TTypeRegister<T> TypeRegister;
				Builder.AppendLine();
				Builder.AppendLine(u8"static TTypeRegister<{1}> {0}TypeRegister {{ TStaticType<{1}>().Type() }};"V, TypeName, QualifiedTypeName);
			}

			// 2 -------------------- CXXRecordDecls
			for (sizet ReflectedCXXRDIndex = 0; ReflectedCXXRDIndex < CXXRecordDecls.Size; ++ReflectedCXXRDIndex, ++ReflectedIndex)
			{
				if (ReflectedIndex > 0)
					Builder.AppendLine();

				clang::CXXRecordDecl * CXXRD = CXXRecordDecls[ReflectedCXXRDIndex];
				clang::Decl::Kind CXXRDKind = CXXRD->getKind();
				FStringV TypeName = CXXRD->getName();
				FString QualifiedPrefix = Utils::GetRelativeQualifiedPrefix(CXXRD->getDeclContext(), XinNamespace);
				FString QualifiedTypeName = QualifiedPrefix ? (QualifiedPrefix + u8"::"V + TypeName) : TypeName;

				FCXXRDGenerator Generator { *this };
				Generator.GenerateCXXRD(Builder, NameContext, *CXXRD);

				clang::ClassTemplateDecl * CTD = CXXRD->getDescribedClassTemplate();
				// TStaticType<T>::Type()
				if (CTD)
				{
					auto [TemplateParameters, TemplateArguments] = NameContext.GenerateTemplateDescriptorsAndParameters(CTD);
					// TStaticType<T>::Type()
					Builder.AppendLine();
					Builder.AppendLine(u8"template<{}>"V, TemplateParameters);
					Builder.AppendLine(u8"struct TStaticType<{0}<{1}>>"V, QualifiedTypeName, TemplateArguments);
					{
						FReflectionTabScope TabScope_TStaticType { Builder, EReflectionScope::Declaration };
						Builder.AppendLine(u8"static FType & Type()"V);
						{
							FReflectionTabScope TabScope_StaticType { Builder, EReflectionScope::Scope };
							Builder.AppendLine(u8"static {0}_Type<{1}> {0}Type {{ u8\"{0}\"N }};"V, TypeName, TemplateArguments);
							Builder.AppendLine(u8"return {0}Type;", TypeName);
						}
					}

					// FType & T::StaticType()
					if (StaticTypeCXXRDs.Contains(CXXRD))
					{
						Builder.AppendLine();
						// Template<T>::StaticType()
						Builder.AppendLine(u8"template<{}>"V, TemplateParameters);
						Builder.AppendLine(u8"FType & {0}<{1}>::StaticType()"V, QualifiedTypeName, TemplateArguments);
						{
							FReflectionTabScope TabScope_StaticType { Builder, EReflectionScope::Scope };
							Builder.AppendLine(u8"return TStaticType<{0}<{1}>>().Type();"V, QualifiedTypeName, TemplateArguments);
						}
					}
				}
				else
				{
					// TStaticType<T>::Type()
					Builder.AppendLine();
					Builder.AppendLine(u8"template<> struct XIN_EXPORT TStaticType<{0}>"V, QualifiedTypeName);
					{
						FReflectionTabScope TabScope_TStaticType { Builder, EReflectionScope::Declaration };

						Builder.AppendLine(u8"static inline {0}_Type {0}Type {{ u8\"{0}\"N }};"V, TypeName);
						Builder.AppendLine();

						Builder.AppendLine(u8"static FType & Type()"V);
						{
							FReflectionTabScope TabScope_StaticType { Builder, EReflectionScope::Scope };
							Builder.AppendLine(u8"return {0}Type;", TypeName);
						}
					}

					if (StaticTypeCXXRDs.Contains(CXXRD))
					{
						// FType & T::StaticType()
						Builder.AppendLine();
						Builder.AppendLine(u8"FType & {0}::StaticType()"V, QualifiedTypeName);
						{
							FReflectionTabScope TabScope_StaticType { Builder, EReflectionScope::Scope };
							Builder.AppendLine(u8"return TStaticType<{0}>().Type();"V, QualifiedTypeName);
						}
					}
					else
					{
						// FType & Typeof<T>()
						Builder.AppendLine();
						Builder.AppendLine(u8"template<> XIN_EXPORT inline FType & Typeof<{0}>()"V, QualifiedTypeName);
						{
							FReflectionTabScope TabScope_StaticType { Builder, EReflectionScope::Scope };
							Builder.AppendLine(u8"return TStaticType<{0}>().Type();"V, QualifiedTypeName);
						}
					}

					//static TTypeRegister<T> TypeRegister
					Builder.AppendLine();
					Builder.AppendLine(u8"static TTypeRegister<{1}> {0}TypeRegister {{ TStaticType<{1}>().Type() }};"V, TypeName, QualifiedTypeName);
				}
			}
		}

		Builder.AppendLine();
		Builder.AppendLine(u8"// Generated code end."V);

		Utils::SaveFileWithMD5(ReflectSettings, ReflectedFilePath, Builder.Result);
	}
}