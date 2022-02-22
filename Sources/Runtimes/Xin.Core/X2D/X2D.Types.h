#pragma once

#include "../Xin.Core.Types.h"

#include "../Media/Color.h"
#include "../Media/Pen.h"
#include "../Media/Brush.h"
#include "../Media/Image.h"

namespace Xin
{
	constexpr float32 X2DFlatteningTolerance = 0.25f;

	constexpr uint16 InvalidGlyphIndex = 0xFFFF;

	class ITextShaper;
	class IBitmap;
	class IPainter;
	class IGraphics;

	enum class EReadingDirection
	{
		Default = 0,
		LeftToRight,
		RightToLeft,
		TopToBottom,
		BottomToTop,
	};

	enum class EBitmapUsage
	{
		None = 0,
		RenderTarget = 0x0001,
		ShaderResource = 0x0002,
		ReadBack = 0x0004,
	};
	template class TEnumWrapper<EBitmapUsage>;


	constexpr uint32 ScriptCodeMakeFourCC(char Bits24, char Bits16, char Bits8, char Bits0)
	{
		return (uint32)(((uint8)(Bits24)) << 24 | (((uint8)(Bits16)) << 16) | ((uint8)(Bits8)) << 8 | ((uint8)(Bits0)));
	}

	enum class EBiDiLevel : uint8
	{
		LTR = 0,
		RTL = 1,

		DefaultLTR = 0xFE, // UBIDI_DEFAULT_LTR
		DefaultRTL = 0xFF, // UBIDI_DEFAULT_RTL
	};

	enum class EScriptCode : uint32
	{
		/*0.0*/ None = ScriptCodeMakeFourCC(0, 0, 0, 0),

		/*1.1*/ Common = ScriptCodeMakeFourCC('Z', 'y', 'y', 'y'),
		/*1.1*/ Inherited = ScriptCodeMakeFourCC('Z', 'i', 'n', 'h'),
		/*5.0*/ Unknown = ScriptCodeMakeFourCC('Z', 'z', 'z', 'z'),

		/*1.1*/ Arabic = ScriptCodeMakeFourCC('A', 'r', 'a', 'b'),
		/*1.1*/ Armenian = ScriptCodeMakeFourCC('A', 'r', 'm', 'n'),
		/*1.1*/ Bengali = ScriptCodeMakeFourCC('B', 'e', 'n', 'g'),
		/*1.1*/ Cyrillic = ScriptCodeMakeFourCC('C', 'y', 'r', 'l'),
		/*1.1*/ Devanagari = ScriptCodeMakeFourCC('D', 'e', 'v', 'a'),
		/*1.1*/ Georgian = ScriptCodeMakeFourCC('G', 'e', 'o', 'r'),
		/*1.1*/ Greek = ScriptCodeMakeFourCC('G', 'r', 'e', 'k'),
		/*1.1*/ Gujarati = ScriptCodeMakeFourCC('G', 'u', 'j', 'r'),
		/*1.1*/ Gurmukhi = ScriptCodeMakeFourCC('G', 'u', 'r', 'u'),
		/*1.1*/ Hangul = ScriptCodeMakeFourCC('H', 'a', 'n', 'g'),
		/*1.1*/ Han = ScriptCodeMakeFourCC('H', 'a', 'n', 'i'),
		/*1.1*/ Hebrew = ScriptCodeMakeFourCC('H', 'e', 'b', 'r'),
		/*1.1*/ Hiragana = ScriptCodeMakeFourCC('H', 'i', 'r', 'a'),
		/*1.1*/ Kannada = ScriptCodeMakeFourCC('K', 'n', 'd', 'a'),
		/*1.1*/ Katakana = ScriptCodeMakeFourCC('K', 'a', 'n', 'a'),
		/*1.1*/ Lao = ScriptCodeMakeFourCC('L', 'a', 'o', 'o'),
		/*1.1*/ Latin = ScriptCodeMakeFourCC('L', 'a', 't', 'n'),
		/*1.1*/ Malayalam = ScriptCodeMakeFourCC('M', 'l', 'y', 'm'),
		/*1.1*/ Oriya = ScriptCodeMakeFourCC('O', 'r', 'y', 'a'),
		/*1.1*/ Tamil = ScriptCodeMakeFourCC('T', 'a', 'm', 'l'),
		/*1.1*/ Telugu = ScriptCodeMakeFourCC('T', 'e', 'l', 'u'),
		/*1.1*/ Thai = ScriptCodeMakeFourCC('T', 'h', 'a', 'i'),

		/*2.0*/ Tibetan = ScriptCodeMakeFourCC('T', 'i', 'b', 't'),

