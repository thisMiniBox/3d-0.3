#include"��Ϣѭ������.h"
//�ı�������Ϣѭ��
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
            current_project->MSG_att = _ALL;
            break;
        }
        case 11:
        {
            current_project->MSG_att = _ERROR;
            break;
        }
        case 12:
        {
            current_project->MSG_att = _WANING;
            break;
        }
        case 13:
        {
            current_project->MSG_att = _REMIND;
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
        HPEN hpen = CreatePen(PS_SOLID, 0, RGB(255, 255, 255)); //���� һ����ɫʵ��
        HGDIOBJ oldhpen = SelectObject(hdc, hpen); // ����ѡ���豸��������������ǰ����
        current_project->updateMsg(hdc);
        switch (current_project->MSG_att)
        {
        case 0:
        {
            TextOut(hdc, current_project->TEXTWND.m_rect.right - 200 - 100, 3, TEXT("��ʾ������Ϣ"), 6);
            break;
        }
        case 1:
        {
            TextOut(hdc, current_project->TEXTWND.m_rect.right - 200 - 100, 3, TEXT("��ʾ������Ϣ"), 6);
            break;
        }
        case 2:
        {
            TextOut(hdc, current_project->TEXTWND.m_rect.right - 200 - 100, 3, TEXT("��ʾ������Ϣ"), 6);
            break;
        }
        case 3:
        {
            TextOut(hdc, current_project->TEXTWND.m_rect.right - 200 - 100, 3, TEXT("��ʾ��ͨ��Ϣ"), 6);
            break;
        }
        }
        SelectObject(hdc, oldhpen); // �ָ���ǰ����
        DeleteObject(hpen); // ɾ����������hpen
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
        current_project->TEXTWND.m_rect.right = LOWORD(lParam);  // ��ÿͻ������
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
        int cxClient = m_rect.right - m_rect.left;  // ��ÿͻ������
        SetScrollRange(hWnd, SB_VERT, 0, 1, false);
        SetScrollRange(hWnd, SB_HORZ, 0, 100, false);
        SetScrollPos(hWnd, SB_HORZ, 0, true);
        SetScrollPos(hWnd, SB_VERT, 0, true);
        wndm.hWnd = CreateWindow(
            TEXT("button"),
            TEXT("��Ϣ"),
            WS_CHILD | WS_VISIBLE,
            cxClient - 50, 0, 50, 20,
            hWnd,
            (HMENU)13,
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);
        current_project->TEXTWND.ChildWnd.push_back(wndm);
        wndm.hWnd = CreateWindow(
            TEXT("button"),
            TEXT("����"),
            WS_CHILD | WS_VISIBLE,
            cxClient - 100, 0, 50, 20,
            hWnd,
            (HMENU)12,
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);
        current_project->TEXTWND.ChildWnd.push_back(wndm);
        wndm.hWnd = CreateWindow(
            TEXT("button"),
            TEXT("����"),
            WS_CHILD | WS_VISIBLE,
            cxClient - 150, 0, 50, 20,
            hWnd,
            (HMENU)11,
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);
        current_project->TEXTWND.ChildWnd.push_back(wndm);
        wndm.hWnd = CreateWindow(
            TEXT("button"),
            TEXT("����"),
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