#pragma once

#include "RHITypes.h"

namespace Xin::RHI
{
	class RHI_API IRHIPipelineState : public IRHIObject
	{
	public:
		IRHIPipelineState() = default;
	};
	using IRHIPipelineStateRef = TReferPtr<IRHIPipelineState>;


	class RHI_API IRHIIndirectSignature: public IRHIObject
	{
	public:
		IRHIIndirectSignature() = default;
	};
	using IRHIIndirectSignatureRef = TReferPtr<IRHIIndirectSignature>;
}
