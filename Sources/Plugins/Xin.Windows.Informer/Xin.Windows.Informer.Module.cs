using ProjectModel;
using System.IO;

namespace Xin;

public class Module : IModule
{
    public Module()
    {
        Type = ModuleType.DynamicLibrary;
        PublicDependencyModules.AddRange(new[] { "Core" });

        PrivateIncludePaths.Add(Path.Combine("Externals\\phnt\\include"));
    }
}
