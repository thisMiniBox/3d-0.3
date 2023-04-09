// 机器学习win.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include "机器学习win.h"

HINSTANCE hIns;
project* current_project = new project;
HBRUSH g_hBrush = NULL; // 全局变量，用于存储画刷
// 此代码模块中包含的函数的前向声明:
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    hIns = hInstance;
    current_project->TEXTWND.className = L"TextWnd";
    current_project->SETWND.className = L"EDIT";
    MyRegisterClass(hInstance, current_project->TEXTWND.className.c_str(), TextWndProc);
    InitCommonControls();
    MSG msg;
    current_project->CreateWind(hInstance);


#ifdef _DEBUG
    AllocConsole();
    FILE* stream;
    freopen_s(&stream, "CON", "r", stdin);//重定向输入流
    freopen_s(&stream, "CON", "w", stdout);//重定向输入流
#endif // DEBUG
    HMENU hMenu = LoadMenu(hIns, MAKEINTRESOURCE(IDC_WIN));
    SetMenu(current_project->hWnd, hMenu);
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WIN));
    // 主消息循环:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        
    }
    delete current_project;
    return (int) msg.wParam;
}

//主窗口消息循环
LRESULT CALLBACK project::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
        if (HIWORD(wParam) == 0)return Menu(current_project->GethInstance(), hWnd, message, wParam, lParam, current_project);
        return DefWindowProc(hWnd, message, wParam, lParam);
        
        break;
    }
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        EndPaint(hWnd, &ps);
        break; 
    }

    case WM_DESTROY:
    {
        PostQuitMessage(0);
        break;
    }
    case WM_SIZE:
    {
        
        int cxClient = LOWORD(lParam);  // 获得客户区宽度
        int cyClient = HIWORD(lParam);
        RECT rect;
        GetClientRect(hWnd, &rect);
        current_project->SetRect(rect);
        MoveWindow(current_project->TEXTWND.hWnd, 0, cyClient - 150, cxClient / 5 * 4, 150, true);
        MoveWindow(current_project->FILEWND->GethWnd(), 0, 50, cxClient / 5, cyClient - 200, true);
        MoveWindow(current_project->DETAWND->GethWnd(), cxClient / 5 * 4, 50, cxClient / 5, cyClient - 5, true);
        MoveWindow(current_project->MAINWND->GethWnd(), cxClient / 5, 50, cxClient / 5 * 3, cyClient - 200, true);
        MoveWindow(current_project->SETWND.hWnd, 0, 0, cxClient, 50, true);
        
        break;
    }
    case WM_GETMINMAXINFO:
    {
        LPMINMAXINFO lpmmi = (LPMINMAXINFO)lParam;
        lpmmi->ptMinTrackSize.x = 640;
        lpmmi->ptMinTrackSize.y = 480;
        lpmmi->ptMaxTrackSize.x = GetSystemMetrics(SM_CXSCREEN);
        lpmmi->ptMaxTrackSize.y = GetSystemMetrics(SM_CYSCREEN);

        return 0;
    }
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
//文件管理窗口消息循环
LRESULT CALLBACK FileWind::FileWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    return cFileWndProc(hWnd, message, wParam, lParam, current_project);
}
//画面循环
LRESULT CALLBACK GDIWND::WndProcGDI(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    return cMainWndProc(hWnd, message, wParam, lParam, current_project);
}
LRESULT CALLBACK D3DWND11::WndProcD3D11(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    return cD3D11WndProc(hWnd, message, wParam, lParam, current_project);
}
LRESULT CALLBACK OpenGLWnd::WndProcOpenGL(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    return cMainWndProc(hWnd, uMsg, wParam, lParam, current_project);
}
//属性窗口循环
LRESULT CALLBACK DetaileWind::DetaileWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    return cDetaileWndProc(hWnd, message, wParam, lParam, current_project);
}
//文本循环
LRESULT CALLBACK TextWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    return cTextWndProc(hWnd, message, wParam, lParam, current_project);
}
//位置控件消息循环
INT_PTR Position_位置控件::Dlgproc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_UPDATE:
    case WM_INITDIALOG:
    {
        Vector v = current_project->DETAWND->GetTarget()->GetPosition();
        HWND hWndEdit = GetDlgItem(hDlg, IDC_X);
        std::wstring wstr = std::to_wstring(v.x);
        SetWindowText(hWndEdit, wstr.c_str());
        hWndEdit = GetDlgItem(hDlg, IDC_Y);
        wstr = std::to_wstring(v.y);
        SetWindowText(hWndEdit, wstr.c_str());
        hWndEdit = GetDlgItem(hDlg, IDC_Z);
        wstr = std::to_wstring(v.z);
        SetWindowText(hWndEdit, wstr.c_str());
        hWndEdit = GetDlgItem(hDlg, IDC_P_ERROR);
        SetWindowText(hWndEdit, L"");
        break;
    }
    case WM_SIZE:
    {
        RECT EditRect;
        RECT hDlgRect;
        GetWindowRect(hDlg, &hDlgRect);
        HWND hWndEdit = GetDlgItem(hDlg, IDC_X);
        GetWindowRect(hWndEdit, &EditRect);
        MoveWindow(hWndEdit, EditRect.left - hDlgRect.left, EditRect.top - hDlgRect.top, LOWORD(lParam) - (EditRect.left - hDlgRect.left) * 1.5, EditRect.bottom - EditRect.top, true);
        hWndEdit = GetDlgItem(hDlg, IDC_Y);
        GetWindowRect(hWndEdit, &EditRect);
        MoveWindow(hWndEdit, EditRect.left - hDlgRect.left, EditRect.top - hDlgRect.top, LOWORD(lParam) - (EditRect.left - hDlgRect.left) * 1.5, EditRect.bottom - EditRect.top, true);
        hWndEdit = GetDlgItem(hDlg, IDC_Z);
        GetWindowRect(hWndEdit, &EditRect);
        MoveWindow(hWndEdit, EditRect.left - hDlgRect.left, EditRect.top - hDlgRect.top, LOWORD(lParam) - (EditRect.left - hDlgRect.left) * 1.5, EditRect.bottom - EditRect.top, true);
        hWndEdit = GetDlgItem(hDlg, IDC_TITLE);
        GetWindowRect(hWndEdit, &EditRect);
        MoveWindow(hWndEdit, EditRect.left - hDlgRect.left, EditRect.top - hDlgRect.top, LOWORD(lParam) - (EditRect.left - hDlgRect.left) * 1.5, EditRect.bottom - EditRect.top, true);
        hWndEdit = GetDlgItem(hDlg, IDC_P_ERROR);
        GetWindowRect(hWndEdit, &EditRect);
        MoveWindow(hWndEdit, EditRect.left - hDlgRect.left, EditRect.top - hDlgRect.top, LOWORD(lParam) - (EditRect.left - hDlgRect.left) * 1.5, EditRect.bottom - EditRect.top, true);
        break;
    }
    case WM_COMMAND:
    {
        if (HIWORD(wParam) == EN_CHANGE)
        {
            HWND hWndEdit = nullptr;
            Vector VEC;
            wchar_t szText[64];
            switch (GetDlgCtrlID(GetFocus()))
            {
            case IDC_X:
            {
                hWndEdit = GetDlgItem(hDlg, IDC_X);
                GetWindowText(hWndEdit, szText, _countof(szText));
                if (IsNumeric(szText))
                {
                    VEC.x = std::wcstod(szText, nullptr) - current_project->DETAWND->GetTarget()->GetPosition().x;
                    hWndEdit = GetDlgItem(hDlg, IDC_P_ERROR);
                    SetWindowText(hWndEdit, L"");
                }
                else 
                {
                    hWndEdit = GetDlgItem(hDlg, IDC_P_ERROR);
                    SetWindowText(hWndEdit, L"请输入正确的数字");
                }
                break;
            }
            case IDC_Y:
            {
                hWndEdit = GetDlgItem(hDlg, IDC_Y);
                GetWindowText(hWndEdit, szText, _countof(szText));
                if (IsNumeric(szText))
                {
                    VEC.y = (std::wcstod(szText, nullptr) - current_project->DETAWND->GetTarget()->GetPosition().y);
                    hWndEdit = GetDlgItem(hDlg, IDC_P_ERROR);
                    SetWindowText(hWndEdit, L"");
                }
                else
                {
                    hWndEdit = GetDlgItem(hDlg, IDC_P_ERROR);
                    SetWindowText(hWndEdit, L"请输入正确的数字");
                }
                break;
            }
            case IDC_Z:
            {
                hWndEdit = GetDlgItem(hDlg, IDC_Z);
                GetWindowText(hWndEdit, szText, _countof(szText));
                if (IsNumeric(szText))
                {
                    VEC.z = std::wcstod(szText, nullptr) - current_project->DETAWND->GetTarget()->GetPosition().z;
                    hWndEdit = GetDlgItem(hDlg, IDC_P_ERROR);
                    SetWindowText(hWndEdit, L"");
                }
                else
                {
                    hWndEdit = GetDlgItem(hDlg, IDC_P_ERROR);
                    SetWindowText(hWndEdit, L"请输入正确的数字");
                }
                break;
            }
            }
            current_project->DETAWND->GetTarget()->Move(VEC);
            InvalidateRect(current_project->MAINWND->GethWnd(), NULL, false);
        }
        
        break;
    }
    case WM_CTLCOLORSTATIC:
    {
        HDC hdcStatic = (HDC)wParam;
        int ctrlID = GetDlgCtrlID((HWND)lParam);

        if (ctrlID == IDC_P_ERROR) // 第一个静态文本框
        {
            if (g_hBrush == NULL) // 如果画刷对象不存在，则创建一个画刷
            {
                g_hBrush = CreateSolidBrush(RGB(255, 255, 0)); // 黄色画刷
            }

            SetTextColor(hdcStatic, RGB(255, 0, 0)); // 红色前景色
            SetBkMode(hdcStatic, TRANSPARENT); // 透明背景色
            SetBkColor(hdcStatic, RGB(255, 255, 0)); // 背景色也设置为黄色
            return (LRESULT)g_hBrush; // 返回画刷句柄
        }
    }
    break;
    case WM_DESTROY:
    {
        // 销毁画刷对象
        if (g_hBrush != NULL)
        {
            DeleteObject(g_hBrush);
            g_hBrush = NULL;
        }
    }
    break;

    case WM_CLOSE:
        // 关闭对话框
        EndDialog(hDlg, 0);
        break;
    default:
        return DefWindowProc(hDlg, uMsg, wParam, lParam);
    }
    return FALSE;
}
//名称控件消息循环
INT_PTR Name_对象名称控件::Dlgproc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_UPDATE:
    case WM_INITDIALOG:
    {
        wchar_t wname[128] = {};
        HWND hWndEdit = GetDlgItem(hDlg, IDC_N_NAME);
        std::string name = current_project->DETAWND->GetTarget()->GetName();
        MultiByteToWideChar(CP_ACP, 0, name.c_str(), name.size(), wname, 128);
        SetWindowText(hWndEdit, wname);
        hWndEdit = GetDlgItem(hDlg, IDC_N_TYPE);
        std::wstring ws;
        switch (current_project->DETAWND->GetTarget()->GetType())
        {
        case OT_FOLDER:
            ws = L"文件夹";
            break;
        case OT_MODEL:
            ws = L"模型";
            break;
        case OT_CAMERA:
            ws = L"摄像机";
            break;
        default:
            ws = L"未知类型";
            break;
        }
        SetWindowText(hWndEdit, ws.c_str());
        break;
    }
    case WM_COMMAND:
    {
        if (HIWORD(wParam) == EN_CHANGE)
        {
            HWND hWndEdit = nullptr;
            wchar_t szText[64];
            switch (GetDlgCtrlID(GetFocus()))
            {
            case IDC_N_NAME:
            {
                hWndEdit = GetDlgItem(hDlg, IDC_N_NAME);
                GetWindowText(hWndEdit, szText, _countof(szText));
                char szBuf[64];
                WideCharToMultiByte(CP_ACP, 0, szText, -1, szBuf, 64, NULL, NULL);
                current_project->DETAWND->GetTarget()->SetName(szBuf);
                current_project->FILEWND->FixItemName(current_project->DETAWND->GetTree(), szText);
                break;
            }
            }
        }
        break;
    }
    case WM_SIZE:
    {
        RECT EditRect;
        RECT hDlgRect;
        GetWindowRect(hDlg, &hDlgRect);
        HWND hWndEdit = GetDlgItem(hDlg, IDC_N_NAME);
        GetWindowRect(hWndEdit, &EditRect);
        MoveWindow(hWndEdit, EditRect.left - hDlgRect.left, EditRect.top - hDlgRect.top, LOWORD(lParam) - (EditRect.left - hDlgRect.left) * 1.5, EditRect.bottom - EditRect.top, true);
        hWndEdit = GetDlgItem(hDlg, IDC_N_TITLE);
        GetWindowRect(hWndEdit, &EditRect);
        MoveWindow(hWndEdit, EditRect.left - hDlgRect.left, EditRect.top - hDlgRect.top, LOWORD(lParam) / 2-10, EditRect.bottom - EditRect.top, true);
        hWndEdit = GetDlgItem(hDlg, IDC_N_TYPE);
        GetWindowRect(hWndEdit, &EditRect);
        MoveWindow(hWndEdit, LOWORD(lParam) / 2, EditRect.top - hDlgRect.top, LOWORD(lParam) / 2-10, EditRect.bottom - EditRect.top, true);

        break;
    }
    case WM_CLOSE:
        // 关闭对话框
        EndDialog(hDlg, 0);
        return TRUE;
        break;
    }
    return FALSE;

}
INT_PTR Rotation_旋转控件::Dlgproc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_UPDATE:
    case WM_INITDIALOG:
    {
        Vector v = current_project->DETAWND->GetTarget()->GetPosition();
        HWND hWndEdit = GetDlgItem(hDlg, IDC_X);
        std::wstring wstr = std::to_wstring(v.x);
        SetWindowText(hWndEdit, wstr.c_str());
        hWndEdit = GetDlgItem(hDlg, IDC_Y);
        wstr = std::to_wstring(v.y);
        SetWindowText(hWndEdit, wstr.c_str());
        hWndEdit = GetDlgItem(hDlg, IDC_Z);
        wstr = std::to_wstring(v.z);
        SetWindowText(hWndEdit, wstr.c_str());
        hWndEdit = GetDlgItem(hDlg, IDC_P_ERROR);
        SetWindowText(hWndEdit, L"");
        break;
    }
    case WM_SIZE:
    {
        RECT EditRect;
        RECT hDlgRect;
        GetWindowRect(hDlg, &hDlgRect);
        HWND hWndEdit = GetDlgItem(hDlg, IDC_X);
        GetWindowRect(hWndEdit, &EditRect);
        MoveWindow(hWndEdit, EditRect.left - hDlgRect.left, EditRect.top - hDlgRect.top, LOWORD(lParam) - (EditRect.left - hDlgRect.left) * 1.5, EditRect.bottom - EditRect.top, true);
        hWndEdit = GetDlgItem(hDlg, IDC_Y);
        GetWindowRect(hWndEdit, &EditRect);
        MoveWindow(hWndEdit, EditRect.left - hDlgRect.left, EditRect.top - hDlgRect.top, LOWORD(lParam) - (EditRect.left - hDlgRect.left) * 1.5, EditRect.bottom - EditRect.top, true);
        hWndEdit = GetDlgItem(hDlg, IDC_Z);
        GetWindowRect(hWndEdit, &EditRect);
        MoveWindow(hWndEdit, EditRect.left - hDlgRect.left, EditRect.top - hDlgRect.top, LOWORD(lParam) - (EditRect.left - hDlgRect.left) * 1.5, EditRect.bottom - EditRect.top, true);
        hWndEdit = GetDlgItem(hDlg, IDC_TITLE);
        GetWindowRect(hWndEdit, &EditRect);
        MoveWindow(hWndEdit, EditRect.left - hDlgRect.left, EditRect.top - hDlgRect.top, LOWORD(lParam) - (EditRect.left - hDlgRect.left) * 1.5, EditRect.bottom - EditRect.top, true);
        hWndEdit = GetDlgItem(hDlg, IDC_P_ERROR);
        GetWindowRect(hWndEdit, &EditRect);
        MoveWindow(hWndEdit, EditRect.left - hDlgRect.left, EditRect.top - hDlgRect.top, LOWORD(lParam) - (EditRect.left - hDlgRect.left) * 1.5, EditRect.bottom - EditRect.top, true);
        break;
    }
    case WM_COMMAND:
    {
        if (HIWORD(wParam) == EN_CHANGE)
        {
            HWND hWndEdit = nullptr;
            Vector VEC;
            wchar_t szText[64];
            switch (GetDlgCtrlID(GetFocus()))
            {
            case IDC_X:
            {
                hWndEdit = GetDlgItem(hDlg, IDC_X);
                GetWindowText(hWndEdit, szText, _countof(szText));
                if (IsNumeric(szText))
                {
                    VEC.x = std::wcstod(szText, nullptr) - current_project->DETAWND->GetTarget()->GetPosition().x;
                    hWndEdit = GetDlgItem(hDlg, IDC_P_ERROR);
                    SetWindowText(hWndEdit, L"");
                }
                else
                {
                    hWndEdit = GetDlgItem(hDlg, IDC_P_ERROR);
                    SetWindowText(hWndEdit, L"请输入正确的数字");
                }
                break;
            }
            case IDC_Y:
            {
                hWndEdit = GetDlgItem(hDlg, IDC_Y);
                GetWindowText(hWndEdit, szText, _countof(szText));
                if (IsNumeric(szText))
                {
                    VEC.y = (std::wcstod(szText, nullptr) - current_project->DETAWND->GetTarget()->GetPosition().y);
                    hWndEdit = GetDlgItem(hDlg, IDC_P_ERROR);
                    SetWindowText(hWndEdit, L"");
                }
                else
                {
                    hWndEdit = GetDlgItem(hDlg, IDC_P_ERROR);
                    SetWindowText(hWndEdit, L"请输入正确的数字");
                }
                break;
            }
            case IDC_Z:
            {
                hWndEdit = GetDlgItem(hDlg, IDC_Z);
                GetWindowText(hWndEdit, szText, _countof(szText));
                if (IsNumeric(szText))
                {
                    VEC.z = std::wcstod(szText, nullptr) - current_project->DETAWND->GetTarget()->GetPosition().z;
                    hWndEdit = GetDlgItem(hDlg, IDC_P_ERROR);
                    SetWindowText(hWndEdit, L"");
                }
                else
                {
                    hWndEdit = GetDlgItem(hDlg, IDC_P_ERROR);
                    SetWindowText(hWndEdit, L"请输入正确的数字");
                }
                break;
            }
            }
            current_project->DETAWND->GetTarget()->Move(VEC);
            InvalidateRect(current_project->MAINWND->GethWnd(), NULL, false);
        }

        break;
    }
    case WM_CTLCOLORSTATIC:
    {
        HDC hdcStatic = (HDC)wParam;
        int ctrlID = GetDlgCtrlID((HWND)lParam);

        if (ctrlID == IDC_P_ERROR) // 第一个静态文本框
        {
            if (g_hBrush == NULL) // 如果画刷对象不存在，则创建一个画刷
            {
                g_hBrush = CreateSolidBrush(RGB(255, 255, 0)); // 黄色画刷
            }

            SetTextColor(hdcStatic, RGB(255, 0, 0)); // 红色前景色
            SetBkMode(hdcStatic, TRANSPARENT); // 透明背景色
            SetBkColor(hdcStatic, RGB(255, 255, 0)); // 背景色也设置为黄色
            return (LRESULT)g_hBrush; // 返回画刷句柄
        }
    }
    break;
    case WM_DESTROY:
    {
        // 销毁画刷对象
        if (g_hBrush != NULL)
        {
            DeleteObject(g_hBrush);
            g_hBrush = NULL;
        }
    }
    break;

    case WM_CLOSE:
        // 关闭对话框
        EndDialog(hDlg, 0);
        break;
    default:
        return DefWindowProc(hDlg, uMsg, wParam, lParam);
    }
    return FALSE;
}