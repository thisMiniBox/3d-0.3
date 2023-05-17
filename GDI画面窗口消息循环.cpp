#include"消息循环声明.h"
//此消息循环由所有画面窗口共享
void S_旋转当前摄像机(Controller* CP_当前项目, const float& x_左右旋转弧度, const float& y_上下旋转弧度)
{
    vec::Vector front;
    front.SetX(cos(x_左右旋转弧度) * cos(y_上下旋转弧度));
    front.SetY(sin(y_上下旋转弧度));
    front.SetZ(sin(x_左右旋转弧度) * cos(y_上下旋转弧度));
    CP_当前项目->view->SetDirection(front);
}

//画面窗口消息循环
LRESULT CALLBACK cMainWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, Controller* current_project)
{
    const float V = 10;
    static bool Mouse_是否鼠标控制 = false;
    static float fx_方向角 = -90;
    static float fy_方向角 = 0;
    static bool is_w_pressed = false;
    static bool is_a_pressed = false;
    static bool is_s_pressed = false;
    static bool is_d_pressed = false;
    static bool is_c_pressed = false;
    static bool is_space_pressed = false;
    static ULONGLONG time_start, time_end;
    static float timec = 0;
    UINT_PTR frameRateController = 0;
    time_start = GetTickCount64();
    timec = (float)(time_start - time_end) / 1000*V;
    if (is_w_pressed) {
        current_project->view->Move(current_project->view->GetDirection() * timec);
    }
    if (is_a_pressed) {
        current_project->view->Move(-current_project->view->GetRight() * timec);
    }
    if (is_s_pressed) {
        current_project->view->Move(-current_project->view->GetDirection() * timec);
    }
    if (is_d_pressed) {
        current_project->view->Move(current_project->view->GetRight() * timec);
    }
    if (is_c_pressed)
    {
        current_project->view->Move(-current_project->view->GetUp() * timec);
    }
    if (is_space_pressed)
    {
        current_project->view->Move(current_project->view->GetUp() * timec);
    }
    time_end = time_start;
    switch (message)
    {
    case WM_PAINT:
    {

        current_project->MAINWND->Draw(current_project->GetModels(), *current_project->view);
        break;
    }
    case WM_TIMER:
    {
        MSG msg;
        while (PeekMessage(&msg, NULL, WM_TIMER, WM_TIMER, PM_REMOVE))
        {
        }
        if ((UINT_PTR)wParam == frameRateController)
        {
            InvalidateRect(hWnd, NULL, false);
        }
        break;
    }
    case WM_MOUSEMOVE:
    {
        if (Mouse_是否鼠标控制)
        {
            POINT pt = { current_project->MAINWND->GetWidth() / 2, current_project->MAINWND->GetHeight() / 2 };
            fx_方向角 -= (pt.x - LOWORD(lParam)) / 5.0f;
            fy_方向角 += (pt.y - HIWORD(lParam)) / 5.0f;
            if (fx_方向角 > 360)fx_方向角 -= 360;
            if (fx_方向角 < -360)fx_方向角 += 360;
            if (fy_方向角 > 89)fy_方向角 = 89;
            if (fy_方向角 < -89)fy_方向角 = -89;
            S_旋转当前摄像机(current_project, (fx_方向角 * PI / 180), (fy_方向角 * PI / 180));
            ClientToScreen(hWnd, &pt);
            SetCursorPos(pt.x, pt.y);
            RECT rc;
            SetRect(&rc, pt.x - 100, pt.y - 100, pt.x + 100, pt.y + 100);
            ClipCursor(&rc);
        }
        else
            ClipCursor(NULL);
        break;
    }
    case WM_MOUSEWHEEL:
    {
        float zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
        float fov = current_project->view->GetFieldOfView();
        if (fov >= 1.0f && fov <= 45.0f)
            fov -= zDelta/120;
        if (fov <= 1.0f)
            fov = 1.0f;
        if (fov >= 45.0f)
            fov = 45.0f;
        current_project->view->SetFieldOfView(fov);
    }
    case WM_KEYDOWN:
        switch (wParam) {
        case 'W':
        case 'w':
            is_w_pressed = true;
            break;
        case 'A':
        case 'a':
            is_a_pressed = true;
            break;
        case 'S':
        case 's':
            is_s_pressed = true;
            break;
        case 'D':
        case 'd':
            is_d_pressed = true;
            break;
        case 'C':
        case 'c':
            is_c_pressed = true;
            break;
        case VK_SPACE:
            is_space_pressed = true;
            break;
        }
        break;
    case WM_KEYUP:
        switch (wParam) {
        case 'W':
        case 'w':
            is_w_pressed = false;
            break;
        case 'A':
        case 'a':
            is_a_pressed = false;
            break;
        case 'S':
        case 's':
            is_s_pressed = false;
            break;
        case 'D':
        case 'd':
            is_d_pressed = false;
            break;
        case 'C':
        case 'c':
            is_c_pressed = false;
            break;
        case VK_SPACE:
            is_space_pressed = false;
            break;
        }
        break;
    case WM_RBUTTONDOWN:
    {
        ShowCursor(Mouse_是否鼠标控制);
        Mouse_是否鼠标控制 = !Mouse_是否鼠标控制;
        if (Mouse_是否鼠标控制)
        {
            POINT pt = { current_project->MAINWND->GetRect().right / 2, current_project->MAINWND->GetRect().bottom / 2 };
            ClientToScreen(hWnd, &pt);
            SetCursorPos(pt.x, pt.y);
            InvalidateRect(hWnd, NULL, false);
        }
        break;
    }
    case WM_MOUSEACTIVATE:
    {
        SetFocus(hWnd);
        break;
    }
    case WM_SIZE:
    {
        RECT m_rect = {};
        GetClientRect(hWnd, &m_rect);
        current_project->MAINWND->SetRect(m_rect);
        current_project->view->SetAspectRatio((float)LOWORD(lParam)/HIWORD(lParam));
        break;
    }
    case WM_CREATE:
    {
        frameRateController = SetTimer(hWnd, 0, 1000 / 60, NULL);
        break;
    }
    case WM_CLOSE:
    {
        KillTimer(hWnd, frameRateController);
        break;
    }
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}