#include"FileWind.h"
FileWind::FileWind(HWND parent, HINSTANCE hInstance)
{
    m_hInst = hInstance;
    m_ClassName = L"FileWnd";
    WNDCLASSEX wcex = { 0 };
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = FileWndProc;
    wcex.hInstance = hInstance;
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszClassName = m_ClassName.c_str();
    // 注册窗口类
    RegisterClassEx(&wcex);
    m_hWnd = CreateWindowW( //创建编辑框
        m_ClassName.c_str(),
        0,
        WS_CHILD | WS_BORDER | WS_VISIBLE,
        0, 50, 100, 100,
        parent,
        (HMENU)ChildWindSign::FileWind,
        m_hInst,
        nullptr);
    if (!m_hWnd)
    {
        OutMessage_g("文件窗口创建失败", _Error);
        std::cout << "文件窗口创建失败" << std::endl;
    }
    HIMAGELIST hImageList = ImageList_Create(16, 16, ILC_COLOR32 | ILC_MASK, 7, 0);
    int number = LoadPngToList(IDB_UNKNOWN, hImageList, hInstance);
    m_PngImglist.insert(std::make_pair(IDB_UNKNOWN, number));

    number = LoadPngToList(IDB_CAMERA, hImageList, hInstance);
    m_PngImglist.insert(std::make_pair(IDB_CAMERA, number));

    number = LoadPngToList(IDB_FOLDER, hImageList, hInstance);
    m_PngImglist.insert(std::make_pair(IDB_FOLDER, number));

    number = LoadPngToList(IDB_PICTURE, hImageList, hInstance);
    m_PngImglist.insert(std::make_pair(IDB_PICTURE, number));

    number = LoadPngToList(IDB_MODEL, hImageList, hInstance);
    m_PngImglist.insert(std::make_pair(IDB_MODEL, number));

    number = LoadPngToList(IDB_MESH, hImageList, hInstance);
    m_PngImglist.insert(std::make_pair(IDB_MESH, number));

    number = LoadPngToList(IDB_MATERIAL, hImageList, hInstance);
    m_PngImglist.insert(std::make_pair(IDB_MATERIAL, number));

    //number = LoadPngToList(IDB_KEYFRAME, hImageList, hInstance);
    //m_PngImglist.insert(std::make_pair(IDB_KEYFRAME, number));

    //number = LoadPngToList(IDB_POINTLIGHT, hImageList, hInstance);
    //m_PngImglist.insert(std::make_pair(IDB_POINTLIGHT, number));

    //number = LoadPngToList(IDB_DIRECTIONALLIGHT, hImageList, hInstance);
    //m_PngImglist.insert(std::make_pair(IDB_DIRECTIONALLIGHT, number));

    //number = LoadPngToList(IDB_SPOTLIGHT, hImageList, hInstance);
    //m_PngImglist.insert(std::make_pair(IDB_SPOTLIGHT, number));

    //number = LoadPngToList(IDB_COLLISION_BOX, hImageList, hInstance);
    //m_PngImglist.insert(std::make_pair(IDB_COLLISION_BOX, number));

    //number = LoadPngToList(IDB_SKYBOX, hImageList, hInstance);
    //m_PngImglist.insert(std::make_pair(IDB_SKYBOX, number));


    m_FileTreeView = new TreeControl(m_hWnd, hInstance, hImageList);
}
FileWind::~FileWind()
{
    // 销毁窗口
    DestroyWindow(m_hWnd);
    // 注销窗口类
    UnregisterClass(m_ClassName.c_str(), m_hInst);
    if (m_FileTreeView)
        delete m_FileTreeView;
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
    case OT_MATERIAL:
    {
        SetNodeImage(hItem, IDB_MATERIAL);
        break;
    }
    default:
        SetNodeImage(hItem, IDB_UNKNOW);
        break;
    }
}
HTREEITEM FileWind::AddItem(const Object& obj, HTREEITEM ptree)
{
    HTREEITEM hItem = m_FileTreeView->AddItem(ptree, strwstr(obj.GetName()).c_str(), IDB_UNKNOWN, (LPARAM)&obj);
    SetMappingBasedOnObjects(obj, hItem);
    return hItem;
}
void FileWind::DeleteItem(HTREEITEM a)
{
    m_FileTreeView->DeleteItem(a);
}
void FileWind::DeleteItem(Object* a)
{
    m_FileTreeView->DeleteItem((LPARAM)a);
}
HTREEITEM FileWind::GetSelectedItem()
{
    return m_FileTreeView->GetSelection();
}
Object* FileWind::GetSelectedItemData()
{
    HTREEITEM item = m_FileTreeView->GetSelection();
    return (Object*)m_FileTreeView->GetParamByItem(item);
}
HTREEITEM FileWind::GetMousePositionItem()
{
    return m_FileTreeView->GetMousePosItem();
}
Object* FileWind::GetMousePositionItemData()
{
    HTREEITEM item = m_FileTreeView->GetMousePosItem();
    return (Object*)m_FileTreeView->GetParamByItem(item);
}
Object* FileWind::GetItemObject(HTREEITEM item)const
{
    return (Object*)m_FileTreeView->GetParamByItem(item);
}
HWND FileWind::GethWnd()
{
    return m_hWnd;
}
void FileWind::FixItemName(HTREEITEM ht, const std::wstring Name)
{
    m_FileTreeView->SetItemText(ht, Name.c_str());
}
void FileWind::ShowFolderRecursion(const Folder& folder, const Object* aim, HTREEITEM Parent)
{
    std::string folderName = folder.GetName();
    HTREEITEM hFolder = AddItem(folder,Parent);
    for (Object* file : folder.GetTheCurrentDirectoryFile())
    {
        switch (file->GetType())
        {
        case OT_FOLDER:
        {
            Folder* subFolder = dynamic_cast<Folder*>(file);
            if (subFolder != nullptr)
            {
                ShowFolderRecursion(*subFolder,aim, hFolder);
            }
            break;
        }
        case OT_MODEL:
        {
            ShowModel(dynamic_cast<Model*>(file), Parent);
            break;
        }
        default:
        {
            std::string fileName = file->GetName();
            HTREEITEM i = AddItem(*file, hFolder);
            break;
        }
        }
    }
}
void FileWind::ShowFolder(const Folder& folder, const Object* aim, HTREEITEM Parent)
{
    m_FileTreeView->ClearAllItem();
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
                ShowFolderRecursion(*subFolder, aim,Parent);
            }
            break;
        }
        case OT_MODEL:
        {
            ShowModel(dynamic_cast<Model*>(file), Parent);
            break;
        }
        default:
        {
            std::string fileName = file->GetName();
            HTREEITEM Item = AddItem(*file, Parent);
            if (aim == file)
                TreeView_Expand(m_hWnd, Item, TVE_EXPAND);
            break;
        }
        }
    }
}
void FileWind::ShowModel(const Model* model, HTREEITEM parent)
{
    HTREEITEM Item = AddItem(*model, parent);
    for (auto& cm : model->GetChildModel())
    {
        ShowModel(cm, Item);
    }
}
void FileWind::MoveTree(int x, int y, int w, int h)
{
    m_FileTreeView->MoveWnd(x, y, w, h);
}
void FileWind::SetNodeImage(HTREEITEM hItem, int imageIndex, int imageIndexSelected)
{
    if (imageIndexSelected == -1)
        imageIndexSelected = imageIndex;
    m_FileTreeView->SetItemImage(hItem, GetPngID(imageIndex), GetPngID(imageIndexSelected));
}
void FileWind::ExploreFolder(HTREEITEM item)
{
    m_FileTreeView->ExploreFile(item);
}
void FileWind::ExploreFolder(Object* obj)
{
    m_FileTreeView->ExploreFile((LPARAM)obj);
}
int FileWind::GetPngID(int pngIndex)
{
    auto item = m_PngImglist.find(pngIndex);
    if (item == m_PngImglist.end())
        return -1;
    return (*item).second;
}
void FileWind::FixItemName(Object* obj, const std::wstring Name)
{
    FixItemName(m_FileTreeView->FindNodeByParam((LPARAM)obj), Name);
}
void FileWind::MoveItem(HTREEITEM moved, HTREEITEM parent)
{
    m_FileTreeView->MoveItem(moved, parent);
}