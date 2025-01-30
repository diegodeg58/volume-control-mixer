#pragma once
#include "CBaseDialog.h"
template <class T>
class CDialogSingleton : public CBaseDialog
{
public:
    static CDialogSingleton *GetInstance(const HWND hParent, const HINSTANCE hInst);

    ~CDialogSingleton();

protected:
    CDialogSingleton(CDialogSingleton &other) = delete;
    void operator=(const CDialogSingleton &) = delete;
    CDialogSingleton(HWND hParent, HINSTANCE hInst) : CBaseDialog(hParent, hInst) {};
    static T *instance;
};
