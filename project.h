#pragma once

#include"�ַ�ת��.h"
#include"LinkList.h"
//#include"Camera.h"
#include"WndData.h"
#include"MainWind.h"
#include"DetaileWind.h"
#include"resource.h"
#include<time.h>
#include<math.h>
#include<CommCtrl.h>
#include"Tree_���ؼ�.h"
#include"FileWind.h"
#define _Error 0x03
#define _Warning 0x02
#define _Message 0x01

class project
{
	HINSTANCE m_hInst;
	Folder m_RootFolder;
	std::vector<Model*>m_Models;
	RECT m_rect;
public:
	//������Ϣ
	HWND hWnd;
	MainWind* MAINWND;
	WndMsg TEXTWND;
	WndMsg SETWND;
	FileWind* FILEWND;
	DetaileWind* DETAWND;
	
	//��Ϣ��Ϣ
	int MSG_att;//��Ϣ��ʾ״̬
	int MAX_runMSG;//�����Ϣ���泤��
	LinkList<runMsg>runMSG;
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
	void upMsg(const std::string&, const char& mode = _Message);
	//�ϴ���Ϣ
	void upMsg(const std::wstring& str, const char& mode = _Message);
	//������Ϣ����
	void updateMsg(const HDC&);
	std::vector<Model*>& UpdateModels();
	void SetRect(RECT rect);
	RECT GetRect()const;
	//����ģ�ͷ��ش�����
	int loadModel(const std::wstring& path);
	//�������ж���
	std::vector<Model*>& GetModels();
	//��Ӷ���
	HTREEITEM AddObject(Object*, std::string address = "0");
	//��Ӷ��󵽴νڵ���
	HTREEITEM AddObject(Object* a, HTREEITEM parent);
	//ɾ������
	void DeleteObject(Object* obj,HTREEITEM=nullptr);
	//��ȡ��ǰʵ��
	HINSTANCE GethInstance()const;
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};