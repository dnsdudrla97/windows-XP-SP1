//***************************************************************************
//
//  Copyright (c) 1997-1999 Microsoft Corporation
//
//  File:  factory.cpp
//
//	Description :
//			Class factory implementation for the command-line event
//			consumer provider
//
//  History:	
//
//***************************************************************************

#include "stdafx.h"
#include "factory.h"
#include "Provider.h"

CProviderFactory::CProviderFactory(CListBox	*pOutputList)
{
	m_cRef = 0L;
	m_pOutputList = pOutputList;
}

CProviderFactory::~CProviderFactory()
{
}

//IUnknown methods
STDMETHODIMP CProviderFactory::QueryInterface(REFIID riid, LPVOID FAR *ppv)
{
    *ppv=NULL;

    if (riid == IID_IUnknown || riid == IID_IClassFactory)
        *ppv=this;

    if (*ppv != NULL)
    {
        ((LPUNKNOWN)*ppv)->AddRef();
        return NOERROR;
    }

    return E_NOINTERFACE;
}


STDMETHODIMP_(ULONG) CProviderFactory::AddRef(void)
{
    return InterlockedIncrement(&m_cRef);
}


STDMETHODIMP_(ULONG) CProviderFactory::Release(void)
{
    int lNewRef = InterlockedDecrement(&m_cRef);
    if(lNewRef == 0)
    {
        delete this;
    }

    return lNewRef;
}

//+---------------------------------------------------------------------------
//
//  Function:   CProviderFactory::CreateInstance
//
//  Synopsis:
//
//  Arguments:  [pUnkOuter]
//              [iid]
//              [ppv]
//
//  Returns:    HRESULT
//
//  Modifies:
//
//----------------------------------------------------------------------------
STDMETHODIMP
CProviderFactory::CreateInstance(IUnknown *pUnkOuter, REFIID riid, LPVOID *ppv)
{
	HRESULT hr;
    CProvider *pProvider = NULL;

    if (pUnkOuter)
        return E_FAIL;

	// create the provider.
    pProvider = new CProvider(m_pOutputList);

    if (pProvider == NULL)
	{
		return E_FAIL;
	}

	// get the interface asked for.
    if (pProvider)
    {
        hr = pProvider->QueryInterface(riid, ppv);
    }
    else
    {
        *ppv = NULL;
        return E_OUTOFMEMORY;
    }

    return NOERROR;
}

//+---------------------------------------------------------------------------
//
//  Function:   CProviderFactory::LockServer
//
//  Synopsis:
//
//  Arguments:  [fLock]
//
//  Returns:    HRESULT
//
//  Modifies:
//
//----------------------------------------------------------------------------
STDMETHODIMP CProviderFactory::LockServer(BOOL fLock)
{
    if (fLock)
        m_cRef++;
    else
        m_cRef--;

    return NOERROR;
}

