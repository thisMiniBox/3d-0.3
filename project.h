#pragma once
#include<thread>
#include <mutex>
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
#include"xzdll.hpp"

class Controller
{
	HINSTANCE m_hInst;
	Folder m_RootFolder;
	Folder* m_FocusFolder;
	std::wstring m_CurrentPath;

	std::vector<Model*>m_Models;
	BottomWindow* m_BottomWind;
	InputOutput* m_IOWind;

	HIMAGELIST m_ImageList;//��ͼ�б�
	std::unordered_map<int, int>m_ImageIndex;//��ͼ����

	void LoadPngFromResources(int);
	ReturnedOfLoadFile LoadObj(const std::string& filePath);
	ReturnedOfLoadFile LoadCommand(const std::wstring& filePath);
	
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
	InputOutput* GetIOWind()const;
	BottomWindow* GetBottom()const;
	void Size(int w, int h);
	//��ȡ��ͼ�б�
	HIMAGELIST GetImageList()const;
	//��ȡ��ͼ�б����ͼ����
	int GetImageListIndex(int);
	std::unordered_map<int, int>& GetImageListIndex();

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
	//�������������
	void Print(const std::wstring&);
	bool Command(const std::wstring&, bool ignoreOutput = false);
	CommandData ParseCommand(const wchar_t* command);
	//�����ļ�
	ReturnedOfLoadFile LoadFile(const std::wstring& path);
	//�������ж���
	std::vector<Model*>& GetModels();
	//��Ӷ���
	HTREEITEM AddObject(Object*, std::string address = "0");
	//��Ӷ��󵽴˽ڵ���
	HTREEITEM AddObject(Object* a, HTREEITEM parent);
	//Ѱ���ļ�
	Object* SearchObject(std::wstring filename);
	//������Ʒ
	Object* CreateObject(Folder* parent = nullptr, std::string Name = "", ObjectType type = ObjectType::OT_FOLDER);
	//ɾ������
	void DeleteObject(Object* obj,HTREEITEM=nullptr);
	//��ȡ��ǰʵ��
	HINSTANCE GethInstance()const;
	//���ý���
	void SetFoucusObjcet(Object*);
	//��ȡ�������
	Object* GetFocusObject()const;
	//���´���
	void UpdateFileView()const;
	void UpdateDetaileViev()const;
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

void loadModelThread(HWND hWnd, Controller* current_project, std::wstring path);