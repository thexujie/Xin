#pragma once

#include "Xin.ClangParser.Types.h"
#include "LLVMIncludes.h"

namespace Xin::Utils
{
	clang::QualType GetSpecializationT0QType(clang::FunctionDecl * SpecialFD);
	FString GetNamespaceName(const clang::DeclContext * DC);
	FString GetRelativeDeclContext(const clang::DeclContext * DC, const clang::DeclContext * ReferenceDC);
	FString GetRelativeQualifiedPrefix(const clang::DeclContext * DC, const clang::DeclContext * ReferenceDC);
	//FString GetRelativeQualifiedDeclName(const clang::DeclContext * DC, clang::Decl * D);

	bool IsAdapterBuiltinType(clang::TypedefNameDecl * TDND);
	FStringV GetCXXBuiltinTypeName(clang::BuiltinType::Kind Kind);

	bool SaveFileWithMD5(FReflectSettings & ReflectSettings, FStringV FilePath, FStringV FileConent);

	clang::XinMetaAttr * FindXinAttr(clang::Decl * D);
	clang::UnresolvedLookupExpr * FindAttrArg(clang::XinMetaAttr * XinAttr, FStringV ArgName);
	clang::UnresolvedLookupExpr * FindXinAttribute(clang::Decl * D, FStringV Name);

	bool IsCXXRecordCopyConstructable(clang::CXXRecordDecl * CXXRD);
	bool IsCXXRecordCopyAssignable(clang::CXXRecordDecl * CXXRD);

	clang::NamedDecl * FindNamedDecl(clang::DeclContext * DC, FStringV Name);
	clang::NamedDecl * FindNamedDecl(clang::DeclContext * DC, FStringV Name, clang::Decl::Kind Kind, bool FindInBaseDecl = true);

	template<typename DeclT>
	DeclT * FindNamedDeclT(clang::DeclContext * DC, FStringV Name, clang::Decl::Kind Kind, bool FindInBaseDecl = true)
	{
		if (clang::NamedDecl * ND = FindNamedDecl(DC, Name, Kind, FindInBaseDecl))
			return dyn_cast<DeclT>(ND);
		return nullptr;
	}
}

// namespace

