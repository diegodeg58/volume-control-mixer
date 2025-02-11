#pragma once
#include <mmdeviceapi.h>
#include <endpointvolume.h>
#include <CommCtrl.h>
#include <functiondiscoverykeys_devpkey.h>
#include "CAudioEndpointVolumeCallback.h"

class UIAudioOutDevice
{
protected:
	HWND levelFader;
	HWND levelValue;
	HWND deviceName;
	IMMDevice* iEndpoint;
	IAudioEndpointVolume* iEndpointVolume;
	CAudioEndpointVolumeCallback iAudioEndpointVolumeCallback;

public:
	UIAudioOutDevice(HWND hParent, HINSTANCE hInst, int xCoord, int yCoord);
	void SetDevice(UINT nDevice, IMMDeviceCollection* deviceOutCollection);
	void SetVolumeScalar(float volume);
	HWND GetLevelFader() const;
	void Release();
	RECT GetRect() const;
};

