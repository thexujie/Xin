using System.IO;
using ProjectModel;

namespace Xin;

/**
 * build llvm with -DLLVM_ENABLE_PROJECTS="clang"
 */
public class Module : IModule
{
    private readonly string LLVMRoot = @"H:\Projs\llvm-project\";
    private readonly string LLVMBuildRoot = @"H:\Projs\llvm-project\build_x64\";

    public Module()
    {
        Type = ModuleType.DynamicLibrary;
        //PrivateDependencyModules.AddRange(new[] { "zlib", "zstd" });
        PublicDependencyModules.AddRange(new[] { "Base" });

        AdditionalFiles.Add(Path.Combine(LLVMRoot, "llvm\\utils\\LLVMVisualizers\\llvm.natvis"));
        AdditionalFiles.Add(Path.Combine(LLVMRoot, "clang\\utils\\ClangVisualizers\\clang.natvis"));

        PrivateIncludePaths.Add(Path.Combine(LLVMRoot, "llvm\\include"));
        PrivateIncludePaths.Add(Path.Combine(LLVMRoot, "clang\\include"));
        PrivateIncludePaths.Add(Path.Combine(LLVMBuildRoot, "include"));
        PrivateIncludePaths.Add(Path.Combine(LLVMBuildRoot, "tools\\clang\\include"));
        
        Debug_x64.PrivateLibraryPaths.Add(Path.Combine(LLVMBuildRoot, "Debug\\lib"));
        Release_x64.PrivateLibraryPaths.Add(Path.Combine(LLVMBuildRoot, "RelWithDebInfo\\lib"));
    }
}
