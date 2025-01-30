#pragma once
#include "AudioDevice.h"
constexpr auto MAX_LOADSTRING = 100;

class MainWindow {

private:
	HINSTANCE hInst;                                // current instance
	WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
	WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
	HWND hWnd;

	AudioDevice** audioOutDevices;
	UINT countOutDevices;
	UINT countInDevices;

	ATOM MyRegisterClass() const;

	static LRESULT CALLBACK sWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT WndProc(UINT message, WPARAM wParam, LPARAM lParam);

	//Listeners
	LRESULT OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify) const;
	BOOL OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct);
	void OnDestroy(HWND hwnd) const;
	void OnPaint(HWND hwnd) const;
	void OnSize(HWND hwnd, UINT state, int cx, int cy) const;
	void OnVScroll(HWND hwnd, HWND hwndCtl, UINT code, int pos);

public:
	MainWindow(HINSTANCE hInstance);
	HWND CreateMainWindow();
};