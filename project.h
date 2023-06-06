#pragma once
#include<thread>
#include <mutex>
#include"字符转换.h"
#include"WndData.h"
#include"MainWind.h"
#include"DetaileWind.h"
#include"resource.h"
#include<time.h>
#include<math.h>
#include<CommCtrl.h>
#include"Tree_树控件.h"
#include"BottomWindow.h"
#include"FileWind.h"
#include"Object.h"
#include"xzdll.hpp"
#include<map>

typedef ComUserCode(*UserCode)(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

class Controller
{
	HINSTANCE m_hInst;				//实例
	Folder m_RootFolder;			//资源文件夹
	Folder* m_FocusFolder;			//操作文件夹
	std::wstring m_CurrentPath;		//操作文件夹地址字符串

	std::vector<Model*>m_Models;	//临时存储模型索引

	BottomWindow* m_BottomWind;		//底部窗口
	InputOutput* m_IOWind;			//控制台窗口
	KeyframeEdit* m_KeyframeWind;	//关键帧编辑窗口

	HIMAGELIST m_ImageList;			//贴图列表
	std::map<int, int>m_ImageIndex;	//贴图索引

	float m_ActualMaxFPS;			//最大帧数
	float m_SetFPS;					//设定帧数

	HMODULE m_hDll;					//dll函数指针

	RUNMODE m_Mode;					//运行状态
	ULONG64 m_StartTime;			//游戏运行时间
	ULONG64 m_RunTime;

	std::map<int, ShaderPath>m_ShaderIDPath;

	void LoadPngFromResources(int);
	ReturnedOfLoadFile LoadObj(const std::string& filePath);
	ReturnedOfLoadFile LoadCommand(const std::wstring& filePath);
	ReturnedOfLoadFile LoadDLL(const std::wstring& filePath);

public:
	//窗口信息
	HWND m_hWnd;

	MainWind* m_MainWind;
	TextOutWind* m_TextWind;
	WndMsg m_MenuWind;
	FileWind* m_FileWind;
	DetaileWind* m_EditWind;
	//文件读取状态
	bool m_FileLoad;
	//模型数据
	char Model_att;//显示模式
	//摄像机（当前视角）
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
	//着色器
	void AddShader(int ID, std::string vsPath, std::string fsPath);
	//运行状态
	void SetRunMode(RUNMODE);
	RUNMODE GetRunMode()const;
	//获取贴图列表
	HIMAGELIST GetImageList()const;
	//获取贴图列表的贴图索引
	int GetImageListIndex(int);
	std::map<int, int>& GetImageListIndex();
	//设置帧数
	void SetFPS(float);
	void __UpdateFPS(float);
	//获取当前帧数
	float GetMaxFPS()const;
	float GetSetFPS()const;
	//上传消息
	void OutMessage(const std::string&, MSGtype type = _Message);
	//上传消息
	void OutMessage(const std::wstring& str, MSGtype eype = _Message);
	//更新消息窗口
	void updateMsg(const HDC&);
	//更新模型数据
	std::vector<Model*>& UpdateModels();
	RECT GetRect()const;
	//修改文件名称
	void SetFileName(Object*, const std::wstring& NewName);
	//控制器窗口输出
	void Print(const std::wstring&);
	bool Command(const std::wstring&, bool ignoreOutput = false);
	bool Command(const CommandData&, bool ignoreOutput = false);
	CommandData ParseCommand(const wchar_t* command);
	//加载文件
	ReturnedOfLoadFile LoadFile(const std::wstring& path);
	//返回所有对象
	std::vector<Model*>& GetModels();
	//添加对象
	HTREEITEM AddObject(Object*, std::string address = "0");
	//添加对象到此节点下
	HTREEITEM AddObject(Object* a, HTREEITEM parent);
	//寻找文件
	Object* SearchObject(std::wstring filename);
	//创建物品
	Object* CreateObject(Folder* parent = nullptr, std::string Name = "", ObjectType type = ObjectType::OT_FOLDER);
	//删除对象
	void DeleteObject(Object* obj,HTREEITEM=nullptr);
	//获取当前实例
	HINSTANCE GethInstance()const;
	//设置焦点
	void SetFoucusObjcet(Object*);
	//获取焦点对象
	Object* GetFocusObject()const;
	//更新窗口
	void UpdateFileView()const;
	void UpdateDetaileViev()const;
	void UpdateKeyframeView()const;
	/*void UpdateBottomView()const;*/
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	//关键帧窗口相关函数

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