#pragma once
#include<vector>
#include<string>
#include <iostream>
#include <fstream>
#include <array>
#include<d3d11.h>
#include<sstream>
#include <unordered_map>
#include"vector_向量.h"
#include"字符转换.h"
enum ObjectType
{
	OT_FOLDER,
	OT_MODEL,
	OT_CAMERA
};
using namespace vec;

// Object 类，表示场景中的物体基类
class Object
{
protected:
	std::string m_Name;  // 物体名称
public:
	// 默认构造函数
	Object();
	// 构造函数，可指定物体名称
	Object(std::string nam);
	// 析构函数
	virtual ~Object();
	// 获取物体名称
	const std::string GetName() const;
	// 设置物体名称
	void SetName(std::string NewName);
	// 获取物体类型，纯虚函数
	virtual int GetType() = 0;
	// 获取物体位置，返回默认值
	virtual Vector GetPosition() const;
	// 获取物体世界坐标，返回默认值
	virtual Vector GetWorldPosition() const;
	// 获取旋转
	virtual Rotation GetRotate()const;
	// 设定旋转
	virtual void SetRotate(const Rotation&);
	// 设置物体位置，空实现
	virtual void SetPosition(vec::Vector);
	// 移动物体，空实现
	virtual void Move(const Vector3&);
	//删除关联物体
	virtual void DeleteChildObject();

	//virtual void UpdateClassInfo(const ClassInfo&) = 0;
	//virtual ClassInfo* GainClassInfo() = 0;
};
//class ClassInfo
//{
//public:
//	std::string m_Name;
//	ClassInfo(Object*);
//
//};
//材质信息结构体
typedef struct _Material {
	std::string name;
	float Ns;
	float Ni;
	float Tr;
	float Ka[3];
	float Kd[3];
	float Ks[3];
	std::string map_Ka;
	std::string map_Kd;
	std::string map_Ks;
	_Material() :Ns(0.0), Ni(0.0), Tr(0.0), Ka(), Kd(), Ks() {}
}Material;
//用于Obj模型加载材质
class ModelShader_模型着色器 {
public:
	std::string file;
	ModelShader_模型着色器() {}
	~ModelShader_模型着色器() {}
	bool read(const std::string& filename);

	Material get_materials(std::string name) const {
		return materials.at(name);
	}

private:
	std::unordered_map<std::string, Material> materials;
};
//GDI输出面数据
typedef struct _OutPoint3
{
	POINT point[3];
	COLORREF color;
	float distance;
	_OutPoint3():point(),distance(0),color(RGB(0,0,0)){}
}Outface;
//面读取信息
typedef struct _fm_面信息
{
	size_t a[9];
	_fm_面信息() :a() {}
}FaceData_面信息;
//三角面数据
typedef struct _face_完整的三角面数据
{
	Vector3 vertexA;
	Vector3 vertexB;
	Vector3 vertexC;
	Vector3 normalA;
	Vector3 normalB;
	Vector3 normalC;
	Vector2 texCoordA;
	Vector2 texCoordB;
	Vector2 texCoordC;
	COLORREF color;
	_face_完整的三角面数据() :color(RGB(155, 155, 155)) {}
	_face_完整的三角面数据(Vector3 A, Vector3 B, Vector3 C,
		Vector3 NA, Vector3 NB, Vector3 NC,
		Vector2 TA, Vector2 TB, Vector2 TC) :
		vertexA(A), vertexB(B), vertexC(C),
		normalA(NA), normalB(NB), normalC(NC),
		texCoordA(TA), texCoordB(TB), texCoordC(TC),color(RGB(155,155,155)){}
	Vector3 GetVertex(int index) const {
		switch (index) {
		case 0:
			return vertexA;
		case 1:
			return vertexB;
		case 2:
			return vertexC;
		default:
			// 如果输入的索引无效，则返回零向量
			return Vector3(0, 0, 0);
		}
	}
}FACE;
using namespace DirectX;
struct Vertex {
	vec::Vector position;    // 顶点坐标
	vec::Vector normal;      // 顶点法线
	vec::Vector2 texCoord;   // 纹理坐标
};
enum ReturnedOfLoadFile :unsigned int
{
	//默认成功消息
	_Succese = 0xffffff00,
	//错误
	_UnknownFormat = 0b0001,
	_DataError = 0b0010,
	_FailToOpenFile = 0b0100,
	_FailedToCreateFile = 0b1000,
	//模型加载细节
	_SuccessfullyLoadedVertex = 0x0100,
	_SuccessfullyLoadedMaterialFile = 0x0200,
	_SuccessfullyLoadedMaterialMaps = 0x0400,

};
#include"stb_image.h"
class Model : public Object
{
public:
	// 默认构造函数
	Model();
	// 构造函数，可指定物体名称
	Model(std::string NAME);
	// 析构函数
	~Model();

	// 创建子模型，返回子模型指针
	Model* CreateChildModel(std::string Name);

	// 获取顶点坐标数据
	const std::vector<vec::Vector>& GetVertexData() const { return vertex_顶点坐标数据; }
	// 获取法向量数据
	const std::vector<vec::Vector>& GetNormalData() const { return normal_法向量数据; }
	// 获取贴图坐标数据
	const std::vector<vec::Vector2>& GetTexCoordData() const { return texCoords_贴图坐标数据; }
	// 获取材质信息
	const Material& GetMaterial() const { return mtl; }
	// 获取模型文件路径
	std::string GetFileAddress()const { return fileAddress; }
	// 获取面信息
	const std::vector<FaceData_面信息>& GetFaceData() const { return face_面的读取位置; }
	// 获取子模型指针列表
	const std::vector<Model*>& GetChildModel()const { return child_子模型指针; }
	// 获取父模型指针
	const Model* GetParentModel()const { return parent_父模型指针; }

