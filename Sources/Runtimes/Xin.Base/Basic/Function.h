#pragma once

#include "Basic.Types.h"
#include "Array.h"
#include "Tuple.h"
#include "Object.h"
#include "Callable.h"

namespace Xin
{
	template<typename CallableT, typename ArgsTupleT, uintx... Indices>
	constexpr auto ApplyImpl(CallableT && Callable, ArgsTupleT && ArgsTuple, IndexSequence<Indices...>)
	{
		return Invoke(Forward<CallableT>(Callable), Get<Indices>(ArgsTuple)...);
	}


	template<typename CallableT, typename ArgsTupleT>
	constexpr auto Apply(CallableT && Callable, ArgsTupleT && ArgsTuple)
	{
		return ApplyImpl(Forward<CallableT>(Callable), Forward<ArgsTupleT>(ArgsTuple), MakeIndexSequence<TupleSizeV<RemoveRefT<ArgsTupleT>>> {});
	}

	class IInvoker
	{
	public:
		static constexpr uintx SmallStorageFullSizeInPointers = 8;
		static constexpr uintx SmallStorageSizeInPointers = SmallStorageFullSizeInPointers - 1;
		static constexpr uintx SmallStorageSizeInBytes = SmallStorageSizeInPointers * sizeof(void *);
		using SmallStorageT = TArray<void *, SmallStorageSizeInPointers>;

		template<typename InvokerT>
		static constexpr bool IsSmallStorage = sizeof(InvokerT) <= SmallStorageSizeInBytes && alignof(InvokerT) <= alignof(std::max_align_t);

	public:
		virtual ~IInvoker() = default;
		virtual void Delete() = 0;
	};

	template<typename ReturnT, typename... ParameterTs>
	class TInvoker : public IInvoker
	{
	public:
		virtual ReturnT Invoke(ParameterTs... Parameters) = 0;
		virtual TInvoker * CloneCopy(SmallStorageT & SmallStorage) const = 0;
		virtual TInvoker * MoveCopy(SmallStorageT & SmallStorage) = 0;
	};

	template<typename CallableT, typename ReturnT, typename... ParameterTs>
	class TFunctorInvoker : public TInvoker<ReturnT, ParameterTs...>
	{
	public:
		using InvokerT = TInvoker<ReturnT, ParameterTs...>;

	public:
		TFunctorInvoker(CallableT && Callable) : Callable(Move(Callable)) { }
		TFunctorInvoker(const TFunctorInvoker & That) = default;
		TFunctorInvoker(TFunctorInvoker && That) noexcept = default;

		InvokerT * CloneCopy(IInvoker::SmallStorageT & SmallStorage) const override
		{
			if constexpr (IInvoker::IsSmallStorage<TFunctorInvoker>)
			{
				TFunctorInvoker * Invoker = reinterpret_cast<TFunctorInvoker *>(&SmallStorage);
				new(Invoker) TFunctorInvoker(*this);
				return Invoker;
			}
			else
				return new TFunctorInvoker(*this);
		}

		InvokerT * MoveCopy(IInvoker::SmallStorageT & SmallStorage) override
		{
			if constexpr (IInvoker::IsSmallStorage<TFunctorInvoker>)
			{
				TFunctorInvoker * Invoker = reinterpret_cast<TFunctorInvoker *>(SmallStorage.Data);
				new(Invoker) TFunctorInvoker(Move(*this));
				return Invoker;
			}
			else
				return new TFunctorInvoker(Move(*this));
		}

		void Delete() override
		{
			if constexpr (IInvoker::IsSmallStorage<TFunctorInvoker>)
				this->~TFunctorInvoker();
			else
				delete this;
		}

		ReturnT Invoke(ParameterTs... Parameters) override
		{
			return Xin::Invoke(Callable, Forward<ParameterTs>(Parameters)...);
		}

	public:
		CallableT Callable;
	};

	template<typename InstanceT, typename CallableT, typename ReturnT, typename... ParameterTs>
	class TInstanceInvoker : public TInvoker<ReturnT, ParameterTs...>
	{
	public:
		using InvokerT = TInvoker<ReturnT, ParameterTs...>;

