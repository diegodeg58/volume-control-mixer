#pragma comment(lib, "comctl32.lib")

#include "framework.h"
#include "Main.h"
#include <commctrl.h>
#include <stdio.h>
#include <mmdeviceapi.h>
#include <endpointvolume.h>
#include <functiondiscoverykeys_devpkey.h>
#include "CAudioEndpointVolumeCallback.h"
#include "MainWindow.h"

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow, MainWindow* mainWindow);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	MainWindow mainWindow(hInstance);

	INITCOMMONCONTROLSEX iCCE{};
	iCCE.dwSize = sizeof(INITCOMMONCONTROLSEX);
	iCCE.dwICC = ICC_BAR_CLASSES;
	InitCommonControlsEx(&iCCE);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow, &mainWindow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PROJECT1));

	MSG msg;

	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow, MainWindow* mainWindow)
{
	HWND hWnd = mainWindow->CreateMainWindow();

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}