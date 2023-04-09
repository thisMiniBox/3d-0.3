#include"WndData.h"
ATOM MyRegisterClass(
    HINSTANCE   hInstance,
    LPCWSTR     lpszClassName,
    LRESULT(*wnd_proc)(HWND, UINT, WPARAM, LPARAM),

    LPCWSTR     lpszMenuName,
    UINT        style,
    HCURSOR     hCursor,
    HBRUSH      hbrBackground,
    HICON       hIconSm,
    HICON       hIcon,
    int         cbClsExtra,
    int         cbWndExtra)
{
    WNDCLASSEXW wcex = {};

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = style;
    wcex.lpfnWndProc = wnd_proc;
    wcex.cbClsExtra = cbClsExtra;
    wcex.cbWndExtra = cbWndExtra;
    wcex.hInstance = hInstance;
    wcex.hIcon = hIcon;
    wcex.hCursor = hCursor;
    wcex.hbrBackground = hbrBackground;
    wcex.lpszMenuName = lpszMenuName;
    wcex.lpszClassName = lpszClassName;
    wcex.hIconSm = hIconSm;
    return RegisterClassExW(&wcex);
}