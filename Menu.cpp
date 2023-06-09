
#include"Menu.h"
#include"resource.h"
Controller* cp = nullptr;

LRESULT __stdcall Menu(HINSTANCE hInst, HWND hWnd, UINT msg, WPARAM wP, LPARAM lP, Controller* Controller)
{
    cp = Controller;
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
        if (path == L"")Controller->OutMessage(L"地址获取失败", _Error);
        else
        {
            std::thread loadthread(loadFileThread,hWnd,Controller,path);
            loadthread.detach();
        }
        break;
    }
    case IDM_SAVE:
    {
        Object* focus = Controller->GetFocusObject();
        if (!focus)
        {
            Controller->OutMessage("未选择任何目标", _Warning);
            break;
        }
        WCHAR szFile[MAX_PATH] = { 0 };
        OPENFILENAME ofn = { 0 };
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = hWnd;
        ofn.lpstrFilter = TEXT("以文本格式保存(*.txt)\0*.txt\0以xlsx格式保存\0*.xlsx\0以二进制格式保存(*.*)\0*.*\0");
        ofn.lpstrFile = szFile;
        ofn.nMaxFile = MAX_PATH;
        ofn.lpstrTitle = TEXT("选择保存文件位置");
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;

        // Display the Save File dialog box
        if (GetSaveFileName(&ofn))
        {
            std::wstring Path = ofn.lpstrFile;
            
            if (ofn.nFilterIndex == 1)
            {
                if (GetFileExtension(Path).empty())
                    Path += L".txt";
                if (focus->SaveFile(Path, SM_TEXT))
                    Controller->OutMessage("保存成功");
                else
                    Controller->OutMessage("保存失败", _Error);
            }
            else if (ofn.nFilterIndex == 2)
            {
                if (GetFileExtension(Path).empty())
                    Path += L".xlsx";
                if (focus->SaveFile(Path, SM_XLSX))
                    Controller->OutMessage("保存成功");
                else
                    Controller->OutMessage("保存失败", _Error);
            }
            else if (ofn.nFilterIndex == 3)
            {
                if (focus->SaveFile(Path, SM_BINARY))
                    Controller->OutMessage("保存成功");
                else
                    Controller->OutMessage("保存失败", _Error);
            }
            else
            {
                if (focus->SaveFile(Path))
                    Controller->OutMessage("保存成功");
                else
                    Controller->OutMessage("保存失败", _Error);
            }
        }
        else
        {
            Controller->OutMessage("获取地址失败", MSGtype::_Error);
        }
        break;
    }
    case ID_GDI:
    {
        Controller->OutMessage("使用GDI绘图");
        delete Controller->GetMainWind();
        Controller->m_MainWind = new GDIWND;
        int cxClient = Controller->GetRect().right - Controller->GetRect().left;
        int cyClient = Controller->GetRect().bottom - Controller->GetRect().top;
        ShowWindow(Controller->m_MainWind->CreateWind(Controller->m_hWnd, cxClient / 5, 50, cxClient / 5 * 3, cyClient - 200), SW_SHOW);
        Controller->SetFPS(Controller->GetSetFPS());
        break;
    }
    case ID_OPENGL:
    {
        Controller->OutMessage("使用OpenGL绘图");
        delete Controller->GetMainWind();
        Controller->m_MainWind = new OpenGLWnd;
        int cxClient = Controller->GetRect().right - Controller->GetRect().left;
        int cyClient = Controller->GetRect().bottom - Controller->GetRect().top;
        Controller->m_MainWind->CreateWind(Controller->m_hWnd, cxClient / 5, 50, cxClient / 5 * 3, cyClient - 200);
        Controller->SetFPS(Controller->GetSetFPS());
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
                GetWindowRect(cp->m_hWnd, &rcWindow);
               /* width = rcWindow.right - rcWindow.left;
                height = rcWindow.bottom - rcWindow.top;*/
                MoveWindow(cp->m_hWnd, rcWindow.left, rcWindow.top, width, height, false);
                if (OpenGLWnd* OW = dynamic_cast<OpenGLWnd*>(cp->GetMainWind()))
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
            GetWindowPlacement(cp->m_hWnd, &wp);

            if (wp.showCmd == SW_SHOWMAXIMIZED) // 如果窗口已被最大化
            {
                ShowWindow(cp->m_hWnd, SW_RESTORE); // 还原窗口大小
                if (OpenGLWnd* OW = dynamic_cast<OpenGLWnd*>(cp->GetMainWind()))
                {
                    OW->ResetOpenGLViewport();
                }
            }
            else // 否则窗口未被最大化
            {
                ShowWindow(cp->m_hWnd, SW_MAXIMIZE); // 最大化窗口
                if (OpenGLWnd* OW = dynamic_cast<OpenGLWnd*>(cp->GetMainWind()))
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
        GetWindowRect(cp->m_hWnd, &rcWindow);
        std::wstring wstr = std::to_wstring(rcWindow.right - rcWindow.left);
        SetWindowText(hwndEditw, wstr.c_str());
        wstr = std::to_wstring(rcWindow.bottom - rcWindow.top);
        SetWindowText(hwndEdith, wstr.c_str());
        HWND hwndMaximize = GetDlgItem(hDlg, IDC_MAXIMIZE);
        if(IsMaximized(cp->m_hWnd))
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