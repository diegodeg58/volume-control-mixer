#include "CDialogSingleton.h"
#include "resource.h"
#include "About.h"

// Explicit instantiation of the template for AboutWnd
template class CDialogSingleton<AboutWnd>;

template <class T>
T *CDialogSingleton<T>::instance = nullptr;

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
CDialogSingleton<T>::~CDialogSingleton()
{
    delete instance;
    instance = nullptr;
}