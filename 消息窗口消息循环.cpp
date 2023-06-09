#include"消息循环声明.h"
//文本窗口消息循环
LRESULT CALLBACK cTextWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, Controller* current_project)
{
    switch (message)
    {
    case WM_COMMAND:
    {
        current_project->m_TextWind->SetTextPosition(Vector2(0, 0));
        switch (LOWORD(wParam))
        {
        case 10:
        {
            current_project->m_TextWind->SetOutMode(MSGMode::_ALL);
            break;
        }
        case 11:
        {
            current_project->m_TextWind->SetOutMode(MSGMode::_ERROR);
            break;
        }
        case 12:
        {
            current_project->m_TextWind->SetOutMode(MSGMode::_WARNING);
            break;
        }
        case 13:
        {
            current_project->m_TextWind->SetOutMode(MSGMode::_REMIND);
            break;
        }
        }
        SetFocus(current_project->m_hWnd);
        InvalidateRect(hWnd, NULL, false);
        break;
    }
    case WM_PAINT:
    {
        PAINTSTRUCT ps;

        HDC hdc = BeginPaint(hWnd, &ps);
        HPEN hpen = CreatePen(PS_SOLID, 0, RGB(255, 255, 255)); //创建 一条红色实线
        HGDIOBJ oldhpen = SelectObject(hdc, hpen); // 画笔选入设备环境，并保留先前画笔
        if (!current_project->m_FileLoad)
            current_project->updateMsg(hdc);
        SelectObject(hdc, oldhpen); // 恢复先前画笔
        DeleteObject(hpen); // 删除创建画笔hpen
        EndPaint(hWnd, &ps);
        break;
    }
    case WM_MOUSEWHEEL:
    {
        int zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
        current_project->m_TextWind->MoveText(Vector2(0, zDelta/15));
        InvalidateRect(current_project->m_TextWind->GethWnd(), NULL, false);
        break;
    }
    case WM_SIZE:
    {
        current_project->m_TextWind->UpdateWindowSize(LOWORD(lParam), HIWORD(lParam));
        break;
    }
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}