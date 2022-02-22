using ProjectModel;

namespace Xin;

public class Module : IModule
{
    public Module()
    {
        Type = ModuleType.DynamicLibrary;
        PublicDependencyModules.AddRange(new[] { "Base" });
        
        PrivateIncludePaths.Add("Externals\\phnt\\include");

        PrivateLibraries.Add("Externals\\FreeImage\\Lib\\x64\\FreeImage.lib");

        Debug_x64.PrivateLibraries.Add("Externals\\astc-encoder\\Lib\\x64\\debug\\astcenc-native-static.lib");
        Release_x64.PrivateLibraries.Add("Externals\\astc-encoder\\Lib\\x64\\release\\astcenc-native-static.lib");

        Debug_x64.PrivateLibraries.Add(@"Externals\DirectXTex\Lib\x64\debug\DirectXTex.lib");
        Release_x64.PrivateLibraries.Add(@"Externals\DirectXTex\Lib\x64\release\DirectXTex.lib");
    }
}
