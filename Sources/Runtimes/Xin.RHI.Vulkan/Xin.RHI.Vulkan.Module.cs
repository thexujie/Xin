using System.IO;
using ProjectModel;

namespace Xin;

public class Module : IModule
{
    private readonly string VulkanSDKRoot = "D:/Files/VulkanSDK/1.4.309.0/";

    public Module()
    {
        Type = ModuleType.DynamicLibrary;
        PrivateDependencyModules.Add("volk");

        PublicDependencyModules.AddRange(new[] { "Core", "Windows", "RHI", "ShaderCompiler" });

        //AdditionalFiles.Add(Path.Combine(LLVMRoot, "llvm\\utils\\LLVMVisualizers\\llvm.natvis"));
        //AdditionalFiles.Add(Path.Combine(LLVMRoot, "clang\\utils\\ClangVisualizers\\clang.natvis"));

        PrivateIncludePaths.Add(Path.Combine(VulkanSDKRoot, "Include"));
        PrivateLibraryPaths.Add(Path.Combine(VulkanSDKRoot, "Lib"));
		
		
        PrivateLibraries.Add("vulkan-1.lib");
    }
}