		/*3.0*/ Bopomofo = ScriptCodeMakeFourCC('B', 'o', 'p', 'o'),
		/*3.0*/ Braille = ScriptCodeMakeFourCC('B', 'r', 'a', 'i'),
		/*3.0*/ CanadianSyllabics = ScriptCodeMakeFourCC('C', 'a', 'n', 's'),
		/*3.0*/ Cherokee = ScriptCodeMakeFourCC('C', 'h', 'e', 'r'),
		/*3.0*/ Ethiopic = ScriptCodeMakeFourCC('E', 't', 'h', 'i'),
		/*3.0*/ Khmer = ScriptCodeMakeFourCC('K', 'h', 'm', 'r'),
		/*3.0*/ Mongolian = ScriptCodeMakeFourCC('M', 'o', 'n', 'g'),
		/*3.0*/ Myanmar = ScriptCodeMakeFourCC('M', 'y', 'm', 'r'),
		/*3.0*/ Ogham = ScriptCodeMakeFourCC('O', 'g', 'a', 'm'),
		/*3.0*/ Runic = ScriptCodeMakeFourCC('R', 'u', 'n', 'r'),
		/*3.0*/ Sinhala = ScriptCodeMakeFourCC('S', 'i', 'n', 'h'),
		/*3.0*/ Syriac = ScriptCodeMakeFourCC('S', 'y', 'r', 'c'),
		/*3.0*/ Thaana = ScriptCodeMakeFourCC('T', 'h', 'a', 'a'),
		/*3.0*/ Yi = ScriptCodeMakeFourCC('Y', 'i', 'i', 'i'),

		/*3.1*/ Deseret = ScriptCodeMakeFourCC('D', 's', 'r', 't'),
		/*3.1*/ Gothic = ScriptCodeMakeFourCC('G', 'o', 't', 'h'),
		/*3.1*/ Old_Italic = ScriptCodeMakeFourCC('I', 't', 'a', 'l'),

		/*3.2*/ Buhid = ScriptCodeMakeFourCC('B', 'u', 'h', 'd'),
		/*3.2*/ Hanunoo = ScriptCodeMakeFourCC('H', 'a', 'n', 'o'),
		/*3.2*/ Tagalog = ScriptCodeMakeFourCC('T', 'g', 'l', 'g'),
		/*3.2*/ Tagbanwa = ScriptCodeMakeFourCC('T', 'a', 'g', 'b'),

		/*4.0*/ Cypriot = ScriptCodeMakeFourCC('C', 'p', 'r', 't'),
		/*4.0*/ Limbu = ScriptCodeMakeFourCC('L', 'i', 'm', 'b'),
		/*4.0*/ LinearB = ScriptCodeMakeFourCC('L', 'i', 'n', 'b'),
		/*4.0*/ Osmanya = ScriptCodeMakeFourCC('O', 's', 'm', 'a'),
		/*4.0*/ Shavian = ScriptCodeMakeFourCC('S', 'h', 'a', 'w'),
		/*4.0*/ TaiLe = ScriptCodeMakeFourCC('T', 'a', 'l', 'e'),
		/*4.0*/ Ugaritic = ScriptCodeMakeFourCC('U', 'g', 'a', 'r'),

		/*4.1*/ Buginese = ScriptCodeMakeFourCC('B', 'u', 'g', 'i'),
		/*4.1*/ Coptic = ScriptCodeMakeFourCC('C', 'o', 'p', 't'),
		/*4.1*/ Glagolitic = ScriptCodeMakeFourCC('G', 'l', 'a', 'g'),
		/*4.1*/ Kharoshthi = ScriptCodeMakeFourCC('K', 'h', 'a', 'r'),
		/*4.1*/ NewTaiLue = ScriptCodeMakeFourCC('T', 'a', 'l', 'u'),
		/*4.1*/ Old_Persian = ScriptCodeMakeFourCC('X', 'p', 'e', 'o'),
		/*4.1*/ SylotiNagri = ScriptCodeMakeFourCC('S', 'y', 'l', 'o'),
		/*4.1*/ Tifinagh = ScriptCodeMakeFourCC('T', 'f', 'n', 'g'),

		/*5.0*/ Balinese = ScriptCodeMakeFourCC('B', 'a', 'l', 'i'),
		/*5.0*/ Cuneiform = ScriptCodeMakeFourCC('X', 's', 'u', 'x'),
		/*5.0*/ Nko = ScriptCodeMakeFourCC('N', 'k', 'o', 'o'),
		/*5.0*/ PhagsPa = ScriptCodeMakeFourCC('P', 'h', 'a', 'g'),
		/*5.0*/ Phoenician = ScriptCodeMakeFourCC('P', 'h', 'n', 'x'),

