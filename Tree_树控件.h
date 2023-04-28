#pragma once
#include<windows.h>
#include<commctrl.h>
#include"�ַ�ת��.h"
#include"Object.h"
HBITMAP LoadPngBitmap(HINSTANCE hInstance, int nID);
class Tree_���ؼ�
{
	HWND hWnd_���ؼ����;
	HIMAGELIST m_hImageList;
public:
	Tree_���ؼ�();
	//�������ؼ����
	HWND Creat_�������б�(HWND parent_������);
	HWND GethWnd_��þ��();
	//�ڵ��λ���á�1��2����ʽ��ʾ���˸�ʽ��ʾ��ӵ���һ���ڵ��µĵڶ����ڵ��У�
	//��1.0�����ʾ��ӵ���һ���ڵ��£���0д��ǰ��������Ϣ��������,
	//���������ᱻ����0����
	HTREEITEM AddItem_��ӽڵ�(const Object& aim_�ڵ�ָ��Ŀ��, std::string adders_�ڵ��λ��);
	HTREEITEM AddItem_��ӽڵ�(const Object& aim_�ڵ�ָ��Ŀ��, HTREEITEM parent_���ڵ� = TVI_ROOT);
	//ɾ��ָ���ڵ�
	void DeleteItem_ɾ���ڵ�(HTREEITEM Item);
	//��ȡ��ǰѡ�еĽڵ�
	Object* GetOption_��ȡ��ѡ�нڵ����();
	HTREEITEM GetItem_��ȡ��ѡ�нڵ�();
	//��ȡ���λ�ýڵ�
	Object* GetMouseOption_��ȡ���λ�ýڵ����();
	HTREEITEM GetMouseItem_��ȡ���λ�����ڵ�();

	void ClearTree_�����();
	void SetItemImage_���ýڵ�ͼ��(HTREEITEM hItem, int imageIndex, int imageIndexSelected);
	void SetItemText_�޸Ľڵ�����(HTREEITEM hItem, std::wstring);
	bool DragDrop_�ϷŽڵ�(HTREEITEM hDragItem, HTREEITEM hDropItem, UINT flags);
	void StartEdit_�༭�ڵ�(HTREEITEM hItem);
	void EndEdit_�����༭(bool saveChanges_�Ƿ񱣴�);

	std::vector<Object*> GetSelectedItems_��ȡѡ�еĽڵ�();
};

