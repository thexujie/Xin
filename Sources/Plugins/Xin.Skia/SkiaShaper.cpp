#include "PCH.h"
#include "SkiaShaper.h"

#include "include/ports/SkTypeface_win.h"

namespace Xin::Skia
{
	static TScopedPtr<hb_font_t, hb_font_destroy> hbFontCreate(SkTypeface * skTypeface)
	{
		int TrueTypeCollectionIndex = 0;
		std::unique_ptr<SkStreamAsset> skStreamAsset(skTypeface->openStream(&TrueTypeCollectionIndex));
		size_t AssetLength = skStreamAsset->getLength();
		TScopedPtr<hb_blob_t, hb_blob_destroy> hbBlob;
		if (const void * MemoryBase = skStreamAsset->getMemoryBase())
		{
			hbBlob = hb_blob_create((char *)MemoryBase, SkToUInt(AssetLength), HB_MEMORY_MODE_READONLY, skStreamAsset.release(), [](void * p) { delete (SkStreamAsset *)p; });
		}
		else
		{
			// SkDebugf("Extra SkStreamAsset copy\n");
			void * AssetData = AssetLength ? sk_malloc_throw(AssetLength) : nullptr;
			skStreamAsset->read(AssetData, AssetLength);
			hbBlob = hb_blob_create((char *)AssetData, SkToUInt(AssetLength), HB_MEMORY_MODE_READONLY, AssetData, sk_free);
		}
		assert(hbBlob);
		hb_blob_make_immutable(hbBlob);

		TScopedPtr<hb_face_t, hb_face_destroy> hbFace(hb_face_create(hbBlob, (unsigned)TrueTypeCollectionIndex));

		assert(hbFace);
		if (!hbFace)
			return nullptr;

		hb_face_set_index(hbFace, (unsigned)TrueTypeCollectionIndex);
		hb_face_set_upem(hbFace, skTypeface->getUnitsPerEm());

		TScopedPtr<hb_font_t, hb_font_destroy> hbFont(hb_font_create(hbFace));
		assert(hbFont);
		if (!hbFont)
			return nullptr;
		hb_ot_font_set_funcs(hbFont);
		int NumCoordinates = skTypeface->getVariationDesignPosition(nullptr, 0);
		if (NumCoordinates > 0)
		{
			TList<SkFontArguments::VariationPosition::Coordinate> Coordinates(NumCoordinates);
			if (skTypeface->getVariationDesignPosition(Coordinates.Data, NumCoordinates) == NumCoordinates)
			{
				hb_font_set_variations(hbFont, reinterpret_cast<hb_variation_t *>(Coordinates.Data), NumCoordinates);
			}
		}
		return hbFont.Detatch();
	}

	FFontFace skGetFontFace(SkTypeface * skTypeface)
	{
		SkString skFamilyName;
		skTypeface->getFamilyName(&skFamilyName);
		SkFontStyle skFontStyle = skTypeface->fontStyle();

		FFontFace FontFace;
		FontFace.FaceName = FEncoding::AsUtf8(skFamilyName.c_str(), skFamilyName.size());
		FontFace.FontWeight = ToFontWeight(SkFontStyle::Weight(skFontStyle.weight()));
		FontFace.FontStyle = ToFontStyle(skFontStyle.slant());
		return FontFace;
	}

	FFontMetrics skGetFontMatrics(SkTypeface * skTypeface)
	{
		SkFont skFont { sk_sp<SkTypeface>(SkRef(skTypeface)), 1.0f };

		SkFontMetrics skFontMetrics;
		SkScalar LineSpace = skFont.getMetrics(&skFontMetrics);

		FFontMetrics FontMetrics;
		FontMetrics.Size = 1.0f;
		FontMetrics.Ascent = Abs(skFontMetrics.fAscent);
		FontMetrics.Descent = skFontMetrics.fDescent;
		FontMetrics.LineSpace = LineSpace;
		FontMetrics.UnitsPerEm = skTypeface->getUnitsPerEm();

		return FontMetrics;
	}

