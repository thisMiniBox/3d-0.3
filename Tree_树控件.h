#pragma once
#include<windows.h>
#include<unordered_map>
#include<commctrl.h>
#include"×Ö·û×ª»».h"
#include"Object.h"

class TreeControl {
public:
    TreeControl(HWND hwndParent, HINSTANCE hInstance, HIMAGELIST imageList = nullptr);
    ~TreeControl();
    HTREEITEM AddItem(HTREEITEM hParent, const wchar_t* pszText, int nImageIndex, LPARAM lParam);
    HTREEITEM GetChildItem(HTREEITEM)const;
    std::vector<HTREEITEM> GetAllChildeItem(HTREEITEM)const;
    void DeleteItem(HTREEITEM item);
    void DeleteItem(LPARAM lParam);
    HTREEITEM FindNodeByParam(LPARAM lParam)const;
    LPARAM GetParamByItem(HTREEITEM item)const;
    void ExploreFile(HTREEITEM item)const;
    void ExploreFile(LPARAM lParam)const;
    HTREEITEM GetSelection()const;
    HTREEITEM GetMousePosItem()const;
    HTREEITEM GetItemOfPos(POINT cursorPos)const;
    HTREEITEM GetDropHiLight()const;
    void SetItemText(HTREEITEM item, const wchar_t* pszText);
    void SetItemImage(HTREEITEM item, int imageIndex, int selectedImageIndex);
    void MoveItem(HTREEITEM moved, HTREEITEM parent);
    void MoveWind(int x, int y, int w, int h);
    void ClearAllItem();
private:
    void DeleteLparamItemListNode(LPARAM);
    std::map<LPARAM, HTREEITEM>m_ItemList;
    HWND m_hwndTree;
    HIMAGELIST m_hImageList;
    static const int ID_TREE_CONTROL = 1001;
};