#pragma once
#include <mmdeviceapi.h>
#include <endpointvolume.h>
#include "CAudioEndpointVolumeCallback.h"

class UIAudioDevice
{
protected:
	HWND levelFader;
	HWND levelValue;
	HWND deviceName;
	IMMDevice* iEndpoint;
	IAudioEndpointVolume* iEndpointVolume;
	CAudioEndpointVolumeCallback iAudioEndpointVolumeCallback;

public:
	UIAudioDevice(HWND hParent, HINSTANCE hInst, int xCoord, int yCoord);
	void SetDevice(UINT nDevice, IMMDeviceCollection* deviceOutCollection);
	void SetVolumeScalar(float volume);
	HWND GetLevelFader() const;
	void Release();
	RECT GetRect() const;
};

