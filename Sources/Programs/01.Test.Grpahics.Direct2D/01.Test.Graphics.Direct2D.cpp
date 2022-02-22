#include "pch.h"
#include "01.Test.Graphics.Direct2D.h"

#include "Programs/01.Test.Graphics/01.Test.Graphics.h"

using namespace Xin;

int main(int argc, const char * args[])
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	D2D::FD2DGraphics D2DGraphics;
	return TestSimple(D2DGraphics);
}
