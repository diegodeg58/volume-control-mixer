#pragma once
#include "framework.h"
#include "windowsx.h"
class CBaseDialog
{
public:
    // static CBaseDialog *GetInstance(const HWND hParent, const HINSTANCE hInst);
    CBaseDialog() = default;
    CBaseDialog(CBaseDialog &other) = delete;
    void operator=(const CBaseDialog &) = delete;

protected:
    HWND hWnd, hParent;
    HINSTANCE hInst;
    // static CBaseDialog* instance;

    static LRESULT CALLBACK sProcedure(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual INT_PTR CALLBACK Procedure(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;
    virtual void Show() = 0;

private:
    inline static CBaseDialog *GetObjectFromWindow(HWND hWnd)
    {
        return reinterpret_cast<CBaseDialog *>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    }

    inline static void SetObjectToWindow(HWND hWnd, CBaseDialog *pWnd)
    {
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
    }
};