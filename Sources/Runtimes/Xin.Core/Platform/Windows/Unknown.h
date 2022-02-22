#pragma once

#include <Unknwnbase.h>

// 'class1' : base-class 'class2' is already a base-class of 'class3'
#pragma warning(disable: 4584)

namespace Xin::Windows
{
	namespace UnknownHelper
	{
		template<typename UnknownT, typename Interface0T, typename ...InterfacesT>
		forceinline HRESULT QueryInterface(UnknownT * Unknown, const IID & InterfaceIID, void ** Object)
		{
			if (__uuidof(Interface0T) == InterfaceIID)
			{
				*Object = static_cast<Interface0T *>(Unknown);
				return S_OK;
			}

			if constexpr (sizeof...(InterfacesT) == 0)
				return E_NOINTERFACE;
			else
				return UnknownHelper::QueryInterface<UnknownT, InterfacesT...>(Unknown, InterfaceIID, Object);
		}
	}

	template<typename ...InterfacesT>
	class TUnknown : public InterfacesT...
	{
	public:
		TUnknown() = default;
		TUnknown(const TUnknown &) = default;
		TUnknown(TUnknown &&) noexcept = default;
		TUnknown & operator = (const TUnknown &) = default;
		virtual ~TUnknown() = default;

		HRESULT QueryInterface(const IID & InterfaceIID, void ** Object) override
		{
			return UnknownHelper::QueryInterface<TUnknown, InterfacesT...>(this, InterfaceIID, Object);;
		}

		ulong32 AddRef() override
		{
			return Atomics::IncFetch(ReferCount);
		}

		ulong32 Release() override
		{
			if (ReferCount == 0)
				throw EError::State;

			return Atomics::DecFetch(ReferCount);
		}

	public:
		ulong32 ReferCount = 1;
	};
}
