#include"消息循环声明.h"
//文本窗口消息循环
LRESULT CALLBACK cTextWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, project* current_project)
{
    switch (message)
    {
    case WM_COMMAND:
    {
        switch (LOWORD(wParam))
        {
        case 10:
        {
            current_project->MSG_att = 0;
            break;
        }
        case 11:
        {
            current_project->MSG_att = 1;
            break;
        }
        case 12:
        {
            current_project->MSG_att = 2;
            break;
        }
        case 13:
        {
            current_project->MSG_att = 3;
            break;
        }
        }
        SetFocus(current_project->hWnd);
        InvalidateRect(hWnd, NULL, false);
        break;
    }
    case WM_PAINT:
    {
        PAINTSTRUCT ps;

        HDC hdc = BeginPaint(hWnd, &ps);
        HPEN hpen = CreatePen(PS_SOLID, 0, RGB(255, 255, 255)); //创建 一条红色实线
        HGDIOBJ oldhpen = SelectObject(hdc, hpen); // 画笔选入设备环境，并保留先前画笔
        current_project->updateMsg(hdc);
        switch (current_project->MSG_att)
        {
        case 0:
        {
            TextOut(hdc, current_project->TEXTWND.m_rect.right - 200 - 100, 3, TEXT("显示所有消息"), 6);
            break;
        }
        case 1:
        {
            TextOut(hdc, current_project->TEXTWND.m_rect.right - 200 - 100, 3, TEXT("显示错误消息"), 6);
            break;
        }
        case 2:
        {
            TextOut(hdc, current_project->TEXTWND.m_rect.right - 200 - 100, 3, TEXT("显示警告消息"), 6);
            break;
        }
        case 3:
        {
            TextOut(hdc, current_project->TEXTWND.m_rect.right - 200 - 100, 3, TEXT("显示普通消息"), 6);
            break;
        }
        }
        SelectObject(hdc, oldhpen); // 恢复先前画笔
        DeleteObject(hpen); // 删除创建画笔hpen
        EndPaint(hWnd, &ps);
        break;
    }
    case WM_VSCROLL:
    {
        switch (LOWORD(wParam))
        {
        case SB_THUMBPOSITION:
        {
            current_project->TEXTWND.m_rect.top = (short int)HIWORD(wParam);
            break;
        }
        case SB_LINEDOWN:
        {
            if (current_project->TEXTWND.m_rect.top < current_project->MAX_runMSG)
                current_project->TEXTWND.m_rect.top++;
            break;
        }
        case SB_LINEUP:
        {
            if (current_project->TEXTWND.m_rect.top > 0)
                current_project->TEXTWND.m_rect.top--;
            break;
        }
        case SB_PAGEDOWN:
        {
            if (current_project->TEXTWND.m_rect.top < current_project->MAX_runMSG)
                current_project->TEXTWND.m_rect.top += 3;
            break;
        }
        case SB_PAGEUP:
        {
            if (current_project->TEXTWND.m_rect.top > 0)
                current_project->TEXTWND.m_rect.top -= 3;
            break;
        }
        }
        SetScrollRange(hWnd, SB_VERT, 0, current_project->runMSG.Long() + 1, false);
        SetScrollPos(hWnd, SB_VERT, current_project->TEXTWND.m_rect.top, true);
        InvalidateRect(hWnd, NULL, false);
        break;
    }
    case WM_SIZE:
    {
        current_project->TEXTWND.m_rect.right = LOWORD(lParam);  // 获得客户区宽度
        current_project->TEXTWND.m_rect.bottom = HIWORD(lParam);
        for (int i = 0; i < current_project->TEXTWND.ChildWnd.size(); i++)
            MoveWindow(current_project->TEXTWND.ChildWnd[i].hWnd, LOWORD(lParam) - 50 * (i + 1), 1, 50, 30, true);
        break;
    }
    case WM_CREATE:
    {
        WndMsg wndm;
        RECT m_rect;
        GetClientRect(hWnd, &m_rect);
        int cxClient = m_rect.right - m_rect.left;  // 获得客户区宽度
        SetScrollRange(hWnd, SB_VERT, 0, 1, false);
        SetScrollRange(hWnd, SB_HORZ, 0, 100, false);
        SetScrollPos(hWnd, SB_HORZ, 0, true);
        SetScrollPos(hWnd, SB_VERT, 0, true);
        wndm.hWnd = CreateWindow(
            TEXT("button"),
            TEXT("消息"),
            WS_CHILD | WS_VISIBLE,
            cxClient - 50, 0, 50, 20,
            hWnd,
            (HMENU)13,
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);
        current_project->TEXTWND.ChildWnd.push_back(wndm);
        wndm.hWnd = CreateWindow(
            TEXT("button"),
            TEXT("警告"),
            WS_CHILD | WS_VISIBLE,
            cxClient - 100, 0, 50, 20,
            hWnd,
            (HMENU)12,
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);
        current_project->TEXTWND.ChildWnd.push_back(wndm);
        wndm.hWnd = CreateWindow(
            TEXT("button"),
            TEXT("错误"),
            WS_CHILD | WS_VISIBLE,
            cxClient - 150, 0, 50, 20,
            hWnd,
            (HMENU)11,
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);
        current_project->TEXTWND.ChildWnd.push_back(wndm);
        wndm.hWnd = CreateWindow(
            TEXT("button"),
            TEXT("所有"),
            WS_CHILD | WS_VISIBLE,
            cxClient - 150, 0, 50, 20,
            hWnd,
            (HMENU)10,
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);
        current_project->TEXTWND.ChildWnd.push_back(wndm);
        break;
    }
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}