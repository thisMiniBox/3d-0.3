#include "framework.h"
#include "机器学习win.h"

HINSTANCE hInstance;
Controller* Central_control = new Controller;
HBRUSH g_hBrush = NULL; // 全局变量，用于存储画刷
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
#ifdef _DEBUG
    AllocConsole();
    FILE* stream;
    freopen_s(&stream, "CON", "r", stdin);//重定向输入流
    freopen_s(&stream, "CON", "w", stdout);//重定向输入流
#endif // DEBUG

    // 初始化 GDI+ 库
    ULONG_PTR gdiplusToken;
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    hInstance = hInstance;
    Central_control->SETWND.className = L"EDIT";
    InitCommonControls();
    MSG msg;
    Central_control->CreateWind(hInstance);

    HMENU hMenu = LoadMenu(hInstance, MAKEINTRESOURCE(IDC_WIN));
    SetMenu(Central_control->m_hWnd, hMenu);
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WIN));

    SendMessage(Central_control->m_hWnd, WM_COMMAND, MAKEWPARAM(ID_OPENGL, 0), 0);

    Central_control->Command(L"loadfile D:\\3d模型\\nanosuit.obj");
    // 主消息循环:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        
    }
    delete Central_control;
    // 释放 GDI+ 资源
    Gdiplus::GdiplusShutdown(gdiplusToken);
#ifdef _DEBUG
    // 关闭重定向的流
    fclose(stdin);
    fclose(stdout);
    // 释放控制台窗口
    FreeConsole();
    PostQuitMessage(0);
#endif // DEBUG
    return (int) msg.wParam;
}