	FSkiaFontFace::FSkiaFontFace(const FFontFace & FontFace, SkFontStyle skFontStyle, SkTypeface * skTypeface) : IFontFace(FontFace, skGetFontMatrics(skTypeface)), skTypeface(SkiaRref(skTypeface))
	{
		hbFont = hbFontCreate(skTypeface);
	}

	FSkiaFontFallbacker::FSkiaFontFallbacker(FSkiaShaper & Shaper) : Shaper(Shaper)
	{
		//PrefferedFontFaces[EScriptCode::arabic] = u8"Courier New"V;
	}

	sk_sp<SkTypeface> FSkiaFontFallbacker::MapCharacter(char32 Utf32Char, EScriptCode ScriptCode, const FSkiaFontFace & BaseFontFace)
	{
		//const icu::Locale & icuDefaultLocal = icu::Locale::getDefault();
		//const size_t NumBcp47Locales = 1;
		//const char * Bcp47Locales[NumBcp47Locales] { icuDefaultLocal.getBaseName() };

		if (auto Iter = PrefferedFontFaces.Find(ScriptCode); Iter != PrefferedFontFaces.End())
		{
			sk_sp<SkTypeface> skMatchedFontFace = Shaper.skDefaultFontMgr->matchFamilyStyleCharacter(FEncoding::AsAnsi(Iter->Second), BaseFontFace.skFontStyle, nullptr, 0, Utf32Char);
			if (skMatchedFontFace)
				return skMatchedFontFace;
		}

		sk_sp<SkTypeface> skMatchedFontFace = Shaper.skDefaultFontMgr->matchFamilyStyleCharacter((const chara *)BaseFontFace.FontFace.FaceName.Data, BaseFontFace.skFontStyle, nullptr, 0, Utf32Char);
		if (!skMatchedFontFace)
		{
			int32 NumFontFamilies = Shaper.skDefaultFontMgr->countFamilies();
			for (int32 FontFamilyIndex = 0; FontFamilyIndex < NumFontFamilies; ++FontFamilyIndex)
			{
				SkString FamilyName;
				Shaper.skDefaultFontMgr->getFamilyName(FontFamilyIndex, &FamilyName);

				skMatchedFontFace = Shaper.skDefaultFontMgr->matchFamilyStyleCharacter(FamilyName.c_str(), BaseFontFace.skFontStyle, nullptr, 0, Utf32Char);
				if (skMatchedFontFace)
					break;
			}
		}
		return skMatchedFontFace;
	}

	void FSkiaTextShape::Shape(FUStringV String, const FFontFace & FontFace, float32 FontSize, EReadingDirection ReadingDirection)
	{
		this->String = String;
		this->FontFace = FontFace;
		this->FontSize = FontSize;
		this->ReadingDirection = ReadingDirection;

		if (!this->FontFace.FaceName.Length)
			this->FontFace.FaceName = FFontFace::Default.FaceName;
		Itemize();
	}

