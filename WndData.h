#pragma once
#include<Windows.h>
#include<vector>
#include<string>
#include<sstream>
#include"resource.h"
enum class ChildWindSign
{
	MainWind = 1,
	GdiWind,
	OpenGLWind,
	D3D11Wind,

	LeftWind,
	FileWind,
	ProjectContentWind,

	BottomWind,
	MessageWind,
	ControlWind,
	KeyframeWind,

	RightWind,
	AttributeWind,

	TopWind,
	MenuWind,

	LeftSeekBar,
	BottomSeekBar,
	RightSeekBar,
	TopSeekBar,
};

//自定义窗口消息
enum UserWinMessage
{
	UM_CREATE_TIMER = WM_USER + 1,//创建定时器，定时器间隔时间
	UM_UPDATE,
};
enum MSGMode
{
	_ALL,
	_ERROR,
	_WARNING,
	_REMIND
};
enum MSGtype
{
	_Message,
	_Error,
	_Warning
};
enum ObjectType
{
	OT_UNKNOWN,
	OT_FOLDER,				//文件夹
	OT_MODEL,				//模型
	OT_CAMERA,				//摄像机
	OT_MESH,				//网格
	OT_PICTURE,				//图片
	OT_MATERIAL,			//材质
	OT_POINTLIGHT,			//点光源
	OT_PARALLELLIGHT,		//平行光光源
	OT_KEYFRAME,			//关键帧
	OT_ROOM,				//房间
	OT_BUTTON,				//交互键
	OT_UI,					//固定ui 
};
enum ReturnedOfLoadFile :unsigned int
{
	//默认状态
	_Fail = 0xffff0000,
	//默认成功消息
	_Succese = 0xffffff00,
	//错误
	_UnknownFormat = 0b0001,
	_DataError = 0b0010,
	_FailToOpenFile = 0b0100,
	_FailedToCreateFile = 0b1000,
	//模型加载细节
	_ModelFail = 0xfffff800,
	_SuccessfullyLoadedVertex = 0x0100,
	_SuccessfullyLoadedMaterialFile = 0x0200,
	_SuccessfullyLoadedMaterialMaps = 0x0400,

};
ReturnedOfLoadFile operator|(ReturnedOfLoadFile a, ReturnedOfLoadFile b);
ReturnedOfLoadFile& operator|=(ReturnedOfLoadFile& a, ReturnedOfLoadFile b);
enum _ControlType
{
	CT_NAME = 0b00001,
	CT_TRANSFORM = 0b00010,
	CT_ROTATE = 0b00100,
	CT_FILEVIEW = 0b01000,
	CT_PICTURE = 0b10000,
};
inline _ControlType operator|(const _ControlType& lhs, const _ControlType& rhs)
{
	return static_cast<_ControlType>(static_cast<int>(lhs) | static_cast<int>(rhs));
}
typedef struct WndMsg
{
	HWND hWnd;
	std::vector<WndMsg>ChildWnd;
	std::wstring className;
	RECT m_rect;
	WndMsg() :hWnd(nullptr) { m_rect = { 0 }; }
}WndMsg;
enum ShaderID
{
	SI_Empty,
	SI_ModelShader,
	SI_LightShader,
	SI_StrokeShader,
	SI_SkyBoxShader,
	SI_User,
};
ATOM MyRegisterClass(HINSTANCE   hInstance,
	LPCWSTR     lpszClassName,
	LRESULT(*wnd_proc)(HWND, UINT, WPARAM, LPARAM),
	LPCWSTR     lpszMenuName = NULL,
	UINT        style = CS_HREDRAW | CS_VREDRAW,
	HCURSOR     hCursor = NULL,
	HBRUSH      hbrBackground = (HBRUSH)(COLOR_WINDOW + 1),
	HICON       hIconSm = NULL,
	HICON       hIcon = NULL,
	int         cbClsExtra = 0,
	int         cbWndExtra = 0);
int getBit(int num, int pos);
enum ModelMode
{
	MM_DEFAULT,
	MM_VISIBLE = 1,
	MM_STATEIC = 2,
};
enum WindowLayout
{
	WL_BottenHeight = 20,
	WL_BottenWidth = 100,
	WL_KeyframeBotten_Width = 120,
	WL_KeyframeBotten_Height = 30,
	WL_KeyframeBotten_BottenSize = 10,
	WL_KeyframeTime_Height = 30,
	WL_KeyframeTime_SmallScale = 5,
	WL_KeyframeTime_BigScale = 10,
	WL_KeyframeTime_TimeLine = 3,
	WL_WindowMaxHeigt = 480,
	WL_WindowMaxWidth = 640,
};