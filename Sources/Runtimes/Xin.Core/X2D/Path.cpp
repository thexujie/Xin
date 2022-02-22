#include "PCH.h"
#include "Path.h"

namespace Xin
{
	/**
	 * @link https://yqnn.github.io/svg-path-editor/
	 * @param String 
	 * @param Path 
	 * @return 
	 */
	uintx IPath::ParsePathCommands(FStringV String, IPath & Path)
	{
		EPathCommand CurrentCommand = EPathCommand::None;
		bool RelativeCoordinate = false;
		uintx CommandCount = 0;
		uintx CharIndex = 0;

		auto SkipWhitespaces = [&]
			{
				while (CharIndex < String.Size)
				{
					if (String[CharIndex] != ' ')
						break;
					++CharIndex;
				}
			};

		auto TestCommandChar = [](char8 Ch) -> TPair<EPathCommand, bool>
			{
				switch (Ch)
				{
				case 'M': return { EPathCommand::Move, false };
				case 'm': return { EPathCommand::Move, true };
				case 'L': return { EPathCommand::Line, false };
				case 'l': return { EPathCommand::Line, true };
				case 'H': return { EPathCommand::HorizontalLine, false };
				case 'h': return { EPathCommand::HorizontalLine, true };
				case 'V': return { EPathCommand::VerticalLine, false };
				case 'v': return { EPathCommand::VerticalLine, true };
				case 'C': return { EPathCommand::CubicBezier, false };
				case 'c': return { EPathCommand::CubicBezier, true };
				case 'Q': return { EPathCommand::QuadraticBezier, false };
				case 'q': return { EPathCommand::QuadraticBezier, true };
				case 'S': return { EPathCommand::SmoothCubicBezier, false };
				case 's': return { EPathCommand::SmoothCubicBezier, true };
				case 'T': return { EPathCommand::SmoothQuadraticBezier, false };
				case 't': return { EPathCommand::SmoothQuadraticBezier, true };
				case 'A': return { EPathCommand::Arc, false };
				case 'a': return { EPathCommand::Arc, true };
				case 'Z': return { EPathCommand::Close, false };
				case 'z': return { EPathCommand::Close, false };
				default: return { EPathCommand::None, false };
				}
			};

		auto ReadInt32 = [&](int32 & Result) -> bool
			{
				SkipWhitespaces();
				uintx FirstIndex = CharIndex;
				for (; CharIndex < String.Size; ++CharIndex)
				{
					char8 Ch = String[CharIndex];
					if ((CharIndex == FirstIndex)
						&& (Ch == '-') || (Ch == '.'))
						continue;

					if ('0' <= Ch && Ch <= '9')
						continue;

					break;
				}

				if (CharIndex == FirstIndex)
					return false;

				Result = Strings::ToInt32(String.SubView(FirstIndex, CharIndex - FirstIndex));
				return true;
			};

		auto ReadFloat = [&](float32 & Result) -> bool
			{
				SkipWhitespaces();
				uintx FirstIndex = CharIndex;
				for (; CharIndex < String.Size; ++CharIndex)
				{
					char8 Ch = String[CharIndex];
					if ((CharIndex == FirstIndex)
						&& (Ch == '-') || (Ch == '.'))
						continue;

					if ('0' <= Ch && Ch <= '9')
						continue;

					break;
				}

				if (CharIndex == FirstIndex)
					return false;

				Result = Strings::ToFloat32(String.SubView(FirstIndex, CharIndex - FirstIndex));
				return true;
			};

		auto ReadFloat2 = [&](PointF & Point) -> bool
			{
				return ReadFloat(Point.X) && ReadFloat(Point.Y);
			};

		PointF LastPoint = PointF::Zero;
		TArray<int32, 16> Ints;
		TArray<float32, 16> Floats;
		TArray<PointF, 16> Points;
		bool Figuring = false;

		while (CharIndex < String.Size)
		{
			SkipWhitespaces();
			char8 Ch = String[CharIndex];

			auto [TestCommand, TestRelative] = TestCommandChar(Ch);
			if (TestCommand != EPathCommand::None)
			{
				CurrentCommand = TestCommand;
				RelativeCoordinate = TestRelative;
				++CharIndex;
			}

			bool ParseSuccess = false;
			switch (CurrentCommand)
			{
			case EPathCommand::Close:
			{
				ParseSuccess = true;
				Path.EndFigure(TestRelative);
				Figuring = false;
			}
			break;
			case EPathCommand::Move:
				if (ReadFloat2(Points[0]))
				{
					if (RelativeCoordinate)
						Points[0] += LastPoint;
					LastPoint = Points[0];
					ParseSuccess = true;
					if (Figuring)
						Path.EndFigure(false);

					Path.BeginFigure(Points[0]);
					Figuring = true;
					//SvgPathSink.AddMove(RelativeCoordinate, Vec2s[0]);
				}
				break;
			case EPathCommand::Line:
				if (ReadFloat2(Points[0]))
				{
					if (RelativeCoordinate)
						Points[0] += LastPoint;
					LastPoint = Points[0];

					ParseSuccess = true;
					Path.AddLine(Points[0]);
					//SvgPathSink.AddLine(RelativeCoordinate, Vec2s[0]);
				}
				break;
			case EPathCommand::HorizontalLine:
				if (ReadFloat(Floats[0]))
				{
					LastPoint.X = RelativeCoordinate ? (LastPoint.X + Floats[0]) : Floats[0];
					ParseSuccess = true;
					Path.AddLine(LastPoint);
					//SvgPathSink.AddLineX(RelativeCoordinate, Floats[0]);
				}
				break;
			case EPathCommand::VerticalLine:
				if (ReadFloat(Floats[0]))
				{
					LastPoint.Y = RelativeCoordinate ? (LastPoint.Y + Floats[0]) : Floats[0];
					ParseSuccess = true;
					Path.AddLine(LastPoint);
					//SvgPathSink.AddLineY(RelativeCoordinate, Floats[0]);
				}
				break;
			case EPathCommand::CubicBezier:
				if (ReadFloat2(Points[0])
					&& ReadFloat2(Points[1])
					&& ReadFloat2(Points[2]))
				{
					if (RelativeCoordinate)
					{
						Points[0] += LastPoint;
						Points[1] += LastPoint;
						Points[2] += LastPoint;
					}
					LastPoint = Points[2];
					ParseSuccess = true;
					Path.AddBezier(Points[0], Points[1], Points[2]);
				}
				break;
			case EPathCommand::SmoothCubicBezier:
				if (ReadFloat2(Points[0])
					&& ReadFloat2(Points[1]))
				{
					if (RelativeCoordinate)
					{
						Points[0] += LastPoint;
						Points[1] += LastPoint;
					}
					PointF S = LastPoint;
					LastPoint = Points[1];

					PointF & ControlPoint2 = Points[0];
					PointF & EndPoint = Points[1];

					ParseSuccess = true;
					{
						Vec2F ES = S - EndPoint;
						Vec2F EC2 = ControlPoint2 - EndPoint;

						// C2xC2 = EC2 - EC2x
						Vec2F ES_nor = ES.Normalized();
						float32 EC2x_len = EC2.Dot(ES_nor);
						Vec2F EC2x = ES_nor * EC2x_len;
						Vec2F C2xC2 = EC2 - EC2x;

						// SC1 = C1xC1 - C1xS;
						const Vec2F & C1xC1 = C2xC2;
						const Vec2F & C1xS = EC2x;
						Vec2F SC1 = C1xC1 - C1xS;

						Vec2F ControlPoint1 = S + SC1;

						Path.AddBezier(ControlPoint1, ControlPoint2, EndPoint);
					}
				}
				break;
			case EPathCommand::Arc:
				// A size rotationAngle isLargeArcFlag sweepDirectionFlag endPoint
				if (ReadFloat2(Points[0]) // size
					&& ReadFloat(Floats[0]) // rotationAngle
					&& ReadInt32(Ints[0]) // isLargeArcFlag
					&& ReadInt32(Ints[1]) // sweepDirectionFlag
					&& ReadFloat2(Points[1])) // endPoint
				{
					if (RelativeCoordinate)
					{
						//Points[0] += LastPoint;
						Points[1] += LastPoint;
					}
					LastPoint = Points[1];

					ParseSuccess = true;
					Path.AddArc(Points[0], Floats[0], Points[1], !!Ints[0], !Ints[1]);
				}
				break;
			default:
				AssertExpr(false);
				break;
			}

			if (ParseSuccess)
				++CommandCount;
			else
				break;
		}

		return CommandCount;
	}
}