	bool FSkiaTextShape::Itemize()
	{
#if XIN_SKIA_DEBUG
		TList<uint8> NumUtf8sOfUtf16s;
#endif
		TList<uint8> NumUtf8sOfUtf32s;
		TList<uint32> Utf8IndicesOfUtf32s;
		TList<uint32> Utf16IndicesOfUtf32s;
		TList<uint32> Utf32IndicesOfUtf16s;

#if XIN_DEBUG
		TList<uint32> Utf32IndicesOfUtf8s;
#endif

		U16String.Reserve(String.Length / 2 + 1);
		U32String.Reserve(String.Length / 2 + 1);

		{
			uintx Utf8Index = 0;
			while (Utf8Index < String.Length)
			{
				char32 Utf32;
				uint32 NumUtf8sOfUtf32 = FEncoding::Utf8ToUtf32(String.Data + Utf8Index, String.Length - Utf8Index, Utf32);
				AssertExpr(NumUtf8sOfUtf32 > 0);

				U32String.Append(Utf32);
				NumUtf8sOfUtf32s.Add(uint8(NumUtf8sOfUtf32));
				Utf8IndicesOfUtf32s.Add(Utf8Index);

#if XIN_DEBUG
				Utf32IndicesOfUtf8s.Add(U32String.Length - 1);
				Utf32IndicesOfUtf8s.AddN(NullIndex, NumUtf8sOfUtf32 - 1);
#endif
				char16 Utf16[2];
				size_t NumUtf16sOfUtf32 = FEncoding::Utf32ToUtf16(Utf32, Utf16, 2);
				AssertExpr(NumUtf16sOfUtf32 > 0);
				Utf16IndicesOfUtf32s.Add(U16String.Length);

				Utf32IndicesOfUtf16s.Add(U32String.Length - 1);
				Utf32IndicesOfUtf16s.AddN(NullIndex, NumUtf16sOfUtf32 - 1);

				U16String.Append({ Utf16, NumUtf16sOfUtf32 });
#if XIN_DEBUG
				NumUtf8sOfUtf16s.Add(uint8(NumUtf16sOfUtf32));
				NumUtf8sOfUtf16s.AddN(NullIndex, NumUtf16sOfUtf32 - 1);
#endif

				Utf8Index += NumUtf8sOfUtf32;
			}
			Utf8IndicesOfUtf32s.Add(String.Length);
#if XIN_DEBUG
			Utf32IndicesOfUtf8s.Add(U32String.Length);
#endif
			Utf32IndicesOfUtf16s.Add(U16String.Length);
		}

		UErrorCode Status = U_ZERO_ERROR;

		TScopedPtr<UBiDi, ubidi_close> ubidiObject(ubidi_openSized((int32_t)U16String.Length, 0, &Status));
		if (!ubidiObject)
			return false;

		TScopedPtr<UText, utext_close> icuText = utext_openUTF8(nullptr, (const chara *)String.Data, int64(String.Length), &Status);

		TUniquePtr<icu::BreakIterator> LineBreaker = { icu::BreakIterator::createLineInstance(Shaper.DefaultLocal, Status) };
		LineBreaker->setText(icuText, Status);

		TList<int32> RuleStatus(String.Length, EInitializeMode::Zero);
		{
			int32 Utf8Index = LineBreaker->current();
			while (Utf8Index != icu::BreakIterator::DONE)
			{
				if (Utf8Index > 0)
					RuleStatus[Utf8Index - 1] = LineBreaker->getRuleStatus();
				Utf8Index = LineBreaker->next();
			}
		}

		//TUniquePtr<icu::BreakIterator> CharacterBreaker = { icu::BreakIterator::createCharacterInstance(Shaper.DefaultLocal, Status) };
		//CharacterBreaker->setText(icuText, Status);

		//int32 TextIndex = CharacterBreaker->current();
		//while (TextIndex != icu::BreakIterator::DONE)
		//{
		//	AssertExpresion(Utf32IndicesOfUtf8s[TextIndex] != NullIndexU32);

		//	if (TextClusterIndices[TextIndex] != NullIndexU32)
		//	{
		//		// 1 cluster may consists with multi utf32, like ดีีี.
		//		ScriptClusters[TextClusterIndices[TextIndex]].BreakCondition = EBreakCondition::Soft;
		//	}
		//	TextIndex = CharacterBreaker->next();
		//}
		ubidi_setPara(ubidiObject, U16String.Data, (int32_t)U16String.Length, 0, nullptr, &Status);

		TScopedPtr<hb_buffer_t, hb_buffer_destroy> hbBuffer(hb_buffer_create());
		hb_buffer_set_content_type(hbBuffer, HB_BUFFER_CONTENT_TYPE_UNICODE);
		hb_buffer_set_cluster_level(hbBuffer, HB_BUFFER_CLUSTER_LEVEL_MONOTONE_GRAPHEMES);
		hb_unicode_funcs_t * hbUnicode = hb_buffer_get_unicode_funcs(hbBuffer);
		//TScopedPtr<hb_unicode_funcs_t, hb_unicode_funcs_destroy> hbUnicode(hb_unicode_funcs_create(nullptr));

		// UBIDI_DEFAULT_LTR
		TView<UBiDiLevel> ubidiLevels = { ubidi_getLevels(ubidiObject, &Status), U16String.Length };
		TList<EScriptCode> ScriptCodes {U32String.Length, EInitializeMode::Zero};
		{
			uintx Utf32Index = 0;
			while (Utf32Index < U32String.Length)
			{
				uintx Utf8Index = Utf8IndicesOfUtf32s[Utf32Index];
				uintx Utf16Index = Utf16IndicesOfUtf32s[Utf32Index];
				char32 Char32 = U32String[Utf32Index];
				EBiDiLevel BiDiLevel = EBiDiLevel(ubidiLevels[Utf16Index]);
				EScriptCode ScriptCode = (EScriptCode)hb_unicode_script(hbUnicode, Char32);
				ScriptCodes[Utf32Index] = ScriptCode;
				
				if (RuleStatus[Utf8Index] == UBRK_LINE_HARD)
					ScriptCode = EScriptCode::None;

				bool ShouldCreateRun = false;
				if (ScriptRuns.Size == 0)
				{
					ShouldCreateRun = true;
				}
				else if (ScriptCode == EScriptCode::Inherited)
				{
					ShouldCreateRun = false;
					AssertExpr(ScriptRuns.Size > 0);
				}
				else if (ScriptCode == EScriptCode::None)
				{
					FSkiaScriptRun & LastScriptRun = ScriptRuns.Back();
					ShouldCreateRun = LastScriptRun.ScriptCode != EScriptCode::None;
				}
				else if (ScriptCode == EScriptCode::Common)
				{
					FSkiaScriptRun & LastScriptRun = ScriptRuns.Back();
					ShouldCreateRun = BiDiLevel != LastScriptRun.BiDiLevel || LastScriptRun.ScriptCode == EScriptCode::None;
				}
				else
				{
					FSkiaScriptRun & LastScriptRun = ScriptRuns.Back();
					if (LastScriptRun.ScriptCode == EScriptCode::Common)
						LastScriptRun.ScriptCode = ScriptCode;

					bool ScriptCodeChanged = ScriptCode != LastScriptRun.ScriptCode;
					bool BiDiLevelChanged = BiDiLevel != LastScriptRun.BiDiLevel;
					ShouldCreateRun = ScriptCodeChanged || BiDiLevelChanged;
				}

				if (ShouldCreateRun)
				{
					AssertExpr(ScriptRuns.Size == 0 || (ScriptCode != EScriptCode::Inherited));

					FSkiaScriptRun & ScriptRun = ScriptRuns.AddDefault();
					ScriptRun.TextIndex = Utf8IndicesOfUtf32s[Utf32Index];
					ScriptRun.TextLength = NumUtf8sOfUtf32s[Utf32Index];
					ScriptRun.Utf32Index = Utf32Index;
					ScriptRun.Utf32Length = 1;

					ScriptRun.ScriptCode = ScriptCode;
					ScriptRun.BiDiLevel = BiDiLevel;
#if XIN_SKIA_DEBUG
					ScriptRun.String = String.View(ScriptRun.TextIndex, ScriptRun.TextLength);
					ScriptRun.U32String = U32String.View(Utf32Index, 1);
#endif
				}
				else
				{
					FSkiaScriptRun & ScriptRun = ScriptRuns.Back();
					ScriptRun.TextLength += NumUtf8sOfUtf32s[Utf32Index];
					ScriptRun.Utf32Length += 1;
#if XIN_SKIA_DEBUG
					ScriptRun.String.Size += NumUtf8sOfUtf32s[Utf32Index];
					ScriptRun.U32String.Size += 1;
#endif
				}
				++Utf32Index;
			}
		}

		//const icu::Locale & icuDefaultLocal = icu::Locale::getDefault();
		const size_t NumBcp47Locales = 4;
		const char * Bcp47Locales[NumBcp47Locales] { std::locale().name().c_str()/*, icuDefaultLocal.getBaseName()*/ };

		FSkiaFontFace * DefaultFontFace = Shaper.FetchFontFace(FontFace);

		for (uintx ScriptRunIndex = 0; ScriptRunIndex < ScriptRuns.Size; ++ScriptRunIndex)
		{
			bool IsDefaultFontShaping = false;
			const FSkiaScriptRun & ScriptRun = ScriptRuns[ScriptRunIndex];
			for (uintx Utf32Index = ScriptRun.Utf32Index; Utf32Index < ScriptRun.Utf32Index + ScriptRun.Utf32Length; ++Utf32Index)
			{
				char32 U32Char = U32String[Utf32Index];

				if (SkGlyphID GlyphId = DefaultFontFace->skTypeface->unicharToGlyph(U32Char); ScriptRun.ScriptCode != EScriptCode::None && !GlyphId)
				{
					sk_sp<SkTypeface> skMatchedFontFace = Shaper.FontFallbacker.MapCharacter(U32Char, ScriptRun.ScriptCode, *DefaultFontFace);
					AssertExpr(skMatchedFontFace);

					if (skMatchedFontFace)
					{
						FSkiaFontFace * MatchedFontFace = Shaper.FetchFontFace(skMatchedFontFace.get());
						FShapeRun & ShapeRun = ShapeRuns.Emplace(*MatchedFontFace);
						ShapeRun.FontSize = FontSize;

						ShapeRun.ScriptCode = ScriptRun.ScriptCode;
						ShapeRun.BiDiLevel = ScriptRun.BiDiLevel;

						ShapeRun.TextIndex = Utf8IndicesOfUtf32s[Utf32Index];
						ShapeRun.Utf32Index = Utf32Index;

						uintx NextUtf32Index = Utf32Index + 1;
						while (NextUtf32Index < ScriptRun.Utf32Index + ScriptRun.Utf32Length)
						{
							char32 NextU32Char = U32String[NextUtf32Index];

							if (ScriptCodes[NextUtf32Index] != EScriptCode::Common)
							{
								SkGlyphID NextGlyphId = DefaultFontFace->skTypeface->unicharToGlyph(NextU32Char);
								if (NextGlyphId != 0)
									break;
							}

							SkGlyphID FallbackGlyphId = skMatchedFontFace->unicharToGlyph(NextU32Char);
							if (!FallbackGlyphId)
								break;

							++NextUtf32Index;
						}

						ShapeRun.TextLength = Utf8IndicesOfUtf32s[NextUtf32Index] - ShapeRun.TextIndex;
						ShapeRun.Utf32Length = NextUtf32Index - ShapeRun.Utf32Index;
#if XIN_DEBUG
						ShapeRun.String = String.View(ShapeRun.TextIndex, ShapeRun.TextLength);
						ShapeRun.U32String = U32String.View(ShapeRun.Utf32Index, ShapeRun.Utf32Length);
#endif

						Utf32Index = NextUtf32Index - 1;
						IsDefaultFontShaping = false;
					}
					else {}
				}
				else
				{
					if (!IsDefaultFontShaping/*Utf32Index == ScriptRun.Utf32Index*/)
					{
						IsDefaultFontShaping = true;

						FShapeRun & ShapeRun = ShapeRuns.Emplace(*DefaultFontFace);
						ShapeRun.FontSize = FontSize;

						ShapeRun.ScriptCode = ScriptRun.ScriptCode;
						ShapeRun.BiDiLevel = ScriptRun.BiDiLevel;

						ShapeRun.TextIndex = Utf8IndicesOfUtf32s[Utf32Index];
						ShapeRun.TextLength = NumUtf8sOfUtf32s[Utf32Index];
						ShapeRun.Utf32Index = Utf32Index;
						ShapeRun.Utf32Length = 1;
#if XIN_SKIA_DEBUG
						ShapeRun.String = String.View(ShapeRun.TextIndex, ShapeRun.TextLength);
						ShapeRun.U32String = U32String.View(ShapeRun.Utf32Index, ShapeRun.Utf32Length);
#endif
					}
					else
					{
						FShapeRun & ShapeRun = ShapeRuns.Back();
						ShapeRun.TextLength += NumUtf8sOfUtf32s[Utf32Index];
						ShapeRun.Utf32Length += 1;
#if XIN_SKIA_DEBUG
						ShapeRun.String = String.View(ShapeRun.TextIndex, ShapeRun.TextLength);
						ShapeRun.U32String = U32String.View(ShapeRun.Utf32Index, ShapeRun.Utf32Length);
#endif
					}
				}
			}
		}

		if (ShapeRuns.Size == 0)
			return true;

		/**
		 * Glyphs will be all same with hb_buffer_add_utf32 or hb_buffer_add_utf8,
		 * the only difference is GlyphClusters, it will be mapped to char32, or char8.
		 * We use hb_buffer_add_utf8 just for convinience.
		 **/

		//hb_buffer_add_utf32(hbBuffer, (const uint32 *)U32String.Data, int32(U32String.Length), 0, int32(U32String.Length));
		hb_buffer_add_utf8(hbBuffer, (const chara *)String.Data, int32(String.Length), 0, int32(String.Length));
		for (uintx ShapeRunIndex = 0; ShapeRunIndex < ShapeRuns.Size; ++ShapeRunIndex)
		{
			FShapeRun & ShapeRun = ShapeRuns[ShapeRunIndex];
			if (ShapeRun.ScriptCode == EScriptCode::None)
			{
				ShapeRun.GlyphIndex = uint32(GlyphIndices.Size);
				ShapeRun.GlyphLength = 0;
				continue;
			}

			hb_buffer_clear_contents(hbBuffer);

			//hb_buffer_add_utf32(hbBuffer, (const uint32 *)U32String.Data, int32(U32String.Length), ShapeRun.Utf32Index, ShapeRun.Utf32Length);
			hb_buffer_add_utf8(hbBuffer, (const chara *)String.Data, int32(String.Length), ShapeRun.TextIndex, ShapeRun.TextLength);
			hb_buffer_set_script(hbBuffer, hb_script_t(ShapeRun.ScriptCode));
			hb_buffer_set_direction(hbBuffer, (ShapeRun.BiDiLevel & EBiDiLevel::RTL) ? HB_DIRECTION_RTL : HB_DIRECTION_LTR);

			// TODO: language
			hb_buffer_guess_segment_properties(hbBuffer);

			hb_shape(StaticCast<FSkiaFontFace>(ShapeRun.FontFace).hbFont, hbBuffer, nullptr, 0);

			if (ShapeRun.BiDiLevel & EBiDiLevel::RTL)
			{
				// Put the clusters back in logical order.
				// Note that the advances remain ltr.
				hb_buffer_reverse(hbBuffer);
				//hb_buffer_reverse_clusters(hbBuffer);
			}

			ShapeRun.GlyphIndex = uint32(GlyphIndices.Size);
			ShapeRun.GlyphLength = hb_buffer_get_length(hbBuffer);

			int32_t hbFontScaleX = 0, hbFontScaleY = 0;
			hb_font_get_scale(StaticCast<FSkiaFontFace>(ShapeRun.FontFace).hbFont, &hbFontScaleX, &hbFontScaleY);

			float32 FontScaleX = ShapeRun.FontSize * 1.0f / hbFontScaleX;
			float32 FontScaleY = ShapeRun.FontSize * 1.0f / hbFontScaleY;

			TView<hb_glyph_info_t> GlyphInfos { hb_buffer_get_glyph_infos(hbBuffer, nullptr), ShapeRun.GlyphLength };
			TView<hb_glyph_position_t> GlyphPositions { hb_buffer_get_glyph_positions(hbBuffer, nullptr), ShapeRun.GlyphLength };

			GlyphIndices.Reserve(GlyphIndices.Size + ShapeRun.GlyphLength);
			GlyphOffsets.Reserve(GlyphIndices.Size + ShapeRun.GlyphLength);
			GlyphAdvances.Reserve(GlyphIndices.Size + ShapeRun.GlyphLength);
			GlyphClusters.Reserve(GlyphClusters.Size + ShapeRun.GlyphLength);

			for (uintx GlyphIndex = 0; GlyphIndex < ShapeRun.GlyphLength; ++GlyphIndex)
			{
				const hb_glyph_info_t & GlyphInfo = GlyphInfos[GlyphIndex];
				const hb_glyph_position_t & GlyphPosition = GlyphPositions[GlyphIndex];

				GlyphIndices.Add(uint16(GlyphInfo.codepoint));
				GlyphOffsets.Add({
					GlyphPosition.x_offset > 0 ? -GlyphPosition.x_offset * FontScaleX : 0.0f,
					GlyphPosition.y_offset > 0.0f ? -GlyphPosition.y_offset * FontScaleY : 0.0f
				});
				GlyphAdvances.Add(GlyphPosition.x_advance * FontScaleX);
				GlyphClusters.Add(GlyphInfo.cluster);

				ShapeRun.Advance += GlyphAdvances.Back();
			}
		}

		if (GlyphIndices.Size == 0 || GlyphClusters.Size == 0)
			return true;

		// break points
		TList<uint32> Utf8ClusterIndices { String.Length, NullIndex, EElementFill::Fill };
		ScriptClusters.ReserveTo(String.Length + 1);

		for (uint32 RunIndex = 0; RunIndex < uint32(ShapeRuns.Size); ++RunIndex)
		{
			FShapeRun & ShapeRun = ShapeRuns[RunIndex];
			ShapeRun.ClusterIndex = uint32(ScriptClusters.Size);
			if (ShapeRun.ScriptCode == EScriptCode::None)
			{
				ScriptClusters.Add({ RunIndex, ShapeRun.TextIndex, ShapeRun.TextLength, ShapeRun.GlyphIndex, 0, EBreakCondition::Hard });
				ShapeRun.ClusterLength = 1;
				continue;
			}


			uint32 LastTextIndex = NullIndex;
			for (uint32 GlyphIndex = ShapeRun.GlyphIndex; GlyphIndex < ShapeRun.GlyphIndex + ShapeRun.GlyphLength; ++GlyphIndex)
			{
				uint32 TextIndex = GlyphClusters[GlyphIndex];
				if (TextIndex != LastTextIndex)
				{
					if (ScriptClusters.Size > 0)
					{
						FScriptCluster & LastCluster = ScriptClusters.Back();
						LastCluster.GlyphLength = GlyphIndex - LastCluster.GlyphIndex;
						LastCluster.TextLength = TextIndex - LastCluster.TextIndex;
					}

					LastTextIndex = TextIndex;
					Utf8ClusterIndices[TextIndex] = uint32(ScriptClusters.Size);
					ScriptClusters.Add({ RunIndex, TextIndex, 1, GlyphIndex, 1, EBreakCondition::None });
				}
				else {}
			}

			if (ScriptClusters.Size > 0)
			{
				FScriptCluster & LastCluster = ScriptClusters.Back();
				LastCluster.GlyphLength = ShapeRun.GlyphIndex + ShapeRun.GlyphLength - LastCluster.GlyphIndex;
				LastCluster.TextLength = ShapeRun.TextIndex + ShapeRun.TextLength - LastCluster.TextIndex;
			}


			ShapeRun.ClusterLength = uint32(ScriptClusters.Size) - ShapeRun.ClusterIndex;
		}

		for (uintx ClusterIndex = 0; ClusterIndex < ScriptClusters.Size; ++ClusterIndex)
		{
			FScriptCluster & Cluster = ScriptClusters[ClusterIndex];

			for (uint32 GlyphIndex = Cluster.GlyphIndex; GlyphIndex < Cluster.GlyphIndex + Cluster.GlyphLength; ++GlyphIndex)
				Cluster.Advance.Width += GlyphAdvances[GlyphIndex];

#if XIN_DEBUG
			Cluster.String = String.View(Cluster.TextIndex, Cluster.TextLength);
#endif
		}

		{

			//int32 TextIndex = LineBreaker->current();
			//while (TextIndex != icu::BreakIterator::DONE)
			//{
			//	if (TextIndex > 0)
			//	{
			//		AssertExpresion(Utf32IndicesOfUtf8s[TextIndex - 1] != NullIndexU32);

			//		if (Utf8ClusterIndices[TextIndex - 1] != NullIndexU32)
			//		{
			//			int32_t RuleStatus = LineBreaker->getRuleStatus();
			//			ScriptClusters[Utf8ClusterIndices[TextIndex - 1]].BreakCondition = RuleStatus == UBRK_LINE_HARD ? EBreakCondition::Hard : EBreakCondition::Soft;
			//		}
			//	}
			//	TextIndex = LineBreaker->next();
			//}
		}

		{
			//TUniquePtr<icu::BreakIterator> LineBreaker = icu::BreakIterator::createInstance(Shaper.DefaultLocal, UBRK_LINE, Status)
		}

		return true;
	}

