#include "PCH.h"
#include "ClassFactory.h"

#include "NavigateToOriginalPathCommandVerb.h"

CClassFactory::~CClassFactory()
{
	Xin::Atomics::DecFetch(DllRefCount);
}

HRESULT CClassFactory::QueryInterface(const IID & riid, void ** ppvObject)
{
    static const QITAB Table[] =
    {
        QITABENT(CClassFactory, IClassFactory),
        { 0 }
    };
    return QISearch(this, Table, riid, ppvObject);
}


HRESULT CClassFactory::CreateInstance(IUnknown * pUnkOuter, const IID & riid, void ** ppvObject)
{
    return pUnkOuter ? CLASS_E_NOAGGREGATION : CreateVerbInstance(riid, ppvObject);
}

HRESULT CClassFactory::LockServer(BOOL fLock)
{
    if (fLock) Xin::Atomics::IncFetch(DllRefCount);
    else Xin::Atomics::DecFetch(DllRefCount);
    return S_OK;
}

