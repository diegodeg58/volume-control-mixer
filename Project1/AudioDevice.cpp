#include "AudioDevice.h"
#include "Utilities.h"
#define CTRL_WIDTH 100
#define Y_OFFSET 40

UIAudioOutDevice::UIAudioOutDevice(HWND hParent, HINSTANCE hInst, int xCoord, int yCoord)
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
	iEndpoint = NULL;
	iEndpointVolume = NULL;

	iAudioEndpointVolumeCallback = CAudioEndpointVolumeCallback();
	iAudioEndpointVolumeCallback.SetHandlers(levelFader, levelValue);
	iAudioEndpointVolumeCallback.AddRef();
}

void UIAudioOutDevice::SetDevice(UINT nDevice, IMMDeviceCollection *deviceOutCollection)
{
	float currentVolumeScalar;
	char text[4], textDevice[100];
	IPropertyStore *pPropertyStore = NULL;
	PROPVARIANT varName;
	HRESULT hr;

	hr = deviceOutCollection->Item(nDevice, &iEndpoint);
	hr = iEndpoint->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_ALL,
							 NULL, (LPVOID *)&iEndpointVolume);
	hr = iEndpointVolume->RegisterControlChangeNotify(&iAudioEndpointVolumeCallback);
	hr = iEndpointVolume->GetMasterVolumeLevelScalar(&currentVolumeScalar);
	sprintf_s(text, "%d", (int)(currentVolumeScalar * MAX_VOL));
	SendMessageA(levelFader, TBM_SETPOS, TRUE, LPARAM((int)MAX_VOL - (currentVolumeScalar * MAX_VOL)));
	SetWindowTextA(levelValue, (LPCSTR)text);

	hr = iEndpoint->OpenPropertyStore(STGM_READ, &pPropertyStore);
	PropVariantInit(&varName);
	hr = pPropertyStore->GetValue(PKEY_Device_FriendlyName, &varName);
	sprintf_s(textDevice, "%S", varName.pwszVal);
	SetWindowTextA(deviceName, (LPCSTR)textDevice);
}

void UIAudioOutDevice::SetVolumeScalar(float volume)
{
	char text[4];
	iEndpointVolume->SetMasterVolumeLevelScalar(volume, NULL);
	sprintf_s(text, "%d", (int)(volume * MAX_VOL));
	SendMessageA(levelFader, TBM_SETPOS, TRUE, LPARAM((int)MAX_VOL - (volume * MAX_VOL)));
	SetWindowTextA(levelValue, (LPCSTR)text);
}

HWND UIAudioOutDevice::GetLevelFader() const
{
	return levelFader;
}

void UIAudioOutDevice::Release()
{
	iEndpointVolume->UnregisterControlChangeNotify(&iAudioEndpointVolumeCallback);
	iEndpointVolume->Release();
	iEndpoint->Release();
	iAudioEndpointVolumeCallback.Release();
}

RECT UIAudioOutDevice::GetRect() const
{
	RECT rect = Utilities::GetLocalCoordinates(deviceName);
	return RECT{
		0,
		0,
		rect.right,
		rect.bottom};
}