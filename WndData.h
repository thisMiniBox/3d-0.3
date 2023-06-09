#pragma once
#include<Windows.h>
#include<vector>
#include<string>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include"resource.h"
#include"vector_向量.h"


int getBit(int num, int pos);
void ReplaceChar(std::string& str, char from, char to);
std::string GetFileExtension(const std::string& path);
std::wstring GetFileExtension(const std::wstring& path);

//子窗口标识
enum class ChildWindSign
{
	UNKNOW,
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
			KF_TimeWind,
			KF_FileWind,
			KF_ButtenWind,
			KF_CanvasWind,

	RightWind,
	AttributeWind,

	TopWind,
	MenuWind,

	LeftSeekBar,
	BottomSeekBar,
	RightSeekBar,
	TopSeekBar,
};

//窗口消息
enum UserWinMessage
{
	UM_CREATE_TIMER = WM_USER + 1,//创建定时器，定时器间隔时间
	UM_UPDATE,
};
//消息窗口运行模式
enum MSGMode
{
	_ALL,
	_ERROR,
	_WARNING,
	_REMIND
};
//消息种类
enum MSGtype
{
	_Message,
	_Error,
	_Warning
};
//物品种类
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
//加载文件返回
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
//物品包含的属性窗口种类
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
//消息存储
typedef struct WndMsg
{
	HWND hWnd;
	std::vector<WndMsg>ChildWnd;
	std::wstring className;
	RECT m_rect;
	WndMsg() :hWnd(nullptr) { m_rect = { 0 }; }
}WndMsg;
//默认着色器标识
enum ShaderID
{
	SI_Empty,
	SI_ModelShader,
	SI_LightShader,
	SI_StrokeShader,
	SI_SkyBoxShader,
	SI_User,
};

//模型模式
//enum ModelMode
//{
//	MM_DEFAULT,
//	MM_VISIBLE = 1,
//	MM_STATEIC = 2,
//};
//窗口布局
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
	WL_KeyframeFile_BottenHeight = 20,
	WL_WindowMaxHeigt = 480,
	WL_WindowMaxWidth = 640,
};

enum OperatingMode
{
	OM_Start,
	OM_Suspend,
};

typedef struct TransForm
{
	glm::vec3 Position;
	glm::vec3 Scale;
	vec::Rotation Rotate;
	TransForm():Position(0.0f),Scale(0.0f){}
	TransForm(const glm::vec3& position, const glm::vec3& scale, const vec::Rotation& rotate) :Position(position), Scale(scale), Rotate(rotate) {}
	TransForm GetKeyframe(const TransForm& next, float ratio)
	{
		return TransForm(Position + (next.Position - Position) * ratio, Scale + (next.Scale - Scale) * ratio, Rotate.getRotationTo(next.Rotate, ratio));
	}
	glm::mat4 GetOpenGLMat()const
	{
		if (Scale == glm::vec3(0.0f))
			return glm::mat4(1.0);
		glm::mat4 m_Transform = glm::mat4(1.0f);
		m_Transform = glm::scale(m_Transform, Scale);
		m_Transform = glm::rotate(m_Transform, (float)Rotate.angle, (glm::vec3)Rotate.axis);
		return glm::translate(m_Transform, Position);
	}
	static std::string GetStrType()
	{
		return "Transform";
	}
	static std::string GetDataType()
	{
		return "POSITION\nSCALE\nROTATE\n";
	}
	std::string GetStrData() const
	{
		std::ostringstream oss;
		oss << Position.x << ' '
			<< Position.y << ' '
			<< Position.z << '\n'
			<< Scale.x << ' '
			<< Scale.y << ' '
			<< Scale.z << '\n'
			<< Rotate.axis.x << ' '
			<< Rotate.axis.y << ' '
			<< Rotate.axis.z << ' '
			<< Rotate.angle << '\n';
		return oss.str();
	}
	void LoadStr(const std::string& data)
	{
		std::stringstream ss(data);

		ss >> Position.x >> Position.y >> Position.z;

		ss >> Scale.x >> Scale.y >> Scale.z;

		ss >> Rotate.axis.x >> Rotate.axis.y >> Rotate.axis.z >> Rotate.angle;
	}
}TransFrame;
enum RUNMODE
{
	RM_EDIT,
	RM_RUN,
};
enum SaveMode
{
	SM_TEXT,
	SM_XLSX,
	SM_BINARY,
};
