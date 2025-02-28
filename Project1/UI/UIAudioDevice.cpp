#include "UIAudioDevice.h"
#include "../Utilities.h"
#include <CommCtrl.h>

#define CTRL_WIDTH 100
#define Y_OFFSET 40

UIAudioDevice::UIAudioDevice(HWND hParent, HINSTANCE hInst, int xCoord, int yCoord)
{
	levelFader = CreateWindowEx(
		0, TRACKBAR_CLASS, NULL,
		WS_CHILD | WS_VISIBLE | WS_TABSTOP | TBS_VERT | TBS_BOTH,
		xCoord + 30,
		yCoord,
		CTRL_WIDTH - 60,
		300,
		hParent, NULL, hInst, NULL);

	levelValue = CreateWindowEx(
		0, L"Static",
		NULL, WS_CHILD | WS_VISIBLE | SS_CENTER,
		xCoord,
		Utilities::GetLocalCoordinates(levelFader).bottom,
		CTRL_WIDTH,
		20,
		hParent, NULL, hInst, NULL);

	deviceName = CreateWindowEx(
		0, L"Static", NULL,
		WS_CHILD | WS_VISIBLE | SS_CENTER,
		xCoord,
		Utilities::GetLocalCoordinates(levelValue).bottom,
		CTRL_WIDTH,
		50,
		hParent, NULL, hInst, NULL);
}

void UIAudioDevice::SetVolumeScalar(float volume)
{
	audioDevice->SetVolumeScalar(volume);
	std::string sVolume = std::to_string((int)(volume * MAX_VOL));
	SendMessageA(levelFader, TBM_SETPOS, TRUE, LPARAM((int)MAX_VOL - (volume * MAX_VOL)));
	SetWindowTextA(levelValue, sVolume.c_str());
}

HWND UIAudioDevice::GetFaderHandle() const
{
	return levelFader;
}

RECT UIAudioDevice::GetRect() const
{
	RECT rect = Utilities::GetLocalCoordinates(deviceName);
	return RECT{0, 0, rect.right, rect.bottom};
}

void UIAudioDevice::InitUI(AudioDevice* audioDevice)
{
	this->audioDevice = audioDevice;
	audioDevice->Subscribe(this);

	float currentVolumeScalar = audioDevice->GetVolumeScalar();
	SendMessageA(levelFader, TBM_SETPOS, TRUE, LPARAM((int)MAX_VOL - (currentVolumeScalar * MAX_VOL)));
	std::string sVolume = std::to_string((int)(currentVolumeScalar * MAX_VOL));
	SetWindowTextA(levelValue, sVolume.c_str());
	std::string sDeviceName = audioDevice->GetDeviceName();
	SetWindowTextA(deviceName, sDeviceName.c_str());
}

void UIAudioDevice::Update(UINT value)
{
	std::string text = std::to_string(value);
	PostMessageA(levelFader, TBM_SETPOS, TRUE, LPARAM((UINT32)(MAX_VOL - value)));
	SetWindowTextA(levelValue, text.c_str());
}

void UIAudioDevice::Release()
{
	audioDevice->Release();
	delete audioDevice;
	DestroyWindow(levelFader);
	DestroyWindow(levelValue);
	DestroyWindow(deviceName);
}