#include "About.h"
#include "resource.h"
#include "windowsx.h"

INT_PTR AboutWnd::Procedure(UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        HANDLE_MSG(hWnd, WM_COMMAND, OnCommand);
    }
    return (INT_PTR)FALSE;
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