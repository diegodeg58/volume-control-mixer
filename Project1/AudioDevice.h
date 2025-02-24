#pragma once
#include <mmdeviceapi.h>
#include <endpointvolume.h>
#include "CAudioEndpointVolumeCallback.h"
#include <string>
#include "Subscriber.h"

class AudioDevice
{
public:
    static HRESULT GetAudioOutDevices(IMMDeviceCollection **deviceInCollection);
    static HRESULT GetAudioInDevices(IMMDeviceCollection **deviceInCollection);
    static unsigned int GetDeviceCount(IMMDeviceCollection *deviceCollection);

    AudioDevice(UINT nDevice, IMMDeviceCollection *deviceCollection);
    AudioDevice() = default;

    std::string GetDeviceName() const;
    float GetVolumeScalar() const;
    void SetVolumeScalar(float volume);
    void Subscribe(Subscriber *subscriber);
    void Release();

protected:
    IMMDevice *iEndpoint;
    IAudioEndpointVolume *iEndpointVolume;
    CAudioEndpointVolumeCallback audioEndpointVolumeCallback;

private:
    static HRESULT GetAudioDevices(IMMDeviceCollection **deviceCollection, EDataFlow dataFlow);
};

class AudioOutDevice : public AudioDevice
{
public:
    AudioOutDevice(UINT nDevice, IMMDeviceCollection *deviceCollection);
};

class AudioInDevice : public AudioDevice
{
public:
    AudioInDevice(UINT nDevice, IMMDeviceCollection *deviceCollection);
};
