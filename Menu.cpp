
#include"Menu.h"
#include"resource.h"
Controller* cp = nullptr;
void loadModelThread(HWND hWnd,Controller* current_project,std::wstring path)
{
    current_project->m_FileLoad = true;
    unsigned int Error = current_project->LoadFile(path);
    if (Error == ReturnedOfLoadFile::_Succese)
    {
        current_project->OutMessage(L"模型加载完毕：" + path, _Message);
        current_project->Model_att = 0x01;
    }
    else if ((Error & 0xff) != 0)
    {
        switch (Error & 0xff)
        {
        case ReturnedOfLoadFile::_UnknownFormat:
            current_project->OutMessage(L"未知文件格式：" + path, _Error);
            break;

        case ReturnedOfLoadFile::_DataError:
            current_project->OutMessage(L"数据错误：" + path, _Error);
            break;

        case ReturnedOfLoadFile::_FailToOpenFile:
            current_project->OutMessage(L"无法打开文件：" + path, _Error);
            break;

        case ReturnedOfLoadFile::_FailedToCreateFile:
            current_project->OutMessage(L"文件创建失败：" + path, _Error);
            break;
        default:
            current_project->OutMessage(L"模型加载时发生未知错误：" + path, _Error);
            break;
        }
    }
    else
    {
        if ((Error & ReturnedOfLoadFile::_SuccessfullyLoadedVertex) != ReturnedOfLoadFile::_SuccessfullyLoadedVertex)
        {
            current_project->OutMessage(L"加载顶点数据失败：" + path, _Warning);
        }
        if ((Error & ReturnedOfLoadFile::_SuccessfullyLoadedMaterialFile) != ReturnedOfLoadFile::_SuccessfullyLoadedMaterialFile)
        {
            current_project->OutMessage(L"加载材质文件失败：" + path, _Warning);
        }
        if ((Error & ReturnedOfLoadFile::_SuccessfullyLoadedMaterialMaps) != ReturnedOfLoadFile::_SuccessfullyLoadedMaterialMaps)
        {
            current_project->OutMessage(L"加载材质贴图失败：" + path, _Warning);
        }
    }
    current_project->m_FileLoad = false;
}
LRESULT __stdcall Menu(HINSTANCE hInst, HWND hWnd, UINT msg, WPARAM wP, LPARAM lP, Controller* current_project)
{
    cp = current_project;
    int wmId = LOWORD(wP);
    // 分析菜单选择:
    switch (wmId)
    {
    case IDM_ABOUT:
    {
        DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
        break;
    }
    case IDM_EXIT:
    {
        DestroyWindow(hWnd);
        break;
    }
    case ID_SETSIZE:
    {
        DialogBox(hInst, MAKEINTRESOURCE(IDD_SETSIZE), hWnd, SetSize);
        break;
    }
    case IDM_OPEN:
    {
        std::wstring path = MenuGetPath();
        if (path == L"")current_project->OutMessage(L"地址获取失败", _Error);
        else
        {
            std::thread loadthread(loadModelThread,hWnd,current_project,path);
            loadthread.detach();
        }
        break;
    }
    case ID_GDI:
    {
        current_project->OutMessage("使用GDI绘图");
        delete current_project->MAINWND;
        current_project->MAINWND = new GDIWND;
        int cxClient = current_project->GetRect().right - current_project->GetRect().left;
        int cyClient = current_project->GetRect().bottom - current_project->GetRect().top;
        ShowWindow(current_project->MAINWND->CreateWind(current_project->hWnd, cxClient / 5, 50, cxClient / 5 * 3, cyClient - 200), SW_SHOW);
        LONG style = GetWindowLong(hWnd, GWL_STYLE);

        // 禁用调整窗口大小功能
        style |= WS_THICKFRAME;
        style |= WS_MAXIMIZEBOX;

        // 更新窗口样式
        SetWindowLong(hWnd, GWL_STYLE, style);
        break;
    }
    case ID_OPENGL:
    {
        current_project->OutMessage("使用OpenGL绘图");
        current_project->OutMessage("由于一直渲染会阻塞消息，所以仅当右键窗口进入控制状态才会持续渲染");
        current_project->OutMessage("拖拽调整窗口大小已被禁用调整窗口大小请前往设置");
        delete current_project->MAINWND;
        current_project->MAINWND = new OpenGLWnd;
        int cxClient = current_project->GetRect().right - current_project->GetRect().left;
        int cyClient = current_project->GetRect().bottom - current_project->GetRect().top;
        current_project->MAINWND->CreateWind(current_project->hWnd, cxClient / 5, 50, cxClient / 5 * 3, cyClient - 200);
        LONG style = GetWindowLong(hWnd, GWL_STYLE);

        // 禁用调整窗口大小功能
        style &= ~WS_THICKFRAME;
        style &= ~WS_MAXIMIZEBOX;

        // 更新窗口样式
        SetWindowLong(hWnd, GWL_STYLE, style);
        break;
    }
    default:
    {
        return DefWindowProc(hWnd, msg, wP, lP);
    }
    }
    return 0;
}
// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    case WM_CLOSE:
        EndDialog(hDlg, IDCANCEL);
        return TRUE;

    }
    return (INT_PTR)FALSE;
}
void SetSizeEditw(HWND hDlg, int w, int h)
{
    HWND hwndEditw = GetDlgItem(hDlg, IDC_width);
    HWND hwndEdith = GetDlgItem(hDlg, IDC_height);
    std::wstring wstr = std::to_wstring(w);
    SetWindowText(hwndEditw, wstr.c_str());
    wstr = std::to_wstring(h);
    SetWindowText(hwndEdith, wstr.c_str());
}
BOOL IsMaximized(HWND hWnd)
{
    WINDOWPLACEMENT wp;
    wp.length = sizeof(WINDOWPLACEMENT);
    GetWindowPlacement(hWnd, &wp);
    return (wp.showCmd == SW_SHOWMAXIMIZED);
}
INT_PTR CALLBACK SetSize(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDC_CANCEL:
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        case IDC_600_400:
        {
            SetSizeEditw(hDlg, 600, 400);
            break;
        }
        case IDC_800_540:
        {
            SetSizeEditw(hDlg, 800, 540);
            break;
        }
        case IDC_1280_720:
        {
            SetSizeEditw(hDlg, 1280, 720);
            break;
        }
        case IDC_1366_768:
        {
            SetSizeEditw(hDlg, 1366, 768);
            break;
        }
        case IDC_1600_900:
        {
            SetSizeEditw(hDlg, 1600, 900);
            break;
        }
        case IDC_1920_1080:
        {
            SetSizeEditw(hDlg, 1920, 1080);
            break;
        }
        case IDC_OK:
        {
            HWND hwndEditw = GetDlgItem(hDlg, IDC_width);
            HWND hwndEdith = GetDlgItem(hDlg, IDC_height);
            int length = GetWindowTextLength(hwndEditw);
            wchar_t* buffer = new wchar_t[length + 1];
            GetWindowText(hwndEditw, buffer, length + 1);
            int width = std::wcstol(buffer, nullptr, 10);
            delete[] buffer;
            length = GetWindowTextLength(hwndEdith);
            buffer = new wchar_t[length + 1];
            GetWindowText(hwndEdith, buffer, length + 1);
            int height = std::wcstol(buffer, nullptr, 10);
            // 使用获取到的文本进行相应操作
            delete[] buffer;
            if (width < 640 || height < 480)
            {
                cp->OutMessage(" 宽度不能小于640，高度不能小于480");
                EndDialog(hDlg, LOWORD(wParam));
                return (INT_PTR)TRUE;
            }
            else
            {
                RECT rcWindow;
                GetWindowRect(cp->hWnd, &rcWindow);
               /* width = rcWindow.right - rcWindow.left;
                height = rcWindow.bottom - rcWindow.top;*/
                MoveWindow(cp->hWnd, rcWindow.left, rcWindow.top, width, height, false);
                if (OpenGLWnd* OW = dynamic_cast<OpenGLWnd*>(cp->MAINWND))
                {
                    OW->ResetOpenGLViewport();
                }
                EndDialog(hDlg, LOWORD(wParam));
                return (INT_PTR)TRUE;
            }
            
        }
        case IDC_MAXIMIZE:
        {
            WINDOWPLACEMENT wp;
            wp.length = sizeof(WINDOWPLACEMENT);
            GetWindowPlacement(cp->hWnd, &wp);

            if (wp.showCmd == SW_SHOWMAXIMIZED) // 如果窗口已被最大化
            {
                ShowWindow(cp->hWnd, SW_RESTORE); // 还原窗口大小
                if (OpenGLWnd* OW = dynamic_cast<OpenGLWnd*>(cp->MAINWND))
                {
                    OW->ResetOpenGLViewport();
                }
            }
            else // 否则窗口未被最大化
            {
                ShowWindow(cp->hWnd, SW_MAXIMIZE); // 最大化窗口
                if (OpenGLWnd* OW = dynamic_cast<OpenGLWnd*>(cp->MAINWND))
                {
                    OW->ResetOpenGLViewport();
                }
            }
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        default:
            break;
        }
        break;
    case WM_CLOSE:
        EndDialog(hDlg, IDCANCEL);
        return TRUE;
    case WM_INITDIALOG:
    {
        HWND hwndEditw = GetDlgItem(hDlg, IDC_width);
        HWND hwndEdith = GetDlgItem(hDlg, IDC_height);
        RECT rcWindow;
        GetWindowRect(cp->hWnd, &rcWindow);
        std::wstring wstr = std::to_wstring(rcWindow.right - rcWindow.left);
        SetWindowText(hwndEditw, wstr.c_str());
        wstr = std::to_wstring(rcWindow.bottom - rcWindow.top);
        SetWindowText(hwndEdith, wstr.c_str());
        HWND hwndMaximize = GetDlgItem(hDlg, IDC_MAXIMIZE);
        if(IsMaximized(cp->hWnd))
            SetWindowText(hwndMaximize, L"还原");
        else
            SetWindowText(hwndMaximize, L"全屏");
        break;
    }
    }

    return (INT_PTR)FALSE;
}
std::wstring MenuGetPath()
{
    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
        COINIT_DISABLE_OLE1DDE);
    PWSTR pszFilePath = {};
    if (SUCCEEDED(hr))
    {
        IFileOpenDialog* pFileOpen;

        // Create the FileOpenDialog object.
        hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
            IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

        if (SUCCEEDED(hr))
        {
            // Show the Open dialog box.
            hr = pFileOpen->Show(NULL);

            // Get the file name from the dialog box.
            if (SUCCEEDED(hr))
            {
                IShellItem* pItem;
                hr = pFileOpen->GetResult(&pItem);
                if (SUCCEEDED(hr))
                {
                    
                    hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

                    // Display the file name to the user.
                    if (SUCCEEDED(hr))
                    {
                        pItem->Release();
                        pFileOpen->Release();
                        CoUninitialize();
                        std::wstring path=pszFilePath;
                        CoTaskMemFree(pszFilePath);
                        return path;
                    }
                    pItem->Release();
                }
            }
            pFileOpen->Release();
        }
        CoUninitialize();
    }
    return L"";
}