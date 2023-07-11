#pragma once
#include<windows.h>
#include<unordered_map>
#include<commctrl.h>
#include"�ַ�ת��.h"
#include"Object.h"

class TreeControl {
public:
    // ���캯�����������ؼ�����ʼ����ز���
    TreeControl(HWND hwndParent, HINSTANCE hInstance, HIMAGELIST imageList = nullptr);

    // �����������ͷ���Դ���������ؼ�
    ~TreeControl();

    // ��ָ���ĸ��ڵ������һ���µ����ڵ㣬�����½ڵ�ľ��
    HTREEITEM AddItem(HTREEITEM hParent, const wchar_t* pszText, int nImageIndex, LPARAM lParam);

    // ��ȡָ���ڵ�ĵ�һ���ӽڵ�ľ��
    HTREEITEM GetChildItem(HTREEITEM hParent) const;

    // ��ȡָ���ڵ�������ӽڵ�ľ����������������ʽ����
    std::vector<HTREEITEM> GetAllChildItem(HTREEITEM hParent) const;

    // ɾ��ָ�������ڵ㣬���Դ���ڵ�����ڵ���Զ��������ɾ���ڵ�
    void DeleteItem(HTREEITEM item);
    void DeleteItem(LPARAM lParam);

    // �����Զ����������ƥ������ڵ㣬�����ؽڵ�ľ��
    HTREEITEM FindNodeByParam(LPARAM lParam) const;

    // ���ݽڵ�����ȡ�ڵ���Զ������
    LPARAM GetParamByItem(HTREEITEM item) const;

    // ���ָ���ڵ��Ӧ���ļ������Դ���ڵ�����ڵ���Զ��������ָ���ڵ�
    void ExploreFile(HTREEITEM item)const;
    void ExploreFile(LPARAM lParam)const;

    // ��ȡ��ǰѡ�е����ڵ�ľ��
    HTREEITEM GetSelection() const;

    // ��ȡ��굱ǰλ�����ڵ����ڵ�ľ��
    HTREEITEM GetMousePosItem() const;

    // ����ָ��������λ�û�ȡ��Ӧ�����ڵ�ľ��
    HTREEITEM GetItemOfPos(POINT cursorPos) const;

    // ��ȡ��ǰ�ϷŲ���������ʾ�����ڵ�ľ��
    HTREEITEM GetDropHiLight() const;

    // ����ָ���ڵ���ı�����
    void SetItemText(HTREEITEM item, const wchar_t* pszText);

    // ����ָ���ڵ��ͼ������
    void SetItemImage(HTREEITEM item, int imageIndex, int selectedImageIndex);

    // ��һ���ڵ��ƶ�����һ���ڵ���ӽڵ��б���
    void MoveItem(HTREEITEM moved, HTREEITEM parent);

    // �ƶ����ؼ���λ�úʹ�С
    void MoveWnd(int x, int y, int w, int h);

    // ɾ�����ؼ��е����нڵ�
    void ClearAllItem();

private:
    std::map<LPARAM, HTREEITEM> m_ItemList;   // �ڵ���Զ��������ڵ���֮���ӳ���ϵ
    HWND m_hwndTree;                         // ���ؼ��ľ��
    HIMAGELIST m_hImageList;                  // ͼ���б�ľ��
    void DeleteLparamItemListNode(LPARAM);
};