#include "PCH.h"
#include <iostream>

#include "ProgramOptions.h"
#include "Xin.Base/Platform/Windows/WindowsHeader.h"

int wmain(int ArgumentCount, const wchar_t * Arguments[])
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#if !XIN_DEBUG
	if (ArgumentCount < 2)
		return 0;
#endif

	FConsole::SetEncoding(FEncoding::UTF8);

	FProgramOptionsParser ProgramOptionsParser(FEncoding::ToUtf8(FWStringV(::GetCommandLineW())));
	FStringV Target = ProgramOptionsParser.FindOption(FProgramOptionsParser::AppTarget);

#if XIN_DEBUG
	//Target = u8R"(H:\Projs\XinSamples\Xin\Sources\Runtimes\Xin.Core.Foundation\Basic\Name.h)"V;
	//Target = u8R"(H:\Projs\XinSamples\Xin\Sources\Runtimes\Xin.Core.Foundation\Basic\Name.h)"V;
	//Target = u8R"(H:\Projs\XinSamples\Xin\Sources\Runtimes\Xin.Core.Foundation\Meta\Type.h)"V;
	//Target = u8R"(H:\Projs\XinSamples\Xin\Sources\Runtimes\Xin.Core.Foundation\)"V;
	//Target = u8R"(H:\Projs\XinSamples\Xin\Sources\Runtimes\Xin.UI\Animations\DoubleAnimation.h)"V;
	//Target = u8R"(H:\Projs\XinSamples\Xin\Sources\Runtimes\Xin.Core.Foundation\Basic\Strings.h)"V;
	//Target = u8R"(H:\Projs\XinSamples\Xin\Sources\Runtimes\Xin.Core.Foundation\Basic\Thickness.h)"V;
	//Target = u8R"(H:\Projs\XinSamples\Xin\Sources\Runtimes\Xin.Core.Foundation\Basic\Archive.h)"V;
	//Target = u8R"(H:\Projs\XinSamples\Xin\Sources\Runtimes\Xin.Core.Foundation\Basic\Vector4.h)"V;
	//Target = u8R"(H:\Projs\XinSamples\Xin\Sources\Runtimes\Xin.Core\Core\Freezable.h)"V;
	//Target = u8R"(H:\Projs\XinSamples\Xin\Sources\Runtimes\Xin.Core\Core\Dependency.h)"V;
	//Target = u8R"(H:\Projs\XinSamples\Xin\Sources\Runtimes\Xin.UI\Animations\Timeline.h)"V;
	Target = u8R"(H:\Projs\XinSamples\Xin\Sources\Runtimes\Xin.UI\Animations\DoubleAnimation.h)"V;
	//Target = u8R"(H:\Projs\XinSamples\Xin\Sources\Runtimes\Xin.UI\Controls\Element.h)"V;
	//Target = u8R"(H:\Projs\XinSamples\Xin\Sources\Runtimes\Xin.UI\Controls\Border.h)"V;
	//Target = u8R"(H:\Projs\XinSamples\Xin\Sources\Runtimes\Xin.UI\Controls\Control.h)"V;
	//Target = u8R"(H:\Projs\XinSamples\Xin\Sources\Runtimes\Xin.UI\Controls\SplitterPanel.h)";