		/*5.1*/ Carian = ScriptCodeMakeFourCC('C', 'a', 'r', 'i'),
		/*5.1*/ Cham = ScriptCodeMakeFourCC('C', 'h', 'a', 'm'),
		/*5.1*/ KayahLi = ScriptCodeMakeFourCC('K', 'a', 'l', 'i'),
		/*5.1*/ Lepcha = ScriptCodeMakeFourCC('L', 'e', 'p', 'c'),
		/*5.1*/ Lycian = ScriptCodeMakeFourCC('L', 'y', 'c', 'i'),
		/*5.1*/ Lydian = ScriptCodeMakeFourCC('L', 'y', 'd', 'i'),
		/*5.1*/ OlChiki = ScriptCodeMakeFourCC('O', 'l', 'c', 'k'),
		/*5.1*/ Rejang = ScriptCodeMakeFourCC('R', 'j', 'n', 'g'),
		/*5.1*/ Saurashtra = ScriptCodeMakeFourCC('S', 'a', 'u', 'r'),
		/*5.1*/ Sundanese = ScriptCodeMakeFourCC('S', 'u', 'n', 'd'),
		/*5.1*/ Vai = ScriptCodeMakeFourCC('V', 'a', 'i', 'i'),

		/*5.2*/ Avestan = ScriptCodeMakeFourCC('A', 'v', 's', 't'),
		/*5.2*/ Bamum = ScriptCodeMakeFourCC('B', 'a', 'm', 'u'),
		/*5.2*/ EgyptianHieroglyphs = ScriptCodeMakeFourCC('E', 'g', 'y', 'p'),
		/*5.2*/ ImperialAramaic = ScriptCodeMakeFourCC('A', 'r', 'm', 'i'),
		/*5.2*/ InscriptionalPahlavi = ScriptCodeMakeFourCC('P', 'h', 'l', 'i'),
		/*5.2*/ InscriptionalParthian = ScriptCodeMakeFourCC('P', 'r', 't', 'i'),
		/*5.2*/ Javanese = ScriptCodeMakeFourCC('J', 'a', 'v', 'a'),
		/*5.2*/ Kaithi = ScriptCodeMakeFourCC('K', 't', 'h', 'i'),
		/*5.2*/ Lisu = ScriptCodeMakeFourCC('L', 'i', 's', 'u'),
		/*5.2*/ MeeteiMayek = ScriptCodeMakeFourCC('M', 't', 'e', 'i'),
		/*5.2*/ Old_SouthArabian = ScriptCodeMakeFourCC('S', 'a', 'r', 'b'),
		/*5.2*/ Old_Turkic = ScriptCodeMakeFourCC('O', 'r', 'k', 'h'),
		/*5.2*/ Samaritan = ScriptCodeMakeFourCC('S', 'a', 'm', 'r'),
		/*5.2*/ TaiTham = ScriptCodeMakeFourCC('L', 'a', 'n', 'a'),
		/*5.2*/ TaiViet = ScriptCodeMakeFourCC('T', 'a', 'v', 't'),

		/*6.0*/ Batak = ScriptCodeMakeFourCC('B', 'a', 't', 'k'),
		/*6.0*/ Brahmi = ScriptCodeMakeFourCC('B', 'r', 'a', 'h'),
		/*6.0*/ Mandaic = ScriptCodeMakeFourCC('M', 'a', 'n', 'd'),

		/*6.1*/ Chakma = ScriptCodeMakeFourCC('C', 'a', 'k', 'm'),
		/*6.1*/ MeroiticCursive = ScriptCodeMakeFourCC('M', 'e', 'r', 'c'),
		/*6.1*/ MeroiticHieroglyphs = ScriptCodeMakeFourCC('M', 'e', 'r', 'o'),
		/*6.1*/ Miao = ScriptCodeMakeFourCC('P', 'l', 'r', 'd'),
		/*6.1*/ Sharada = ScriptCodeMakeFourCC('S', 'h', 'r', 'd'),
		/*6.1*/ SoraSompeng = ScriptCodeMakeFourCC('S', 'o', 'r', 'a'),
		/*6.1*/ Takri = ScriptCodeMakeFourCC('T', 'a', 'k', 'r'),

