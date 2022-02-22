using ProjectModel;

namespace Xin;

public class Module : IModule
{
    public Module()
    {
        Type = ModuleType.DynamicLibrary;
        PrivateDependencyModules.AddRange(new[] { "cityhash" });
    }
}
