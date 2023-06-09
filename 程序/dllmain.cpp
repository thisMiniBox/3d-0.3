#include<Windows.h>
#include<string>
#include"D:\\vs project\\win32游戏框架\\xzdll.hpp"
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    return TRUE;
}
#define MYDLL_EXPORTS
#ifdef MYDLL_EXPORTS
#define MYDLL_API __declspec(dllexport)
#else
#define MYDLL_API __declspec(dllimport)
#endif
using namespace std;

extern "C" MYDLL_API char* Get_string(const char* str) {
    string lstr = str;
    lstr += "来自dll";
    char* a = new char[lstr.size() + 1];
    strcpy_s(a, lstr.size() + 1, lstr.c_str());
    return a;
}

extern "C" MYDLL_API void Free_string(char* str) {
    delete[] str;
}

extern "C" MYDLL_API CommandUser UserMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    CommandUser out;
    switch (message)
    {
    case WM_CHAR:
    {
        switch (wParam)
        {
        case 'u':
            out.Act = CommandAct::CREATE_FOLDER;
            lstrcpyW(out.Parameter, L"来自dll");
            break;
        }
        break;
    }
    default:
        break;
    }
    return out;
}