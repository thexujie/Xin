using ProjectModel;

namespace Xin;

public class Module : IModule
{
    public Module()
    {
        Type = ModuleType.DynamicLibrary;
        SubType = ModuleSubType.NativeUnitTestProject;
        PrivateIncludePaths.Add("$(VCInstallDir)UnitTest\\include");
        PrivateLibraryPaths.Add("$(VCInstallDir)UnitTest\\lib");
        PublicDependencyModules.AddRange(new[] { "Xin.Core", });
    }
}
