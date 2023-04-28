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
#include"FileWind.h"
#include"TextOutWind.h"
#include"Object.h"
class project
{
	HINSTANCE m_hInst;
	Folder m_RootFolder;
	std::vector<Model*>m_Models;
	RECT m_rect;
	ReturnedOfLoadFile LoadObj(const std::string& filePath);
public:
	//������Ϣ
	HWND hWnd;
	MainWind* MAINWND;
	TextOutWind* TEXTWND;
	WndMsg SETWND;
	FileWind* FILEWND;
	DetaileWind* DETAWND;
	//�ļ���ȡ״̬
	bool m_FileLoad;
	//ģ������
	char Model_att;//��ʾģʽ
	//std::vector<POINT>PerPIT_�����ŵ���Ļ;
	std::list<ModelTriData>ModelPoint3;//����2d��Ⱦʱ������ͶӰ��Ϣ
	//���������ǰ�ӽǣ�
	Camera* view;

	project();
	~project();
	HWND CreateWind(HINSTANCE hInst);
	//�ϴ���Ϣ
	void OutMessage(const std::string&, const char& mode = _Message);
	//�ϴ���Ϣ
	void OutMessage(const std::wstring& str, const char& mode = _Message);
	//������Ϣ����
	void updateMsg(const HDC&);
	//����ģ������
	std::vector<Model*>& UpdateModels();
	void SetRect(RECT rect);
	RECT GetRect()const;
	//�޸��ļ�����
	void SetFileName(Object*, const std::wstring& NewName);
	////����ģ�ͷ��ش�����
	//int loadModel(const std::wstring& path);
	
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
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};