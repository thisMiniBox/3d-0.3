#include "Tree_树控件.h"
Tree_树控件::Tree_树控件()
{
	hWnd_树控件句柄 = nullptr;
}
HWND Tree_树控件::Creat_创建树列表(HWND parent_父窗口)
{
    RECT m_rect;
    GetWindowRect(parent_父窗口, &m_rect);
    return hWnd_树控件句柄 = CreateWindow((LPCWSTR)L"SysTreeView32", NULL,
        WS_CHILD | WS_BORDER | WS_VISIBLE | TVS_HASLINES | TVS_HASBUTTONS | TVS_LINESATROOT,
        0, 0, m_rect.right - m_rect.left, m_rect.bottom - m_rect.top,
        parent_父窗口, NULL, NULL, NULL);
}
HWND Tree_树控件::GethWnd_获得句柄()
{
    return hWnd_树控件句柄;
}
HTREEITEM Tree_树控件::AddItem_添加节点(const Object& aim_节点指定目标, std::string adders_节点的位置)
{
    if (adders_节点的位置.size() == 0)return AddItem_添加节点(aim_节点指定目标);
    HTREEITEM hti = TreeView_GetRoot(hWnd_树控件句柄);
    TVINSERTSTRUCT Phti;
    Phti.hParent = nullptr;
    if (!hti)return AddItem_添加节点(aim_节点指定目标);
    std::wstring wstr;
    int d_逗号位置 = 0;
    int number = 0;
    while (1)
    {
        d_逗号位置 = adders_节点的位置.find_first_of(",");
        if (d_逗号位置 == -1)
        {
            number = std::stol(adders_节点的位置.c_str());
            adders_节点的位置.clear();
        }
        else
        {
            number = std::stol(adders_节点的位置.substr(0, d_逗号位置));
            adders_节点的位置 = adders_节点的位置.substr(d_逗号位置 + 1);
        }
        if (number == 0)return AddItem_添加节点(aim_节点指定目标, Phti.hParent);
        for (int i = 1; i < number; i++)
            hti = TreeView_GetNextSibling(hWnd_树控件句柄, hti);
        if (!hti)return AddItem_添加节点(aim_节点指定目标, TreeView_GetParent(hWnd_树控件句柄, hti));
        Phti.hParent = hti;
        hti = TreeView_GetChild(hWnd_树控件句柄, hti);
        if (!hti || adders_节点的位置.size() == 0)return AddItem_添加节点(aim_节点指定目标, Phti.hParent);
    }
}
HTREEITEM Tree_树控件::AddItem_添加节点(const Object& aim_节点指定目标, HTREEITEM parent_父节点)
{
    std::wstring wstr = str_wstr((aim_节点指定目标.GetName()).c_str());
    TVINSERTSTRUCT tvInsert;
    tvInsert.hParent = parent_父节点;
    tvInsert.hInsertAfter = TVI_LAST;
    tvInsert.itemex.mask = TVIF_TEXT | TVIF_PARAM;
    tvInsert.itemex.pszText = (wchar_t*)wstr.c_str();
    tvInsert.itemex.lParam = (LPARAM)&aim_节点指定目标;
    return TreeView_InsertItem(hWnd_树控件句柄, &tvInsert);
}
void Tree_树控件::DeleteItem_删除节点(HTREEITEM Item)
{
    TreeView_DeleteItem(hWnd_树控件句柄, Item);
}
HTREEITEM Tree_树控件::GetItem_获取被选中节点()
{
    return TreeView_GetSelection(hWnd_树控件句柄);
}
Object* Tree_树控件::GetOption_获取被选中节点对象()
{
    // 获取选中节点的参数
    HTREEITEM hSelectedItem = TreeView_GetSelection(hWnd_树控件句柄);
    if (hSelectedItem != NULL)
    {
        TVITEMEX tvItem;
        tvItem.hItem = hSelectedItem;
        tvItem.mask = TVIF_PARAM;
        TreeView_GetItem(hWnd_树控件句柄, &tvItem);
        return (Object*)tvItem.lParam;
    }
    return nullptr;
}
HTREEITEM Tree_树控件::GetMouseItem_获取鼠标位置树节点()
{
    POINT pt;
    GetCursorPos(&pt);
    ScreenToClient(hWnd_树控件句柄, &pt);

    // 检测指定点是否位于树控件上
    TVHITTESTINFO htInfo;
    ZeroMemory(&htInfo, sizeof(htInfo));
    htInfo.pt = pt;
    return TreeView_HitTest(hWnd_树控件句柄, &htInfo);
}
Object* Tree_树控件::GetMouseOption_获取鼠标位置节点对象()
{
    // 获取鼠标当前位置
    POINT pt;
    GetCursorPos(&pt);
    ScreenToClient(hWnd_树控件句柄, &pt);

    // 检测指定点是否位于树控件上
    TVHITTESTINFO htInfo;
    ZeroMemory(&htInfo, sizeof(htInfo));
    htInfo.pt = pt;
    HTREEITEM hHitItem = TreeView_HitTest(hWnd_树控件句柄, &htInfo);
    
    // 如果命中了一个节点，则获取选中节点
    if (hHitItem != NULL)
    {
        TVITEM tvItem;
        ZeroMemory(&tvItem, sizeof(tvItem));
        tvItem.mask = TVIF_PARAM;
        tvItem.hItem = hHitItem;

        TreeView_GetItem(hWnd_树控件句柄, &tvItem);
        return(Object*)tvItem.lParam;
    }
    return nullptr;
}
void Tree_树控件::SetItemImage_设置节点图标(HTREEITEM hItem, int imageIndex_图片索引)
{
    TVITEM item;
    item.mask = TVIF_HANDLE | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
    item.hItem = hItem;
    item.iImage = imageIndex_图片索引;
    item.iSelectedImage = imageIndex_图片索引;
    TreeView_SetItem(hWnd_树控件句柄, &item);
}
bool Tree_树控件::DragDrop_拖放节点(HTREEITEM hDragItem, HTREEITEM hDropItem, UINT flags)
{
    TVINSERTSTRUCT tvins;

    tvins.itemex.mask = TVIF_HANDLE | TVIF_STATE | TVIF_PARAM | TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;

    tvins.hParent = TreeView_GetParent(hWnd_树控件句柄, hDropItem);
    tvins.hInsertAfter = hDropItem;
    tvins.itemex.hItem = hDragItem;
    tvins.itemex.state = 0;
    tvins.itemex.stateMask = TVIS_DROPHILITED;
    tvins.itemex.pszText = LPSTR_TEXTCALLBACK;
    tvins.itemex.cchTextMax = MAX_PATH;
    tvins.itemex.iImage = I_IMAGECALLBACK;
    tvins.itemex.iSelectedImage = I_IMAGECALLBACK;

    return TreeView_InsertItem(hWnd_树控件句柄, &tvins) != NULL;
}
void Tree_树控件::StartEdit_编辑节点(HTREEITEM hItem)
{
    TreeView_EditLabel(hWnd_树控件句柄, hItem);
}
void Tree_树控件::EndEdit_结束编辑(bool saveChanges_是否保存)
{
    if (saveChanges_是否保存)
        TreeView_EndEditLabelNow(hWnd_树控件句柄, TRUE);
    else
        TreeView_EndEditLabelNow(hWnd_树控件句柄, FALSE);
}
void Tree_树控件::ClearTree_清空树()
{
    TreeView_DeleteAllItems(hWnd_树控件句柄);
}

std::vector<Object*> Tree_树控件::GetSelectedItems_获取选中的节点()
{
    std::vector<Object*> selectedItems;

    HTREEITEM hItem = TreeView_GetSelection(hWnd_树控件句柄);

    while (hItem != NULL)
    {
        TVITEM item;
        item.mask = TVIF_HANDLE | TVIF_PARAM;
        item.hItem = hItem;
        TreeView_GetItem(hWnd_树控件句柄, &item);

        selectedItems.push_back((Object*)item.lParam);

        hItem = TreeView_GetNextSibling(hWnd_树控件句柄, hItem);
    }

    return selectedItems;
}
void Tree_树控件::SetItemText_修改节点名称(HTREEITEM hItem, std::wstring wstr)
{
    TVITEM tvi;
    tvi.mask = TVIF_TEXT;
    tvi.hItem = hItem; 
    tvi.pszText = (wchar_t*)wstr.c_str(); 
    TreeView_SetItem(hWnd_树控件句柄, &tvi);
}