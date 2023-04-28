#pragma once
#include<windows.h>
#include<commctrl.h>
#include"字符转换.h"
#include"Object.h"
HBITMAP LoadPngBitmap(HINSTANCE hInstance, int nID);
class Tree_树控件
{
	HWND hWnd_树控件句柄;
	HIMAGELIST m_hImageList;
public:
	Tree_树控件();
	//返回树控件句柄
	HWND Creat_创建树列表(HWND parent_父窗口);
	HWND GethWnd_获得句柄();
	//节点的位置用【1，2】格式表示，此格式表示添加到第一个节点下的第二个节点中，
	//【1.0】则表示添加到第一个节点下，若0写在前面后面的信息将被忽略,
	//非正数都会被当作0处理
	HTREEITEM AddItem_添加节点(const Object& aim_节点指定目标, std::string adders_节点的位置);
	HTREEITEM AddItem_添加节点(const Object& aim_节点指定目标, HTREEITEM parent_父节点 = TVI_ROOT);
	//删除指定节点
	void DeleteItem_删除节点(HTREEITEM Item);
	//获取当前选中的节点
	Object* GetOption_获取被选中节点对象();
	HTREEITEM GetItem_获取被选中节点();
	//获取鼠标位置节点
	Object* GetMouseOption_获取鼠标位置节点对象();
	HTREEITEM GetMouseItem_获取鼠标位置树节点();

	void ClearTree_清空树();
	void SetItemImage_设置节点图标(HTREEITEM hItem, int imageIndex, int imageIndexSelected);
	void SetItemText_修改节点名称(HTREEITEM hItem, std::wstring);
	bool DragDrop_拖放节点(HTREEITEM hDragItem, HTREEITEM hDropItem, UINT flags);
	void StartEdit_编辑节点(HTREEITEM hItem);
	void EndEdit_结束编辑(bool saveChanges_是否保存);

	std::vector<Object*> GetSelectedItems_获取选中的节点();
};

