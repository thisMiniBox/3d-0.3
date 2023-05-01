#include "Tree_树控件.h"
#include"resource.h"
#include <wingdi.h>
#include "stb_image.h"
Tree_树控件::Tree_树控件()
{
    m_hInstance = nullptr;
	hWnd_树控件句柄 = nullptr;
    m_hImageList = nullptr;
}
int Tree_树控件::LoadPngToList(int nID)
{
    // 加载 PNG 图片资源
    HRSRC hResource = FindResource(m_hInstance, MAKEINTRESOURCE(nID), L"PNG");
    if (hResource == nullptr) {
        return -1;
    }
    HGLOBAL hData = LoadResource(m_hInstance, hResource);
    if (hData == nullptr) {
        return -1;
    }
    LPBYTE pData = (LPBYTE)LockResource(hData);
    DWORD dwSize = SizeofResource(m_hInstance, hResource);
    if (pData == nullptr || dwSize == 0) {
        return -1;
    }

    // 解码 PNG 图片数据
    int width, height, channels;
    unsigned char* image_data = stbi_load_from_memory(pData, dwSize, &width, &height, &channels, STBI_rgb_alpha);
    FreeResource(hData);
    if (image_data == nullptr) {
        return -1;
    }

    // 创建 DIB 对象
    BITMAPINFO bmi;
    ZeroMemory(&bmi, sizeof(bmi));
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = width;
    bmi.bmiHeader.biHeight = -height; // 指定负数即可让像素从上到下排列
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;
    void* pBits = nullptr;
    HBITMAP hBitmap = CreateDIBSection(nullptr, &bmi, DIB_RGB_COLORS, &pBits, nullptr, 0);
    if (hBitmap == nullptr || pBits == nullptr) {
        stbi_image_free(image_data);
        return -1;
    }

    // 将像素数据复制到 DIB 对象中
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int index = y * width + x;
            unsigned char r = image_data[index * 4 + 0];
            unsigned char g = image_data[index * 4 + 1];
            unsigned char b = image_data[index * 4 + 2];
            unsigned char a = image_data[index * 4 + 3];
            ((BYTE*)pBits)[index * 4 + 0] = b;
            ((BYTE*)pBits)[index * 4 + 1] = g;
            ((BYTE*)pBits)[index * 4 + 2] = r;
            ((BYTE*)pBits)[index * 4 + 3] = a;
        }
    }

    // 释放 PNG 图片数据内存
    stbi_image_free(image_data);

    int out = ImageList_AddMasked(m_hImageList, hBitmap, RGB(0, 0, 0));
    m_listID[nID] = out;
    DeleteObject(hBitmap);
    return out;
}
HWND Tree_树控件::Creat_创建树列表(HWND parent_父窗口)
{
    HINSTANCE m_hInst = GetModuleHandle(NULL);
    m_hInstance = m_hInst;
    RECT m_rect;
    GetWindowRect(parent_父窗口, &m_rect);
    hWnd_树控件句柄 = CreateWindow((LPCWSTR)L"SysTreeView32", NULL,
        WS_CHILD | WS_BORDER | WS_VISIBLE | TVS_HASLINES | TVS_HASBUTTONS | TVS_LINESATROOT,
        0, 0, m_rect.right - m_rect.left, m_rect.bottom - m_rect.top,
        parent_父窗口, NULL, m_hInst, NULL);

    
    m_hImageList = ImageList_Create(GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON),
        ILC_COLOR32 | ILC_MASK, 7, 0);
    LoadPngToList(IDB_UNKNOW);
    LoadPngToList(IDB_FOLDER);
    LoadPngToList(IDB_FOLDER_OPENED_NON_EMPTY);
    LoadPngToList(IDB_MODEL);
    LoadPngToList(IDB_MESH);
    LoadPngToList(IDB_PICTURE);
    LoadPngToList(IDB_CAMERA);

    // 将图像列表与树形控件关联
    TreeView_SetImageList(hWnd_树控件句柄, m_hImageList, TVSIL_NORMAL);
    return hWnd_树控件句柄;
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
void Tree_树控件::SetItemImage_设置节点图标(HTREEITEM hItem, int imageIndex, int imageIndexSelected)
{
    TVITEMEX tvItem = { 0 };
    tvItem.mask = TVIF_HANDLE | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
    tvItem.hItem = hItem; // hTreeItem 是要设置的树控件节点句柄
    tvItem.iImage = m_listID[imageIndex]; // 设置非选中时的图像索引
    tvItem.iSelectedImage = m_listID[imageIndexSelected]; // 设置选中时的图像索引

    TreeView_SetItem(hWnd_树控件句柄, &tvItem);
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