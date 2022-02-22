using System.IO;
using ProjectModel;

namespace Xin;

public class Module : IModule
{
    public Module()
    {
        Type = ModuleType.Application;
        PublicDependencyModules.AddRange(new[] { "01.Test.Graphics", "Direct2D" });
    }
}
