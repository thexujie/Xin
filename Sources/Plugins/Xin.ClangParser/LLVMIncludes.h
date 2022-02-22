#pragma once

#include "Xin.ClangParser.Types.h"

#pragma warning(push)
#pragma warning(disable: 4189 4702 4324 4624 4267 4146 4245 4127 4996 4244 4291 4456 4389 4459 4819)

#define CLANG_BUILD_STATIC 1

#include "clang/Lex/Lexer.h"
#include "clang/Lex/MacroArgs.h"
#include "clang/Lex/PreprocessorOptions.h"

#include "clang/Parse/Parser.h"

#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/AST/Attr.h"

#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Frontend/MultiplexConsumer.h"
#include "clang/Frontend/TextDiagnosticPrinter.h"
#include "clang/Serialization/ASTWriter.h"

#include "clang/Driver/Compilation.h"
#include "clang/Driver/Driver.h"
#include "clang/Driver/Job.h"
#include "clang/Driver/Options.h"
#include "clang/Driver/Tool.h"

#include "llvm/IRReader/IRReader.h"
#include "llvm/TargetParser/Host.h"
#include "llvm/Option/OptTable.h"
//#include "clang/Tooling/Tooling.h"

#pragma warning(pop)


namespace Xin
{
	template<>
	struct TAdapter<llvm::StringRef, FStringV>
	{
		FStringV operator()(const llvm::StringRef & LLVMString)
		{
			return { (const char8 *)LLVMString.data(), LLVMString.size() };
		}
	};
	static_assert(IsAdapterAbleV<llvm::StringRef, FStringV>);
	static_assert(IsAdapterFormatAbleV<llvm::StringRef>);

	template<typename T>
	struct TAdapter<llvm::ArrayRef<T>, TView<T>>
	{
		TView<T> operator()(llvm::ArrayRef<T> LLVMArrayRef)
		{
			return { LLVMArrayRef.data(), LLVMArrayRef.size() };
		}
	};

	class FRawStringOstream : public llvm::raw_ostream {
		FString & OS;
		void write_impl(const char * Ptr, size_t Size) override
		{
			OS.Append((const char8 *)Ptr, Size);
		}

		uint64_t current_pos() const override { return OS.Length; }

	public:
		explicit FRawStringOstream(FString & OS) : OS(OS) {
			SetUnbuffered();
		}

		void reserveExtraSpace(uint64_t ExtraSize) override {
			//OS.reserve(tell() + ExtraSize);
			throw 0;
		}
	};
}
