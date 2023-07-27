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
    Central_control->InitWindow(hInstance);

    HMENU hMenu = LoadMenu(hInstance, MAKEINTRESOURCE(IDC_WIN));
    SetMenu(Central_control->m_hWnd, hMenu);
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WIN));

    //切换为OpenGL渲染
    SendMessage(Central_control->m_hWnd, WM_COMMAND, MAKEWPARAM(ID_OPENGL, 0), 0);

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
template <typename T>
T findClosestDivisible(T a, T b);
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
          
        EndDialog(hDlg, 0);
        return DefWindowProc(hDlg, uMsg, wParam, lParam);
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
        Object* focusOBJ = Central_control->GetFocusObject();
        if (!focusOBJ)
            break;
        TransForm_变换控件::UpdateDialog(hDlg, focusOBJ);
        break;
    }
    case UM_UPDATE:
    {
        Object* focusOBJ = Central_control->GetFocusObject();
        if (!focusOBJ)
            break;
        TransForm_变换控件::UpdateDialog(hDlg, focusOBJ);
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
        break;
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
        ULONG64 timeStart;
        ULONG64 timeRange;
        Central_control->GetTime(&timeStart, &timeRange);
        // 获取窗口大小
        RECT rect;
        GetClientRect(hWnd, &rect);
        int width = rect.right - rect.left;
        int height = rect.bottom - rect.top;

        // 计算比例尺
        double scale = (double)(width-20) / (timeRange);
        // 计算实际步长
        ULONG64 stepSize = Central_control->GetKeyframeEditStepSize();

        int startY = WL_KeyframeTime_TimeLine;
        int endX = width - 20;

        // 绘制背景色
        HBRUSH hBrushBg = CreateSolidBrush(RGB(242, 247, 251));
        FillRect(hdc, &rect, hBrushBg);
        DeleteObject(hBrushBg);

        // 绘制时间线
        HPEN hPenTimeLine = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
        HGDIOBJ old = SelectObject(hdc, hPenTimeLine);
        MoveToEx(hdc, 10, WL_KeyframeTime_TimeLine, NULL);
        LineTo(hdc, width - 10, WL_KeyframeTime_TimeLine);
        
        ULONG64 start = (stepSize - timeStart % stepSize);
        for (ULONG64 t = (start < stepSize) ? start : 0; t < timeRange; t += stepSize)
        {
            ULONG64 i = (t + timeStart);
            int x = 10 + t * scale;
            if (i % (stepSize * 10) == 0)
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
            else if (i % (stepSize * 5) == 0)
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
            else
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
        short shiftKeyState = GetKeyState(VK_SHIFT);
        if (shiftKeyState & 0x8000)
            Central_control->ScaleKeyframeEditTime(zDelta / 6);
        else
            Central_control->MoveKeyframeEditTime(zDelta / 60);
        InvalidateRect(hWnd, nullptr, true);
        Central_control->UpdateKeyframeView(ChildWindSign::KF_CanvasWind);
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
        ULONG64 leftTime, longTime;
        RECT rect;
        GetClientRect(hWnd, &rect);
        Central_control->GetTime(&leftTime, &longTime);
        if (focus)
        {
            focus->KeyframeCanvasPaint(hWnd, leftTime, longTime);
        }
        ULONG64 runTime = Central_control->GetTime();
        double scale = (double)(rect.right - 20) / (longTime);
        int x = (runTime - leftTime) * scale + 10;
        MoveToEx(hdc, x, 0,nullptr);
        LineTo(hdc, x, rect.bottom);

        EndPaint(hWnd, &ps);
        break;
    }
    case WM_LBUTTONDOWN:
    {
 
        int xPos = LOWORD(lParam) - 10;
        if (xPos < 0)
            xPos = 0;
        ULONG64 leftTime;
        ULONG64 rangeTime;
        Central_control->GetTime(&leftTime, &rangeTime);
        int interval = rangeTime;
        RECT rect;
        GetClientRect(hWnd, &rect);

        double scale = (double)(rect.right - 20) / (rangeTime);
        int tickInterval = Central_control->GetKeyframeEditStepSize();

        // 初始化绑定线位置为第一个刻度位置
        int bindingLinePos = 10;
        ULONG64 Time = 0;

        ULONG64 start = (tickInterval - leftTime % tickInterval);

        for (ULONG64 t = (start < tickInterval) ? start : 0; t <= rangeTime; t += tickInterval)
        {
            ULONG64 i = (t + leftTime);
            int x = t * scale;
            if (i % tickInterval == 0)
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
                std::string name = focus->GetName();
                name += "关键帧";
                Keyframe<TransForm>* key = new Keyframe<TransForm>(name);
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
        return DefWindowProcW(hWnd, message, wParam, lParam);
        break;
    }
    return 0;
}
ULONG64 GetRunTime_g()
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
template <typename T>
T findClosestDivisible(T a, T b) {
    T remainder = a % b;
    T c = a + (b - remainder);

    if (c <= a) {
        c += b;
    }

    return c;
}
Vector GetHighLightColor_g()
{
    if (Central_control)
        return Central_control->GetHighLightColor();
    return Vector();
}