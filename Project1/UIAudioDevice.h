#pragma once
#include "AudioDevice.h"
#include "Subscriber.h"

class UIAudioDevice : public Subscriber
{
protected:
	HWND levelFader;
	HWND levelValue;
	HWND deviceName;
	AudioDevice* audioDevice;

public:
	UIAudioDevice(HWND hParent, HINSTANCE hInst, int xCoord, int yCoord);
	void SetVolumeScalar(float volume);
	HWND GetFaderHandle() const;
	RECT GetRect() const;
	void InitUI(AudioDevice* audioDevice);
	void Update(UINT value);
	void Release();
};
