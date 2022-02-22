using ProjectModel;

namespace Xin;

public class Module : IModule
{
    public Module()
    {
        Type = ModuleType.Makefile;
        Files.AddRange( new []
        {
            "Xin.natvis",
            "D3D12.natvis",
            "fbxsdk.natvis",
        });
    }
}
