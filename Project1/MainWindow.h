#pragma once
#include "resource.h"
#include "framework.h"
#include <commctrl.h>
#include <mmdeviceapi.h>
#include <endpointvolume.h>
#include <functiondiscoverykeys_devpkey.h>
#include "CAudioEndpointVolumeCallback.h"
#include "windowsx.h"
constexpr auto MAX_LOADSTRING = 100;

class MainWindow {

private:
	HINSTANCE hInst;                                // current instance
	WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
	WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
	HWND hWnd;

	HWND* fader;
	HWND* textFader;
	IMMDevice** iEndpointDevice;
	IAudioEndpointVolume** iEndpointVolume;
	CAudioEndpointVolumeCallback** ppCAudioEndpointVolumeCallback;
	UINT countDevices = NULL;

	ATOM MyRegisterClass(HINSTANCE hInstance) const;

	static LRESULT CALLBACK sWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK sAbout(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT WndProc(UINT message, WPARAM wParam, LPARAM lParam);
	INT_PTR About(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	//Listeners
	LRESULT OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
	BOOL OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct);
	void OnDestroy(HWND hwnd);
	void OnPaint(HWND hwnd) const;
	void OnSize(HWND hwnd, UINT state, int cx, int cy) const;
	void OnVScroll(HWND hwnd, HWND hwndCtl, UINT code, int pos);

public:
	MainWindow(HINSTANCE hInstance);
	HWND CreateMainWindow();
};