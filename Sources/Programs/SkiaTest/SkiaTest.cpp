#include "PCH.h"
using namespace Xin;
using namespace Xin::Graphics;
using namespace Xin::Graphics::Skia;

void Test()
{
	FFontFamily FontFamily;
	FontFamily.Name = u8"Î¢ÈíÑÅºÚ"S;

	FFontStyle FontStyle;
	FFontMetrics FontMetrics = Xin::Graphics::Skia::fontmetrics(FontFamily, FontStyle);
	LogInfo(u8"M.H = {0}"S, FontMetrics.Ascent);
}

int _tmain(int argc, const char ** args)
{
	Test();
	return 0;
}
