using System.IO;
using ProjectModel;

namespace Xin;

public class Module : IModule
{
    public Module()
    {
        Type = ModuleType.Application;
        PublicDependencyModules.AddRange(new[] { "01.Test.Graphics", "Skia" });
        
        PrivateIncludePaths.Add(Path.Combine("Externals\\harfbuzz\\src\\"));
        PrivateIncludePaths.Add(Path.Combine("Externals\\icu\\include\\"));
        PrivateIncludePaths.Add(Path.Combine("Externals\\skia\\"));

        //PrivateLibraries.Add("..\\..\\Externals\\skia\\bin\\x64\\skia.dll.lib");
        //PrivateLibraries.Add("..\\..\\Externals\\harfbuzz\\bin\\x64\\harfbuzz.lib");
        //PrivateLibraries.Add("..\\..\\Externals\\icu\\bin\\x64\\icuuc.lib");
    }
}
