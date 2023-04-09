#include "Tree_���ؼ�.h"
Tree_���ؼ�::Tree_���ؼ�()
{
	hWnd_���ؼ���� = nullptr;
}
HWND Tree_���ؼ�::Creat_�������б�(HWND parent_������)
{
    RECT m_rect;
    GetWindowRect(parent_������, &m_rect);
    return hWnd_���ؼ���� = CreateWindow((LPCWSTR)L"SysTreeView32", NULL,
        WS_CHILD | WS_BORDER | WS_VISIBLE | TVS_HASLINES | TVS_HASBUTTONS | TVS_LINESATROOT,
        0, 0, m_rect.right - m_rect.left, m_rect.bottom - m_rect.top,
        parent_������, NULL, NULL, NULL);
}
HWND Tree_���ؼ�::GethWnd_��þ��()
{
    return hWnd_���ؼ����;
}
HTREEITEM Tree_���ؼ�::AddItem_��ӽڵ�(const Object& aim_�ڵ�ָ��Ŀ��, std::string adders_�ڵ��λ��)
{
    if (adders_�ڵ��λ��.size() == 0)return AddItem_��ӽڵ�(aim_�ڵ�ָ��Ŀ��);
    HTREEITEM hti = TreeView_GetRoot(hWnd_���ؼ����);
    TVINSERTSTRUCT Phti;
    Phti.hParent = nullptr;
    if (!hti)return AddItem_��ӽڵ�(aim_�ڵ�ָ��Ŀ��);
    std::wstring wstr;
    int d_����λ�� = 0;
    int number = 0;
    while (1)
    {
        d_����λ�� = adders_�ڵ��λ��.find_first_of(",");
        if (d_����λ�� == -1)
        {
            number = std::stol(adders_�ڵ��λ��.c_str());
            adders_�ڵ��λ��.clear();
        }
        else
        {
            number = std::stol(adders_�ڵ��λ��.substr(0, d_����λ��));
            adders_�ڵ��λ�� = adders_�ڵ��λ��.substr(d_����λ�� + 1);
        }
        if (number == 0)return AddItem_��ӽڵ�(aim_�ڵ�ָ��Ŀ��, Phti.hParent);
        for (int i = 1; i < number; i++)
            hti = TreeView_GetNextSibling(hWnd_���ؼ����, hti);
        if (!hti)return AddItem_��ӽڵ�(aim_�ڵ�ָ��Ŀ��, TreeView_GetParent(hWnd_���ؼ����, hti));
        Phti.hParent = hti;
        hti = TreeView_GetChild(hWnd_���ؼ����, hti);
        if (!hti || adders_�ڵ��λ��.size() == 0)return AddItem_��ӽڵ�(aim_�ڵ�ָ��Ŀ��, Phti.hParent);
    }
}
HTREEITEM Tree_���ؼ�::AddItem_��ӽڵ�(const Object& aim_�ڵ�ָ��Ŀ��, HTREEITEM parent_���ڵ�)
{
    std::wstring wstr = str_wstr((aim_�ڵ�ָ��Ŀ��.GetName()).c_str());
    TVINSERTSTRUCT tvInsert;
    tvInsert.hParent = parent_���ڵ�;
    tvInsert.hInsertAfter = TVI_LAST;
    tvInsert.itemex.mask = TVIF_TEXT | TVIF_PARAM;
    tvInsert.itemex.pszText = (wchar_t*)wstr.c_str();
    tvInsert.itemex.lParam = (LPARAM)&aim_�ڵ�ָ��Ŀ��;
    return TreeView_InsertItem(hWnd_���ؼ����, &tvInsert);
}
void Tree_���ؼ�::DeleteItem_ɾ���ڵ�(HTREEITEM Item)
{
    TreeView_DeleteItem(hWnd_���ؼ����, Item);
}
HTREEITEM Tree_���ؼ�::GetItem_��ȡ��ѡ�нڵ�()
{
    return TreeView_GetSelection(hWnd_���ؼ����);
}
Object* Tree_���ؼ�::GetOption_��ȡ��ѡ�нڵ����()
{
    // ��ȡѡ�нڵ�Ĳ���
    HTREEITEM hSelectedItem = TreeView_GetSelection(hWnd_���ؼ����);
    if (hSelectedItem != NULL)
    {
        TVITEMEX tvItem;
        tvItem.hItem = hSelectedItem;
        tvItem.mask = TVIF_PARAM;
        TreeView_GetItem(hWnd_���ؼ����, &tvItem);
        return (Object*)tvItem.lParam;
    }
    return nullptr;
}
HTREEITEM Tree_���ؼ�::GetMouseItem_��ȡ���λ�����ڵ�()
{
    POINT pt;
    GetCursorPos(&pt);
    ScreenToClient(hWnd_���ؼ����, &pt);

    // ���ָ�����Ƿ�λ�����ؼ���
    TVHITTESTINFO htInfo;
    ZeroMemory(&htInfo, sizeof(htInfo));
    htInfo.pt = pt;
    return TreeView_HitTest(hWnd_���ؼ����, &htInfo);
}
Object* Tree_���ؼ�::GetMouseOption_��ȡ���λ�ýڵ����()
{
    // ��ȡ��굱ǰλ��
    POINT pt;
    GetCursorPos(&pt);
    ScreenToClient(hWnd_���ؼ����, &pt);

    // ���ָ�����Ƿ�λ�����ؼ���
    TVHITTESTINFO htInfo;
    ZeroMemory(&htInfo, sizeof(htInfo));
    htInfo.pt = pt;
    HTREEITEM hHitItem = TreeView_HitTest(hWnd_���ؼ����, &htInfo);
    
    // ���������һ���ڵ㣬���ȡѡ�нڵ�
    if (hHitItem != NULL)
    {
        TVITEM tvItem;
        ZeroMemory(&tvItem, sizeof(tvItem));
        tvItem.mask = TVIF_PARAM;
        tvItem.hItem = hHitItem;

        TreeView_GetItem(hWnd_���ؼ����, &tvItem);
        return(Object*)tvItem.lParam;
    }
    return nullptr;
}
void Tree_���ؼ�::SetItemImage_���ýڵ�ͼ��(HTREEITEM hItem, int imageIndex_ͼƬ����)
{
    TVITEM item;
    item.mask = TVIF_HANDLE | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
    item.hItem = hItem;
    item.iImage = imageIndex_ͼƬ����;
    item.iSelectedImage = imageIndex_ͼƬ����;
    TreeView_SetItem(hWnd_���ؼ����, &item);
}
bool Tree_���ؼ�::DragDrop_�ϷŽڵ�(HTREEITEM hDragItem, HTREEITEM hDropItem, UINT flags)
{
    TVINSERTSTRUCT tvins;

    tvins.itemex.mask = TVIF_HANDLE | TVIF_STATE | TVIF_PARAM | TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;

    tvins.hParent = TreeView_GetParent(hWnd_���ؼ����, hDropItem);
    tvins.hInsertAfter = hDropItem;
    tvins.itemex.hItem = hDragItem;
    tvins.itemex.state = 0;
    tvins.itemex.stateMask = TVIS_DROPHILITED;
    tvins.itemex.pszText = LPSTR_TEXTCALLBACK;
    tvins.itemex.cchTextMax = MAX_PATH;
    tvins.itemex.iImage = I_IMAGECALLBACK;
    tvins.itemex.iSelectedImage = I_IMAGECALLBACK;

    return TreeView_InsertItem(hWnd_���ؼ����, &tvins) != NULL;
}
void Tree_���ؼ�::StartEdit_�༭�ڵ�(HTREEITEM hItem)
{
    TreeView_EditLabel(hWnd_���ؼ����, hItem);
}
void Tree_���ؼ�::EndEdit_�����༭(bool saveChanges_�Ƿ񱣴�)
{
    if (saveChanges_�Ƿ񱣴�)
        TreeView_EndEditLabelNow(hWnd_���ؼ����, TRUE);
    else
        TreeView_EndEditLabelNow(hWnd_���ؼ����, FALSE);
}
void Tree_���ؼ�::ClearTree_�����()
{
    TreeView_DeleteAllItems(hWnd_���ؼ����);
}

std::vector<Object*> Tree_���ؼ�::GetSelectedItems_��ȡѡ�еĽڵ�()
{
    std::vector<Object*> selectedItems;

    HTREEITEM hItem = TreeView_GetSelection(hWnd_���ؼ����);

    while (hItem != NULL)
    {
        TVITEM item;
        item.mask = TVIF_HANDLE | TVIF_PARAM;
        item.hItem = hItem;
        TreeView_GetItem(hWnd_���ؼ����, &item);

        selectedItems.push_back((Object*)item.lParam);

        hItem = TreeView_GetNextSibling(hWnd_���ؼ����, hItem);
    }

    return selectedItems;
}
void Tree_���ؼ�::SetItemText_�޸Ľڵ�����(HTREEITEM hItem, std::wstring wstr)
{
    TVITEM tvi;
    tvi.mask = TVIF_TEXT;
    tvi.hItem = hItem; 
    tvi.pszText = (wchar_t*)wstr.c_str(); 
    TreeView_SetItem(hWnd_���ؼ����, &tvi);
}