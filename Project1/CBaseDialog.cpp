#include "CBaseDialog.h"

LRESULT CALLBACK CBaseDialog::sProcedure(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    CBaseDialog *pWnd;
    if (uMsg == WM_INITDIALOG)
    {
        pWnd = reinterpret_cast<CBaseDialog *>(lParam);
        pWnd->hWnd = hwnd;
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
    }
    pWnd = GetObjectFromWindow(hwnd);

    if (pWnd)
        return pWnd->Procedure(uMsg, wParam, lParam);
    else
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
}