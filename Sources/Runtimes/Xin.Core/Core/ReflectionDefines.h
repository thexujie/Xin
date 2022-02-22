#pragma once

#ifndef NONE_API
#define NONE_API

#define PropertyRW(Type, Reader, Writer) __declspec(property(get = Reader, put = Writer)) Type
#define PropertyR(Type, Reader) __declspec(property(get = Reader)) Type
#define PropertyW(Type, Writer) __declspec(property(put = Writer)) Type


#define XinClass(...) static_assert(true)
#define XinFunction(...) static_assert(true)
#define XinProperty(...) static_assert(true)

namespace Xin
{
	template<typename T>
	class TEnableReflection
	{
	};
}

#endif