	FSkiaShaper::FSkiaShaper(FSkiaGraphics & SkiaGraphics)
	{
		skDefaultFontMgr = SkFontMgr_New_DirectWrite();

		DefaultLocal = icu::Locale::getDefault();
		//std::string StdLocal = std::setlocale(LC_ALL, nullptr);
		//StdLocal = std::locale().name();
		//StdLocal = "zh-CN";
		//DefaultLocal = icu::Locale::createFromName(StdLocal.c_str());

		//UErrorCode ErrorCode;
		//icu::Locale::setDefault(DefaultLocal, ErrorCode);
		//icu::locale_available_init();
	}

	FSkiaShaper::~FSkiaShaper() {}

	IFontFamilyRef FSkiaShaper::CreateFontFamily(FStringV FamilyName)
	{
		return nullptr;
	}

	IFontFaceRef FSkiaShaper::CreateFontFace(const FFontFace & FontFace)
	{
		return FetchFontFace(FontFace);
	}

	ITextBlobRef FSkiaShaper::CreateTextBlob(FStringV String, const FFontFace & FontFace, float32 FontSize, const SizeF & LayoutSize, ETextLayoutFlags LayoutFlags)
	{
		TReferPtr<FSkiaTextBlob> RectTextBlob = new FSkiaTextBlob(*this);
		RectTextBlob->Shape(String, FontFace, FontSize, EReadingDirection::Default);
		RectTextBlob->Layout(LayoutSize);
		return RectTextBlob;
	}

