#include "Tree_���ؼ�.h"
#include"resource.h"
#include <wingdi.h>
#include "stb_image.h"
TreeControl::TreeControl(HWND hwndParent, HINSTANCE hInstance, HIMAGELIST imageList)
{
    // ��ʼ�������ؼ���
    InitCommonControls();
    m_hImageList = imageList;
    // �������ؼ�
    m_hwndTree = CreateWindowEx(0, WC_TREEVIEW, NULL, WS_VISIBLE | WS_CHILD | TVS_HASBUTTONS | TVS_LINESATROOT,
        0, 0, 200, 300, hwndParent, (HMENU)ID_TREE_CONTROL, hInstance, NULL);

    if (m_hwndTree == NULL)
    {
        MessageBox(NULL, L"Failed to create tree control.", L"Error", MB_OK | MB_ICONERROR);
        return;
    }
    else
        if(imageList)
            SendMessage(m_hwndTree, TVM_SETIMAGELIST, TVSIL_NORMAL, (LPARAM)m_hImageList);
    DWORD_PTR dwStyle = GetWindowLongPtr(m_hwndTree, GWL_EXSTYLE);
    SetWindowLongPtr(m_hwndTree, GWL_EXSTYLE, dwStyle | CS_DBLCLKS);

}
HTREEITEM TreeControl::AddItem(HTREEITEM hParent, const wchar_t* pszText, int nImageIndex, LPARAM lParam)
{
    TVINSERTSTRUCT tvInsert;
    ZeroMemory(&tvInsert, sizeof(tvInsert));
    tvInsert.hParent = hParent;
    tvInsert.hInsertAfter = TVI_LAST;
    tvInsert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
    tvInsert.item.pszText = const_cast<wchar_t*>(pszText);
    tvInsert.item.iImage = (nImageIndex);
    tvInsert.item.iSelectedImage = (nImageIndex);
    tvInsert.item.lParam = lParam;
    HTREEITEM item = (HTREEITEM)SendMessage(m_hwndTree, TVM_INSERTITEM, 0, (LPARAM)&tvInsert);
    if (item)
        m_ItemList.insert(std::make_pair(lParam, item));
    return item;
}
TreeControl::~TreeControl()
{
    // ִ����������������ͷ���Դ
    if (m_hwndTree != NULL) 
    {
        DestroyWindow(m_hwndTree);
        m_hwndTree = NULL;
    }
}
HTREEITEM TreeControl::FindNodeByParam(LPARAM lParam)const
{
    auto item = m_ItemList.find(lParam);
    if (item == m_ItemList.end())
        return nullptr;
    return (*item).second;
}
void TreeControl::ExploreFile(HTREEITEM item)const
{
    if (item)
        TreeView_Expand(m_hwndTree, item, TVE_EXPAND);
}
void TreeControl::ExploreFile(LPARAM lParam)const
{
    auto item = FindNodeByParam(lParam);
    if (item)
        ExploreFile(item);
}

