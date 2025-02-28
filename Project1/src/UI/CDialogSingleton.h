#pragma once
#include "CBaseDialog.h"
template <class T>
class CDialogSingleton : public CBaseDialog
{
public:
    static CDialogSingleton *GetInstance(const HWND hParent, const HINSTANCE hInst);

protected:
    ~CDialogSingleton();
    CDialogSingleton(HWND hParent, HINSTANCE hInst) : CBaseDialog(hParent, hInst) {};

private:
    static T *instance;
    CDialogSingleton(CDialogSingleton &other) = delete;
    void operator=(const CDialogSingleton &) = delete;
};