#endif

	if (FDirectory::Exists(Target))
	{
		FReflectSettings ReflectSettings;
		ReflectSettings.ProjectRoot = ProgramOptionsParser.FindOption(u8"ProjectRoot"N, u8R"(H:\Projs\XinSamples\)");
		ReflectSettings.PCHIncludeFile = ProgramOptionsParser.FindOption(u8"PCHIncludeFile"N, u8"PCH.h"V);;
		ReflectSettings.ModuleRoot = FPath::IsAbsolute(Target) ? Target : FPath::GetAbsolutePath(Target);
		ReflectSettings.ForceGenerate = ProgramOptionsParser.FindBoolOption(u8"ForceGenerate"N, true);
		//ReflectFile(ReflectContext, u8R"(H:\Projs\XinSamples\Xin\Sources\Runtimes\Xin.UI\Element.h)");
		ReflectModule(ReflectSettings);
	}
	else if (FFile::Exists(Target))
	{
		FString ModuleRoot = FPath::GetDirectory(Target);
		while(!FFile::Exists(FPath::Combine(ModuleRoot, u8"*.Module.cs"V)))
			ModuleRoot = FPath::GetDirectory(ModuleRoot);

		if (ModuleRoot.Empty())
			return 0;

		FReflectSettings ReflectSettings;
		ReflectSettings.ProjectRoot = ProgramOptionsParser.FindOption(u8"ProjectRoot"N, u8R"(H:\Projs\XinSamples\)");
		ReflectSettings.PCHIncludeFile = ProgramOptionsParser.FindOption(u8"PCHIncludeFile"N, u8"PCH.h"V);;
		ReflectSettings.ModuleRoot = ModuleRoot;
		ReflectSettings.ForceGenerate = ProgramOptionsParser.FindBoolOption(u8"ForceGenerate"N, false);
		//ReflectFile(ReflectSettings, u8R"(H:\Projs\XinSamples\Xin\Sources\Runtimes\Xin.UI\Element.h)");
		ReflectFile(ReflectSettings, Target);
	}
	else
	{
		FConsole::WriteLine(u8"FString.Name={0}"V, Typeof<FString>().Name);
		FConsole::WriteLine(u8"FWString.Name={0}"V, Typeof<FWString>().Name);

		FReflectSettings ReflectContext;
		ReflectContext.ProjectRoot = u8R"(H:\Projs\XinSamples\)";
		ReflectContext.PCHIncludeFile = u8"PCH.h"V;

		float64 TimePoint0 = FDateTime::HighResolution();
		ReflectContext.ModuleRoot = u8R"(H:\Projs\XinSamples\Xin\Sources\Runtimes\Xin.Core)";
		//ReflectFile(ReflectContext, u8R"(H:\Projs\XinSamples\Xin\Sources\Runtimes\Xin.Core\Basic\Object.h)");
		//ReflectModule(ReflectContext);

		ReflectContext.ModuleRoot = u8R"(H:\Projs\XinSamples\Xin\Sources\Runtimes\Xin.UI)";
		ReflectFile(ReflectContext, u8R"(H:\Projs\XinSamples\Xin\Sources\Runtimes\Xin.UI\Controls\GridPanel.h)");
		//ReflectModule(ReflectContext);

		ReflectContext.ModuleRoot = u8R"(H:\Projs\XinSamples\Sources\Programs\00.Test.Core)";
		//ReflectFile(ReflectContext, u8R"(H:\Projs\XinSamples\Sources\Programs\00.Test.Core\Strings.h)");
		//ReflectModule(ReflectContext);

		//GeneratePCHFile(ReflectContext, u8R"(H:\Projs\XinSamples\Xin\Sources\Runtimes\Xin.Core\Xin.Core.h)", u8"Xin.Core.pch"V);
		//GeneratePCHFile(ReflectContext, u8R"(H:\Projs\XinSamples\Sources\Programs\00.Test.Core\PCH.h)", u8"00.Test.Core.pch"V);
		//ReflectFile(ReflectContext, FilePath);
		float64 TimePoint1 = FDateTime::HighResolution();
		ReflectContext.ModuleRoot = u8R"(H:\Projs\XinSamples\Sources\Programs\00.Test.Core)";
		//ReflectContext.PCHIncludeFile = FStringV::None;
		//ReflectFile(ReflectContext, FilePath);
		float64 TimePoint2 = FDateTime::HighResolution();
		FConsole::WriteLine(u8"TimeCost: {:.2f}s (with pch), {:.2f}s (without pch)"V, TimePoint1 - TimePoint0, TimePoint2 - TimePoint1);
	}

#if XIN_DEBUG
	FConsole::ReadKey(u8"按任意键退出..."V);
#endif
	return 0;
}
