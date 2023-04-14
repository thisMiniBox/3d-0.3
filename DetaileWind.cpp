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
    // 销毁窗口
    DestroyWindow(m_hWnd);
    // 注销窗口类
    UnregisterClass(WindClassName.c_str(), m_hInstance);
}
void DetaileWind::UpdateChildControl()
{
    int x = m_rect.left;  // 子窗口的 x 坐标
    int y = m_ControlPos;   // 子窗口的 y 坐标
    int width = m_rect.right - m_rect.left;   // 父窗口的宽度
    for (const auto& i : m_ChildControl)
    {
        if (i.second->IsVisible())
            y += i.second->MoveWind_移动窗口(x, y, width);
    }
}
void DetaileWind::SetRect(RECT Rect)
{
    m_rect = Rect;
    int x = m_rect.left;  // 子窗口的 x 坐标
    int y = m_ControlPos;   // 子窗口的 y 坐标
    int width = m_rect.right - m_rect.left;   // 父窗口的宽度
    for (const auto& i : m_ChildControl)
    {
        if (i.second->IsVisible())
        y += i.second->MoveWind_移动窗口(x, y, width);
    }
}
HWND DetaileWind::CreateWind(HWND Parent)
{
    DestroyWindow(m_hWnd);
    m_hWnd = CreateWindow( //创建编辑框
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
            m_ChildControl[type] = new Name_对象名称控件(m_hInstance, m_hWnd, x, y, w);
            break;
        case CT_POSITION:
            m_ChildControl[type] = new Position_位置控件(m_hInstance, m_hWnd, x, y, w);
            break;
        case CT_ROTATE:
            m_ChildControl[type] = new Rotation_旋转控件(m_hInstance, m_hWnd, x, y, w);
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