	// 加载模型文件
	int loadModelFile_加载模型文件(const std::wstring& filename);
	// 获取三角形面片数据
	const std::vector<FACE>& GetTriFace();
	// 获取顶点数据
	const std::vector<Vertex>& GetVertices();

	// 设置缩放比例
	void SetScale(Vector s) { m_Scale = s; }
	// 更新三角形面片数据
	void UpdateFACEdata() { TriFace.clear(); }

	// 获取缩放比例
	Vector GetScale()const { return m_Scale; }

	// 获取物体位置
	virtual Vector GetPosition()const override { return m_Position; }
	// 获取物体世界坐标
	virtual Vector GetWorldPosition() const override;
	// 获取旋转
	virtual Rotation GetRotate()const override;
	//设定旋转四元数
	virtual void SetRotate(const Rotation&) override;
	// 设置物体位置
	virtual void SetPosition(vec::Vector p)override;
	// 移动物体
	virtual void Move(const Vector3& v) override { m_Position += v; if (!TriFace.empty())TriFace.clear(); }
	// 获取物体类型
	virtual int GetType()override;

private:
	void DistributeDataToChildModels();  // 将模型数据分配给子模型
	int loadobj(const std::wstring& path, FILE* file);  // 加载 obj 文件
	std::vector<vec::Vector> vertex_顶点坐标数据;  // 顶点坐标数据
	std::vector<vec::Vector> normal_法向量数据;  // 法向量数据
	std::vector<vec::Vector2> texCoords_贴图坐标数据;  // 贴图坐标数据
	std::vector<FACE> TriFace;  // 三角形面片数据
	std::vector<Vertex> ModelVertex;  // 顶点数据
	Material mtl;  // 材质信息
	std::vector<FaceData_面信息> face_面的读取位置;  // 面信息
	Model* parent_父模型指针;  // 父模型指针
	ModelShader_模型着色器* mtl_材质;  // 模型着色器
	std::vector<Model*>child_子模型指针;  // 子模型指针列表
	std::string fileAddress;  // 模型文件路径

	Vector m_Position;  // 物体位置
	Vector m_Scale;  // 缩放比例
	Rotation m_Rotate;//旋转
};
#include<unordered_map>
//对象文件/占位子类
class Folder : public Object
{
	std::unordered_map<std::string,Object*> m_Child;
	Folder* m_Parent;
public:
	Folder() { m_Name = "新建文件夹"; }
	Folder(std::string NAME);
	Object* CreateFile_创建文件(std::string Name, int type = OT_FOLDER);
	void AddFile_添加文件(Object*);
	//仅寻找当前目录
	Object* FindFile_寻找文件(const std::string& Name)const;
	void SetFileName(Object*, const std::string& NewName);
	std::vector<Object*> GetTheCurrentDirectoryFile()const;
	void ClearFolder_清空文件夹();
	void DeleteFile_删除文件(Object*);
	std::vector<Model*> GetAllModleFile_找到所有模型()const;
	int GetType();
	virtual Vector GetPosition()const override { return Vector(0, 0, 0); }
	virtual void DeleteChildObject()override;
};
//对象摄像机子类
class Camera : public Object {
private:
	vec::Vector3 m_Position;
	vec::Vector3 m_Target;
	vec::Vector3 m_Direction;
	vec::Vector3 m_Right;
	vec::Vector3 m_Up;
	vec::Vector3 m_CameraUp;
	float m_Ratio;
	float m_Field;
	float m_Near;
	float m_Far;
public:
	Camera();
	Camera(std::string name,
		Vector3 Position, Vector3 Target, Vector3 Cameraup,
		float aspect_ratio, float field_of_view=45.0f, float near_clip_plane=0.01f, float far_clip_plane=150.0f);
	virtual void SetPosition(vec::Vector p)override { m_Position = p; SetDirection(m_Target - p);}
	void SetDirection(const Vector&);
	void SetTarget(const Vector3&);
	void SetCameraUP(const Vector3&);
	void Move(const Vector3&)override;
	void SetAspectRatio(float aspect_ratio) { m_Ratio = aspect_ratio; }
	void SetFieldOfView(float FieldOfView) { m_Field = FieldOfView; }
	void SetNear(float Near) { m_Near = Near; }
	void SetFar(float Far) { m_Far = Far; }

	virtual Vector GetPosition()const override { return m_Position; }
	Vector3 GetDirection()const { return m_Direction; }
	Vector3 GetRight()const { return m_Right; }
	Vector3 GetUp()const { return m_Up; }
	Vector3 GetTarget() const { return m_Target; }
	Vector3 GetCameraUp() const { return m_CameraUp; }
	float GetAspectRatio()const { return m_Ratio; }
	float GetFieldOfView() const { return m_Field; }
	float GetNear() const { return m_Near; }
	float GetFar() const { return m_Far; }
	virtual int GetType()override { return OT_CAMERA; }

	Matrix4 GetView()const;
	Matrix4 GetGLMView()const;
	Matrix4 GetProjection()const;

};

class PointLight :public Object
{
	Vector m_Positon;
	Vector m_LightColor;
	float m_Intensity;
	float m_Range;
	float m_SoftShadow;
};
