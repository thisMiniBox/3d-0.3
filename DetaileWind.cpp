#include "DetaileWind.h"
DetaileWind::DetaileWind() :m_TreeTarget(nullptr)
{
    m_ControlPos = 0;
    m_hInstance = GetModuleHandle(NULL);
    m_hWnd = nullptr;
    m_rect = {};
    WindClassName = L"Detaile_Wind";
    WNDCLASSEX wcex = { 0 };
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = DetaileWndProc;
    wcex.hInstance = m_hInstance;
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszClassName = WindClassName.c_str();
    RegisterClassEx(&wcex);
}
void DetaileWind::ControlPosMove(int change)
{
    m_ControlPos += change; 
    UpdateChildControl();
}
DetaileWind::~DetaileWind()
{
    for (const auto& i : m_ChildControl)
        delete i.second;
    m_ChildControl.clear();
    // ���ٴ���
    DestroyWindow(m_hWnd);
    // ע��������
    UnregisterClass(WindClassName.c_str(), m_hInstance);
}
void DetaileWind::UpdateChildControl()
{
    int x = m_rect.left;  // �Ӵ��ڵ� x ����
    int y = m_ControlPos;   // �Ӵ��ڵ� y ����
    int width = m_rect.right - m_rect.left;   // �����ڵĿ��
    for (const auto& i : m_ChildControl)
    {
        if (i.second->IsVisible())
            y += i.second->MoveWind_�ƶ�����(x, y, width);
    }
}
void DetaileWind::SetRect(RECT Rect)
{
    m_rect = Rect;
    int x = m_rect.left;  // �Ӵ��ڵ� x ����
    int y = m_ControlPos;   // �Ӵ��ڵ� y ����
    int width = m_rect.right - m_rect.left;   // �����ڵĿ��
    for (const auto& i : m_ChildControl)
    {
        if (i.second->IsVisible())
        y += i.second->MoveWind_�ƶ�����(x, y, width);
    }
}
HWND DetaileWind::CreateWind(HWND Parent)
{
    DestroyWindow(m_hWnd);
    m_hWnd = CreateWindow( //�����༭��
        WindClassName.c_str(),
        0,
        WS_CHILD | WS_BORDER | WS_VISIBLE,
        0, 0, 600, 400,
        Parent,
        (HMENU)3,
        GetModuleHandle(NULL),
        nullptr);
    if (m_hWnd)
    {
        GetClientRect(m_hWnd, &m_rect);
    }
    return m_hWnd;
}
void DetaileWind::SetView(Object* obj)
{
    for (const auto& i : m_ChildControl)
    {
        i.second->Hide();
    }
    m_target = obj;
    if (!obj)return;
    int y = m_ControlPos;
    switch (obj->GetType())
    {
    case OT_FOLDER:
    {
        y += CreateContrle(CT_NAME, 0, y, m_rect.right);
        break;
    }
    case OT_MODEL:
    {
        y += CreateContrle(CT_NAME, 0, y, m_rect.right);
        y += CreateContrle(CT_POSITION, 0, y, m_rect.right);
        y += CreateContrle(CT_ROTATE, 0, y, m_rect.right);
        break;
    }
    case OT_CAMERA:
    {
        y += CreateContrle(CT_NAME, 0, y, m_rect.right);
        y += CreateContrle(CT_POSITION, 0, y, m_rect.right);

        break;
    }
    }

}
void DetaileWind::SetTree(HTREEITEM htree)
{
    m_TreeTarget = htree;
}
int DetaileWind::CreateContrle(int type, int x, int y, int w)
{
    if (!m_ChildControl[type])
        switch (type)
        {
        case CT_NAME:
            m_ChildControl[type] = new Name_�������ƿؼ�(m_hInstance, m_hWnd, x, y, w);
            break;
        case CT_POSITION:
            m_ChildControl[type] = new Position_λ�ÿؼ�(m_hInstance, m_hWnd, x, y, w);
            break;
        case CT_ROTATE:
            m_ChildControl[type] = new Rotation_��ת�ؼ�(m_hInstance, m_hWnd, x, y, w);
        }
    ShowWindow(m_ChildControl[type]->GethWnd(), SW_SHOW);
    UpDate();
    return m_ChildControl[type]->GetHeight();
}
HWND DetaileWind::CreateDialogToWind(LRESULT(CALLBACK* aWNDPROC)(HWND, UINT, WPARAM, LPARAM), LPCWSTR lpTemplate)
{
    return CreateDialog(GetModuleHandle(NULL), lpTemplate, m_hWnd, aWNDPROC);
}
void DetaileWind::UpDate(int type)
{
    if (!m_target)return;
    if (type == -1)
        for (const auto& i : m_ChildControl)
            PostMessage(i.second->GethWnd(), WM_UPDATE, 0, 0);
    else
        PostMessage(m_ChildControl[type]->GethWnd(), WM_UPDATE, 0, 0);
}
