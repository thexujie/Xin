using ProjectModel;

namespace Xin;

public class Module : IModule
{
    public Module()
    {
        Type = ModuleType.DynamicLibrary;
        PublicDependencyModules.AddRange(new[] { "Core" });
        PrivateIncludePaths.Add("Externals/fbxsdk/include");
        
        Debug_x64.PrivateLibraries.Add("Externals/fbxsdk/lib/x64/debug/libfbxsdk-md.lib");
        Debug_x64.PrivateLibraries.Add("Externals/fbxsdk/lib/x64/debug/libxml2-md.lib");
        Debug_x64.PrivateLibraries.Add("Externals/fbxsdk/lib/x64/debug/zlib-md.lib");
        
        Release_x64.PrivateLibraries.Add("Externals/fbxsdk/lib/x64/release/libfbxsdk-md.lib");
        Release_x64.PrivateLibraries.Add("Externals/fbxsdk/lib/x64/release/libxml2-md.lib");
        Release_x64.PrivateLibraries.Add("Externals/fbxsdk/lib/x64/release/zlib-md.lib");
    }
}
