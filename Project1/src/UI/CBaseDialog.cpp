#include "CBaseDialog.h"
#include "../../resource.h"

void CBaseDialog::Show()
{
    DialogBoxParam(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hParent, sProcedure, (LPARAM)this);
}

LRESULT CALLBACK CBaseDialog::sProcedure(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    CBaseDialog *pWnd;
    if (uMsg == WM_INITDIALOG)
    {
        pWnd = reinterpret_cast<CBaseDialog *>(lParam);
        pWnd->hWnd = hwnd;
        SetObjectToWindow(hwnd, pWnd);
    }
    pWnd = GetObjectFromWindow(hwnd);

    if (pWnd)
        return pWnd->Procedure(uMsg, wParam, lParam);
    else
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
}