#include "AudioDevice.h"

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