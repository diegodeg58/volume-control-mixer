#pragma once
#include "UI/CDialogSingleton.h"
class AboutWnd : public CDialogSingleton<AboutWnd>
{
private:
    friend CDialogSingleton *CDialogSingleton<AboutWnd>::GetInstance(const HWND hParent, const HINSTANCE hInst);
    INT_PTR Procedure(UINT message, WPARAM wParam, LPARAM lParam) override;

    LRESULT OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify) const;
    AboutWnd(HWND hParent, HINSTANCE hInst) : CDialogSingleton(hParent, hInst) {};
};
