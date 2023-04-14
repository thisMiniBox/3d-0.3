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

        // �����༭��ؼ�
        m_hWnd = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"", Style | WS_CHILD,
            m_x, m_y, m_width, m_height, m_hWndParent, NULL, m_hInstance, NULL);
    }

    // ��ȡ�༭���е��ı�
    std::wstring GetText() const {
        wchar_t buffer[1024];
        GetWindowText(m_hWnd, buffer, sizeof(buffer) / sizeof(wchar_t));
        return std::wstring(buffer);
    }

    // ���ñ༭���е��ı�
    void SetText(const std::wstring& text) {
        SetWindowText(m_hWnd, text.c_str());
    }

    // ��ȡ�༭��ľ��
    HWND GetHandle() const {
        return m_hWnd;
    }

    // �����༭��Ĵ�С��λ��
    void SetPosition(int x, int y, int width, int height) {
        MoveWindow(m_hWnd, x, y, width, height, TRUE);
    }

    // ���ñ༭��
    void Disable() {
        EnableWindow(m_hWnd, FALSE);
    }

    // ���ñ༭��
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

