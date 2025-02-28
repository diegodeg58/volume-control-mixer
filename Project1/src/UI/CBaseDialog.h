#pragma once
#include "../../framework.h"
class CBaseDialog
{
public:
    CBaseDialog(HWND hParent, HINSTANCE hInst) : hParent(hParent), hInst(hInst), hWnd(NULL) {};
    void Show();

protected:
    HWND hWnd;
    const HWND hParent;
    const HINSTANCE hInst;

    virtual INT_PTR CALLBACK Procedure(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;

private:
    CBaseDialog() = delete;
    CBaseDialog(CBaseDialog &other) = delete;
    void operator=(const CBaseDialog &) = delete;
    static LRESULT CALLBACK sProcedure(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    inline static CBaseDialog *GetObjectFromWindow(HWND hWnd)
    {
        return reinterpret_cast<CBaseDialog *>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    }

    inline static void SetObjectToWindow(HWND hWnd, CBaseDialog *pWnd)
    {
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
    }
};