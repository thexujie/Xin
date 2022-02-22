#pragma once

#include "Xin.Core/Xin.Core.h"
#include "Xin.RHI/Xin.RHI.h"
#include "Xin.RHI.UI/Xin.RHI.UI.h"
#include "Xin.Rendering/Xin.Rendering.h"
#include "Xin.ShaderCompiler/Xin.ShaderCompiler.h"

using namespace Xin;
using namespace Xin::UI;
using namespace Xin::RHI;
using namespace Xin::RHI::UI;
using namespace Xin::Rendering;

#if XIN_WINDOWS
#ifdef _00_TEST_RHI_UI_Module_
#define TESTRHIUI_API __declspec(dllexport)
#else
#define TESTRHIUI_API __declspec(dllimport)
#endif
#endif
