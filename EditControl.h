#pragma once
#include<windows.h>
#include<string>
class EditControl
{
public:
    EditControl(HINSTANCE hInstance, HWND hWndParent, int x, int y, int width, int height, DWORD Style,bool isNumeric = false) {
        m_hInstance = hInstance;
        m_hWndParent = hWndParent;
        m_x = x;
        m_y = y;
        m_width = width;
        m_height = height;
        m_isNumeric = isNumeric;

        // 创建编辑框控件
        m_hWnd = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"", Style | WS_CHILD,
            m_x, m_y, m_width, m_height, m_hWndParent, NULL, m_hInstance, NULL);
    }

    // 获取编辑框中的文本
    std::wstring GetText() const {
        wchar_t buffer[1024];
        GetWindowText(m_hWnd, buffer, sizeof(buffer) / sizeof(wchar_t));
        return std::wstring(buffer);
    }

    // 设置编辑框中的文本
    void SetText(const std::wstring& text) {
        SetWindowText(m_hWnd, text.c_str());
    }

    // 获取编辑框的句柄
    HWND GetHandle() const {
        return m_hWnd;
    }

    // 调整编辑框的大小和位置
    void SetPosition(int x, int y, int width, int height) {
        MoveWindow(m_hWnd, x, y, width, height, TRUE);
    }

    // 禁用编辑框
    void Disable() {
        EnableWindow(m_hWnd, FALSE);
    }

    // 启用编辑框
    void Enable() {
        EnableWindow(m_hWnd, TRUE);
    }

private:
    HINSTANCE m_hInstance;
    HWND m_hWndParent;
    HWND m_hWnd;
    int m_x;
    int m_y;
    int m_width;
    int m_height;
    bool m_isNumeric;
};

