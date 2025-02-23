#pragma once
#include <mmdeviceapi.h>
#include <endpointvolume.h>
#include "CAudioEndpointVolumeCallback.h"

class AudioDevice
{
public:
    static HRESULT GetAudioOutDevices(IMMDeviceCollection **deviceInCollection);
    static HRESULT GetAudioInDevices(IMMDeviceCollection **deviceInCollection);
    static unsigned int GetDeviceCount(IMMDeviceCollection *deviceCollection);
private:
    static HRESULT GetAudioDevices(IMMDeviceCollection **deviceCollection, EDataFlow dataFlow);
};

class AudioOutDevice : public AudioDevice
{
public:
    AudioOutDevice();
    ~AudioOutDevice();
    void SetDevice(UINT nDevice, IMMDeviceCollection *deviceOutCollection);
    void SetVolumeScalar(float volume);
    void Release();
    HWND GetLevelFader() const;
    RECT GetRect() const;
private:
    HWND levelFader;
    HWND levelValue;
    HWND deviceName;
    IMMDevice *iEndpoint;
    IAudioEndpointVolume *iEndpointVolume;
    CAudioEndpointVolumeCallback iAudioEndpointVolumeCallback;
};

class AudioInDevice : public AudioDevice
{
public:
    AudioInDevice();
    ~AudioInDevice();
    void SetDevice(UINT nDevice, IMMDeviceCollection *deviceInCollection);
    void SetVolumeScalar(float volume);
    void Release();
    HWND GetLevelFader() const;
    RECT GetRect() const;
private:
    HWND levelFader;
    HWND levelValue;
    HWND deviceName;
    IMMDevice *iEndpoint;
    IAudioEndpointVolume *iEndpointVolume;
    CAudioEndpointVolumeCallback iAudioEndpointVolumeCallback;
};
