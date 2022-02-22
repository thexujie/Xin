using ProjectModel;

namespace Xin;

public class Module : IModule
{
    public Module()
    {
        Type = ModuleType.StaticLibrary;
        PublicDependencyModules.AddRange(new[] { "Xin.Core", "Xin.RHI", "Xin.Rendering", "Xin.ShaderCompiler" });

        PublicDependencyModules.AddRange(new[] { "Xin.FBX" });
        AdditionalFiles.Add("Assets/*");
    }
}
