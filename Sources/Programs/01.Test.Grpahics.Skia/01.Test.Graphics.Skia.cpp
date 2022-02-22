#include "pch.h"
#include "01.Test.Graphics.Skia.h"

#include "Programs/01.Test.Graphics/01.Test.Graphics.h"

using namespace Xin;

int main(int argc, const char * args[])
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(3986);

	Skia::FSkiaGraphics SkiaGraphics;
	return TestSimple(SkiaGraphics);
}
