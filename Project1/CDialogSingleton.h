#pragma once
#include "CBaseDialog.h"
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
    static CDialogSingleton *instance;
    virtual INT_PTR Procedure(UINT uMsg, WPARAM wParam, LPARAM lParam);
};
