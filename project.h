#pragma once

#include"�ַ�ת��.h"
#include"WndData.h"
#include"MainWind.h"
#include"DetaileWind.h"
#include"resource.h"
#include<time.h>
#include<math.h>
#include<CommCtrl.h>
#include"Tree_���ؼ�.h"
#include"BottomWindow.h"
#include"FileWind.h"
#include"Object.h"
class Controller
{
	HINSTANCE m_hInst;
	Folder m_RootFolder;
	Folder* m_FocusFolder;
	std::vector<Model*>m_Models;
	BottomWindow* m_BottomWind;

	ReturnedOfLoadFile LoadObj(const std::string& filePath);

public:
	//������Ϣ
	HWND m_hWnd;
	MainWind* MAINWND;
	TextOutWind* TEXTWND;
	WndMsg SETWND;
	FileWind* FILEWND;
	DetaileWind* DETAWND;
	//�ļ���ȡ״̬
	bool m_FileLoad;
	//ģ������
	char Model_att;//��ʾģʽ
	//���������ǰ�ӽǣ�
	Camera* view;

	Controller();
	~Controller();
	HWND CreateWind(HINSTANCE hInst);
	HWND GetBottomWindhWnd()const;
	BottomWindow* GetBottom()const;
	void Size(int w, int h);
	//�ϴ���Ϣ
	void OutMessage(const std::string&, const char& mode = _Message);
	//�ϴ���Ϣ
	void OutMessage(const std::wstring& str, const char& mode = _Message);
	//������Ϣ����
	void updateMsg(const HDC&);
	//����ģ������
	std::vector<Model*>& UpdateModels();
	RECT GetRect()const;
	//�޸��ļ�����
	void SetFileName(Object*, const std::wstring& NewName);

	//�����ļ�
	ReturnedOfLoadFile LoadFile(const std::wstring& path);
	//�������ж���
	std::vector<Model*>& GetModels();
	//��Ӷ���
	HTREEITEM AddObject(Object*, std::string address = "0");
	//��Ӷ��󵽴˽ڵ���
	HTREEITEM AddObject(Object* a, HTREEITEM parent);
	//������Ʒ
	Object* CreateObject(Folder* parent = nullptr, std::string Name = "", int type = OT_FOLDER);
	//ɾ������
	void DeleteObject(Object* obj,HTREEITEM=nullptr);
	//��ȡ��ǰʵ��
	HINSTANCE GethInstance()const;
	//��ȡ�������
	Object* GetFocusObject()const;
	//�����Ҵ���
	void UpdateRightWind();
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};