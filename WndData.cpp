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
int getBit(int num, int pos)
{
    return (num >> pos) & 1;
}
ReturnedOfLoadFile operator|(ReturnedOfLoadFile a, ReturnedOfLoadFile b)
{
    return static_cast<ReturnedOfLoadFile>(static_cast<unsigned int>(a) | static_cast<unsigned int>(b));
}
ReturnedOfLoadFile& operator|=(ReturnedOfLoadFile& a, ReturnedOfLoadFile b)
{
    a = static_cast<ReturnedOfLoadFile>(static_cast<unsigned int>(a) | static_cast<unsigned int>(b));
    return a;
}