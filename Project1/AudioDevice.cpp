#include "AudioDevice.h"

AudioDevice::AudioDevice(HWND hParent, HINSTANCE hInst, int x, int y)
{
	hFader = CreateWindowEx(
		0, TRACKBAR_CLASS, NULL,
		WS_CHILD | WS_TABSTOP | TBS_VERT | TBS_BOTH,
		x + 10, y, 50, 300, hParent, NULL, hInst, NULL);
	hValue = CreateWindowEx(
		0, L"Static",
		NULL, WS_CHILD | WS_VISIBLE,
		x + 20, y + 300, 50, 30, hParent, NULL, hInst, NULL);
	hName = CreateWindowEx(
		0, L"Static", NULL,
		WS_CHILD | WS_VISIBLE,
		x, y + 330, 100, 100, hParent, NULL, hInst, NULL);
}

void AudioDevice::Activate(UINT nDevice, IMMDeviceCollection *deviceOutCollection)
{
	HRESULT hr = deviceOutCollection->Item(nDevice, &iEndpoint);
	hr = iEndpoint->Activate(
		__uuidof(IAudioEndpointVolume), CLSCTX_ALL,
		NULL, (LPVOID*)&endpointVolume);
	hr = endpointVolume->RegisterControlChangeNotify(
		pCAudioEndpointVolumeCallback = new CAudioEndpointVolumeCallback(hFader, hValue));
}
