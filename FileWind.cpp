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
    // ע�ᴰ����
    RegisterClassEx(&wcex);
}
HWND FileWind::CreateWind(HWND parent)
{
    m_hWnd = CreateWindowW( //�����༭��
        m_ClassName.c_str(),
        0,
        WS_CHILD | WS_BORDER | WS_VISIBLE,
        0, 50, 100, 100,
        parent,
        (HMENU)4,
        m_hInst,
        nullptr);
    if (m_hWnd)
        m_Tree = m_FileTree_�ļ���.Creat_�������б�(m_hWnd);
    else
    {
        m_Tree = nullptr;
        std::cout << "�ļ����ڴ���ʧ��" << std::endl;
        return nullptr;
    }
    return m_hWnd;
}
FileWind::~FileWind()
{
    // ���ٴ���
    DestroyWindow(m_hWnd);
    // ע��������
    UnregisterClass(m_ClassName.c_str(), m_hInst);
}
HTREEITEM FileWind::AddItem(const Object& obj,const std::string& add)
{
    return m_FileTree_�ļ���.AddItem_��ӽڵ�(obj, add);
}
HTREEITEM FileWind::AddItem(const Object& obj, HTREEITEM ptree)
{
    return m_FileTree_�ļ���.AddItem_��ӽڵ�(obj, ptree);
}
void FileWind::DeleteItem(HTREEITEM a)
{
    m_FileTree_�ļ���.DeleteItem_ɾ���ڵ�(a);
}
HTREEITEM FileWind::GetSelectedItem()
{
    return m_FileTree_�ļ���.GetItem_��ȡ��ѡ�нڵ�();
}
Object* FileWind::GetSelectedItemData()
{
    return m_FileTree_�ļ���.GetOption_��ȡ��ѡ�нڵ����();
}
HTREEITEM FileWind::GetMousePositionItem()
{
    return m_FileTree_�ļ���.GetMouseItem_��ȡ���λ�����ڵ�();
}
Object* FileWind::GetMousePositionItemData()
{
    return m_FileTree_�ļ���.GetMouseOption_��ȡ���λ�ýڵ����();
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
    m_FileTree_�ļ���.SetItemText_�޸Ľڵ�����(ht, Name);
}
#include<queue>
void FileWind::ShowFolder(const Folder& folder,HTREEITEM Parent)
{
    m_FileTree_�ļ���.ClearTree_�����();
    std::string folderName = folder.GetName();
    HTREEITEM hFolder = m_FileTree_�ļ���.AddItem_��ӽڵ�(folder, Parent);
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
            m_FileTree_�ļ���.AddItem_��ӽڵ�(*file, Parent);
            break;
        }
        }
    }
}
void FileWind::MoveTree(int x, int y, int w, int h)
{
    MoveWindow(m_Tree, x, y, w, h, true);
}