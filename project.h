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
#include<map>

typedef ComUserCode(*UserCode)(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

class Controller
{
	HINSTANCE m_hInst;				//ʵ��
	Folder m_RootFolder;			//��Դ�ļ���
	Folder* m_FocusFolder;			//�����ļ���
	std::wstring m_CurrentPath;		//�����ļ��е�ַ�ַ���

	std::vector<Model*>m_Models;	//��ʱ�洢ģ������

	BottomWindow* m_BottomWind;		//�ײ�����
	InputOutput* m_IOWind;			//����̨����
	KeyframeEdit* m_KeyframeWind;	//�ؼ�֡�༭����

	HIMAGELIST m_ImageList;			//��ͼ�б�
	std::map<int, int>m_ImageIndex;	//��ͼ����

	float m_ActualMaxFPS;			//���֡��
	float m_SetFPS;					//�趨֡��

	HMODULE m_hDll;					//dll����ָ��

	RUNMODE m_Mode;					//����״̬
	ULONG64 m_StartTime;			//��Ϸ����ʱ��
	ULONG64 m_RunTime;

	std::map<int, ShaderPath>m_ShaderIDPath;

	void LoadPngFromResources(int);
	ReturnedOfLoadFile LoadObj(const std::string& filePath);
	ReturnedOfLoadFile LoadCommand(const std::wstring& filePath);
	ReturnedOfLoadFile LoadDLL(const std::wstring& filePath);

public:
	//������Ϣ
	HWND m_hWnd;

	MainWind* m_MainWind;
	TextOutWind* m_TextWind;
	WndMsg m_MenuWind;
	FileWind* m_FileWind;
	DetaileWind* m_EditWind;
	//�ļ���ȡ״̬
	bool m_FileLoad;
	//ģ������
	char Model_att;//��ʾģʽ
	//���������ǰ�ӽǣ�
	Camera* view;
	UserCode MainWindUserCode;
	Controller();
	~Controller();
	HWND CreateWind(HINSTANCE hInst);
	HWND GetBottomWindhWnd()const;
	MainWind* GetMainWind();
	InputOutput* GetIOWind();
	BottomWindow* GetBottom();
	KeyframeEdit* GetKeyframeWind()const;
	void Size(int w, int h);
	//��ɫ��
	void AddShader(int ID, std::string vsPath, std::string fsPath);
	//����״̬
	void SetRunMode(RUNMODE);
	RUNMODE GetRunMode()const;
	//��ȡ��ͼ�б�
	HIMAGELIST GetImageList()const;
	//��ȡ��ͼ�б����ͼ����
	int GetImageListIndex(int);
	std::map<int, int>& GetImageListIndex();
	//����֡��
	void SetFPS(float);
	void __UpdateFPS(float);
	//��ȡ��ǰ֡��
	float GetMaxFPS()const;
	float GetSetFPS()const;
	//�ϴ���Ϣ
	void OutMessage(const std::string&, MSGtype type = _Message);
	//�ϴ���Ϣ
	void OutMessage(const std::wstring& str, MSGtype eype = _Message);
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
	bool Command(const CommandData&, bool ignoreOutput = false);
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
	void UpdateKeyframeView()const;
	/*void UpdateBottomView()const;*/
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	//�ؼ�֡������غ���

	void Run();
	void Suspend();
	void Stop();
	void SetTime(ULONG64 time);
	void Reset();
	ULONG64 GetTime();
	ULONG64 GetStartTime()const;
	void GetTime(ULONG64* left, ULONG64* right)const;
	int GetKeyframeEditY()const;
	void MoveKeyframeEditY(int y);
	void MoveKeyframeEditTime(int x);
};

void loadFileThread(HWND hWnd, Controller* current_project, std::wstring path);