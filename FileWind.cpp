#include"FileWind.h"
FileWind::FileWind(HINSTANCE hInst)
{
    m_hInst = hInst;
    m_ClassName = L"FileWnd";
    WNDCLASSEX wcex = { 0 };
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = FileWndProc;
    wcex.hInstance = hInst;
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszClassName = m_ClassName.c_str();
    // 注册窗口类
    RegisterClassEx(&wcex);
}
HWND FileWind::CreateWind(HWND parent)
{
    m_hWnd = CreateWindowW( //创建编辑框
        m_ClassName.c_str(),
        0,
        WS_CHILD | WS_BORDER | WS_VISIBLE,
        0, 50, 100, 100,
        parent,
        (HMENU)4,
        m_hInst,
        nullptr);
    if (m_hWnd)
        m_Tree = m_FileTree_文件树.Creat_创建树列表(m_hWnd);
    else
    {
        m_Tree = nullptr;
        std::cout << "文件窗口创建失败" << std::endl;
        return nullptr;
    }
    return m_hWnd;
}
FileWind::~FileWind()
{
    // 销毁窗口
    DestroyWindow(m_hWnd);
    // 注销窗口类
    UnregisterClass(m_ClassName.c_str(), m_hInst);
}
HTREEITEM FileWind::AddItem(const Object& obj,const std::string& add)
{
    return m_FileTree_文件树.AddItem_添加节点(obj, add);
}
HTREEITEM FileWind::AddItem(const Object& obj, HTREEITEM ptree)
{
    return m_FileTree_文件树.AddItem_添加节点(obj, ptree);
}
void FileWind::DeleteItem(HTREEITEM a)
{
    m_FileTree_文件树.DeleteItem_删除节点(a);
}
HTREEITEM FileWind::GetSelectedItem()
{
    return m_FileTree_文件树.GetItem_获取被选中节点();
}
Object* FileWind::GetSelectedItemData()
{
    return m_FileTree_文件树.GetOption_获取被选中节点对象();
}
HTREEITEM FileWind::GetMousePositionItem()
{
    return m_FileTree_文件树.GetMouseItem_获取鼠标位置树节点();
}
Object* FileWind::GetMousePositionItemData()
{
    return m_FileTree_文件树.GetMouseOption_获取鼠标位置节点对象();
}
HWND FileWind::GethWnd()
{
    return m_hWnd;
}
HWND FileWind::GetTree()
{
    return m_Tree;
}
void FileWind::FixItemName(HTREEITEM ht, const std::wstring Name)
{
    m_FileTree_文件树.SetItemText_修改节点名称(ht, Name);
}
#include<queue>
void FileWind::ShowFolder(const Folder& folder,HTREEITEM Parent)
{
    m_FileTree_文件树.ClearTree_清空树();
    std::string folderName = folder.GetName();
    HTREEITEM hFolder = m_FileTree_文件树.AddItem_添加节点(folder, Parent);
    for (Object* file : folder.GetTheCurrentDirectoryFile())
    {
        switch (file->GetType())
        {
        case OT_FOLDER:
        {
            Folder* subFolder = dynamic_cast<Folder*>(file);
            if (subFolder != nullptr)
            {
                // Recursively show the subfolder
                ShowFolder(*subFolder, Parent);
            }
            break;
        }
        default:
        { 
            std::string fileName = file->GetName();
            m_FileTree_文件树.AddItem_添加节点(*file, Parent);
            break;
        }
        }
    }
}
void FileWind::MoveTree(int x, int y, int w, int h)
{
    MoveWindow(m_Tree, x, y, w, h, true);
}