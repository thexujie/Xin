#pragma once

#include "Xin.ClangParser.Types.h"
#include "LLVMIncludes.h"

namespace Xin
{
	struct FReflectionMacroAttribute
	{
		FString Name;
		FString Argument;
		TList<FString> Args;
		TList<clang::tok::TokenKind> ArgKinds;
	};

	class IActionSink : public ISharedRefer
	{
	public:
		IActionSink(FReflectSettings & ReflectSettings) : ReflectSettings(ReflectSettings) {}

	public:
		virtual void OnParsed() = 0;

	public:
		clang::TypeAliasDecl * FindTypeAlias(clang::CXXRecordDecl * CXXRD);

	public:
		FReflectSettings & ReflectSettings;

	public:
		TList<const clang::BuiltinType *> BuiltinTypes;
		TList<clang::EnumDecl *> EnumDecls;
		TList<clang::CXXRecordDecl *> CXXRecordDecls;

		clang::NamespaceDecl * XinNamespace = nullptr;
		// Typeof<T>
		clang::FunctionTemplateDecl * TypeofFTD = nullptr;
		TList<clang::FunctionDecl *> TypeofSpecialFDs;
		TList<clang::TypeDecl *> TypeofCXXRDs;

		// T::StaticType()
		TList<clang::CXXRecordDecl *> StaticTypeCXXRDs;

		TList<clang::TypeAliasDecl *> TADs;
	};

	bool RunPCHAction(FStringV FilePath, FStringV PCHFilePath, FReflectSettings & ReflectSettings);
	bool RunASTAction(FStringV FilePath, FStringV PCHFilePath, IActionSink & ActionSink);
	bool RunASTAction(TView<FStringV> FilePaths, FStringV PCHFilePath, IActionSink & ActionSink);
}
