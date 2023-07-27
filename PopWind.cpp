#include "PopWind.h"

ULONG64 g_StaticMessgeData = 0;
ULONG64 g_StaticMessgeData2 = 0;

INT_PTR PopWind_Vector_Dlgproc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);


vec::Vector3 GetVectorByPopWind(vec::Vector3 Dvec,const std::wstring& text, const std::wstring& Xtext, 
	const std::wstring& Ytext, const std::wstring& Ztext,HWND parent)
{
    g_StaticMessgeData2 = (ULONG64)&Dvec;
	HINSTANCE hInst = GetModuleHandle(NULL);
	HWND hWnd = CreateDialog(hInst, MAKEINTRESOURCE(IDD_PopVectorWind), parent, PopWind_Vector_Dlgproc);
    SetWindowText(hWnd, text.c_str());
    SendMessage(hWnd, UM_UPDATE, UMW_STRING, (LPARAM)Xtext.c_str());
    SendMessage(hWnd, UM_UPDATE, UMW_STRING, (LPARAM)Ytext.c_str());
    SendMessage(hWnd, UM_UPDATE, UMW_STRING, (LPARAM)Ztext.c_str());

    SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    MSG msg;
    while (GetMessage(&msg,NULL, 0, 0))
    {
        if (IsDialogMessage(hWnd, &msg)) // 处理对话框消息
            continue;

        TranslateMessage(&msg);
        DispatchMessage(&msg);

        // 如果收到关闭消息，则跳出循环
        
        if (msg.message == WM_CLOSE && msg.hwnd == hWnd)
            break;
    }

    Vector out = *(Vector*)g_StaticMessgeData;
    g_StaticMessgeData = 0;
    g_StaticMessgeData2 = 0;
	return out;
}
INT_PTR CALLBACK PopWind_Vector_Dlgproc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case UM_UPDATE:
    {
        HWND ch = nullptr;
        switch (g_StaticMessgeData)
        {
        case 0:
        {
            ch = GetDlgItem(hDlg, IDC_PW_TEXTX);
            SetWindowText(ch, (wchar_t*)(lParam));
            break;
        }
        case 1:
        {
            ch = GetDlgItem(hDlg, IDC_PW_TEXTY);
            SetWindowText(ch, (wchar_t*)(lParam));
            break;
        }
        case 2:
        {
            ch = GetDlgItem(hDlg, IDC_PW_TEXTZ);
            SetWindowText(ch, (wchar_t*)(lParam));
            break;
        }
        default:
            break;
        }
        g_StaticMessgeData++;
        break;
    }
    case WM_INITDIALOG:
    {
        g_StaticMessgeData = 0;
        Vector3 Dvec = *(Vector*)g_StaticMessgeData2;
        HWND ch = GetDlgItem(hDlg, IDC_PW_X);
        if (Dvec.x < 0)
        {
            SetWindowTextW(ch, std::to_wstring(-Dvec.x).c_str());
            ch = GetDlgItem(hDlg, IDC_PW_CX);
            SendMessageW(ch, BM_SETCHECK, BST_CHECKED, 0);
        }
        else
            SetWindowTextW(ch, std::to_wstring(Dvec.x).c_str());
        
        ch = GetDlgItem(hDlg, IDC_PW_Y);
        if (Dvec.y < 0)
        {
            SetWindowTextW(ch, std::to_wstring(-Dvec.y).c_str());
            ch = GetDlgItem(hDlg, IDC_PW_CY);
            SendMessageW(ch, BM_SETCHECK, BST_CHECKED, 0);
        }
        else
        SetWindowTextW(ch, std::to_wstring(Dvec.y).c_str());
        ch = GetDlgItem(hDlg, IDC_PW_Z);
        if (Dvec.z < 0)
        {
            SetWindowTextW(ch, std::to_wstring(-Dvec.z).c_str());
            ch = GetDlgItem(hDlg, IDC_PW_CZ);
            SendMessageW(ch, BM_SETCHECK, BST_CHECKED, 0);
        }
        else
            SetWindowTextW(ch, std::to_wstring(Dvec.z).c_str());
        break;
    }
    case WM_CLOSE:
    {
        Vector out;
        if (g_StaticMessgeData == 2)
        {
            out = *(Vector*)g_StaticMessgeData2;
        }
        else
        {
            WCHAR wstr[32] = {};
            HWND ch = GetDlgItem(hDlg, IDC_PW_X);
            int csize = GetWindowText(ch, wstr, 32);
            if (csize > 0)
                swscanf_s(wstr, L"%f", &out.x);
            else
                out.x = 0;
            ch = GetDlgItem(hDlg, IDC_PW_Y);
            csize = GetWindowText(ch, wstr, 32);
            if (csize > 0)
                swscanf_s(wstr, L"%f", &out.y);
            else
                out.y = 0;
            ch = GetDlgItem(hDlg, IDC_PW_Z);
            csize = GetWindowText(ch, wstr, 32);
            if (csize > 0)
                swscanf_s(wstr, L"%f", &out.z);
            else
                out.z = 0;
            int isChecked = IsDlgButtonChecked(hDlg, IDC_PW_CX);
            if (isChecked == BST_CHECKED)
            {
                out.x = -out.x;
            }
            isChecked = IsDlgButtonChecked(hDlg, IDC_PW_CY);
            if (isChecked == BST_CHECKED)
            {
                out.y = -out.y;
            }
            isChecked = IsDlgButtonChecked(hDlg, IDC_PW_CZ);
            if (isChecked == BST_CHECKED)
            {
                out.z = -out.z;
            }
        }
        g_StaticMessgeData = (ULONG64)&out;
        EndDialog(hDlg, 0);
        PostQuitMessage(0);
        return DefWindowProc(hDlg, uMsg, wParam, lParam);
        break;
    }
    case WM_COMMAND:
    {
        int controlId = LOWORD(wParam);
        int notifyCode = HIWORD(wParam);

        if (controlId == ID_PW_OK)
        {
            g_StaticMessgeData = 1;
            SendMessage(hDlg, WM_CLOSE, 0, 0);
        }
        else if (controlId == ID_PW_CANCEL)
        {
            g_StaticMessgeData = 2;
            SendMessage(hDlg, WM_CLOSE, 0, 0);
        }
        
        break;
    }

    default:
        return DefWindowProc(hDlg, uMsg, wParam, lParam);
    }
    return FALSE;
}
