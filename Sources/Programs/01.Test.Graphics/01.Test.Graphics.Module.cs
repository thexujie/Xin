using System.IO;
using ProjectModel;

namespace Xin;

public class Module : IModule
{
    public Module()
    {
        Type = ModuleType.StaticLibrary;
        PublicDependencyModules.AddRange(new[] { "Core" });
    }
}
