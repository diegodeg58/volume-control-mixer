#pragma comment(lib, "comctl32.lib")

#include "framework.h"
#include "Project1.h"
#include <commctrl.h>
#include <stdio.h>
#include <mmdeviceapi.h>
#include <endpointvolume.h>
#include <functiondiscoverykeys_devpkey.h>
#include "CAudioEndpointVolumeCallback.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

HWND* fader;
HWND* textFader;
IMMDevice** iEndpointDevice;
IAudioEndpointVolume** iEndpointVolume;
CAudioEndpointVolumeCallback** ppCAudioEndpointVolumeCallback;

UINT countDevices = NULL;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    INITCOMMONCONTROLSEX iCCE;
    iCCE.dwSize = sizeof(INITCOMMONCONTROLSEX);
    iCCE.dwICC = ICC_BAR_CLASSES;
    InitCommonControlsEx(&iCCE);

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_PROJECT1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
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

    return (int) msg.wParam;
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PROJECT1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_MENU +1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_PROJECT1);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_MINIMIZEBOX | WS_SYSMENU,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

BOOL CreateSliders(HWND hWnd) {
    HRESULT hr;
    hr = CoInitialize(NULL);
    IMMDeviceEnumerator* deviceEnumerator = NULL;
    hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER, __uuidof(IMMDeviceEnumerator), (LPVOID*)&deviceEnumerator);

    IMMDeviceCollection* deviceCollection = NULL;
    hr = deviceEnumerator->EnumAudioEndpoints(eRender, DEVICE_STATE_ACTIVE, &deviceCollection);

    deviceEnumerator->Release();
    deviceEnumerator = NULL;

    hr = deviceCollection->GetCount(&countDevices);
    fader = new HWND[countDevices];
    textFader = new HWND[countDevices];
    iEndpointDevice = new IMMDevice * [countDevices];
    iEndpointVolume = new IAudioEndpointVolume * [countDevices];
    ppCAudioEndpointVolumeCallback = new CAudioEndpointVolumeCallback* [countDevices];

    int x, y = 480;

    for (ULONG i = 0; i < countDevices; i++) {
        fader[i] = CreateWindowEx(0, TRACKBAR_CLASS, NULL, WS_CHILD | WS_VISIBLE | WS_TABSTOP | TBS_VERT | TBS_BOTH, 10 + i * 150, 10, 50, 300, hWnd, NULL, hInst, NULL);
        textFader[i] = CreateWindowEx(0, L"Static", NULL, WS_CHILD | WS_VISIBLE, 25 + i * 150, 310, 50, 30, hWnd, NULL, hInst, NULL);
        HWND hTextDevice = CreateWindowEx(0, L"Static", NULL, WS_CHILD | WS_VISIBLE, 25 + i * 150, 340, 100, 100, hWnd, NULL, hInst, NULL);

        hr = deviceCollection->Item(i, &iEndpointDevice[i]);
        hr = iEndpointDevice[i]->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_INPROC_SERVER, NULL, (LPVOID*)&iEndpointVolume[i]);

        ppCAudioEndpointVolumeCallback[i] = new CAudioEndpointVolumeCallback(fader[i], textFader[i]);
        hr = iEndpointVolume[i]->RegisterControlChangeNotify(ppCAudioEndpointVolumeCallback[i]);

        float currentVolume;
        hr = iEndpointVolume[i]->GetMasterVolumeLevelScalar(&currentVolume);
        char text[4];
        sprintf_s(text, "%d", (int)(currentVolume * MAX_VOL));
        SendMessageA(fader[i], TBM_SETPOS, TRUE, (int)MAX_VOL - (currentVolume * MAX_VOL));
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

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {

    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_SIZE: {
        RECT rect;
        GetWindowRect(hWnd, &rect);
        break;
    }
    case WM_CREATE: {
        CreateSliders(hWnd);
        break;
    }
    case WM_VSCROLL: {
        if (LOWORD(wParam) == TB_THUMBPOSITION || LOWORD(wParam) == TB_THUMBTRACK) {
            int value = (int)MAX_VOL - HIWORD(wParam);
            char text[5];
            sprintf_s(text, "%d", value);

            char textControl[20];
            for (int i = 0; i < countDevices; i++) {
                if (fader[i] == (HWND)lParam) {
                    sprintf_s(textControl, "%d", i);
                    float newValue = (float)(value) / MAX_VOL;
                    iEndpointVolume[i]->SetMasterVolumeLevelScalar(newValue, NULL);
                    SetWindowTextA(textFader[i], (LPCSTR)text);
                }
            }
        }
        break;
    }
    case WM_DESTROY:
        CoUninitialize();
        for (int i = 0; i < countDevices; i++) {
            iEndpointVolume[i]->UnregisterControlChangeNotify(ppCAudioEndpointVolumeCallback[i]);
        }
        delete[] fader;
        delete[] textFader;
        delete[] iEndpointVolume;
        delete[] iEndpointDevice;
        delete[] ppCAudioEndpointVolumeCallback;
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}