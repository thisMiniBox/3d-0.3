#pragma once

#include"字符转换.h"
#include"LinkList.h"
//#include"Camera.h"
#include"WndData.h"
#include"MainWind.h"
#include"DetaileWind.h"
#include"resource.h"
#include<time.h>
#include<math.h>
#include<CommCtrl.h>
#include"Tree_树控件.h"
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
	//窗口信息
	HWND hWnd;
	MainWind* MAINWND;
	WndMsg TEXTWND;
	WndMsg SETWND;
	FileWind* FILEWND;
	DetaileWind* DETAWND;
	
	//消息信息
	int MSG_att;//消息显示状态
	int MAX_runMSG;//最大消息储存长度
	LinkList<runMsg>runMSG;
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
	void upMsg(const std::string&, const char& mode = _Message);
	//上传消息
	void upMsg(const std::wstring& str, const char& mode = _Message);
	//更新消息窗口
	void updateMsg(const HDC&);
	std::vector<Model*>& UpdateModels();
	void SetRect(RECT rect);
	RECT GetRect()const;
	//加载模型返回错误码
	int loadModel(const std::wstring& path);
	//返回所有对象
	std::vector<Model*>& GetModels();
	//添加对象
	HTREEITEM AddObject(Object*, std::string address = "0");
	//添加对象到次节点下
	HTREEITEM AddObject(Object* a, HTREEITEM parent);
	//删除对象
	void DeleteObject(Object* obj,HTREEITEM=nullptr);
	//获取当前实例
	HINSTANCE GethInstance()const;
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};