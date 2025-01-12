#pragma once
#include "resource.h"
#include "framework.h"
#include "windowsx.h"
#include "CBaseDialog.h"
class AboutWnd : public CBaseDialog
{
private:
    HINSTANCE hInst;
    static AboutWnd* instance;

    INT_PTR Procedure(UINT message, WPARAM wParam, LPARAM lParam);

    LRESULT OnInitdialog(HWND hWnd, HWND wParam, LPARAM lParam);
    LRESULT OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify) const;
    AboutWnd(HWND hParent, HINSTANCE hInst);

public:
    static AboutWnd *GetInstance(const HWND hParent, const HINSTANCE hInst);
    AboutWnd(AboutWnd &other) = delete;
    void operator=(const AboutWnd &) = delete;

    void Show();
    ~AboutWnd();
};
