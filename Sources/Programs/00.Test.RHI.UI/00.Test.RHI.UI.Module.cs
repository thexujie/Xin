using ProjectModel;

namespace Xin;

public class Module : IModule
{
    public Module()
    {
        Type = ModuleType.Application;

        PublicDependencyModules.AddRange(new[] { "Core", "RHI", "Rendering", "RHI.UI" });

        PublicDependencyModules.AddRange(new[] { "Skia", "Direct2D"});

        PublicDependencyModules.AddRange(new[] { "RHI.Vulkan", "RHI.D3D12"});
    }
}
