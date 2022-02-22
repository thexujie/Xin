#pragma once

#include "LLVMIncludes.h"
#include "Visitor.h"

namespace Xin
{
	class FReflectBuilder
	{
	public:
		FReflectBuilder(FStringV LineEnd, FStringV Tab);

		void AppendLine(uintx LineCount = 1);
		void AppendLine(TView<FStringV> Contents);
		void AppendLine(FStringV Content);

		template<FormatAbleC ...ArgTs>
		void AppendLine(FStringV Formal, const ArgTs & ...Args)
		{
			AppendLine(Format(Formal, Args...));
		}

	public:
		void BeginNamespace(FStringV Namespace);
		void EndNamespace();

	public:
		FString Result;

		FString LineEnd;
		FString LineEndx16;

		uintx TabLevel = 0;
		FString Tab;
		FString Tabx16;
	};

	enum class EReflectionScope
	{
		None = 0,
		Scope, // {}
		Declaration, // {};
	};

	class FReflectionScope
	{
	public:
		FReflectionScope(FReflectBuilder & Builder, EReflectionScope ReflectionScope);
		~FReflectionScope();

	private:
		FReflectBuilder & Builder;
		EReflectionScope ReflectionScopeType;
	};

	class FReflectionTab
	{
	public:
		FReflectionTab(FReflectBuilder & Builder);
		~FReflectionTab();

	private:
		FReflectBuilder & Builder;
	};

	class FReflectionTabScope
	{
	public:
		FReflectionTabScope(FReflectBuilder & Builder, EReflectionScope ReflectionScope);
		~FReflectionTabScope();

	private:
		FReflectBuilder & Builder;
		EReflectionScope ReflectionScope;
	};

	class FReflectionNamedStruct
	{
	public:
		FReflectionNamedStruct(FReflectBuilder & Builder, FStringV Name);
		~FReflectionNamedStruct();

	private:
		FReflectBuilder & Builder;
		FString Name;
	};

	class FNameContext
	{
	public:
		TList<TPair<FString, FString>> GenerateMetadata(clang::Decl & D);
		TPair<FString, FString> GenerateTemplateDescriptorsAndParameters(clang::TemplateDecl * TD);

		FString GenerateCXXRecordName(clang::NamedDecl * Decl);
		FString GenerateCXXRecordIdentifier(clang::CXXRecordDecl * CXXRD);

		FString GenerateQualTypeName(clang::QualType QType);
		FString GenerateQualTypeIdentifier(clang::QualType QType);

		FString GenerateCXXMethodParametersIdentifier(const clang::CXXMethodDecl & CXXMD);
		FString GenerateCXXMethodParametersSignature(const clang::CXXMethodDecl & CXXMD);
		FString GenerateCXXMethodSignature(const clang::CXXMethodDecl & CXXMD);

		FString GenerateCXXFunctorSignature(const clang::CXXMethodDecl & CXXMD);
		FString GenerateCXXMethodIdentifier(const clang::CXXMethodDecl & CXXMD);

		clang::NamedDecl * GetTemplateParam(unsigned Depth, unsigned Index)
		{
			return TemplateDecls[Depth]->getTemplateParameters()->getParam(Index);
		}
	public:
		TList<clang::TemplateDecl *> TemplateDecls;
		clang::DeclContext * ReferenceDC = nullptr;
	};

	class FActionSink;
	class FCXXRDGenerator
	{
	public:
		FCXXRDGenerator(FActionSink & ActionSink) : ActionSink(ActionSink) {}

		void GenerateCXXRD(FReflectBuilder & Builder, FNameContext & NameContext, clang::CXXRecordDecl & CXXRD);

	public:
		FActionSink & ActionSink;
	};

	class FEDGenerator
	{
	public:
		FEDGenerator(FActionSink & ActionSink) : ActionSink(ActionSink) {}

		void GenerateED(FReflectBuilder & Builder, FNameContext & NameContext, clang::EnumDecl & ED);

	public:
		FActionSink & ActionSink;
	};

	class FActionSink : public IActionSink
	{
	public:
		FActionSink(FReflectSettings & ReflectSettings) : IActionSink(ReflectSettings) {}

		void OnParsed() override;

	public:
		FString IncludeFilePath;
		FString ReflectedFilePath;
		bool Sinked = false;
	};
}
