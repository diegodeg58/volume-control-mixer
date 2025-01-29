#pragma once
#include "CDialogSingleton.h"
class AboutWnd : public CDialogSingleton
{
private:
    INT_PTR Procedure(UINT message, WPARAM wParam, LPARAM lParam) override;

    LRESULT OnInitdialog(HWND hWnd, HWND wParam, LPARAM lParam);
    LRESULT OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify) const;
    AboutWnd(HWND hParent, HINSTANCE hInst) : CDialogSingleton(hParent, hInst) {};
};
