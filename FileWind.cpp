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
void FileWind::ExploreFolder(HTREEITEM type)
{
    TreeView_Expand(m_hWnd, type, TVE_EXPAND);
    InvalidateRect(m_hWnd, NULL, TRUE);
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
void FileWind::SetMappingBasedOnObjects(const Object& obj, HTREEITEM hItem)
{
    switch (obj.GetType())
    {
    case OT_FOLDER:
    {
        SetNodeImage(hItem, IDB_FOLDER, IDB_FOLDER_OPENED_NON_EMPTY);
        break;
    }
    case OT_MODEL:
    {
        SetNodeImage(hItem, IDB_MODEL, IDB_MODEL);
        break;
    }
    case OT_CAMERA:
    {
        SetNodeImage(hItem, IDB_CAMERA, IDB_CAMERA);
        break;
    }
    case OT_MESH:
    {
        SetNodeImage(hItem, IDB_MESH, IDB_MESH);
        break;
    }
    case OT_PICTURE:
    {
        SetNodeImage(hItem, IDB_PICTURE);
        break;
    }
    default:
        SetNodeImage(hItem, IDB_UNKNOW);
        break;
    }
}
HTREEITEM FileWind::AddItem(const Object& obj,const std::string& add)
{
    HTREEITEM hItem = m_FileTree_文件树.AddItem_添加节点(obj, add);
    SetMappingBasedOnObjects(obj, hItem);
    return hItem;
}
HTREEITEM FileWind::AddItem(const Object& obj, HTREEITEM ptree)
{
    HTREEITEM hItem = m_FileTree_文件树.AddItem_添加节点(obj, ptree);
    SetMappingBasedOnObjects(obj, hItem);
    return hItem;
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
void FileWind::ShowFolderRecursion(const Folder& folder, HTREEITEM Parent)
{
    std::string folderName = folder.GetName();
    HTREEITEM hFolder = m_FileTree_文件树.AddItem_添加节点(folder, Parent);
    SetMappingBasedOnObjects(folder, hFolder);
    for (Object* file : folder.GetTheCurrentDirectoryFile())
    {
        switch (file->GetType())
        {
        case OT_FOLDER:
        {
            Folder* subFolder = dynamic_cast<Folder*>(file);
            if (subFolder != nullptr)
            {
                ShowFolderRecursion(*subFolder, hFolder);
            }
            break;
        }
        case OT_MODEL:
        {
            HTREEITEM i = m_FileTree_文件树.AddItem_添加节点(*file, hFolder);
            SetMappingBasedOnObjects(*file, i);
            Model* Mod = dynamic_cast<Model*>(file);
            for (auto& cm : Mod->GetChildModel())
            {
                HTREEITEM j = m_FileTree_文件树.AddItem_添加节点(*cm, i);
                SetMappingBasedOnObjects(*cm, j);
            }
            break;
        }
        default:
        {
            std::string fileName = file->GetName();
            HTREEITEM i = m_FileTree_文件树.AddItem_添加节点(*file, hFolder);
            SetMappingBasedOnObjects(*file, i);
            break;
        }
        }
    }
}
void FileWind::ShowFolder(const Folder& folder,HTREEITEM Parent)
{
    m_FileTree_文件树.ClearTree_清空树();
    std::string folderName = folder.GetName();
    for (Object* file : folder.GetTheCurrentDirectoryFile())
    {
        switch (file->GetType())
        {
        case OT_FOLDER:
        {
            Folder* subFolder = dynamic_cast<Folder*>(file);
            if (subFolder != nullptr)
            {
                ShowFolderRecursion(*subFolder, Parent);
            }
            break;
        }
        case OT_MODEL:
        {
            HTREEITEM i = m_FileTree_文件树.AddItem_添加节点(*file, Parent);
            SetMappingBasedOnObjects(*file, i);
            Model* Mod = dynamic_cast<Model*>(file);
            for (auto& cm : Mod->GetChildModel())
            {
                HTREEITEM j = m_FileTree_文件树.AddItem_添加节点(*cm, i);
                SetMappingBasedOnObjects(*cm, j);
            }
            break;
        }
        default:
        {
            std::string fileName = file->GetName();
            HTREEITEM Item = m_FileTree_文件树.AddItem_添加节点(*file, Parent);
            SetMappingBasedOnObjects(*file, Item);
            break;
        }
        }
    }
}
void FileWind::MoveTree(int x, int y, int w, int h)
{
    MoveWindow(m_Tree, x, y, w, h, true);
}
void FileWind::SetNodeImage(HTREEITEM hItem, int imageIndex, int imageIndexSelected)
{
    if (imageIndexSelected == -1)
        imageIndexSelected = imageIndex;
    m_FileTree_文件树.SetItemImage_设置节点图标(hItem, imageIndex, imageIndexSelected);
}