#include "PCH.h"
#include "Types_Allocator.inl"
#include <crtdbg.h>
#include <stacktrace>

#include "Basic/Formatter.h"
#include "Platform/Platform.h"
#include "System/Console.h"

#define XIN_DEBUG_MALLOC 0

#define DEBUG_MALLOC 0

template <typename T, typename... Rest>
static void hash_combine(std::size_t& seed, const T& v, const Rest&... rest)
{
	seed ^= std::hash<T>{}(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	(hash_combine(seed, rest), ...);
}

template <>
struct std::hash<std::stacktrace>
{
	size_t operator()(const std::stacktrace & StackTrace) noexcept
	{
		size_t Seed = 0x9e3779b9;
		hash_combine(Seed, StackTrace.size());
		for (const std::stacktrace_entry & entry : StackTrace)
			hash_combine(Seed, entry.source_file(), entry.source_line());
		return Seed;
	}
};

namespace Xin
{
#if DEBUG_MALLOC
	static std::unique_ptr<std::map<size_t, std::stacktrace>> GMallocStack;
	static std::unique_ptr<std::map<size_t, intx>> GMallocCounter;
	static std::unique_ptr<std::multimap<size_t, void *>> GMallocInstances;
#endif

#if !XIN_PUBLISH

	uint64 Hash(const std::stacktrace & StackTrace)
	{
		return 0;
	}

	void * Malloc(uintx Size, uintx Alignment)
	{
#if XIN_DEBUG_MALLOC
		auto StackTrace = std::stacktrace::current();
		auto StackFrame = StackTrace[StackOffset + 1];
		auto SourceFile = StackFrame.source_file();
		auto SourceLine = StackFrame.source_line();
		assert(Size > 0);
		void * Block;
		if (Alignment > __STDCPP_DEFAULT_NEW_ALIGNMENT__)
			//Block = ::_aligned_malloc_dbg(Size, Alignment, __FUNCTION__, __LINE__);
		Block = ::_malloc_dbg(Size, _CLIENT_BLOCK, __FUNCTION__, __LINE__);
		else
			Block = ::_malloc_dbg(Size, _CLIENT_BLOCK, __FUNCTION__, __LINE__);
		assert(Block != nullptr);
#else
		assert(Size > 0);
		void * Block;
		if (Alignment > __STDCPP_DEFAULT_NEW_ALIGNMENT__)
			Block = ::_aligned_malloc(Size, Alignment);
		else
			Block = ::malloc(Size);

		if (Block == nullptr)
		{
			FConsole::WriteLine(Format(u8"Malloc {} bytes with alignment {} failed."V, Size, Alignment));
			OutputDebugStringW(Format(L"Malloc {} bytes with alignment {} failed."V, Size, Alignment));
		}
		assert(Block != nullptr);
#endif

#if DEBUG_MALLOC
		if (!GMallocStack)
			GMallocStack = std::make_unique<std::map<size_t, std::stacktrace>>();
		if (!GMallocCounter)
			GMallocCounter = std::make_unique<std::map<size_t, intx>>();
		if (!GMallocInstances)
			GMallocInstances = std::make_unique<std::multimap<size_t, void *>>();
		auto StackFrame = std::stacktrace::current();
		size_t StackId = std::hash<std::stacktrace>()(StackFrame);
		if ((*GMallocCounter).contains(StackId))
			++(*GMallocCounter)[StackId];
		else
			(*GMallocCounter)[StackId] = 1;
		assert((*GMallocCounter)[StackId] < 0xffff);

		(*GMallocStack).insert_or_assign(StackId, StackFrame);
		(*GMallocInstances).insert({ StackId, Block });
#endif
		static int BlockIndex = 0;
		++BlockIndex;
		return Block;
	}

	void Free(void * Block, uintx Alignment)
	{
		if (!Block)
			return;

#if DEBUG_MALLOC
		size_t Count = std::erase_if(*GMallocInstances, [&](const auto & Pair)
		{
			auto [StackId, B] = Pair;
			if (B == Block)
			{
				assert((*GMallocCounter).contains(StackId));
				assert((*GMallocCounter)[StackId] > 0);
				--(*GMallocCounter)[StackId];
				return true;
			}
			return false;
		});
		assert(Count == 1);
#endif

#if XIN_DEBUG_MALLOC
		if (Alignment > __STDCPP_DEFAULT_NEW_ALIGNMENT__)
			::_aligned_free_dbg(Block);
		else
			::_free_dbg(Block, _CLIENT_BLOCK);
#else
		if (Alignment > __STDCPP_DEFAULT_NEW_ALIGNMENT__)
			::_aligned_free(Block);
		else
			::free(Block);
#endif
	}
#endif
#if _XIN_DEBUG_POINTERS
	//std::mutex * GDebugPointers_mutex = nullptr;
	//std::map<void *, uintx> * GDebugPointers = nullptr;
	//std::map<void *, uintx> * GDebugPointers_History = nullptr;
	
	static std::mutex * GDebugPointers_mutex = new std::mutex();
	static std::map<void *, uintx> * GDebugPointers = new std::map<void *, uintx>();
	static std::map<void *, uintx> * GDebugPointers_History = new std::map<void *, uintx>();

	void GDebugPointers_Deleter()
	{
		delete GDebugPointers_mutex;
		delete GDebugPointers;
		delete GDebugPointers_History;

		GDebugPointers = nullptr;
	}
	struct GDebugPointers_AtExit
	{
		GDebugPointers_AtExit()
		{
			std::atexit(GDebugPointers_Deleter);
		}
	}GDebugPointers_AtExit_Instance;
#endif
//	
//	void * Malloc(uintx Size, uintx Alignment)
//	{
//		assert(Size > 0);
//		void * Result = nullptr;
//		if (Alignment > __STDCPP_DEFAULT_NEW_ALIGNMENT__)
//			Result = ::_aligned_malloc(Size, Alignment);
//		else
//			Result = ::malloc(Size);
//
//		Assert(Result != nullptr);
//#if _XIN_DEBUG_POINTERS
//		if (GDebugPointers)
//		{
//			std::unique_lock lock(*GDebugPointers_mutex);
//			GDebugPointers->insert_or_assign(Result, Alignment);
//		}
//#endif
//		return Result;
//	}

//	void Free(void * Address, uintx Alignment)
//	{
//		if (!Address)
//			return;
//
//#if _XIN_DEBUG_POINTERS
//		if (GDebugPointers)
//		{
//			std::unique_lock lock(*GDebugPointers_mutex);
//			if (!GDebugPointers->contains(Address))
//				_wassert(L"Not contains", nullptr, 0);
//			if (Alignment && GDebugPointers->at(Address) != Alignment)
//				_wassert(L"Invalid alignment", nullptr, 0);
//			GDebugPointers->erase(GDebugPointers->find(Address));
//		}
//#endif
//		if (Alignment > __STDCPP_DEFAULT_NEW_ALIGNMENT__)
//			::_aligned_free(Address);
//		else
//			::free(Address);
//	}
}
