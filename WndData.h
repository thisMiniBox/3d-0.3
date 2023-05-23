#pragma once
#include<Windows.h>
#include<vector>
#include<string>
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
	OT_FOLDER,				//�ļ���
	OT_MODEL,				//ģ��
	OT_CAMERA,				//�����
	OT_MESH,				//����
	OT_PICTURE,				//ͼƬ
	OT_MATERIAL,			//����
	OT_POINTLIGHT,			//���Դ
	OT_PARALLELLIGHT,		//ƽ�й��Դ
	OT_KEYFRAME,			//�ؼ�֡
	OT_ROOM,				//����
	OT_BUTTON,				//������
	OT_UI,					//�̶�ui 
};
enum ReturnedOfLoadFile :unsigned int
{
	//Ĭ��״̬
	_Fail = 0xffff0000,
	//Ĭ�ϳɹ���Ϣ
	_Succese = 0xffffff00,
	//����
	_UnknownFormat = 0b0001,
	_DataError = 0b0010,
	_FailToOpenFile = 0b0100,
	_FailedToCreateFile = 0b1000,
	//ģ�ͼ���ϸ��
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