//主窗口消息循环
LRESULT CALLBACK Controller::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
        if (HIWORD(wParam) == 0)return Menu(Central_control->GethInstance(), hWnd, message, wParam, lParam, Central_control);
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
        int cxClient = LOWORD(lParam);
        int cyClient = HIWORD(lParam);
        Central_control->Size(cxClient, cyClient);
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
    return cFileWndProc(hWnd, message, wParam, lParam, Central_control);
}
//画面循环**********************始
LRESULT CALLBACK GDIWND::WndProcGDI(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    return cMainWndProc(hWnd, message, wParam, lParam, Central_control);
}
LRESULT CALLBACK D3DWND11::WndProcD3D11(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    return cD3D11WndProc(hWnd, message, wParam, lParam, Central_control);
}
LRESULT CALLBACK OpenGLWnd::WndProcOpenGL(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    return cMainWndProc(hWnd, uMsg, wParam, lParam, Central_control);
}
//画面循环**********************终
//属性窗口循环
LRESULT CALLBACK DetaileWind::DetaileWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    return cDetaileWndProc(hWnd, message, wParam, lParam, Central_control);
}
//文本循环
LRESULT CALLBACK TextOutWind::TextWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    return cTextWndProc(hWnd, message, wParam, lParam, Central_control);
}
//内容控件消息循环
INT_PTR FileContentView::Dlgproc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_UPDATE:
    {
        HWND hWndList = GetDlgItem(hDlg, IDC_FILE_VIEW);
        return Central_control->GetFocusObject()->ListControlView(hWndList,Central_control->GetImageList(),Central_control->GetImageListIndex());
    }
    case WM_SIZE:
    {
        RECT crect;
        GetClientRect(hDlg, &crect);
        HWND hControl = GetDlgItem(hDlg, IDC_FILE_VIEW);
        MoveWindow(hControl, 10, 10, crect.right - 20, crect.bottom - 20, true);
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
                hWndEdit = (HWND)lParam;
                GetWindowText(hWndEdit, szText, _countof(szText));
                if (IsNumeric(szText))
                {
                    VEC.x = std::wcstod(szText, nullptr) - Central_control->DETAWND->GetTarget()->GetPosition().x;
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
                    VEC.y = (std::wcstod(szText, nullptr) - Central_control->DETAWND->GetTarget()->GetPosition().y);
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
                    VEC.z = std::wcstod(szText, nullptr) - Central_control->DETAWND->GetTarget()->GetPosition().z;
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
            Central_control->DETAWND->GetTarget()->Move(VEC);
            InvalidateRect(Central_control->MAINWND->GethWnd(), NULL, false);
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
        std::string name = Central_control->DETAWND->GetTarget()->GetName();
        MultiByteToWideChar(CP_ACP, 0, name.c_str(), name.size(), wname, 128);
        SetWindowText(hWndEdit, wname);
        hWndEdit = GetDlgItem(hDlg, IDC_N_TYPE);
        std::wstring ws;
        switch (Central_control->DETAWND->GetTarget()->GetType())
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
        case OT_MESH:
            ws = L"网格";
            break;
        case OT_MATERIAL:
            ws = L"材质";
            break;
        case OT_PICTURE:
            ws = L"图片";
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
            wchar_t szText[128];
            switch (GetDlgCtrlID(GetFocus()))
            {
            case IDC_N_NAME:
            {
                hWndEdit = GetDlgItem(hDlg, IDC_N_NAME);
                GetWindowText(hWndEdit, szText, _countof(szText));
                Central_control->SetFileName(Central_control->DETAWND->GetTarget(), szText);
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
    case WM_INITDIALOG:
    {
        HWND hWndControl = GetDlgItem(hDlg, IDC_RO_FOV);
        SendMessage(hWndControl, TBM_SETRANGEMIN, TRUE, -180);
        SendMessage(hWndControl, TBM_SETRANGEMAX, TRUE, 180);
    }
    case WM_UPDATE:
    {
        Rotation v = Central_control->DETAWND->GetTarget()->GetRotate();
        HWND hWndControl = GetDlgItem(hDlg, IDC_RO_XEDIT);
        std::wstring wstr = NumberToWString(v.axis.x);
        SetWindowText(hWndControl, wstr.c_str());
        hWndControl = GetDlgItem(hDlg, IDC_RO_YEDIT);
        wstr = NumberToWString(v.axis.y);
        SetWindowText(hWndControl, wstr.c_str());
        hWndControl = GetDlgItem(hDlg, IDC_RO_ZEDIT);
        wstr = NumberToWString(v.axis.z);
        SetWindowText(hWndControl, wstr.c_str());
        hWndControl = GetDlgItem(hDlg, IDC_RO_WEDIT);
        wstr = NumberToWString(vec::RadToDeg(v.angle));
        SetWindowText(hWndControl, wstr.c_str());
        hWndControl = GetDlgItem(hDlg, IDC_RO_FOV);
        SendMessage(hWndControl, TBM_SETPOS, TRUE, vec::RadToDeg(v.angle));
        break;
    }
    case WM_SIZE:
    {
        RECT hDlgRect;
        GetWindowRect(hDlg, &hDlgRect);
        int TextW = 20;

        struct ControlParams
        {
            int id;
            int x;
            int y;
            int width;
            int height;
            bool isSpecial;
        };

        ControlParams ParamsList[] = {
            {IDC_RO_X, 0, 0, TextW, 0, true},
            {IDC_RO_Y, 0, 0, TextW, 0, true},
            {IDC_RO_Z, 0, 0, TextW, 0, true},
            {IDC_RO_XEDIT, TextW, 0, 0, 0, false},
            {IDC_RO_YEDIT, TextW, 0, 0, 0, false},
            {IDC_RO_ZEDIT, TextW, 0, 0, 0, false},
            {IDC_RO_TITLE, 0, 0, 0, 0, false},
            {IDC_RO_TIP, 0, 0, 0, 0, false},
            {IDC_RO_W, 0, 0, 0, 0, false},
            {IDC_RO_FOV, 0, 0, 0, 0, false},
            {IDC_RO_WEDIT, 0, 0, 0, 0, false},
            {IDC_RO_ERROR, 0, 0, 0, 0, false},
            {IDC_RO_QUATERNION, 0, 0, LOWORD(lParam) / 2, 0, true},
            {IDC_RO_EULERANGLE, LOWORD(lParam) / 2, 0, LOWORD(lParam) / 2, 0, true}
        };

        for (auto& params : ParamsList)
        {
            HWND hWndEdit = GetDlgItem(hDlg, params.id);
            RECT EditRect;
            GetWindowRect(hWndEdit, &EditRect);
            params.height = EditRect.bottom - EditRect.top;
            params.y = EditRect.top - hDlgRect.top;
            if (params.isSpecial)
            {
                params.width = EditRect.right - EditRect.left;
            }
            else
            {
                params.width = LOWORD(lParam) - params.x;
            }

            MoveWindow(hWndEdit, params.x, params.y, params.width, params.height, true);
            if (!params.isSpecial && params.x == 0)
            {
                params.width = LOWORD(lParam);
            }
            else if (!params.isSpecial)
            {
                params.width = LOWORD(lParam) / 2;
            }
            params.x = params.width;
        }
        break;
    }
    case WM_COMMAND:
    {
        if (HIWORD(wParam) == EN_CHANGE)
        {
            HWND hWndEdit = nullptr;
            Rotation Qua = Central_control->DETAWND->GetTarget()->GetRotate();
            Vector Aixs = Qua.axis;
            wchar_t szText[64];
            switch (GetDlgCtrlID(GetFocus()))
            {
            case IDC_RO_XEDIT:
            {
                hWndEdit = GetDlgItem(hDlg, IDC_RO_XEDIT);
                GetWindowText(hWndEdit, szText, _countof(szText));
                if (IsNumeric(szText))
                {
                    Aixs.x = std::wcstod(szText, nullptr);
                    hWndEdit = GetDlgItem(hDlg, IDC_RO_ERROR);
                    SetWindowText(hWndEdit, L"");
                }
                else
                {
                    hWndEdit = GetDlgItem(hDlg, IDC_RO_ERROR);
                    SetWindowText(hWndEdit, L"请输入正确的数字");
                }
                break;
            }
            case IDC_RO_YEDIT:
            {
                hWndEdit = GetDlgItem(hDlg, IDC_RO_YEDIT);
                GetWindowText(hWndEdit, szText, _countof(szText));
                if (IsNumeric(szText))
                {
                    Aixs.y = std::wcstod(szText, nullptr);
                    hWndEdit = GetDlgItem(hDlg, IDC_RO_ERROR);
                    SetWindowText(hWndEdit, L"");
                }
                else
                {
                    hWndEdit = GetDlgItem(hDlg, IDC_RO_ERROR);
                    SetWindowText(hWndEdit, L"请输入正确的数字");
                }
                break;
            }
            case IDC_RO_ZEDIT:
            {
                hWndEdit = GetDlgItem(hDlg, IDC_RO_ZEDIT);
                GetWindowText(hWndEdit, szText, _countof(szText));
                if (IsNumeric(szText))
                {
                    Aixs.z = std::wcstod(szText, nullptr);
                    hWndEdit = GetDlgItem(hDlg, IDC_RO_ERROR);
                    SetWindowText(hWndEdit, L"");
                }
                else
                {
                    hWndEdit = GetDlgItem(hDlg, IDC_RO_ERROR);
                    SetWindowText(hWndEdit, L"请输入正确的数字");
                }
                break;
            }
            case IDC_RO_WEDIT:
            {
                hWndEdit = GetDlgItem(hDlg, IDC_RO_WEDIT);
                GetWindowText(hWndEdit, szText, _countof(szText));
                if (IsNumeric(szText))
                {
                    hWndEdit = GetDlgItem(hDlg, IDC_RO_FOV);
                    int angle = (std::wcstod(szText, nullptr));
                    SendMessage(hWndEdit, TBM_SETPOS, TRUE, (angle));
                    Qua.angle = vec::DegToRad(angle);
                    hWndEdit = GetDlgItem(hDlg, IDC_RO_ERROR);
                    SetWindowText(hWndEdit, L"");
                }
                else
                {
                    hWndEdit = GetDlgItem(hDlg, IDC_RO_ERROR);
                    SetWindowText(hWndEdit, L"请输入正确的数字");
                }
                break;
            }
            }
            if (Aixs != Vector(0, 0, 0))
                Qua.axis = Aixs;
            Central_control->DETAWND->GetTarget()->SetRotate(Qua);
            InvalidateRect(Central_control->MAINWND->GethWnd(), NULL, false);
        }

        break;
    }
    case WM_HSCROLL:
    {
        int nScrollBar = LOWORD(wParam);
        HWND hWndCtl = (HWND)lParam;
        int nPos = (short)HIWORD(wParam);
        switch (nScrollBar) 
        {
        case SB_THUMBTRACK: // 用户正在拖动滑块
        case SB_THUMBPOSITION: // 用户释放滑块
        {
            HWND hWndControl = GetDlgItem(hDlg, IDC_RO_WEDIT);
            std::wstring wstr = std::to_wstring(nPos);
            SetWindowText(hWndControl, wstr.c_str());
            Rotation OldQuat = Central_control->DETAWND->GetTarget()->GetRotate();
            OldQuat.angle = DegToRad(nPos);
            Central_control->DETAWND->GetTarget()->SetRotate(OldQuat);
            break;
        }
        default:
            break;
        }
        break;
    }
    case WM_CTLCOLORSTATIC:
    {
        HDC hdcStatic = (HDC)wParam;
        int ctrlID = GetDlgCtrlID((HWND)lParam);

        if (ctrlID == IDC_RO_ERROR) // 第一个静态文本框
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

void MoveDlgItem(HWND hDlg, int id, int x, int y, int cx, int cy) 
{
    HWND hWnd = GetDlgItem(hDlg, id);
    MoveWindow(hWnd, x, y, cx, cy, TRUE);
}
INT_PTR TransForm_变换控件::Dlgproc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_INITDIALOG:
    {
        HWND hWndControl = GetDlgItem(hDlg, IDC_TRAN_ROT_SLIDER);
        SendMessage(hWndControl, TBM_SETRANGEMIN, TRUE, -180);
        SendMessage(hWndControl, TBM_SETRANGEMAX, TRUE, 180);
    }
    case WM_UPDATE:
    {
        Vector v = Central_control->GetFocusObject()->GetPosition();
        HWND hWndControl = GetDlgItem(hDlg, IDC_TRAN_POS_X);
        std::wstring wstr = NumberToWString(v.x);
        SetWindowText(hWndControl, wstr.c_str());
        hWndControl = GetDlgItem(hDlg, IDC_TRAN_POS_Y);
        wstr = NumberToWString(v.y);
        SetWindowText(hWndControl, wstr.c_str());
        hWndControl = GetDlgItem(hDlg, IDC_TRAN_POS_Z);
        wstr = NumberToWString(v.z);
        SetWindowText(hWndControl, wstr.c_str());
        v = Central_control->GetFocusObject()->GetScale();
        hWndControl = GetDlgItem(hDlg, IDC_TRAN_SCA_X);
        wstr = NumberToWString(v.x);
        SetWindowText(hWndControl, wstr.c_str());
        hWndControl = GetDlgItem(hDlg, IDC_TRAN_SCA_Y);
        wstr = NumberToWString(v.y);
        SetWindowText(hWndControl, wstr.c_str());
        hWndControl = GetDlgItem(hDlg, IDC_TRAN_SCA_Z);
        wstr = NumberToWString(v.z);
        SetWindowText(hWndControl, wstr.c_str());
        Rotation rot = Central_control->GetFocusObject()->GetRotate();
        v = rot.axis;
        hWndControl = GetDlgItem(hDlg, IDC_TRAN_ROT_X);
        wstr = NumberToWString(v.x);
        SetWindowText(hWndControl, wstr.c_str());
        hWndControl = GetDlgItem(hDlg, IDC_TRAN_ROT_Y);
        wstr = NumberToWString(v.y);
        SetWindowText(hWndControl, wstr.c_str());
        hWndControl = GetDlgItem(hDlg, IDC_TRAN_ROT_Z);
        wstr = NumberToWString(v.z);
        SetWindowText(hWndControl, wstr.c_str());
        hWndControl = GetDlgItem(hDlg, IDC_TRAN_ROT_ANG);
        wstr = NumberToWString(vec::RadToDeg(rot.angle));
        SetWindowText(hWndControl, wstr.c_str());
        hWndControl = GetDlgItem(hDlg, IDC_TRAN_ROT_SLIDER);
        SendMessage(hWndControl, TBM_SETPOS, TRUE, vec::RadToDeg(rot.angle));
        break;
    }
    case WM_SIZE:
    {
        // 计算子窗口区域大小和位置
        int cxClient = LOWORD(lParam);
        int cyClient = HIWORD(lParam);
        float cxUnit = (float)cxClient / 30;
        float cyUnit = (float)cyClient / 4;

        // 移动子窗口
        MoveDlgItem(hDlg, IDC_TRAN_POS_TEXT, 0, 0, cxUnit * 7, cyUnit);
        MoveDlgItem(hDlg, IDC_TRAN_POS_X, cxUnit * 7, 0, cxUnit * 7, cyUnit);
        MoveDlgItem(hDlg, IDC_TRAN_POS_Y, cxUnit * 7 * 2 + cxUnit, 0, cxUnit * 7, cyUnit);
        MoveDlgItem(hDlg, IDC_TRAN_POS_Z, cxUnit * 7 * 3 + cxUnit * 2, 0, cxUnit * 7, cyUnit);
        MoveDlgItem(hDlg, IDC_TRAN_SCA_TEXT, 0, cyUnit, cxUnit * 7, cyUnit);
        MoveDlgItem(hDlg, IDC_TRAN_SCA_X, cxUnit * 7, cyUnit, cxUnit * 7, cyUnit);
        MoveDlgItem(hDlg, IDC_TRAN_SCA_Y, cxUnit * 7 * 2 + cxUnit, cyUnit, cxUnit * 7, cyUnit);
        MoveDlgItem(hDlg, IDC_TRAN_SCA_Z, cxUnit * 7 * 3 + cxUnit * 2, cyUnit, cxUnit * 7, cyUnit);
        MoveDlgItem(hDlg, IDC_TRAN_ROT_TEXT1, 0, cyUnit * 2, cxUnit * 7, cyUnit);
        MoveDlgItem(hDlg, IDC_TRAN_ROT_X, cxUnit * 7, cyUnit * 2, cxUnit * 7, cyUnit);
        MoveDlgItem(hDlg, IDC_TRAN_ROT_Y, cxUnit * 7 * 2 + cxUnit, cyUnit * 2, cxUnit * 7, cyUnit);
        MoveDlgItem(hDlg, IDC_TRAN_ROT_Z, cxUnit * 7 * 3 + cxUnit * 2, cyUnit * 2, cxUnit * 7, cyUnit);
        MoveDlgItem(hDlg, IDC_TRAN_ROT_TEXT2, 0, cyUnit * 3, cxUnit * 7, cyUnit);
        MoveDlgItem(hDlg, IDC_TRAN_ROT_SLIDER, cxUnit * 7, cyUnit * 3, cxUnit * 7 * 2 + cxUnit, cyUnit);
        MoveDlgItem(hDlg, IDC_TRAN_ROT_ANG, cxUnit * 7 * 3 + cxUnit * 2, cyUnit * 3, cxUnit * 7, cyUnit);
        break;
    }
    case WM_COMMAND:
    {
        if (HIWORD(wParam) == EN_CHANGE)
        {
            HWND hWndControl = (HWND)lParam;
            Vector Position;
            Vector Scale = Central_control->GetFocusObject()->GetScale();
            Rotation rotation = Central_control->GetFocusObject()->GetRotate();
            wchar_t szText[64];
            switch (GetDlgCtrlID(GetFocus()))
            {
            case IDC_TRAN_POS_X:
            {
                GetWindowText(hWndControl, szText, _countof(szText));
                if (IsNumeric(szText))
                {
                    Position.x = std::wcstod(szText, nullptr) - Central_control->DETAWND->GetTarget()->GetPosition().x;
                }
                break;
            }
            case IDC_TRAN_POS_Y:
            {
                GetWindowText(hWndControl, szText, _countof(szText));
                if (IsNumeric(szText))
                {
                    Position.y = std::wcstod(szText, nullptr) - Central_control->DETAWND->GetTarget()->GetPosition().y;
                }
                break;
            }
            case IDC_TRAN_POS_Z:
            {
                GetWindowText(hWndControl, szText, _countof(szText));
                if (IsNumeric(szText))
                {
                    Position.z = std::wcstod(szText, nullptr) - Central_control->DETAWND->GetTarget()->GetPosition().z;
                }
                break;
            }
            case IDC_TRAN_SCA_X:
            {
                GetWindowText(hWndControl, szText, _countof(szText));
                if (IsNumeric(szText))
                {
                    Scale.x = std::wcstod(szText, nullptr);
                }
                break;
            }
            case IDC_TRAN_SCA_Y:
            {
                GetWindowText(hWndControl, szText, _countof(szText));
                if (IsNumeric(szText))
                {
                    Scale.y = std::wcstod(szText, nullptr);
                }
                break;
            }
            case IDC_TRAN_SCA_Z:
            {
                GetWindowText(hWndControl, szText, _countof(szText));
                if (IsNumeric(szText))
                {
                    Scale.z = std::wcstod(szText, nullptr);
                }
                break;
            }
            case IDC_TRAN_ROT_X:
            {
                GetWindowText(hWndControl, szText, _countof(szText));
                if (IsNumeric(szText))
                {
                    rotation.axis.x = std::wcstod(szText, nullptr);
                }
                break;
            }
            case IDC_TRAN_ROT_Y:
            {
                GetWindowText(hWndControl, szText, _countof(szText));
                if (IsNumeric(szText))
                {
                    rotation.axis.y = std::wcstod(szText, nullptr);
                }
                break;
            }
            case IDC_TRAN_ROT_Z:
            {
                GetWindowText(hWndControl, szText, _countof(szText));
                if (IsNumeric(szText))
                {
                    rotation.axis.z = std::wcstod(szText, nullptr);
                }
                break;
            }
            case IDC_TRAN_ROT_ANG:
            {
                GetWindowText(hWndControl, szText, _countof(szText));
                if (IsNumeric(szText))
                {
                    double Angle = std::wcstod(szText, nullptr);
                    while (Angle > 180)
                        Angle -= 360;
                    while (Angle < -180)
                        Angle += 360;
                    hWndControl = GetDlgItem(hDlg, IDC_TRAN_ROT_SLIDER);
                    SendMessage(hWndControl, TBM_SETPOS, TRUE, Angle);
                    rotation.angle = vec::DegToRad(Angle);
                }
                break;
            }
            }
            Central_control->DETAWND->GetTarget()->Move(Position);
            Central_control->GetFocusObject()->SetScale(Scale);
            Central_control->GetFocusObject()->SetRotate(rotation);
            InvalidateRect(Central_control->MAINWND->GethWnd(), NULL, false);
        }

        break;
    }
    case WM_HSCROLL:
    {
        int nScrollBar = LOWORD(wParam);
        HWND hWndCtl = (HWND)lParam;
        int nPos = (short)HIWORD(wParam);
        switch (nScrollBar)
        {
        case SB_THUMBTRACK: // 用户正在拖动滑块
        case SB_THUMBPOSITION: // 用户释放滑块
        {
            HWND hWndControl = GetDlgItem(hDlg, IDC_TRAN_ROT_ANG);
            std::wstring wstr = std::to_wstring(nPos);
            SetWindowText(hWndControl, wstr.c_str());
            Rotation OldQuat = Central_control->DETAWND->GetTarget()->GetRotate();
            OldQuat.angle = DegToRad(nPos);
            Central_control->DETAWND->GetTarget()->SetRotate(OldQuat);
            break;
        }
        default:
            break;
        }
        break;
    }
    case WM_CLOSE:
        EndDialog(hDlg, 0);
        break;
    default:
        return DefWindowProc(hDlg, uMsg, wParam, lParam);
    }
    return FALSE;
}

LRESULT CALLBACK DetaileWind::PictureProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_LBUTTONDBLCLK:
    {

        break;
    }
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

LRESULT CALLBACK BottomWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) 
{
    switch (message)
    {
    case WM_COMMAND:
    {
        if ((HWND)lParam)
            Central_control->GetBottom()->Select((HWND)lParam);
    }
    case WM_SIZE:
    {
        if (Central_control->GetBottom())
        {
            RECT rect;
            GetClientRect(hWnd, &rect);
            Central_control->GetBottom()->Size(rect.right, rect.bottom);
        }
        break;
    }
    case WM_CREATE:
    {

        break;
    }
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
        break;
    }
    return 0;
}

LRESULT CALLBACK InputOutput::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
        switch (LOWORD(wParam))
        {
        case IDC_IO_OK:
        {
            wchar_t szText[128];
            HWND hWndControl = GetDlgItem(hWnd, IDC_IO_IN);
            GetWindowText(hWndControl, szText, _countof(szText));
            Central_control->Command(szText);
            SetFocus(hWndControl);
            break;
        }
        }
        break;
    }
    case WM_SIZE:
    {
        Central_control->GetIOWind()->Size();
        break;
    }
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
        break;
    }
    return 0;
}