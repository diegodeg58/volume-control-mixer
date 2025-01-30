#pragma once
#include "CBaseDialog.h"
template <class T>
class CDialogSingleton : public CBaseDialog
{
public:
    static CDialogSingleton *GetInstance(const HWND hParent, const HINSTANCE hInst);
    CDialogSingleton(CDialogSingleton &other) = delete;
    void operator=(const CDialogSingleton &) = delete;

    void Show();
    ~CDialogSingleton();

protected:
    CDialogSingleton(HWND hParent, HINSTANCE hInst);
    static T *instance;
    virtual INT_PTR Procedure(UINT uMsg, WPARAM wParam, LPARAM lParam);
};