void TreeControl::SetItemImage(HTREEITEM item, int imageIndex, int selectedImageIndex)
{

    TVITEM tvItem = {};
    tvItem.mask = TVIF_HANDLE | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
    tvItem.hItem = item;
    tvItem.iImage =(imageIndex);
    tvItem.iSelectedImage = (selectedImageIndex);

    // ���ýڵ�����ԣ�����ͼ������
    SendMessage(m_hwndTree, TVM_SETITEM, 0, (LPARAM)&tvItem);
}
void TreeControl::MoveWind(int x, int y, int w, int h)
{
    MoveWindow(m_hwndTree, x, y, w, h, true);
}
HTREEITEM TreeControl::GetChildItem(HTREEITEM item)const
{
    return TreeView_GetChild(m_hwndTree, item);
}
std::vector<HTREEITEM> TreeControl::GetAllChildeItem(HTREEITEM item)const
{
    // ����һ���洢�ӽڵ������
    std::vector<HTREEITEM> childNodes;
    // ʹ�� GetChildItem ������ȡ��һ���ӽڵ�ľ��
    HTREEITEM child = TreeView_GetChild(m_hwndTree, item);
    // ���������ӽڵ㣬����������ӵ�������
    while (child != NULL)
    {
        childNodes.push_back(child);
        child = TreeView_GetNextSibling(m_hwndTree, child);
    }
    return childNodes;
}
void TreeControl::MoveItem(HTREEITEM moved, HTREEITEM parent)
{
    std::vector<HTREEITEM> ChildNodes = GetAllChildeItem(moved);
    // ��ȡҪ�ƶ��ڵ����Ϣ
    TVITEM item;
    item.mask = TVIF_HANDLE | TVIF_CHILDREN | TVIF_PARAM | TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
    item.hItem = moved;
    item.pszText = new TCHAR[MAXCHAR];
    item.cchTextMax = MAXCHAR;
    SendMessage(m_hwndTree, TVM_GETITEM, 0, (LPARAM)&item);
    // ��ӽڵ㵽ָ�����ڵ���
    HTREEITEM inserted = AddItem(parent, item.pszText, item.iImage, item.lParam);
    // ������Դ
    delete[] item.pszText;
    for (auto ci : ChildNodes)
    {
        MoveItem(ci, inserted);
    }
    // ɾ���ڵ�
    DeleteItem(moved);
}

void TreeControl::ClearAllItem()
{
    SendMessage(m_hwndTree, TVM_DELETEITEM, 0, (LPARAM)TVI_ROOT);
}
void TreeControl::DeleteItem(HTREEITEM item)
{
    SendMessage(m_hwndTree, TVM_DELETEITEM, 0, (LPARAM)item);
    DeleteLparamItemListNode(GetParamByItem(item));
}
void TreeControl::DeleteItem(LPARAM lParam)
{
    SendMessage(m_hwndTree, TVM_DELETEITEM, 0, (LPARAM)FindNodeByParam(lParam));
    DeleteLparamItemListNode(lParam);
}
void TreeControl::DeleteLparamItemListNode(LPARAM lparam)
{
    auto node = m_ItemList.find(lparam);
    if (node == m_ItemList.end())
        return;
    m_ItemList.erase(node);
}
HTREEITEM TreeControl::GetSelection()const
{
    return (HTREEITEM)SendMessage(m_hwndTree, TVM_GETNEXTITEM, TVGN_CARET, 0);
}
HTREEITEM TreeControl::GetMousePosItem()const
{
    POINT cursorPos;
    GetCursorPos(&cursorPos);
    return GetItemOfPos(cursorPos);
}
HTREEITEM TreeControl::GetItemOfPos(POINT cursorPos)const
{
    ScreenToClient(m_hwndTree, &cursorPos);
    TVHITTESTINFO tvHitTestInfo;
    ZeroMemory(&tvHitTestInfo, sizeof(tvHitTestInfo));
    tvHitTestInfo.pt = cursorPos;

    return (HTREEITEM)SendMessage(m_hwndTree, TVM_HITTEST, 0, (LPARAM)&tvHitTestInfo);
}
LPARAM TreeControl::GetParamByItem(HTREEITEM item)const
{
    // ����TVITEM�ṹ��
    TVITEM tvItem;
    ZeroMemory(&tvItem, sizeof(tvItem));
    tvItem.mask = TVIF_PARAM;
    tvItem.hItem = item;

    // ��ȡ�ڵ�ĸ�������
    SendMessage(m_hwndTree, TVM_GETITEM, 0, (LPARAM)&tvItem);

    // ��ȡ��������
    return tvItem.lParam;
}
void TreeControl::SetItemText(HTREEITEM item, const wchar_t* pszText)
{
    TVITEM tvItem;
    ZeroMemory(&tvItem, sizeof(tvItem));
    tvItem.hItem = item;
    tvItem.mask = TVIF_TEXT;
    tvItem.pszText = const_cast<LPWSTR>(pszText);

    SendMessage(m_hwndTree, TVM_SETITEM, 0, (LPARAM)&tvItem);
}
HTREEITEM TreeControl::GetDropHiLight()const
{
    return TreeView_GetDropHilight(m_hwndTree);
}