	public:
		TInstanceInvoker(InstanceT * Instance, CallableT && Callable) : Instance(Instance), Callable(Move(Callable)) { }
		TInstanceInvoker(const TInstanceInvoker & That) noexcept = default;
		TInstanceInvoker(TInstanceInvoker && That) noexcept : Instance(Move(That.Instance)), Callable(Move(That.Callable)) { That.Instance = nullptr; }

		InvokerT * CloneCopy(IInvoker::SmallStorageT & SmallStorage) const override
		{
			if constexpr (IInvoker::IsSmallStorage<TInstanceInvoker>)
			{
				TInstanceInvoker * Invoker = reinterpret_cast<TInstanceInvoker *>(&SmallStorage);
				new(Invoker) TInstanceInvoker(*this);
				return Invoker;
			}
			else
				return new TInstanceInvoker(*this);
		}

		InvokerT * MoveCopy(IInvoker::SmallStorageT & SmallStorage) override
		{
			if constexpr (IInvoker::IsSmallStorage<TInstanceInvoker>)
			{
				TInstanceInvoker * Invoker = reinterpret_cast<TInstanceInvoker *>(SmallStorage.Data);
				new(Invoker) TInstanceInvoker(Move(*this));
				return Invoker;
			}
			else
				return new TInstanceInvoker(Move(*this));
		}

		void Delete() override
		{
			if constexpr (IInvoker::IsSmallStorage<TInstanceInvoker>)
				this->~TInstanceInvoker();
			else
				delete this;
		}

		ReturnT Invoke(ParameterTs... Parameters) override
		{
			return Xin::Invoke(Callable, Instance, Forward<ParameterTs>(Parameters)...);
		}

	public:
		InstanceT * Instance = nullptr;
		CallableT Callable;
	};

	template<DerivedOfC<IObject> ObjectT, typename CallableT, typename ReturnT, typename... ParameterTs>
	class TObjectInvoker : public TInvoker<ReturnT, ParameterTs...>
	{
	public:
		using InvokerT = TInvoker<ReturnT, ParameterTs...>;

	public:
		TObjectInvoker(ObjectT * Object, CallableT && Callable) : Object(Object), Callable(Move(Callable)) { }
		TObjectInvoker(const TObjectInvoker & That) = default;
		TObjectInvoker(TObjectInvoker && That) noexcept = default;

		InvokerT * CloneCopy(IInvoker::SmallStorageT & SmallStorage) const override
		{
			if constexpr (IInvoker::IsSmallStorage<TObjectInvoker>)
			{
				TObjectInvoker * Invoker = reinterpret_cast<TObjectInvoker *>(&SmallStorage);
				new(Invoker) TObjectInvoker(*this);
				return Invoker;
			}
			else
				return new TObjectInvoker(*this);
		}

		InvokerT * MoveCopy(IInvoker::SmallStorageT & SmallStorage) override
		{
			if constexpr (IInvoker::IsSmallStorage<TObjectInvoker>)
			{
				TObjectInvoker * Invoker = reinterpret_cast<TObjectInvoker *>(SmallStorage.Data);
				new(Invoker) TObjectInvoker(Move(*this));
				return Invoker;
			}
			else
				return new TObjectInvoker(Move(*this));
		}

		void Delete() override
		{
			if constexpr (IInvoker::IsSmallStorage<TObjectInvoker>)
				this->~TObjectInvoker();
			else
				delete this;
		}

		ReturnT Invoke(ParameterTs... Arguments) override
		{
			if (TReferPtr<ObjectT> Pointer = Object.Lock())
				return Xin::Invoke(Callable, Pointer.Get(), Forward<ParameterTs>(Arguments)...);
			else
				return ReturnT {};
		}

	public:
		TEntryPtr<ObjectT> Object;
		CallableT Callable;
	};

	template<DerivedOfC<IObject> ObjectT, typename LambdaT, typename ReturnT, typename... ParameterTs>
	class TObjectLambda: public TInvoker<ReturnT, ParameterTs...>
	{
	public:
		using InvokerT = TInvoker<ReturnT, ParameterTs...>;

	public:
		TObjectLambda(ObjectT * Object, LambdaT && Callable) : Object(Object), Callable(Move(Callable)) { }
		TObjectLambda(const TObjectLambda & That) = default;
		TObjectLambda(TObjectLambda && That) noexcept = default;

