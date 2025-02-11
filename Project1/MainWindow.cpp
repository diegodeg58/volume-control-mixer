#include "MainWindow.h"
#include "About.h"
#include "resource.h"
#include <commctrl.h>
#include <windowsx.h>

void CenterWindow(HWND hwnd) {
	RECT rc;
	GetWindowRect(hwnd, &rc);
	int width = rc.right - rc.left;
	int height = rc.bottom - rc.top;
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	SetWindowPos(hwnd, 0, (screenWidth - width) / 2, (screenHeight - height) / 2, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
}

MainWindow::MainWindow(HINSTANCE hInstance) {
	hInst = hInstance;
	LoadStringW(hInst, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInst, IDC_PROJECT1, szWindowClass, MAX_LOADSTRING);
	audioOutDevices = {};
	countOutDevices = 0;
	countInDevices = 0;
	MyRegisterClass();
}

HWND MainWindow::CreateMainWindow() {
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

LRESULT CALLBACK MainWindow::sWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	MainWindow* pMainWindow;
	if (message == WM_NCCREATE) {
		LPCREATESTRUCT lpcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
		pMainWindow = static_cast<MainWindow*>(lpcs->lpCreateParams);
		pMainWindow->hWnd = hWnd;
		SetWindowLongPtrW(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pMainWindow));
	}
	else {
		pMainWindow = reinterpret_cast<MainWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	}
	if (pMainWindow)
		return pMainWindow->WndProc(message, wParam, lParam);
	return false;
}

LRESULT MainWindow::OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify) const {
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
	HRESULT hr;
	hr = CoInitialize(NULL);
	IMMDeviceEnumerator* deviceEnumerator = NULL;
	hr = CoCreateInstance(
		__uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER,
		__uuidof(IMMDeviceEnumerator), (LPVOID*)&deviceEnumerator);

	IMMDeviceCollection* deviceOutCollection = NULL;
	IMMDeviceCollection* deviceInCollection = NULL;
	hr = deviceEnumerator->EnumAudioEndpoints(eRender, DEVICE_STATE_ACTIVE, &deviceOutCollection);
	hr = deviceEnumerator->EnumAudioEndpoints(eCapture, DEVICE_STATE_ACTIVE, &deviceInCollection);

	deviceEnumerator->Release();
	deviceEnumerator = NULL;

	hr = deviceOutCollection->GetCount(&countOutDevices);
	hr = deviceInCollection->GetCount(&countInDevices);

	ULONG x = 10, y = 10;
	audioOutDevices.reserve(countOutDevices);
	for (ULONG i = 0; i < countOutDevices; i++, x = i * 120) {
		audioOutDevices.push_back(UIAudioOutDevice(hWnd, hInst, x, y));
		audioOutDevices.back().SetDevice(i, deviceOutCollection);
	}
	RECT rect;
	GetWindowRect(hWnd, &rect);
	MoveWindow(hWnd, (int)rect.left, (int)rect.top, x, 575, true);
	CenterWindow(hWnd);

	return TRUE;
}

void MainWindow::OnDestroy(HWND hwnd)
{
	CoUninitialize();
	//release all devices
	for (unsigned int i = 0; i < countOutDevices; i++) {
		audioOutDevices[i].Release();
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
	if (code == TB_THUMBPOSITION || code == TB_THUMBTRACK) {
		value = (int)MAX_VOL - pos;
	}
	else {
		value = MAX_VOL - (int)SendMessage(hwndCtl, TBM_GETPOS, NULL, NULL);
	}
	char text[5];
	sprintf_s(text, "%d", value);

	for (unsigned int i = 0; i < countOutDevices; i++) {
		if (hwndCtl == audioOutDevices[i].GetLevelFader()) {
			float newValue = (float)(value) / MAX_VOL;
			audioOutDevices[i].SetVolumeScalar(newValue);
		}
	}
}

LRESULT MainWindow::WndProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
		HANDLE_MSG(hWnd, WM_COMMAND, OnCommand);
		HANDLE_MSG(hWnd, WM_CREATE, OnCreate);
		HANDLE_MSG(hWnd, WM_DESTROY, OnDestroy);
		HANDLE_MSG(hWnd, WM_PAINT, OnPaint);
		HANDLE_MSG(hWnd, WM_SIZE, OnSize);
		HANDLE_MSG(hWnd, WM_VSCROLL, OnVScroll);

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}