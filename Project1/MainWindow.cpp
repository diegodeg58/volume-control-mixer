#include "MainWindow.h"
#include "About.h"
#include "resource.h"
#include <windowsx.h>
#include <commctrl.h>
#include "Utilities.h"
#include "AudioDevice.h"

static void CenterWindow(HWND hwnd)
{
	RECT rc;
	GetWindowRect(hwnd, &rc);
	int width = rc.right - rc.left;
	int height = rc.bottom - rc.top;
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	SetWindowPos(hwnd, 0, (screenWidth - width) / 2, (screenHeight - height) / 2, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
}

static LRESULT CALLBACK ChildWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_VSCROLL:
	{
		HWND hParent = GetParent(GetParent(hwnd));
		SendMessage(hParent, message, wParam, lParam);
		return 0;
	}
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
}

MainWindow::MainWindow(HINSTANCE hInstance)
{
	hInst = hInstance;
	LoadStringW(hInst, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInst, IDC_PROJECT1, szWindowClass, MAX_LOADSTRING);
	UIAudioOutDevices = {};
	UIAudioInDevices = {};
	countOutDevices = 0;
	countInDevices = 0;
	MyRegisterClass();

	deviceOutCollection = NULL;
	deviceInCollection = NULL;

	HRESULT hr = CoInitialize(NULL);
	hr = AudioDevice::GetAudioOutDevices(&deviceOutCollection);
	hr = AudioDevice::GetAudioInDevices(&deviceInCollection);
	countOutDevices = AudioDevice::GetDeviceCount(deviceOutCollection);
	countInDevices = AudioDevice::GetDeviceCount(deviceInCollection);
}

HWND MainWindow::CreateMainWindow()
{
	return CreateWindowExW(0L, szWindowClass, szTitle,
						   WS_GROUP | WS_SYSMENU, ((int)0x80000000), 0,
						   ((int)0x80000000), 0, nullptr, nullptr, hInst, this);
}

ATOM MainWindow::MyRegisterClass() const
{
	WNDCLASSEXW wcex{};

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = sWndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInst;
	wcex.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_PROJECT1));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_MENU + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_PROJECT1);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(hInst, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

LRESULT CALLBACK MainWindow::sWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	MainWindow *pMainWindow;
	if (message == WM_NCCREATE)
	{
		LPCREATESTRUCT lpcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
		pMainWindow = static_cast<MainWindow *>(lpcs->lpCreateParams);
		pMainWindow->hWnd = hWnd;
		SetWindowLongPtrW(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pMainWindow));
	}
	else
	{
		pMainWindow = reinterpret_cast<MainWindow *>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	}
	if (pMainWindow)
		return pMainWindow->WndProc(message, wParam, lParam);
	return false;
}

LRESULT MainWindow::OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify) const
{
	switch (id)
	{
	case IDM_ABOUT:
		AboutWnd::GetInstance(hWnd, hInst)->Show();
		break;
	case IDM_EXIT:
		DestroyWindow(hWnd);
		break;
	default:
		FORWARD_WM_COMMAND(hWnd, id, hwndCtl, codeNotify, DefWindowProc);
		return false;
	}
	return true;
}

