using ProjectModel;

namespace Xin;

public class Module : IModule
{
    public Module()
    {
        Type = ModuleType.StaticLibrary;
        Files.AddRange(new string[] {"*.h", "*.cpp", "*.cc"});
    }
}
