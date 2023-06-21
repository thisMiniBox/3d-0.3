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
    Central_control->m_MenuWind.className = L"EDIT";
    InitCommonControls();
    MSG msg;
    Central_control->CreateWind(hInstance);

    HMENU hMenu = LoadMenu(hInstance, MAKEINTRESOURCE(IDC_WIN));
    SetMenu(Central_control->m_hWnd, hMenu);
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WIN));

    //切换为OpenGL渲染
    SendMessage(Central_control->m_hWnd, WM_COMMAND, MAKEWPARAM(ID_OPENGL, 0), 0);
    //预加载树模型
#ifdef _DEBUG
    Central_control->Command(L"loadfile C:\\Users\\xujiacheng\\Desktop\\程序\\Model\\背景.obj");
#endif
#ifndef _DEBUG
    Central_control->Command(L"loadfile Model\\背景.obj");
#endif // _RELEASE

    PointLight* PL = new PointLight("点光源1", Vector3(-2, 0, 10), Vector(1.0f), 5, 10);
    Central_control->AddObject(PL);
    PL = new PointLight("点光源2", Vector3(2, 0, 10), Vector(1.0f), 5, 10);
    Central_control->AddObject(PL);
    DirectionalLight* DL = new DirectionalLight;
    Central_control->AddObject(DL);
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
    static UINT_PTR timer = 10;
    switch (message)
    {
    case WM_COMMAND:
    {
        if (HIWORD(wParam) == 0)return Menu(Central_control->GethInstance(), hWnd, message, wParam, lParam, Central_control);
        return DefWindowProc(hWnd, message, wParam, lParam);
        
        break;
    }
    case WM_TIMER:
    {
        if ((UINT_PTR)wParam == timer)
        {
            std::wstring text = L"最大帧数：" + std::to_wstring(Central_control->GetMaxFPS());
            SetWindowText(Central_control->m_MenuWind.hWnd, text.c_str());
            if (GetRunMode_g() == RUNMODE::RM_RUN)
            {
                Central_control->UpdateKeyframeView(ChildWindSign::KF_CanvasWind);
                Central_control->UpdateKeyframeView(ChildWindSign::KF_TimeWind);
            }
        }
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
    case WM_CREATE:
    {
        SetTimer(hWnd, timer, 500, NULL);
        break;
    }
    case WM_GETMINMAXINFO:
    {
        MINMAXINFO* pmmi = (MINMAXINFO*)lParam;
        pmmi->ptMinTrackSize.x = WL_WindowMaxWidth;
        pmmi->ptMinTrackSize.y = WL_WindowMaxHeigt;
        pmmi->ptMaxTrackSize.x = INT_MAX;
        pmmi->ptMaxTrackSize.y = INT_MAX;
        return 0;
    }
    break;
    case WM_CLOSE:
    {
        KillTimer(hWnd, timer);
        return DefWindowProc(hWnd, message, wParam, lParam);
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
    case UM_UPDATE:
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
    case UM_UPDATE:
    case WM_INITDIALOG:
    {
        wchar_t wname[128] = {};
        HWND hWndEdit = GetDlgItem(hDlg, IDC_N_NAME);
        std::string name = Central_control->m_EditWind->GetTarget()->GetName();
        MultiByteToWideChar(CP_ACP, 0, name.c_str(), name.size(), wname, 128);
        SetWindowText(hWndEdit, wname);
        hWndEdit = GetDlgItem(hDlg, IDC_N_TYPE);
        std::wstring ws;
        switch (Central_control->m_EditWind->GetTarget()->GetType())
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
                Central_control->SetFileName(Central_control->m_EditWind->GetTarget(), szText);
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
    case UM_UPDATE:
    {
        Rotation v = Central_control->m_EditWind->GetTarget()->GetRotate();
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
            Rotation Qua = Central_control->m_EditWind->GetTarget()->GetRotate();
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
            Central_control->m_EditWind->GetTarget()->SetRotate(Qua);
            InvalidateRect(Central_control->GetMainWind()->GethWnd(), NULL, false);
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
            Rotation OldQuat = Central_control->m_EditWind->GetTarget()->GetRotate();
            OldQuat.angle = DegToRad(nPos);
            Central_control->m_EditWind->GetTarget()->SetRotate(OldQuat);
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
    case UM_UPDATE:
    {
        Object* focusOBJ = Central_control->GetFocusObject();
        if (!focusOBJ)
            break;
        Vector v = focusOBJ->GetPosition();
        HWND hWndControl = GetDlgItem(hDlg, IDC_TRAN_POS_X);
        std::wstring wstr = NumberToWString(v.x);
        SetWindowText(hWndControl, wstr.c_str());
        hWndControl = GetDlgItem(hDlg, IDC_TRAN_POS_Y);
        wstr = NumberToWString(v.y);
        SetWindowText(hWndControl, wstr.c_str());
        hWndControl = GetDlgItem(hDlg, IDC_TRAN_POS_Z);
        wstr = NumberToWString(v.z);
        SetWindowText(hWndControl, wstr.c_str());
        v = focusOBJ->GetScale();
        hWndControl = GetDlgItem(hDlg, IDC_TRAN_SCA_X);
        wstr = NumberToWString(v.x);
        SetWindowText(hWndControl, wstr.c_str());
        hWndControl = GetDlgItem(hDlg, IDC_TRAN_SCA_Y);
        wstr = NumberToWString(v.y);
        SetWindowText(hWndControl, wstr.c_str());
        hWndControl = GetDlgItem(hDlg, IDC_TRAN_SCA_Z);
        wstr = NumberToWString(v.z);
        SetWindowText(hWndControl, wstr.c_str());
        Rotation rot = focusOBJ->GetRotate();
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
        switch (focusOBJ->GetType())
        {
        case OT_POINT_LIGHT:
        {
            hWndControl= GetDlgItem(hDlg, IDC_TRAN_POS_TEXT);
            SetWindowText(hWndControl, L"位置");
            hWndControl = GetDlgItem(hDlg, IDC_TRAN_SCA_TEXT);
            SetWindowText(hWndControl, L"颜色");
            hWndControl = GetDlgItem(hDlg, IDC_TRAN_ROT_TEXT1);
            SetWindowText(hWndControl, L"强度");
            hWndControl = GetDlgItem(hDlg, IDC_TRAN_ROT_Y);
            SetWindowText(hWndControl, L"半径");
            hWndControl = GetDlgItem(hDlg, IDC_TRAN_ROT_TEXT2);
            SetWindowText(hWndControl, L"未启用");
            break;
        }
        case OT_CAMERA:
        {
            hWndControl = GetDlgItem(hDlg, IDC_TRAN_POS_TEXT);
            SetWindowText(hWndControl, L"位置");
            hWndControl = GetDlgItem(hDlg, IDC_TRAN_SCA_TEXT);
            SetWindowText(hWndControl, L"未启用");
            hWndControl = GetDlgItem(hDlg, IDC_TRAN_ROT_TEXT1);
            SetWindowText(hWndControl, L"方向");
            hWndControl = GetDlgItem(hDlg, IDC_TRAN_ROT_TEXT2);
            SetWindowText(hWndControl, L"视野");
            break;
        }
        default:
        {
            hWndControl = GetDlgItem(hDlg, IDC_TRAN_POS_TEXT);
            SetWindowText(hWndControl, L"位置");
            hWndControl = GetDlgItem(hDlg, IDC_TRAN_SCA_TEXT);
            SetWindowText(hWndControl, L"缩放");
            hWndControl = GetDlgItem(hDlg, IDC_TRAN_ROT_TEXT1);
            SetWindowText(hWndControl, L"旋转");
            hWndControl = GetDlgItem(hDlg, IDC_TRAN_ROT_TEXT2);
            SetWindowText(hWndControl, L"旋转角");
        }
            break;
        }
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
            Object* focusOBJ = Central_control->GetFocusObject();
            if (!focusOBJ)
                break;
            Central_control->SetRunMode(RUNMODE::RM_EDIT);
            Vector Position;
            Vector Scale = focusOBJ->GetScale();
            Rotation rotation = focusOBJ->GetRotate();
            wchar_t szText[64];
            switch (GetDlgCtrlID(GetFocus()))
            {
            case IDC_TRAN_POS_X:
            {
                GetWindowText(hWndControl, szText, _countof(szText));
                if (IsNumeric(szText))
                {
                    Position.x = std::wcstod(szText, nullptr) - Central_control->m_EditWind->GetTarget()->GetPosition().x;
                }
                break;
            }
            case IDC_TRAN_POS_Y:
            {
                GetWindowText(hWndControl, szText, _countof(szText));
                if (IsNumeric(szText))
                {
                    Position.y = std::wcstod(szText, nullptr) - Central_control->m_EditWind->GetTarget()->GetPosition().y;
                }
                break;
            }
            case IDC_TRAN_POS_Z:
            {
                GetWindowText(hWndControl, szText, _countof(szText));
                if (IsNumeric(szText))
                {
                    Position.z = std::wcstod(szText, nullptr) - Central_control->m_EditWind->GetTarget()->GetPosition().z;
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
            focusOBJ->Move(Position);
            focusOBJ->SetScale(Scale);
            focusOBJ->SetRotate(rotation);
            InvalidateRect(Central_control->GetMainWind()->GethWnd(), NULL, false);
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
            Rotation OldQuat = Central_control->m_EditWind->GetTarget()->GetRotate();
            OldQuat.angle = DegToRad(nPos);
            Central_control->m_EditWind->GetTarget()->SetRotate(OldQuat);
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
//属性窗口
LRESULT CALLBACK DetaileWind::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
//底部窗口
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
//IO窗口
LRESULT CALLBACK InputOutput::IOWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
//关键帧编辑窗口消息
LRESULT CALLBACK KeyframeEdit::KeyframeWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_SIZE:
    {
        if(Central_control->GetKeyframeWind())
        Central_control->GetKeyframeWind()->MoveSize(LOWORD(lParam), HIWORD(lParam));
        break;
    }
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
        break;
    }
    return 0;
}
LRESULT CALLBACK KeyframeEdit::KeyframeTimeProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        ULONG64 leftTime;
        ULONG64 rightTime;
        Central_control->GetTime(&leftTime, &rightTime);
        // 获取窗口大小
        RECT rect;
        GetClientRect(hWnd, &rect);
        int width = rect.right - rect.left;
        int height = rect.bottom - rect.top;

        // 计算比例尺
        double scale = (double)(width-20) / (rightTime - leftTime);

        float tickInterval = 1.0 / scale * 2; // 刻度间隔

        int startY = WL_KeyframeTime_TimeLine;
        int endX = width-20;

        // 绘制背景色
        HBRUSH hBrushBg = CreateSolidBrush(RGB(242, 247, 251));
        FillRect(hdc, &rect, hBrushBg);
        DeleteObject(hBrushBg);

        // 绘制时间线
        HPEN hPenTimeLine = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
        HGDIOBJ old = SelectObject(hdc, hPenTimeLine);
        MoveToEx(hdc, 10, WL_KeyframeTime_TimeLine, NULL);
        LineTo(hdc, width - 10, WL_KeyframeTime_TimeLine);

        for (int i = leftTime; i <= rightTime; i+=tickInterval)
        {
            int x = (i - leftTime) * scale + 10;
            if (i % 1000 == 0)
            {
                // 绘制长刻度线和标签
                MoveToEx(hdc, x, startY, NULL);
                LineTo(hdc, x, startY + 10);

                // 绘制数字
                float num = (float)i / 1000;
                //WCHAR digit[8];
                //wsprintfW(digit, L"%.2f", num);
                std::wstringstream oss;
                oss << std::setprecision(4) << num;

                RECT rect;
                rect.left = x - 40;
                rect.top = WL_KeyframeTime_TimeLine + WL_KeyframeTime_BigScale;
                rect.right = x + 40;
                rect.bottom = WL_KeyframeTime_TimeLine + WL_KeyframeTime_BigScale + WL_BottenHeight;
                DrawTextW(hdc, oss.str().c_str(), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
            }
            else if (i % 100 == 0)
            {
                // 绘制数字
                float num = (float)i / 1000;
                //WCHAR digit[8];
                //wsprintfW(digit, L"%.2f", num);
                std::wstringstream oss;
                oss << std::setprecision(4) << num;

                RECT rect;
                rect.left = x - 40;
                rect.top = WL_KeyframeTime_TimeLine + WL_KeyframeTime_BigScale;
                rect.right = x + 40;
                rect.bottom = WL_KeyframeTime_TimeLine + WL_KeyframeTime_BigScale + WL_BottenHeight;
                DrawTextW(hdc, oss.str().c_str(), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
                // 绘制中刻度线
                MoveToEx(hdc, x, startY, NULL);
                LineTo(hdc, x, startY + 6);
            }
            else if (i % 10 == 0)
            {
                // 绘制短刻度线
                MoveToEx(hdc, x, startY, NULL);
                LineTo(hdc, x, startY + 4);
            }
        }
        SelectObject(hdc, old);
        DeleteObject(hPenTimeLine);
        EndPaint(hWnd, &ps);
        break;
    }
    case WM_MOUSEWHEEL:
    {
        float zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
        Central_control->MoveKeyframeEditTime(zDelta / 6);
        InvalidateRect(hWnd, nullptr, true);
        break;
    }
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
        break;
    }
    return 0;
}
LRESULT CALLBACK KeyframeEdit::KeyframeFileProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        Object* focus = Central_control->GetFocusObject();
        if (!focus)break;
        RECT rect;
        GetClientRect(hWnd, &rect);
        rect.top += Central_control->GetKeyframeEditY();
        rect.bottom += Central_control->GetKeyframeEditY();
        if (!focus->IsStatic())
        {
            switch (focus->GetType())
            {
            case OT_MODEL:
            {
                int h = WL_BottenHeight;
                rect.bottom = rect.top + h;
                DrawTextW(hdc, strwstr(focus->GetName()).c_str(), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
                rect.top += h;
                rect.bottom += h;
                DrawTextW(hdc, L"位置", -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
                rect.top += h;
                rect.bottom += h;
                DrawTextW(hdc, L"旋转", -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
                rect.top += h;
                rect.bottom += h;
                DrawTextW(hdc, L"缩放", -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
                break;
            }
            case OT_CAMERA:
            {
                int h = WL_BottenHeight;
                rect.bottom = rect.top + h;
                DrawTextW(hdc, strwstr(focus->GetName()).c_str(), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
                rect.top += h;
                rect.bottom += h;
                DrawTextW(hdc, L"位置", -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
                rect.top += h;
                rect.bottom += h;
                DrawTextW(hdc, L"旋转", -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
                break;
            }
            default:
                DrawTextW(hdc, strwstr(focus->GetName()).c_str(), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
                break;
            }
        }
        else
        {
            DrawTextW(hdc, L"不可编辑关键帧", -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
        }

        EndPaint(hWnd, &ps);
        break;
    }
    case WM_MOUSEWHEEL:
    {
        float zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
        Central_control->MoveKeyframeEditY(zDelta/60);
        Central_control->UpdateKeyframeView();
        break;
    }
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
        break;
    }
    return 0;
}
LRESULT CALLBACK KeyframeEdit::KeyframeButtenProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        // 获取窗口客户区矩形大小
        RECT rcClient;
        GetClientRect(hWnd, &rcClient);

        float width = rcClient.right - rcClient.left;
        float height = rcClient.bottom - rcClient.top;

        HBRUSH hBrush = CreateSolidBrush(RGB(200, 200, 200));  
        HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);   

        Rectangle(hdc, 0, 0, width, height);
        DeleteObject(hBrush);
        RUNMODE rm = GetRunMode_g();
        switch (rm)
        {
        case RUNMODE::RM_VIEW:
        case RUNMODE::RM_EDIT:
        {
            hBrush = CreateSolidBrush(RGB(0, 255, 0));
            SelectObject(hdc, hBrush);
            POINT tri[3] = {
                {width / 6 - WL_KeyframeBotten_BottenSize,height / 2 - WL_KeyframeBotten_BottenSize},
                {width / 6 + WL_KeyframeBotten_BottenSize,height / 2},
                {width / 6 - WL_KeyframeBotten_BottenSize,height / 2 + WL_KeyframeBotten_BottenSize} };
            Polygon(hdc, tri, 3);
            DeleteObject(hBrush);
        }
            break;
        case RUNMODE::RM_RUN:
        {

            hBrush = CreateSolidBrush(RGB(255, 0, 0));
            SelectObject(hdc, hBrush);
            Rectangle(hdc, width / 6 - WL_KeyframeBotten_BottenSize, height / 2 - WL_KeyframeBotten_BottenSize, width / 6 - 2, height / 2 + WL_KeyframeBotten_BottenSize);
            Rectangle(hdc, width / 6 + 2, height / 2 - WL_KeyframeBotten_BottenSize, width / 6 + WL_KeyframeBotten_BottenSize, height / 2 + WL_KeyframeBotten_BottenSize);
            DeleteObject(hBrush);
        }
            break;
        default:
            break;
        }
        hBrush = CreateSolidBrush(RGB(255, 0, 0));
        SelectObject(hdc, hBrush);
        Rectangle(hdc, width / 2 - WL_KeyframeBotten_BottenSize, height / 2 - WL_KeyframeBotten_BottenSize, width / 2 + WL_KeyframeBotten_BottenSize, height / 2 + WL_KeyframeBotten_BottenSize);
        DeleteObject(hBrush);

        int space = 3;
        HPEN hpen;
        if (Central_control->GetKeyframeLoop())
        {
            hpen = CreatePen(PS_SOLID, 0, RGB(0, 0, 180));
        }
        else
            hpen = CreatePen(PS_SOLID, 0, RGB(0, 180, 180));
        HGDIOBJ oldhpen = SelectObject(hdc, hpen);
        MoveToEx(hdc, width / 6 * 5 - WL_KeyframeBotten_BottenSize + space, height / 2 - WL_KeyframeBotten_BottenSize + space, NULL);
        LineTo(hdc, width / 6 * 5 - WL_KeyframeBotten_BottenSize + space, height / 2 + WL_KeyframeBotten_BottenSize - space);
        LineTo(hdc, width / 6 * 5 + WL_KeyframeBotten_BottenSize - space, height / 2 + WL_KeyframeBotten_BottenSize - space);
        LineTo(hdc, width / 6 * 5 + WL_KeyframeBotten_BottenSize - space, height / 2 - WL_KeyframeBotten_BottenSize + space);
        LineTo(hdc, width / 6 * 5, height / 2 - WL_KeyframeBotten_BottenSize + space);
        LineTo(hdc, width / 6 * 5 + 3, height / 2 - WL_KeyframeBotten_BottenSize);
        MoveToEx(hdc, width / 6 * 5, height / 2 - WL_KeyframeBotten_BottenSize + space, nullptr);
        LineTo(hdc, width / 6 * 5 + 3, height / 2 - WL_KeyframeBotten_BottenSize + space * 2);
        DeleteObject(hpen); // 删除创建画笔hpen
        hpen = CreatePen(PS_SOLID, 0, RGB(0, 0, 0));
        SelectObject(hdc, hpen);
        MoveToEx(hdc, width / 3, 0, nullptr);
        LineTo(hdc, width / 3, height);
        MoveToEx(hdc, width / 3 * 2, 0, nullptr);
        LineTo(hdc, width / 3 * 2, height);

        SelectObject(hdc, oldhpen);
        SelectObject(hdc, hOldBrush);
        DeleteObject(hpen);


        EndPaint(hWnd, &ps);
        break;
    }
    case WM_LBUTTONDOWN:
    {
        int xPos = LOWORD(lParam);
        RECT rect;
        GetClientRect(hWnd, &rect);
        if (xPos < rect.right / 3)
        {
            Central_control->SwitchRunMode();
        }
        else if (xPos < rect.right * 2 / 3)
        {
            Central_control->Stop();
        }
        else
        {
            Central_control->SwitchKeyframeLoop();
        }
        Central_control->UpdateKeyframeView();
        break;
    }
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
        break;
    }
    return 0;
}
LRESULT CALLBACK KeyframeEdit::KeyframeCanvasProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        Object* focus = Central_control->GetFocusObject();
        RECT rect;
        GetClientRect(hWnd, &rect);
        if (focus)
        {
            ULONG64 leftTime;
            ULONG64 rightTime;
            Central_control->GetTime(&leftTime, &rightTime);
            HBRUSH hBrushA = CreateSolidBrush(RGB(239, 239, 239));
            HBRUSH hBrushB = CreateSolidBrush(RGB(249, 249, 249));
            switch (focus->GetType())
            {
            case OT_MODEL:
            {
                Model* model = dynamic_cast<Model*>(focus);
                auto keyframe = model->GetKeyframeData();
                if (!keyframe)
                {
                    DrawTextW(hdc, L"未创建关键帧，双击创建", -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
                    break;
                }

                rect.top += Central_control->GetKeyframeEditY();
                int h = WL_BottenHeight;
                rect.bottom = rect.top + h;
                FillRect(hdc, &rect, hBrushA);
                rect.top += h;
                rect.bottom += h;
                FillRect(hdc, &rect, hBrushB);
                rect.top += h;
                rect.bottom += h;
                FillRect(hdc, &rect, hBrushA);
                rect.top += h;
                rect.bottom += h;
                FillRect(hdc, &rect, hBrushB);

                int width = rect.right;
                rect.top = 0;
                for (auto k : *keyframe)
                {
                    if (k.first < leftTime)
                        continue;
                    if (k.first > rightTime)
                        break;
                    int x = (int)(((float)k.first - leftTime) / (rightTime - leftTime) * (width - 20)) + 10;
                    rect.left = x - 2;
                    rect.right = x + 2;
                    HBRUSH hBrushC = CreateSolidBrush(RGB(180, 20, 20));
                    FillRect(hdc, &rect, hBrushC);
                    DeleteObject(hBrushC); // 释放创建的画刷对象
                }
                break;
            }
            case OT_CAMERA:
            {
                rect.top += Central_control->GetKeyframeEditY();
                int h = WL_BottenHeight;
                rect.bottom = rect.top + h;
                FillRect(hdc, &rect, hBrushA);
                rect.top += h;
                rect.bottom += h;
                FillRect(hdc, &rect, hBrushB);
                rect.top += h;
                rect.bottom += h;
                FillRect(hdc, &rect, hBrushA);
                break;
            }
            default:
                DrawTextW(hdc, L"目标不可用", -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
                break;
            }

            // 释放创建的画刷对象
            DeleteObject(hBrushA);
            DeleteObject(hBrushB);
        }

        else
        {
            DrawTextW(hdc, L"无目标", -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
        }
        ULONG64 leftTime;
        ULONG64 rightTime;
        Central_control->GetTime(&leftTime, &rightTime);
        HPEN hPenTimeLine = CreatePen(PS_SOLID, 0, RGB(0, 0, 0));
        HGDIOBJ old = SelectObject(hdc, hPenTimeLine);
        GetClientRect(hWnd, &rect);
        int x = (int)(((float)Central_control->GetTime() - leftTime) / (rightTime - leftTime) * (rect.right - 20)) + 10;
        MoveToEx(hdc, x, 0, NULL);
        LineTo(hdc, x, rect.bottom);
        SelectObject(hdc, old);
        DeleteObject(hPenTimeLine);
        EndPaint(hWnd, &ps);
        break;
    }
    case WM_LBUTTONDOWN:
    {
 
        int xPos = LOWORD(lParam) - 10;
        if (xPos < 0)
            xPos = 0;
        ULONG64 leftTime;
        ULONG64 rightTime;
        Central_control->GetTime(&leftTime, &rightTime);
        int interval = rightTime - leftTime;
        RECT rect;
        GetClientRect(hWnd, &rect);

        double scale = (double)(rect.right - 20) / (rightTime - leftTime);
        int tickInterval = 1.0 / scale * 2;

        // 初始化绑定线位置为第一个刻度位置
        int bindingLinePos = (leftTime - leftTime) * scale + 10;
        ULONG64 Time = 0;
        // 遍历所有的刻度位置，找到最接近鼠标位置的刻度位置
        for (int i = leftTime; i <= rightTime; i += tickInterval)
        {
            int x = (i - leftTime) * scale;
            if (i % 10 == 0)
            {
                // 如果该刻度位置比当前绑定线位置更接近鼠标单击位置，则更新绑定线位置
                if (abs(x - xPos) < abs(bindingLinePos - xPos))
                {
                    bindingLinePos = x;
                    Time = i;
                }
            }
        }
        Central_control->SetRunMode(RUNMODE::RM_VIEW);
        // 设置绑定线的位置为最接近鼠标单击位置的刻度位置
        Central_control->SetTime(Time);
        InvalidateRect(hWnd, nullptr, true);
        break;
    }
    case WM_LBUTTONDBLCLK:
    {
        Object* focus = Central_control->GetFocusObject();
        if (!focus)
            break;
        switch (focus->GetType())
        {
        case OT_MODEL:
        {
            Model* fModel = dynamic_cast<Model*>(focus);
            if (!fModel->SetKeyframe(Central_control->GetTime()))
            {
                Keyframe<TransForm>* key = new Keyframe<TransForm>;
                Central_control->AddObject(key);
                fModel->SetKeyframe(key);
            }
            break;
        }
        default:
            break;
        }
        InvalidateRect(hWnd, nullptr, true);
        break;
    }
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
        break;
    }
    return 0;
}
ULONG64 GetTime()
{
    return Central_control->GetTime();
}
RUNMODE GetRunMode_g()
{
    return Central_control->GetRunMode();
}
void OutMessage_g(const std::string& str, MSGtype type)
{
    Central_control->OutMessage(str, type);
}
std::vector<PointLight*>& GetAllPointLight()
{
    return Central_control->GetAllPointLight();
}