#include"��Ϣѭ������.h"
//�ı�������Ϣѭ��
LRESULT CALLBACK cTextWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, project* current_project)
{
    switch (message)
    {
    case WM_COMMAND:
    {
        current_project->TEXTWND->SetTextPosition(Vector2(0, 0));
        switch (LOWORD(wParam))
        {
        case 10:
        {
            current_project->TEXTWND->SetOutMode(MSGMode::_ALL);
            break;
        }
        case 11:
        {
            current_project->TEXTWND->SetOutMode(MSGMode::_ERROR);
            break;
        }
        case 12:
        {
            current_project->TEXTWND->SetOutMode(MSGMode::_WARNING);
            break;
        }
        case 13:
        {
            current_project->TEXTWND->SetOutMode(MSGMode::_REMIND);
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

        SelectObject(hdc, oldhpen); // �ָ���ǰ����
        DeleteObject(hpen); // ɾ����������hpen
        EndPaint(hWnd, &ps);
        break;
    }
    case WM_MOUSEWHEEL:
    {
        int zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
        current_project->TEXTWND->MoveText(Vector2(0, zDelta/15));
        InvalidateRect(current_project->TEXTWND->GethWnd(), NULL, false);
        break;
    }
    case WM_SIZE:
    {
        current_project->TEXTWND->UpdateWindowSize(LOWORD(lParam), HIWORD(lParam));
        break;
    }
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}