BOOL MainWindow::OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct)
{
	hTabControl = CreateWindow(
		WC_TABCONTROL, L"", WS_CHILD | WS_VISIBLE, 0, 0, 0, 0,
		hWnd, NULL, hInst, NULL);

	TCITEM tie[2]{};
	tie[0].mask = TCIF_TEXT;
	tie[0].pszText = const_cast<LPWSTR>(L"Output");
	TabCtrl_InsertItem(hTabControl, 0, &tie[0]);
	tie[1].mask = TCIF_TEXT;
	tie[1].pszText = const_cast<LPWSTR>(L"Input");
	TabCtrl_InsertItem(hTabControl, 1, &tie[1]);

	hOutputs = CreateWindow(
		L"Static", NULL, WS_CHILD | WS_VISIBLE | SS_CENTER,
		0, 0, 0, 0, hTabControl, NULL, hInst, NULL);
	SetWindowLongPtr(hOutputs, GWLP_WNDPROC, (LONG_PTR)ChildWndProc);
	hInputs = CreateWindow(
		L"Static", NULL, WS_CHILD | SS_CENTER,
		0, 0, 0, 0, hTabControl, NULL, hInst, NULL);
	SetWindowLongPtr(hInputs, GWLP_WNDPROC, (LONG_PTR)ChildWndProc);

	ULONG x = 0, y = 0;
	UIAudioOutDevices.reserve(countOutDevices);
	for (ULONG i = 0; i < countOutDevices; i++, x = i * 110)
	{
		UIAudioOutDevices.push_back(UIAudioDevice(hOutputs, hInst, x - 15, y));
		UIAudioOutDevices.back().InitUI(new AudioDevice(i, deviceOutCollection));
	}

	ULONG right = UIAudioOutDevices.back().GetRect().right;
	ULONG bottom = UIAudioOutDevices.back().GetRect().bottom;
	MoveWindow(hTabControl, 0, 0, right + 20, bottom + 30, true);
	RECT rectTC{};
	GetClientRect(hTabControl, &rectTC);
	MoveWindow(hOutputs, 10, rectTC.top + 27, rectTC.right - rectTC.left - 17, rectTC.bottom - 33, true);
	MoveWindow(hWnd, 0, 0, rectTC.right + 13, rectTC.bottom + 55, true);

	UIAudioInDevices.reserve(countInDevices);
	for (ULONG i = 0; i < countInDevices; i++, x = i * 110)
	{
		UIAudioInDevices.push_back(UIAudioDevice(hInputs, hInst, x - 15, y));
		UIAudioInDevices.back().InitUI(new AudioDevice(i, deviceInCollection));
	}
	MoveWindow(hInputs, -50, rectTC.top + 27, rectTC.right - rectTC.left - 17, rectTC.bottom - 33, true);
	
	CenterWindow(hWnd);

	return TRUE;
}

void MainWindow::OnDestroy(HWND hwnd)
{
	CoUninitialize();
	// release all devices
	for (unsigned int i = 0; i < countOutDevices; i++)
	{
		UIAudioOutDevices[i].Release();
	}
	for (unsigned int i = 0; i < countInDevices; i++)
	{
		UIAudioInDevices[i].Release();
	}
	PostQuitMessage(0);
}

void MainWindow::OnPaint(HWND hwnd) const
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps);
	// TODO: Add any drawing code that uses hdc here...
	EndPaint(hWnd, &ps);
}

void MainWindow::OnSize(HWND hwnd, UINT state, int cx, int cy) const
{
	RECT rect;
	GetWindowRect(hWnd, &rect);
}

void MainWindow::OnVScroll(HWND hwnd, HWND hwndCtl, UINT code, int pos)
{
	int value;
	if (code == TB_THUMBPOSITION || code == TB_THUMBTRACK)
		value = (int)MAX_VOL - pos;
	else
		value = MAX_VOL - (int)SendMessage(hwndCtl, TBM_GETPOS, NULL, NULL);
	char text[5];
	sprintf_s(text, "%d", value);

	for (unsigned int i = 0; i < countOutDevices; i++)
	{
		if (hwndCtl == UIAudioOutDevices[i].GetFaderHandle())
		{
			float newValue = (float)(value) / MAX_VOL;
			UIAudioOutDevices[i].SetVolumeScalar(newValue);
			return;
		}
	}
	for (unsigned int i = 0; i < countInDevices; i++)
	{
		if (hwndCtl == UIAudioInDevices[i].GetFaderHandle())
		{
			float newValue = (float)(value) / MAX_VOL;
			UIAudioInDevices[i].SetVolumeScalar(newValue);
			return;
		}
	}
}

LRESULT MainWindow::OnNotify(HWND hwnd, int id, LPNMHDR pnmhdr) const
{
	if (pnmhdr->code == TCN_SELCHANGE)
	{
		int iPage = TabCtrl_GetCurSel(hTabControl);
		if (iPage == 0)
		{
			ShowWindow(hOutputs, SW_SHOW);
			ShowWindow(hInputs, SW_HIDE);
		}
		else
		{
			ShowWindow(hOutputs, SW_HIDE);
			ShowWindow(hInputs, SW_SHOW);
		}
	}
	return 0;
}

LRESULT MainWindow::WndProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		HANDLE_MSG(hWnd, WM_COMMAND, OnCommand);
		HANDLE_MSG(hWnd, WM_CREATE, OnCreate);
		HANDLE_MSG(hWnd, WM_DESTROY, OnDestroy);
		HANDLE_MSG(hWnd, WM_PAINT, OnPaint);
		HANDLE_MSG(hWnd, WM_SIZE, OnSize);
		HANDLE_MSG(hWnd, WM_VSCROLL, OnVScroll);
		HANDLE_MSG(hWnd, WM_NOTIFY, OnNotify);

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}