		/*
		* since: 0.9.30
		*/
		/*7.0*/ BassaVah = ScriptCodeMakeFourCC('B', 'a', 's', 's'),
		/*7.0*/ CaucasianAlbanian = ScriptCodeMakeFourCC('A', 'g', 'h', 'b'),
		/*7.0*/ Duployan = ScriptCodeMakeFourCC('D', 'u', 'p', 'l'),
		/*7.0*/ Elbasan = ScriptCodeMakeFourCC('E', 'l', 'b', 'a'),
		/*7.0*/ Grantha = ScriptCodeMakeFourCC('G', 'r', 'a', 'n'),
		/*7.0*/ Khojki = ScriptCodeMakeFourCC('K', 'h', 'o', 'j'),
		/*7.0*/ Khudawadi = ScriptCodeMakeFourCC('S', 'i', 'n', 'd'),
		/*7.0*/ LinearA = ScriptCodeMakeFourCC('L', 'i', 'n', 'a'),
		/*7.0*/ Mahajani = ScriptCodeMakeFourCC('M', 'a', 'h', 'j'),
		/*7.0*/ Manichaean = ScriptCodeMakeFourCC('M', 'a', 'n', 'i'),
		/*7.0*/ MendeKikakui = ScriptCodeMakeFourCC('M', 'e', 'n', 'd'),
		/*7.0*/ Modi = ScriptCodeMakeFourCC('M', 'o', 'd', 'i'),
		/*7.0*/ Mro = ScriptCodeMakeFourCC('M', 'r', 'o', 'o'),
		/*7.0*/ Nabataean = ScriptCodeMakeFourCC('N', 'b', 'a', 't'),
		/*7.0*/ Old_NorthArabian = ScriptCodeMakeFourCC('N', 'a', 'r', 'b'),
		/*7.0*/ Old_Permic = ScriptCodeMakeFourCC('P', 'e', 'r', 'm'),
		/*7.0*/ PahawhHmong = ScriptCodeMakeFourCC('H', 'm', 'n', 'g'),
		/*7.0*/ Palmyrene = ScriptCodeMakeFourCC('P', 'a', 'l', 'm'),
		/*7.0*/ PauCinHau = ScriptCodeMakeFourCC('P', 'a', 'u', 'c'),
		/*7.0*/ PsalterPahlavi = ScriptCodeMakeFourCC('P', 'h', 'l', 'p'),
		/*7.0*/ Siddham = ScriptCodeMakeFourCC('S', 'i', 'd', 'd'),
		/*7.0*/ Tirhuta = ScriptCodeMakeFourCC('T', 'i', 'r', 'h'),
		/*7.0*/ WarangCiti = ScriptCodeMakeFourCC('W', 'a', 'r', 'a'),

		/*8.0*/ Ahom = ScriptCodeMakeFourCC('A', 'h', 'o', 'm'),
		/*8.0*/ AnatolianHieroglyphs = ScriptCodeMakeFourCC('H', 'l', 'u', 'w'),
		/*8.0*/ Hatran = ScriptCodeMakeFourCC('H', 'a', 't', 'r'),
		/*8.0*/ Multani = ScriptCodeMakeFourCC('M', 'u', 'l', 't'),
		/*8.0*/ Old_Hungarian = ScriptCodeMakeFourCC('H', 'u', 'n', 'g'),
		/*8.0*/ Signwriting = ScriptCodeMakeFourCC('S', 'g', 'n', 'w'),

		/*
		* since 1.3.0
		*/
		/*9.0*/ Adlam = ScriptCodeMakeFourCC('A', 'd', 'l', 'm'),
		/*9.0*/ Bhaiksuki = ScriptCodeMakeFourCC('B', 'h', 'k', 's'),
		/*9.0*/ Marchen = ScriptCodeMakeFourCC('M', 'a', 'r', 'c'),
		/*9.0*/ Osage = ScriptCodeMakeFourCC('O', 's', 'g', 'e'),
		/*9.0*/ Tangut = ScriptCodeMakeFourCC('T', 'a', 'n', 'g'),
		/*9.0*/ Newa = ScriptCodeMakeFourCC('N', 'e', 'w', 'a'),

		/*
		* since 1.6.0
		*/
		/*10.0*/MasaramGondi = ScriptCodeMakeFourCC('G', 'o', 'n', 'm'),
		/*10.0*/Nushu = ScriptCodeMakeFourCC('N', 's', 'h', 'u'),
		/*10.0*/Soyombo = ScriptCodeMakeFourCC('S', 'o', 'y', 'o'),
		/*10.0*/ZanabazarSquare = ScriptCodeMakeFourCC('Z', 'a', 'n', 'b'),

		/*
		* since 1.8.0
		*/
		/*11.0*/Dogra = ScriptCodeMakeFourCC('D', 'o', 'g', 'r'),
		/*11.0*/GunjalaGondi = ScriptCodeMakeFourCC('G', 'o', 'n', 'g'),
		/*11.0*/HanifiRohingya = ScriptCodeMakeFourCC('R', 'o', 'h', 'g'),
		/*11.0*/Makasar = ScriptCodeMakeFourCC('M', 'a', 'k', 'a'),
		/*11.0*/Medefaidrin = ScriptCodeMakeFourCC('M', 'e', 'd', 'f'),
		/*11.0*/Old_Sogdian = ScriptCodeMakeFourCC('S', 'o', 'g', 'o'),
		/*11.0*/Sogdian = ScriptCodeMakeFourCC('S', 'o', 'g', 'd'),
	};

	class CORE_API IX2DObject: public ISharedRefer
	{
	public:
	};
	using IX2DObjectRef = TReferPtr<IX2DObject>;
}
