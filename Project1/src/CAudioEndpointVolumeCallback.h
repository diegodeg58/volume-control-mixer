#pragma once
#include <endpointvolume.h>
#include "Subscriber.h"
constexpr auto MAX_VOL = 100;
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
    Subscriber* subscriber = nullptr;

public:
    CAudioEndpointVolumeCallback();
    ~CAudioEndpointVolumeCallback();
    void Susbcribe(Subscriber* subscriber);

    // IUnknown methods -- AddRef, Release, and QueryInterface
    ULONG STDMETHODCALLTYPE AddRef();
    ULONG STDMETHODCALLTYPE Release();
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, VOID** ppvInterface);

    // Callback method for endpoint-volume-change notifications.
    HRESULT STDMETHODCALLTYPE OnNotify(PAUDIO_VOLUME_NOTIFICATION_DATA pNotify);
};