#include "Utilities.h"

RECT Utilities::GetLocalCoordinates(HWND hWnd)
{
    RECT rect;
    GetWindowRect(hWnd, &rect);
    MapWindowPoints(HWND_DESKTOP, GetParent(hWnd), (LPPOINT) &rect, 2);
    return rect;
}