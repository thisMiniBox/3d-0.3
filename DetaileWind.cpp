#include "DetaileWind.h"
DetaileWind::DetaileWind()
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
    wcex.lpfnWndProc = WndProc;
    wcex.lpszClassName = L"pictureproc";
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
    UnregisterClass(L"pictureproc", m_hInstance);
}
void DetaileWind::UpdateChildControl()
{
    int x = m_rect.left;  // 子窗口的 x 坐标
    int y = m_ControlPos;   // 子窗口的 y 坐标
    int width = m_rect.right - m_rect.left;   // 父窗口的宽度
    for (const auto& i : m_ChildControl)
    {
        if (i.second && i.second->IsVisible())
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
        if (i.second)
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
        (HMENU)ChildWindSign::AttributeWind,
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
        if (i.second)
            i.second->Hide();
    }
    m_target = obj;
    if (!obj)return;
    int y = m_ControlPos;
    _ControlType ct = obj->SetDetaileView();
    int type = 0;
    for (int i = 0; i < sizeof(int) * 8; i++)
    {
        type = getBit(ct, i) << i;
        if (type != 0)
            y += CreateContrle(type, 0, y, m_rect.right, obj);
    }
    UpDate();
}
int DetaileWind::CreateContrle(int type, int x, int y, int w, Object* obj)
{
    if (type == CT_PICTURE && m_ChildControl[type])
    {
        delete m_ChildControl[type];
        m_ChildControl[type] = nullptr;
        m_ChildControl.erase(type);
    }

    if (!m_ChildControl[type])
        switch (type)
        {
        case CT_NAME:
            m_ChildControl[type] = new Name_对象名称控件(m_hInstance, m_hWnd, x, y, w);
            break;
        case CT_FILEVIEW:
            m_ChildControl[type] = new FileContentView(m_hInstance, m_hWnd, x, y, w, dynamic_cast<Folder*>(obj));
            break;
        case CT_ROTATE:
            m_ChildControl[type] = new Rotation_旋转控件(m_hInstance, m_hWnd, x, y, w);
            break;
        case CT_TRANSFORM:
            m_ChildControl[type] = new TransForm_变换控件(m_hInstance, m_hWnd, x, y, w);
            break;
        case CT_PICTURE:
            m_ChildControl[type] = new PictureControl(m_hInstance, m_hWnd, x, y, w, dynamic_cast<Picture*>(obj));
            break;
        }
    ShowWindow(m_ChildControl[type]->GethWnd(), SW_SHOW);
    int h = m_ChildControl[type]->GetHeight();
    MoveWindow(m_ChildControl[type]->GethWnd(), 0, y, w, h, true);
    return h;
}
HWND DetaileWind::CreateDialogToWind(LRESULT(CALLBACK* aWNDPROC)(HWND, UINT, WPARAM, LPARAM), LPCWSTR lpTemplate)
{
    return CreateDialog(GetModuleHandle(NULL), lpTemplate, m_hWnd, aWNDPROC);
}
void DetaileWind::UpDate(int type)
{
    if (!m_target)return;
    if (type == -1)
    {
        for (const auto& i : m_ChildControl)
        {
            if (i.second && i.second->IsVisible())
                PostMessage(i.second->GethWnd(), UM_UPDATE, 0, 0);
        }
    }
    else
        if (m_ChildControl[type] && m_ChildControl[type]->IsVisible())
            PostMessage(m_ChildControl[type]->GethWnd(), UM_UPDATE, 0, 0);
}

