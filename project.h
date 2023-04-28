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
	//窗口信息
	HWND hWnd;
	MainWind* MAINWND;
	TextOutWind* TEXTWND;
	WndMsg SETWND;
	FileWind* FILEWND;
	DetaileWind* DETAWND;
	//文件读取状态
	bool m_FileLoad;
	//模型数据
	char Model_att;//显示模式
	//std::vector<POINT>PerPIT_点缩放到屏幕;
	std::list<ModelTriData>ModelPoint3;//利用2d渲染时三角面投影信息
	//摄像机（当前视角）
	Camera* view;

	project();
	~project();
	HWND CreateWind(HINSTANCE hInst);
	//上传消息
	void OutMessage(const std::string&, const char& mode = _Message);
	//上传消息
	void OutMessage(const std::wstring& str, const char& mode = _Message);
	//更新消息窗口
	void updateMsg(const HDC&);
	//更新模型数据
	std::vector<Model*>& UpdateModels();
	void SetRect(RECT rect);
	RECT GetRect()const;
	//修改文件名称
	void SetFileName(Object*, const std::wstring& NewName);
	////加载模型返回错误码
	//int loadModel(const std::wstring& path);
	
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
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};