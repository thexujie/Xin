using ProjectModel;

namespace Xin;

public class Module : IModule
{
    public Module()
    {
        Type = ModuleType.Application;

        PublicDependencyModules.AddRange(new[] { "Core", "UI", "Direct2D", "Skia" });
        PublicDependencyModules.Add("00.Test.UI");
    }
}
