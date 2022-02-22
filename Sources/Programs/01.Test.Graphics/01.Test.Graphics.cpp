#include "pch.h"
#include "01.Test.Graphics.h"

int32 TestSimple(IGraphics & Graphics)
{
	FStringV String = u8"♒强烈❤️❤️❤️推荐♒\n✅运行正常✅\n⚡亭亭玉立⚡\n♈恭喜发财♈\n✨新年快乐✨\n🏅大吉大利🏅\n♒今晚吃鸡♒"V;
	//FStringV String = u8"𰻞你好，我是谁ABCDดีดีดีีีดีดีlālálǎlàوالعدل والسلام靐𰻞好𰻞𰻞 في العالم"V;
	Graphics.CreateTextBlob(String, FFontFace{ }, 80.0f, { 300, 720.0f }, ETextLayoutFlags::None);
	return 0;
}
