#include"消息循环声明.h"
using namespace DirectX;
LRESULT CALLBACK cD3D11WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, project* current_project)
{
    switch (message)
    {
    case WM_SIZE:
    {
        RECT rect;
        GetClientRect(hWnd, &rect);
        current_project->MAINWND->SetRect(rect);
        break;
    }
    case WM_PAINT:
    {
        current_project->MAINWND->Draw(current_project->GetModels(), *current_project->view);
        break;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;

}