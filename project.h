#pragma once

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
class Controller
{
	HINSTANCE m_hInst;
	Folder m_RootFolder;
	Folder* m_FocusFolder;
	std::vector<Model*>m_Models;
	BottomWindow* m_BottomWind;

	ReturnedOfLoadFile LoadObj(const std::string& filePath);

public:
	//窗口信息
	HWND m_hWnd;
	MainWind* MAINWND;
	TextOutWind* TEXTWND;
	WndMsg SETWND;
	FileWind* FILEWND;
	DetaileWind* DETAWND;
	//文件读取状态
	bool m_FileLoad;
	//模型数据
	char Model_att;//显示模式
	//摄像机（当前视角）
	Camera* view;

	Controller();
	~Controller();
	HWND CreateWind(HINSTANCE hInst);
	HWND GetBottomWindhWnd()const;
	BottomWindow* GetBottom()const;
	void Size(int w, int h);
	//上传消息
	void OutMessage(const std::string&, const char& mode = _Message);
	//上传消息
	void OutMessage(const std::wstring& str, const char& mode = _Message);
	//更新消息窗口
	void updateMsg(const HDC&);
	//更新模型数据
	std::vector<Model*>& UpdateModels();
	RECT GetRect()const;
	//修改文件名称
	void SetFileName(Object*, const std::wstring& NewName);

	//加载文件
	ReturnedOfLoadFile LoadFile(const std::wstring& path);
	//返回所有对象
	std::vector<Model*>& GetModels();
	//添加对象
	HTREEITEM AddObject(Object*, std::string address = "0");
	//添加对象到此节点下
	HTREEITEM AddObject(Object* a, HTREEITEM parent);
	//创建物品
	Object* CreateObject(Folder* parent = nullptr, std::string Name = "", int type = OT_FOLDER);
	//删除对象
	void DeleteObject(Object* obj,HTREEITEM=nullptr);
	//获取当前实例
	HINSTANCE GethInstance()const;
	//获取焦点对象
	Object* GetFocusObject()const;
	//更新右窗口
	void UpdateRightWind();
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};