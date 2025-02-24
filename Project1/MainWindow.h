#pragma once
#include "UIAudioDevice.h"
#include <vector>
constexpr auto MAX_LOADSTRING = 100;

class MainWindow {

private:
	HINSTANCE hInst;                                // current instance
	WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
	WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
	HWND hWnd;
	HWND hTabControl;
	HWND hOutputs, hInputs;

	std::vector<UIAudioDevice> UIAudioOutDevices;
	std::vector<UIAudioDevice> UIAudioInDevices;
	UINT countOutDevices;
	UINT countInDevices;
	IMMDeviceCollection *deviceOutCollection = NULL;
	IMMDeviceCollection *deviceInCollection = NULL;

	ATOM MyRegisterClass() const;

	static LRESULT CALLBACK sWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT WndProc(UINT message, WPARAM wParam, LPARAM lParam);

	//Listeners
	LRESULT OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify) const;
	BOOL OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct);
	void OnDestroy(HWND hwnd);
	void OnPaint(HWND hwnd) const;
	void OnSize(HWND hwnd, UINT state, int cx, int cy) const;
	void OnVScroll(HWND hwnd, HWND hwndCtl, UINT code, int pos);
	LRESULT OnNotify(HWND hwnd, int id, LPNMHDR pnmhdr) const;

public:
	MainWindow(HINSTANCE hInstance);
	HWND CreateMainWindow();
};