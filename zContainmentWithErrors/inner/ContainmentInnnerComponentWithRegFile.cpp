#include<windows.h>
#include "ContainmentInnerComponentWithRegFile.h"

class CMultiplicationDivision : public IMultiplication, IDivision {

private:
	long m_cRef;
public:
	CMultiplicationDivision(void);
	~CMultiplicationDivision(void);

	HRESULT __stdcall QueryInterface(REFIID, void **);
	ULONG __stdcall AddRef(void);
	ULONG __stdcall Release(void);

	HRESULT __stdcall MultiplicationOfTwoIntegers(int, int, int*);
	HRESULT __stdcall DivisionOfTwoIntegers(int, int, int*);

};


class CMultiplicationDivisionClassFactory : public IClassFactory {
private:
	long m_cRef;

public:
	CMultiplicationDivisionClassFactory(void);
	~CMultiplicationDivisionClassFactory(void);

	HRESULT __stdcall QueryInterface(REFIID, void **);
	ULONG __stdcall AddRef(void);
	ULONG __stdcall Release(void);

	HRESULT __stdcall CreateInstance(IUnknown *, REFIID, void **);
	HRESULT __stdcall LockServer(BOOL);
};

long glNumberOfActiveComponents = 0;
long glNumberOfServerLocks = 0;

BOOL WINAPI DllMain(HINSTANCE hDll, DWORD dwReason, LPVOID Reserved) {
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		break;
	case DLL_PROCESS_DETACH:
		break;
	}

	return(TRUE);
}

CMultiplicationDivision::CMultiplicationDivision(void) {
	m_cRef = 1;
	InterlockedIncrement(&glNumberOfActiveComponents);
}

CMultiplicationDivision::~CMultiplicationDivision(void) {
	InterlockedDecrement(&glNumberOfActiveComponents);
}

HRESULT CMultiplicationDivision::QueryInterface(REFIID riid, void **ppv) {

	if (riid == IID_IUnknown)
		*ppv = static_cast<IMultiplication *>(this);
	else if (riid == IID_IMultiplication)
		*ppv = static_cast<IMultiplication *>(this);
	else if (riid == IID_IDivision)
		*ppv = static_cast<IDivision *>(this);
	else {
		*ppv = NULL;
		MessageBox(NULL, TEXT("CMultiplicationDivision -> QI riid not found error"), TEXT("Error"), MB_OK);
		return(E_NOINTERFACE);
	}
	reinterpret_cast<IUnknown *>(*ppv)->AddRef();
	return(S_OK);
}

ULONG CMultiplicationDivision::AddRef(void) {
	InterlockedIncrement(&m_cRef);
	return(m_cRef);
}

ULONG CMultiplicationDivision::Release(void) {
	InterlockedDecrement(&m_cRef);

	if (m_cRef == 0) {
		delete(this);
		return(0);
	}
	return m_cRef;
}

HRESULT CMultiplicationDivision::MultiplicationOfTwoIntegers(int num1, int num2, int *num3) {
	*num3 = num1 * num2;
	return(S_OK);
}

HRESULT CMultiplicationDivision::DivisionOfTwoIntegers(int num1, int num2, int *num3) {
	*num3 = num1 / num2;
	return(S_OK);
}

CMultiplicationDivisionClassFactory::CMultiplicationDivisionClassFactory(void) {
	m_cRef = 1;

}

CMultiplicationDivisionClassFactory::~CMultiplicationDivisionClassFactory(void) {

}

HRESULT CMultiplicationDivisionClassFactory::QueryInterface(REFIID riid, void **ppv) {
	if (riid == IID_IUnknown)
		*ppv = static_cast<IClassFactory *>(this);
	if (riid == IID_IClassFactory)
		*ppv = static_cast<IClassFactory *>(this);

	else {
		*ppv = NULL;
		MessageBox(NULL, TEXT("CMultiplicationDivisionClassFactory -> QI riid not found"), TEXT("Erorr"), MB_OK);
		return(E_NOINTERFACE);
	}
	reinterpret_cast<IUnknown *>(*ppv)->AddRef();
	return(S_OK);
}

ULONG CMultiplicationDivisionClassFactory::AddRef() {
	InterlockedIncrement(&m_cRef);
	return(m_cRef);
}

ULONG CMultiplicationDivisionClassFactory::Release() {
	InterlockedDecrement(&m_cRef);
	if (m_cRef == 0) {
		delete(this);
		return(0);
	}
	return(m_cRef);
}

HRESULT CMultiplicationDivisionClassFactory::CreateInstance(IUnknown * pUnkOuter, REFIID riid, void **ppv) {
	CMultiplicationDivision *pCMultiplicationDivision = NULL;
	HRESULT hr;
	if (pUnkOuter != NULL)
		return(CLASS_E_NOAGGREGATION);

	pCMultiplicationDivision = new CMultiplicationDivision;

	if (pCMultiplicationDivision == NULL) {
		MessageBox(NULL, TEXT("CMultiplicationDivisionClassFactory->CreateInstace: pCMultiplicationDivision not found"),
			TEXT("Error"), MB_OK);
		return(E_OUTOFMEMORY);
	}

	hr = pCMultiplicationDivision->QueryInterface(riid, ppv);
	pCMultiplicationDivision->Release();
	return(hr);
}


HRESULT CMultiplicationDivisionClassFactory::LockServer(BOOL fLock) {
	if (fLock)
	{
		InterlockedIncrement(&glNumberOfServerLocks);
	}
	else
		InterlockedDecrement(&glNumberOfServerLocks);

	return(S_OK);
}

HRESULT __stdcall DllGetClassObject(REFCLSID rclsid, REFIID riid, void **ppv) {
	CMultiplicationDivisionClassFactory *pCMultiplicationDivisionClassFactory = NULL;

	if (rclsid != CLSID_MultiplicationDivision) {
		MessageBox(NULL, TEXT("DllGetClassObject CLSID not found"), TEXT("Error"), MB_OK);
		return(CLASS_E_CLASSNOTAVAILABLE);
	}

	pCMultiplicationDivisionClassFactory = new CMultiplicationDivisionClassFactory;
	HRESULT hr;

	if (pCMultiplicationDivisionClassFactory == NULL) {
		MessageBox(NULL, TEXT("DllGetClassObject CMultiplicationDivision not found"), TEXT("Error"), MB_OK);
		return(E_OUTOFMEMORY);
	}

	hr = pCMultiplicationDivisionClassFactory->QueryInterface(riid, ppv);
	pCMultiplicationDivisionClassFactory->Release();
	return(hr);
}


HRESULT __stdcall DllCanUnloadNow(void) {
	if ((glNumberOfActiveComponents == 0) && (glNumberOfServerLocks == 0))
		return(S_OK);
	else
		return(S_FALSE);
}




