#include "CAudioEndpointVolumeCallback.h"

CAudioEndpointVolumeCallback::CAudioEndpointVolumeCallback(HWND hWndFader, HWND hWndTextFader) :
	_cRef(1), hWndFader(hWndFader), hWndTextFader(hWndTextFader)
{
};

CAudioEndpointVolumeCallback::~CAudioEndpointVolumeCallback()
{
	delete this;
}

// IUnknown methods -- AddRef, Release, and QueryInterface

ULONG CAudioEndpointVolumeCallback::AddRef()
{
	return InterlockedIncrement(&_cRef);
}

ULONG CAudioEndpointVolumeCallback::Release()
{
	ULONG ulRef = InterlockedDecrement(&_cRef);
	if (0 == ulRef)
	{
		delete this;
	}
	return ulRef;
}

HRESULT CAudioEndpointVolumeCallback::QueryInterface(REFIID riid, VOID** ppvInterface)
{
	if (IID_IUnknown == riid)
	{
		AddRef();
		*ppvInterface = (IUnknown*)this;
	}
	else if (__uuidof(IAudioEndpointVolumeCallback) == riid)
	{
		AddRef();
		*ppvInterface = (IAudioEndpointVolumeCallback*)this;
	}
	else
	{
		*ppvInterface = NULL;
		return E_NOINTERFACE;
	}
	return S_OK;
}

// Callback method for endpoint-volume-change notifications.
HRESULT CAudioEndpointVolumeCallback::OnNotify(PAUDIO_VOLUME_NOTIFICATION_DATA pNotify)
{
	if (pNotify == NULL)
	{
		return E_INVALIDARG;
	}

	float currentVolume = pNotify->fMasterVolume;
	char text[4];
	sprintf_s(text, "%d", (int)(currentVolume * MAX_VOL));
	PostMessageA(hWndFader, TBM_SETPOS, TRUE, LPARAM((UINT32)(MAX_VOL - currentVolume * MAX_VOL)));
	SetWindowTextA(hWndTextFader, (LPCSTR)text);

	return S_OK;
}