	FSkiaFontFace * FSkiaShaper::FetchFontFace(const FFontFace & FontFace)
	{
		if (auto Iter = FontFaces.Find(FontFace); Iter != FontFaces.End())
			return Iter->Second;


		SkFontStyle skFontStyle(FromFontWeight(FontFace.FontWeight), SkFontStyle::Width::kNormal_Width, FromFontStyle(FontFace.FontStyle));

		sk_sp<SkTypeface> skTypeface = skDefaultFontMgr->matchFamilyStyle((const char *)(FontFace.FaceName.Data), skFontStyle);
		FSkiaFontFaceRef SkiaFontFace = new FSkiaFontFace(FontFace, skFontStyle, skTypeface.get());
		FontFaces.Insert({ FontFace, SkiaFontFace });
		return SkiaFontFace;
	}

	FSkiaFontFace * FSkiaShaper::FetchFontFace(SkTypeface * skTypeface)
	{
		for (auto Pair : FontFaces)
		{
			if (Pair.Second->skTypeface.get() == skTypeface)
				return Pair.Second;
		}

		FFontFace FontFace = skGetFontFace(skTypeface);

		SkFontStyle skFontStyle(FromFontWeight(FontFace.FontWeight), SkFontStyle::Width::kNormal_Width, FromFontStyle(FontFace.FontStyle));
		FSkiaFontFaceRef SkiaFontFace = new FSkiaFontFace(FontFace, skFontStyle, skTypeface);
		FontFaces.Insert({ FontFace, SkiaFontFace });
		return SkiaFontFace;
	}

	FSkiaTextBlob::FSkiaTextBlob(FSkiaShaper & Shaper) : Shaper(Shaper) {}

	void FSkiaTextBlob::Shape(FUStringV String, const FFontFace & FontFace, float32 FontSize, EReadingDirection ReadingDirection)
	{
		TextShape.Shape(String, FontFace, FontSize, ReadingDirection);
	}

	void FSkiaTextBlob::Layout(const SizeF & LayoutSize, ETextLayoutFlags LayoutFlags)
	{
		FTextRectLayouter TextRectLayouter { TextShape, *this };
		TextRectLayouter.Layout(LayoutSize);
	}
}
