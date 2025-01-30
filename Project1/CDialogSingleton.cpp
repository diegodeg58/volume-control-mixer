#include "CDialogSingleton.h"
#include "resource.h"

template <class T>
T *CDialogSingleton<T>::instance = nullptr;

template <class T>
CDialogSingleton<T>::CDialogSingleton(HWND hParent, HINSTANCE hInst)
{
    this->hParent = hParent;
    this->hInst = hInst;
    this->hWnd = NULL;
}

template <class T>
INT_PTR CDialogSingleton<T>::Procedure(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    return (INT_PTR)FALSE;
}

template <class T>
CDialogSingleton<T> *CDialogSingleton<T>::GetInstance(const HWND hParent, const HINSTANCE hInst)
{
    if (!instance)
    {
        instance = new T(hParent, hInst);
    }
    return instance;
}

template <class T>
void CDialogSingleton<T>::Show()
{
    DialogBoxParam(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hParent, sProcedure, (LPARAM)this);
}

template <class T>
CDialogSingleton<T>::~CDialogSingleton()
{
    delete instance;
    instance = nullptr;
}