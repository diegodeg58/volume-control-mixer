#include <endpointvolume.h>
#include <stdio.h>
#include <CommCtrl.h>
#define MAX_VOL  100
//-----------------------------------------------------------
// Client implementation of IAudioEndpointVolumeCallback
// interface. When a method in the IAudioEndpointVolume
// interface changes the volume level or muting state of the
// endpoint device, the change initiates a call to the
// client's IAudioEndpointVolumeCallback::OnNotify method.
//-----------------------------------------------------------
class CAudioEndpointVolumeCallback : public IAudioEndpointVolumeCallback
{
    LONG _cRef;
    const HWND hWndFader;
    const HWND hWndTextFader;

public:
    CAudioEndpointVolumeCallback(HWND hWndFader, HWND hWndTextFader) :
        _cRef(1), hWndFader(hWndFader), hWndTextFader(hWndTextFader)
    {
    }

    ~CAudioEndpointVolumeCallback()
    {
        delete this;
    }

    // IUnknown methods -- AddRef, Release, and QueryInterface

    ULONG STDMETHODCALLTYPE AddRef()
    {
        return InterlockedIncrement(&_cRef);
    }

    ULONG STDMETHODCALLTYPE Release()
    {
        ULONG ulRef = InterlockedDecrement(&_cRef);
        if (0 == ulRef)
        {
            delete this;
        }
        return ulRef;
    }

    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, VOID** ppvInterface)
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

    HRESULT STDMETHODCALLTYPE OnNotify(PAUDIO_VOLUME_NOTIFICATION_DATA pNotify)
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
};