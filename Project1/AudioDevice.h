#pragma once
#include <mmdeviceapi.h>
#include <endpointvolume.h>
#include <CommCtrl.h>
#include "CAudioEndpointVolumeCallback.h"

class AudioDevice
{
protected:
	HWND hFader;
	HWND hValue;
	HWND hName;
	IMMDevice* iEndpoint;
	IAudioEndpointVolume* endpointVolume;
	IAudioEndpointVolumeCallback* pCAudioEndpointVolumeCallback;
	unsigned int x, y;

public:
	AudioDevice(HWND hParent, HINSTANCE hInst, int x, int y);
	void Activate(UINT nDevice, IMMDeviceCollection* deviceOutCollection);
};

