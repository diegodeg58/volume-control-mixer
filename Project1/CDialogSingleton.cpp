#include "CDialogSingleton.h"
#include "resource.h"

CDialogSingleton *CDialogSingleton::instance = nullptr;

CDialogSingleton::CDialogSingleton(HWND hParent, HINSTANCE hInst)
{
    this->hParent = hParent;
    this->hInst = hInst;
    this->hWnd = NULL;
}

INT_PTR CDialogSingleton::Procedure(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    return (INT_PTR)FALSE;
}

CDialogSingleton *CDialogSingleton::GetInstance(const HWND hParent, const HINSTANCE hInst)
{
    if (!instance)
    {
        instance = new CDialogSingleton(hParent, hInst);
    }
    return instance;
}

void CDialogSingleton::Show()
{
    DialogBoxParam(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hParent, sProcedure, (LPARAM)this);
}

CDialogSingleton::~CDialogSingleton()
{
    delete instance;
    instance = nullptr;
}