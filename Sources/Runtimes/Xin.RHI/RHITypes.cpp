#include "PCH.h"
#include "RHITypes.h"

namespace Xin::RHI
{
	const FClearValue FClearValue::None;
	const FDepthStencilState FDepthStencilState::Disabled { EComparison::None, false };
	const FDepthStencilState FDepthStencilState::Default;
	const FDepthStencilState FDepthStencilState::DefaultReversedZ { EComparison::GreaterEqual, true};
	const FRasterizeState FRasterizeState::Default;
	const FRasterizeState FRasterizeState::DefaultWireframe =
	{
		.Wireframe = true
	};

	const FBlendMethod FBlendMethod::Default;
	const FBlendMethod FBlendMethod::Transparent = 
	{
		true,
		EBlendFactor::SrcAlpha, EBlendFactor::SrcAlphaInv, EBlendAlgorithm::Add,
		EBlendFactor::One, EBlendFactor::SrcAlphaInv, EBlendAlgorithm::Add,
		EWriteMasks::All
	};
	const FBlendMethod FBlendMethod::Premultiplied =
	{
		true,
		EBlendFactor::One, EBlendFactor::SrcAlphaInv, EBlendAlgorithm::Add,
		EBlendFactor::One, EBlendFactor::SrcAlphaInv, EBlendAlgorithm::Add,
		EWriteMasks::All
	};

	const FBlendState FBlendState::Default;
	const FBlendState FBlendState::Transparent = 
	{
		false, false, { 1.0f, 1.0f, 1.0f, 1.0f },
		{
			FBlendMethod::Transparent,
			FBlendMethod::Transparent,
			FBlendMethod::Transparent,
			FBlendMethod::Transparent,
			FBlendMethod::Transparent,
			FBlendMethod::Transparent,
			FBlendMethod::Transparent,
			FBlendMethod::Transparent,
		}
	};
	const FBlendState FBlendState::Premultiplied =
	{
		false, false, { 1.0f, 1.0f, 1.0f, 1.0f },
		{
			FBlendMethod::Premultiplied,
			FBlendMethod::Premultiplied,
			FBlendMethod::Premultiplied,
			FBlendMethod::Premultiplied,
			FBlendMethod::Premultiplied,
			FBlendMethod::Premultiplied,
			FBlendMethod::Premultiplied,
			FBlendMethod::Premultiplied,
		}
	};
}