		InvokerT * CloneCopy(IInvoker::SmallStorageT & SmallStorage) const override
		{
			if constexpr (IInvoker::IsSmallStorage<TObjectLambda>)
			{
				TObjectLambda * Invoker = reinterpret_cast<TObjectLambda *>(&SmallStorage);
				new(Invoker) TObjectLambda(*this);
				return Invoker;
			}
			else
				return new TObjectLambda(*this);
		}

		InvokerT * MoveCopy(IInvoker::SmallStorageT & SmallStorage) override
		{
			if constexpr (IInvoker::IsSmallStorage<TObjectLambda>)
			{
				TObjectLambda * Invoker = reinterpret_cast<TObjectLambda *>(SmallStorage.Data);
				new(Invoker) TObjectLambda(Move(*this));
				return Invoker;
			}
			else
				return new TObjectLambda(Move(*this));
		}

		void Delete() override
		{
			if constexpr (IInvoker::IsSmallStorage<TObjectLambda>)
				this->~TObjectLambda();
			else
				delete this;
		}

		ReturnT Invoke(ParameterTs... Arguments) override
		{
			if (TReferPtr<ObjectT> Pointer = Object.Lock())
				return Xin::Invoke(Callable, Forward<ParameterTs>(Arguments)...);
			else
				return ReturnT { };
		}

	public:
		TEntryPtr<ObjectT> Object;
		LambdaT Callable;
	};

	template<typename T>
	struct TFunction
	{
		static_assert(FalseTypeV<T>, "TCallable does not accept non-function types as template arguments.");
	};

	template<typename ReturnT, typename... ParameterTs>
	struct TFunction<ReturnT(ParameterTs...)>
	{
	public:
		using ReturnType = ReturnT;

		template<sizet ParameterIndex>
		using ParameterType = TPackIndexing<ParameterIndex, ParameterTs...>;

	public:
		using InvokerT = TInvoker<ReturnT, ParameterTs...>;

	public:
		TFunction() noexcept = default;
		TFunction(NullptrT) noexcept {}

		auto operator ==(const TFunction & That) const noexcept
		{
			if (IsSmallStorage())
				return That.IsSmallStorage() && SmallStorage == That.SmallStorage;
			else
				return Invoker == That.Invoker;
		}

		auto operator <=>(const TFunction & That) const noexcept
		{
			// Use SmallStorage
			if (static_cast<void *>(That.Invoker) == That.SmallStorage.Data)
				return SmallStorage <=> That.SmallStorage;
			else
				return Invoker <=> That.Invoker;
		}

		TFunction(const TFunction & That) noexcept
		{
			if (That.Invoker)
				Invoker = That.Invoker->CloneCopy(SmallStorage);
		}

		TFunction(TFunction && That) noexcept
		{
			if (That.Invoker)
			{
				Invoker = That.Invoker->MoveCopy(SmallStorage);
				That.Invoker->Delete();
				That.Invoker = nullptr;
				That.SmallStorage = {};
			}
		}

		template<typename CallableT> requires (InvocableC<DecayT<CallableT>, ParameterTs...> && !SameC<RemoveCVRefT<CallableT>, TFunction>)
		TFunction(CallableT && Callable) noexcept
		{
			using InvokerT = TFunctorInvoker<DecayT<CallableT>, ReturnT, ParameterTs...>;
			if constexpr (IInvoker::IsSmallStorage<InvokerT>)
			{
				Invoker = reinterpret_cast<InvokerT *>(SmallStorage.Data);
				new(Invoker) InvokerT(Forward<DecayT<CallableT>>(Callable));
			}
			else
				Invoker = new InvokerT(Forward<DecayT<CallableT>>(Callable));
		}

		template<typename InstanceT, typename CallableT>
			requires InvocableC<DecayT<CallableT>, InstanceT, ParameterTs...> && ConvertibleToC<InvokeResultT<CallableT, InstanceT, ParameterTs...>, ReturnT>
		TFunction(InstanceT * Instance, CallableT && Callable) noexcept
		{
			using CallableInvokerT = TInstanceInvoker<InstanceT, DecayT<CallableT>, ReturnT, ParameterTs...>;
			if constexpr (IInvoker::IsSmallStorage<CallableInvokerT>)
			{
				Invoker = reinterpret_cast<CallableInvokerT *>(SmallStorage.Data);
				new(Invoker) CallableInvokerT(Instance, Forward<DecayT<CallableT>>(Callable));
			}
			else if (IsDerivedOfV<InstanceT, IObject>)
				Invoker = new CallableInvokerT(Instance, Forward<DecayT<CallableT>>(Callable));
		}

