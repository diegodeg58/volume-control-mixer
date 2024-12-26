#include "MainWindow.h"
#include "About.h"

MainWindow::MainWindow(HINSTANCE hInstance) {
	hInst = hInstance;
	LoadStringW(hInst, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInst, IDC_PROJECT1, szWindowClass, MAX_LOADSTRING);
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

	fader = new HWND[countOutDevices];
	textFader = new HWND[countOutDevices];
	iEndpointDevice = new IMMDevice * [countOutDevices];
	iEndpointVolume = new IAudioEndpointVolume * [countOutDevices];
	ppCAudioEndpointVolumeCallback = new CAudioEndpointVolumeCallback * [countOutDevices];

	int x, y = 480;

	for (ULONG i = 0; i < countOutDevices; i++) {
		fader[i] = CreateWindowEx(
			0, TRACKBAR_CLASS, NULL,
			WS_CHILD | WS_VISIBLE | WS_TABSTOP | TBS_VERT | TBS_BOTH,
			35 + i * 150, 10, 50, 300, hWnd, NULL, hInst, NULL);
		textFader[i] = CreateWindowEx(
			0, L"Static",
			NULL, WS_CHILD | WS_VISIBLE,
			45 + i * 150, 310, 50, 30, hWnd, NULL, hInst, NULL);
		HWND hTextDevice = CreateWindowEx(
			0, L"Static", NULL,
			WS_CHILD | WS_VISIBLE,
			25 + i * 150, 340, 100, 100, hWnd, NULL, hInst, NULL);

		hr = deviceOutCollection->Item(i, &iEndpointDevice[i]);
		hr = iEndpointDevice[i]->Activate(
			__uuidof(IAudioEndpointVolume), CLSCTX_INPROC_SERVER, NULL, (LPVOID*)&iEndpointVolume[i]);

		ppCAudioEndpointVolumeCallback[i] = new CAudioEndpointVolumeCallback(fader[i], textFader[i]);
		hr = iEndpointVolume[i]->RegisterControlChangeNotify(ppCAudioEndpointVolumeCallback[i]);

		float currentVolume;
		hr = iEndpointVolume[i]->GetMasterVolumeLevelScalar(&currentVolume);
		char text[4];
		sprintf_s(text, "%d", (int)(currentVolume * MAX_VOL));
		SendMessageA(fader[i], TBM_SETPOS, TRUE, LPARAM((int)MAX_VOL - (currentVolume * MAX_VOL)));
		SetWindowTextA(textFader[i], (LPCSTR)text);

		IPropertyStore* pProps = NULL;
		hr = iEndpointDevice[i]->OpenPropertyStore(STGM_READ, &pProps);

		PROPVARIANT varName;
		PropVariantInit(&varName);
		hr = pProps->GetValue(PKEY_Device_FriendlyName, &varName);
		char textDevice[100];
		sprintf_s(textDevice, "%S", varName.pwszVal);
		SetWindowTextA(hTextDevice, (LPCSTR)textDevice);

		x = 150 + i * 150;
	}
	RECT rect;
	GetWindowRect(hWnd, &rect);
	MoveWindow(hWnd, (int)rect.left, (int)rect.top, x, y, true);

	return TRUE;
}

void MainWindow::OnDestroy(HWND hwnd)
{
	CoUninitialize();
	for (unsigned int i = 0; i < countOutDevices; i++) {
		iEndpointVolume[i]->UnregisterControlChangeNotify(ppCAudioEndpointVolumeCallback[i]);
	}
	delete[] fader;
	delete[] textFader;
	delete[] iEndpointVolume;
	delete[] iEndpointDevice;
	delete[] ppCAudioEndpointVolumeCallback;
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

	char textControl[20];
	for (unsigned int i = 0; i < countOutDevices; i++) {
		if (fader[i] == hwndCtl) {
			sprintf_s(textControl, "%d", i);
			float newValue = (float)(value) / MAX_VOL;
			iEndpointVolume[i]->SetMasterVolumeLevelScalar(newValue, NULL);
			SetWindowTextA(textFader[i], (LPCSTR)text);
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