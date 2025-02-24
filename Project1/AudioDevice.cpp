#include "AudioDevice.h"
#include <functiondiscoverykeys_devpkey.h>
#include "atlstr.h"

HRESULT AudioDevice::GetAudioDevices(IMMDeviceCollection** deviceCollection, EDataFlow dataFlow)
{
    HRESULT hr;
    IMMDeviceEnumerator* deviceEnumerator = NULL;
    hr = CoCreateInstance(
        __uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER,
        __uuidof(IMMDeviceEnumerator), (LPVOID*)&deviceEnumerator);
    
    hr = deviceEnumerator->EnumAudioEndpoints(dataFlow, DEVICE_STATE_ACTIVE, deviceCollection);

    deviceEnumerator->Release();
    deviceEnumerator = NULL;

    return hr;
}

HRESULT AudioDevice::GetAudioOutDevices(IMMDeviceCollection** deviceOutCollection)
{
    return AudioDevice::GetAudioDevices(deviceOutCollection, eRender);
}

HRESULT AudioDevice::GetAudioInDevices(IMMDeviceCollection** deviceInCollection)
{
    return AudioDevice::GetAudioDevices(deviceInCollection, eCapture);
}

unsigned int AudioDevice::GetDeviceCount(IMMDeviceCollection* deviceCollection)
{
    unsigned int count;
    deviceCollection->GetCount(&count);
    return count;
}

std::string AudioDevice::GetDeviceName() const
{
    IPropertyStore* pPropertyStore = NULL;
    PROPVARIANT varName;
    HRESULT hr;
    hr = iEndpoint->OpenPropertyStore(STGM_READ, &pPropertyStore);
    hr = pPropertyStore->GetValue(PKEY_Device_FriendlyName, &varName);
    std::string deviceName = std::string(CW2A(varName.pwszVal));
    PropVariantClear(&varName);
    pPropertyStore->Release();
    return deviceName;
}

float AudioDevice::GetVolumeScalar() const
{
    float volume;
    iEndpointVolume->GetMasterVolumeLevelScalar(&volume);
    return volume;
}

void AudioDevice::SetVolumeScalar(float volume)
{
    iEndpointVolume->SetMasterVolumeLevelScalar(volume, NULL);
}

void AudioDevice::Subscribe(Subscriber* subscriber)
{
    audioEndpointVolumeCallback.Susbcribe(subscriber);
}

void AudioDevice::Release()
{
    iEndpointVolume->UnregisterControlChangeNotify(&audioEndpointVolumeCallback);
    iEndpointVolume->Release();
    iEndpoint->Release();
    audioEndpointVolumeCallback.Release();
}

AudioDevice::AudioDevice(UINT nDevice, IMMDeviceCollection* deviceCollection)
{
    HRESULT hr;
    hr = deviceCollection->Item(nDevice, &iEndpoint);
    hr = iEndpoint->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_ALL,
                             NULL, (LPVOID*)&iEndpointVolume);
    audioEndpointVolumeCallback = CAudioEndpointVolumeCallback();
    audioEndpointVolumeCallback.AddRef();
    iEndpointVolume->RegisterControlChangeNotify(&audioEndpointVolumeCallback);
}

AudioOutDevice::AudioOutDevice(UINT nDevice, IMMDeviceCollection* deviceCollection) :
    AudioDevice(nDevice, deviceCollection)
{
}

AudioInDevice::AudioInDevice(UINT nDevice, IMMDeviceCollection* deviceCollection) :
    AudioDevice(nDevice, deviceCollection)
{
}