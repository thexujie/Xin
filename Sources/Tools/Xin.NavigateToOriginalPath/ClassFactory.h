#pragma once

template<typename CommandVerbT>
HRESULT TCreateVerbInstance(const IID & riid, void ** ppvObject)
{
	CommandVerbT * pVerb = new (std::nothrow) CommandVerbT();
	HRESULT hResult = pVerb ? S_OK : E_OUTOFMEMORY;
	if (SUCCEEDED(hResult))
	{
		pVerb->QueryInterface(riid, ppvObject);
		pVerb->Release();
	}
	return hResult;
}

class CClassFactory final : public Xin::Win32::TUnknown<IClassFactory>
{
public:
	template<typename T>
	CClassFactory(Xin::TDummy<T>) noexcept
	{
		CreateVerbInstance = TCreateVerbInstance<T>;
	}
	
	~CClassFactory() override;
	
	ULONG AddRef() override
	{
		return TUnknown<IClassFactory>::AddRef();
	}

	ULONG Release() override
	{
		return TUnknown<IClassFactory>::Release();
	}
	
	HRESULT QueryInterface(const IID & riid, void ** ppvObject) override;

	HRESULT CreateInstance(IUnknown * pUnkOuter, const IID & riid, void ** ppvObject) override;
	HRESULT LockServer(BOOL fLock) override;

private:
	HRESULT(*CreateVerbInstance)(const IID & riid, void ** ppvObject) = nullptr;
};

