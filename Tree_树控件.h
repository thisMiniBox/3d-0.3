#pragma once
#include<windows.h>
#include<unordered_map>
#include<commctrl.h>
#include"字符转换.h"
#include"Object.h"

class TreeControl {
public:
    // 构造函数，创建树控件并初始化相关参数
    TreeControl(HWND hwndParent, HINSTANCE hInstance, HIMAGELIST imageList = nullptr);

    // 析构函数，释放资源和清理树控件
    ~TreeControl();

    // 在指定的父节点下添加一个新的树节点，返回新节点的句柄
    HTREEITEM AddItem(HTREEITEM hParent, const wchar_t* pszText, int nImageIndex, LPARAM lParam);

    // 获取指定节点的第一个子节点的句柄
    HTREEITEM GetChildItem(HTREEITEM hParent) const;

    // 获取指定节点的所有子节点的句柄，并以向量的形式返回
    std::vector<HTREEITEM> GetAllChildItem(HTREEITEM hParent) const;

    // 删除指定的树节点，可以传入节点句柄或节点的自定义参数来删除节点
    void DeleteItem(HTREEITEM item);
    void DeleteItem(LPARAM lParam);

    // 根据自定义参数查找匹配的树节点，并返回节点的句柄
    HTREEITEM FindNodeByParam(LPARAM lParam) const;

    // 根据节点句柄获取节点的自定义参数
    LPARAM GetParamByItem(HTREEITEM item) const;

    // 浏览指定节点对应的文件，可以传入节点句柄或节点的自定义参数来指定节点
    void ExploreFile(HTREEITEM item)const;
    void ExploreFile(LPARAM lParam)const;

    // 获取当前选中的树节点的句柄
    HTREEITEM GetSelection() const;

    // 获取鼠标当前位置所在的树节点的句柄
    HTREEITEM GetMousePosItem() const;

    // 根据指定的坐标位置获取对应的树节点的句柄
    HTREEITEM GetItemOfPos(POINT cursorPos) const;

    // 获取当前拖放操作高亮显示的树节点的句柄
    HTREEITEM GetDropHiLight() const;

    // 设置指定节点的文本内容
    void SetItemText(HTREEITEM item, const wchar_t* pszText);

    // 设置指定节点的图像索引
    void SetItemImage(HTREEITEM item, int imageIndex, int selectedImageIndex);

    // 将一个节点移动到另一个节点的子节点列表中
    void MoveItem(HTREEITEM moved, HTREEITEM parent);

    // 移动树控件的位置和大小
    void MoveWnd(int x, int y, int w, int h);

    // 删除树控件中的所有节点
    void ClearAllItem();

private:
    std::map<LPARAM, HTREEITEM> m_ItemList;   // 节点的自定义参数与节点句柄之间的映射关系
    HWND m_hwndTree;                         // 树控件的句柄
    HIMAGELIST m_hImageList;                  // 图像列表的句柄
    void DeleteLparamItemListNode(LPARAM);
};