using System.IO;
using ProjectModel;

namespace Xin;

public class Module : IModule
{
    public Module()
    {
        Type = ModuleType.DynamicLibrary;
        PublicDependencyModules.AddRange(new[] { "Core", "Windows" });
        
        PrivateIncludePaths.Add(Path.Combine("Externals\\harfbuzz\\src\\"));
        PrivateIncludePaths.Add(Path.Combine("Externals\\icu\\include\\"));

        PrivateIncludePaths.Add(Path.Combine("Externals\\skia\\"));

        Debug_x64.PrivateLibraries.Add("..\\..\\Externals\\skia\\bin\\x64d\\skia.dll.lib");
        Debug_x64.PrivateLibraries.Add("..\\..\\Externals\\harfbuzz\\bin\\x64d\\harfbuzz.lib");
        Debug_x64.PrivateLibraries.Add("..\\..\\Externals\\icu\\bin\\x64d\\icudt.lib");
        Debug_x64.PrivateLibraries.Add("..\\..\\Externals\\icu\\bin\\x64d\\icuucd.lib");
        
        Release_x64.PrivateLibraries.Add("..\\..\\Externals\\skia\\bin\\x64\\skia.dll.lib");
        Release_x64.PrivateLibraries.Add("..\\..\\Externals\\harfbuzz\\bin\\x64\\harfbuzz.lib");
        Release_x64.PrivateLibraries.Add("..\\..\\Externals\\icu\\bin\\x64\\icudt.lib");
        Release_x64.PrivateLibraries.Add("..\\..\\Externals\\icu\\bin\\x64\\icuuc.lib");
    }
}
