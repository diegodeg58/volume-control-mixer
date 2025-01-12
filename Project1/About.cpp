#include "About.h"

INT_PTR AboutWnd::Procedure(UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        HANDLE_MSG(hWnd, WM_INITDIALOG, OnInitdialog);
        HANDLE_MSG(hWnd, WM_COMMAND, OnCommand);
    }
    return (INT_PTR)FALSE;
}

LRESULT AboutWnd::OnInitdialog(HWND hWnd, HWND wParam, LPARAM lParam)
{
    return (INT_PTR)TRUE;
}

LRESULT AboutWnd::OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify) const
{
    switch (id)
    {
    case IDOK:
    case IDCANCEL:
        EndDialog(hWnd, LOWORD(id));
        return (INT_PTR)TRUE;
    }
    return (INT_PTR)FALSE;
}

AboutWnd::AboutWnd(HWND hParent, HINSTANCE hInst)
{
    this->hParent = hParent;
    this->hInst = hInst;
    this->hWnd = NULL;
}

AboutWnd *AboutWnd::GetInstance(const HWND hParent, const HINSTANCE hInst)
{
    if (!instance)
    {
        instance = new AboutWnd(hParent, hInst);
    }
    return instance;
}

void AboutWnd::Show()
{
    DialogBoxParam(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hParent, sProcedure, (LPARAM)this);
}

AboutWnd::~AboutWnd()
{
    delete instance;
    instance = nullptr;
}

AboutWnd* AboutWnd::instance = nullptr;