		template<DerivedOfC<IObject> ObjectT, typename ThisCallableT>
			requires InvocableC<DecayT<ThisCallableT>, ObjectT, ParameterTs...> && ConvertibleToC<InvokeResultT<ThisCallableT, ObjectT, ParameterTs...>, ReturnT>
		TFunction(ObjectT * Object, ThisCallableT && ThisCallable) noexcept
		{
			using CallableInvokerT = TObjectInvoker<ObjectT, DecayT<ThisCallableT>, ReturnT, ParameterTs...>;
			if constexpr (IInvoker::IsSmallStorage<CallableInvokerT>)
			{
				Invoker = reinterpret_cast<CallableInvokerT *>(SmallStorage.Data);
				new(Invoker) CallableInvokerT(Object, Forward<DecayT<ThisCallableT>>(ThisCallable));
			}
			else if (IsDerivedOfV<ObjectT, IObject>)
				Invoker = new CallableInvokerT(Object, Forward<DecayT<ThisCallableT>>(ThisCallable));
		}

		template<DerivedOfC<IObject> ObjectT, typename LambdaCallableT>
			requires InvocableC<DecayT<LambdaCallableT>, ParameterTs...> && ConvertibleToC<InvokeResultT<LambdaCallableT, ParameterTs...>, ReturnT>
		TFunction(ObjectT * Object, LambdaCallableT && ThisCallable) noexcept
		{
			using CallableInvokerT = TObjectLambda<ObjectT, DecayT<LambdaCallableT>, ReturnT, ParameterTs...>;
			if constexpr (IInvoker::IsSmallStorage<CallableInvokerT>)
			{
				Invoker = reinterpret_cast<CallableInvokerT *>(SmallStorage.Data);
				new(Invoker) CallableInvokerT(Object, Forward<DecayT<LambdaCallableT>>(ThisCallable));
			}
			else if (IsDerivedOfV<ObjectT, IObject>)
				Invoker = new CallableInvokerT(Object, Forward<DecayT<LambdaCallableT>>(ThisCallable));
		}

		template<DerivedOfC<IObject> ObjectT, typename LambdaCallableT>
			requires InvocableC<DecayT<LambdaCallableT>, ParameterTs...> && ConvertibleToC<InvokeResultT<LambdaCallableT, ParameterTs...>, ReturnT>
		TFunction(TReferPtr<ObjectT> Object, LambdaCallableT && ThisCallable) noexcept
			: TFunction(Object.Get(), Forward<LambdaCallableT>(ThisCallable))
		{
		}

		~TFunction()
		{
			if (Invoker)
			{
				Invoker->Delete();
				Invoker = nullptr;
				SmallStorage = {};
			}
		}

		template<typename... ArgumentTs>
		ReturnT operator ()(ArgumentTs &&... Arguments) const
		{
			return Invoker->Invoke(Arguments...);
		}

		TFunction & operator =(std::nullptr_t) noexcept
		{
			Reset();
			return *this;
		}

		TFunction & operator =(const TFunction & That) noexcept
		{
			Reset();
			if (That.Invoker)
				Invoker = That.Invoker->CloneCopy(SmallStorage);
			return *this;
		}

		TFunction & operator =(TFunction && That) noexcept
		{
			Reset();
			if (That.Invoker)
			{
				Invoker = That.Invoker->MoveCopy(SmallStorage);
				That.Invoker->Delete();
				That.Invoker = nullptr;
				That.SmallStorage = {};
			}
			return *this;
		}

		bool IsSmallStorage() const noexcept { return static_cast<const void *>(Invoker) == static_cast<const void *>(SmallStorage.Data); }

		operator bool() const { return !!Invoker; }

		void Reset()
		{
			if (Invoker)
			{
				Invoker->Delete();
				Invoker = nullptr;
				SmallStorage = {};
			}
		}

	public:
		InvokerT * Invoker = nullptr;
		TArray<void *, IInvoker::SmallStorageSizeInPointers> SmallStorage {};
	};
}
