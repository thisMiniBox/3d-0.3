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
#include<glm/gtc/matrix_transform.hpp>
//声明类（方便定位）
class Object;
class Folder;
class Mesh;
class Picture;
class Material;
class Model;
class Camera;




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
};
using namespace vec;
std::wstring ObjectTypeToWstr(ObjectType);
std::string ObjectTypeTostr(ObjectType);
ObjectType  StrToObjectType(const std::string&);
ObjectType  WStrToObjectType(const std::wstring&);
// Object 类，表示场景中的物体基类
class Object
{
protected:
	bool m_Selected;
	Folder* m_ParentFolder;
	std::string m_Name;  // 物体名称
public:
	// 默认构造函数
	Object();
	// 构造函数，可指定物体名称
	Object(std::string nam);
	// 析构函数
	virtual ~Object();
	
	void SetParent(Folder*); 
	Folder* GetParent()const;
	// 获取物体名称
	const std::string GetName() const;
	// 设置物体名称
	void SetName(std::string NewName);
	//切换选中状态
	void ToggleSelection();
	void Selected();
	void Unselected();
	//获取选中状态
	bool IsSelected();
	// 获取物体类型，纯虚函数
	virtual ObjectType GetType()const = 0;
	// 获取物体位置，返回默认值
	virtual Vector GetPosition() const;
	// 获取缩放
	virtual Vector GetScale() const;
	// 获取物体世界坐标，返回默认值
	virtual Vector GetWorldPosition() const;
	// 获取旋转
	virtual Rotation GetRotate()const;
	// 设定旋转
	virtual void SetRotate(const Rotation&);
	//旋转
	virtual void Rotate(const Rotation&);
	// 设置物体位置，空实现
	virtual void SetPosition(const vec::Vector&);
	// 设置缩放
	virtual void SetScale(const Vector3&);
	//缩放
	virtual void Scale(const Vector3&);
	// 移动物体，空实现
	virtual void Move(const Vector3&);
	//删除关联物体
	virtual void DeleteChildObject();

	//virtual void UpdateClassInfo(const ClassInfo&) = 0;
	//virtual ClassInfo* GainClassInfo() = 0;
};
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
}TMaterial;

//用于Obj模型加载材质
class ModelShader_模型着色器 {
public:
	std::string file;
	ModelShader_模型着色器() {}
	~ModelShader_模型着色器() {}
	bool read(const std::string& filename);

	TMaterial get_materials(std::string name) const {
		return materials.at(name);
	}

private:
	std::unordered_map<std::string, TMaterial> materials;
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
	_ModelFail=0xfffff800,
	_SuccessfullyLoadedVertex = 0x0100,
	_SuccessfullyLoadedMaterialFile = 0x0200,
	_SuccessfullyLoadedMaterialMaps = 0x0400,

};
ReturnedOfLoadFile operator|(ReturnedOfLoadFile a, ReturnedOfLoadFile b);
ReturnedOfLoadFile& operator|=(ReturnedOfLoadFile& a, ReturnedOfLoadFile b);
#include"stb_image.h"

class Mesh :public Object
{
	std::vector<Vertex>m_Data;
public:
	Mesh() {}
	Mesh(std::string&);
	~Mesh() {}
	std::vector<vec::Vector> m_VertexPosition;  // 顶点坐标数据
	std::vector<vec::Vector> m_Normal;  // 法向量数据
	std::vector<vec::Vector2> m_TexCoords;  // 贴图坐标数据
	std::vector<FaceData_面信息> m_FaceIndices;  // 面信息
	const std::vector<Vertex>& GetVertexData();
	virtual ObjectType GetType() const override { return  ObjectType::OT_MESH; }
};
#include"glad.h"

typedef struct _PictureData
{
	unsigned char* m_Data;
	int m_Width, m_Height, m_NrComponents;
}PictureData;
class Picture :public Object
{
	unsigned char* m_Data;
	unsigned int m_ID;
	int m_Width, m_Height, m_NrComponents;
public:
	Picture();
	Picture(std::string& name);
	~Picture();
	//加载图片
	PictureData LoadPicture(const std::string& path);
	PictureData GetPicture()const;
	//加载图片并载入到OpenGl
	unsigned int loadTexture();
	unsigned int GetID() { return m_ID; }

	int GetHeight() { return m_Height; }
	int GetWidth() { return m_Width; }
	//释放图片
	void FreePictureData();
	void FreeOpenGL();
	virtual ObjectType GetType() const override { return OT_PICTURE; }
};
class Material :public Object
{
public:
	// 构造函数
	Material();
	Material(std::string& name) { m_Name = name; }
	// 析构函数
	~Material();

	// 访问器方法，用于设置或读取私有成员变量的值
	float getNs() const;
	void setNs(float ns);
	float getNi() const;
	void setNi(float ni);
	float getTr() const;
	void setTr(float tr);
	const float* getKa() const;
	void setKa(const float ka[3]);
	const float* getKd() const;
	void setKd(const float kd[3]);
	const float* getKs() const;
	void setKs(const float ks[3]);
	Picture* getMapKa() const;
	void setMapKa(Picture* mapKa);
	Picture* getMapKd() const;
	void setMapKd(Picture* mapKd);
	Picture* getMapKs() const;
	void setMapKs(Picture* mapKs);
	void CleanUpOpenglImageCache();
	virtual ObjectType GetType() const override { return OT_MATERIAL; }
private:
	// 私有成员变量
	float m_Ns;
	float m_Ni;
	float m_Tr;
	float m_Ka[3];
	float m_Kd[3];
	float m_Ks[3];
	Picture* m_MapKa;
	Picture* m_MapKd;
	Picture* m_MapKs;
};
//用于读取与临时存储材质
class MatFileReader {
public:
	// 构造函数
	MatFileReader() {}
	//析构函数
	~MatFileReader() {}

	//读取mat文件
	bool read(const std::string& filename);

	//获取材质
	Material* getMaterial(const std::string& name) const;

	std::unordered_map<std::string, Material*> m_Materials;
	std::unordered_map<std::string, Picture*> m_Picture;
};

class Model : public Object {
public:
	Model();
	Model(std::string& name);
	~Model();

	// 针对当前模型进行变换
	void move(const Vector3& offset, bool add = true);
	void scale(const Vector3& scaling, bool multiply = true);
	void rotate(const Rotation& quaternion, bool multiply = true);

	// 针对当前模型及其所有子模型进行变换
	void moveAll(const Vector3& offset, bool add = true);
	void scaleAll(const Vector3& scaling, bool multiply = true);
	void rotateAll(const Rotation& quaternion, bool multiply = true);

	// 获取或设置相对于父模型的变换属性
	Vector3 getPosition() const;
	void setPosition(const Vector3& position);
	Vector3 getScale() const;
	void setScale(const Vector3& scaling);
	Rotation getRotation() const;
	void setRotation(const Rotation& quaternion);

	// 获取或设置相对于世界坐标系的变换属性
	Vector3 getWorldPosition() const;
	void setWorldPosition(const Vector3& position);
	Vector3 getWorldScale() const;
	void setWorldScale(const Vector3& scaling);
	Rotation getWorldRotation() const;
	void setWorldRotation(const Rotation& quaternion);

	// 添加或删除子模型
	void addChildModel(Model* model);
	void removeChildModel(Model* model);

	//修改父模型
	void SetModelParent(Model* model);
	Model* GetModelParent() const{ return m_Parent; }
	//材质
	void SetMaterial(Material* material) { m_Material = material; }
	Material* GetMaterial() const{ return m_Material; }
	//网格
	Mesh* GetMesh()const;
	void SetMesh(Mesh*);
	const std::vector<Model*>& GetChildModel()const { return m_ChildModel; }
	virtual ObjectType GetType() const override { return OT_MODEL; }

	glm::mat4 GetGLTransform()const;

	virtual void Move(const Vector3&)override;
	// 获取物体位置，返回默认值
	virtual Vector GetPosition() const override;
	// 获取物体世界坐标，返回默认值
	virtual Vector GetWorldPosition() const override;
	// 获取旋转
	virtual Rotation GetRotate()const override;
	// 设定旋转
	virtual void SetRotate(const Rotation&)override;
	//旋转
	virtual void Rotate(const Rotation&)override;
	// 设置物体位置
	virtual void SetPosition(const vec::Vector&)override;
	//删除关联物体
	virtual void DeleteChildObject()override;
	// 获取缩放
	virtual Vector3 GetScale()const override;
	//设置缩放
	virtual void SetScale(const Vector3&)override;
	//缩放
	virtual void Scale(const Vector3&)override;
	//GDI渲染
	const std::vector<FACE>& GetTriFace();
private:
	// 成员变量（包括父模型指针、子模型向量、网格指针、材质指针、位置、缩放和旋转）
	Model* m_Parent;
	std::vector<Model*> m_ChildModel;
	Mesh* m_ModelMesh;
	Material* m_Material;
	Vector3 m_Position;
	Vector3 m_Scale;
	Rotation m_Rotate;
	glm::mat4 m_Transform;
	std::vector<FACE> m_GDI_TriFaceData;
};
#include<unordered_map>
//对象文件/占位子类
class Folder : public Object
{
	std::unordered_map<std::string,Object*> m_Child;
public:
	Folder() { m_Name = "新建文件夹"; }
	Folder(std::string NAME);
	~Folder();
	Folder* GetParent()const;
	template<typename T>
	T* CreateFile_创建文件(std::string Name)
	{
		T* a;
		if (Name.empty() || Name == "")
		{
			a = new T;
			AddFile_添加文件(a);
		}
		else
		{
			a = new T(Name);
			AddFile_添加文件(a);
		}
		a->SetParent(this);
		return a;
	}
	std::vector<Object*> GetFileContent()const;
	void AddFile_添加文件(Object*);
	//仅寻找当前目录
	Object* FindFile_寻找文件(const std::string& Name)const;
	void SetFileName(Object*, const std::string& NewName);
	std::vector<Object*> GetTheCurrentDirectoryFile()const;
	void ClearFolder_清空文件夹();
	void DeleteFile_删除文件(Object*);
	void DeleteIndex(Object*);
	std::vector<Model*> GetAllModleFile_找到所有模型()const;
 	virtual ObjectType GetType()const override;
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
		Vector3 Position = (3, 3, 3), Vector3 Target = (0, 0, 0), Vector3 Cameraup = Vector(0, 1, 0),
		float aspect_ratio = (4.0f / 3), float field_of_view = 45.0f, float near_clip_plane = 0.01f, float far_clip_plane = 150.0f);
	virtual void SetPosition(const vec::Vector& p)override { m_Position = p; SetDirection(m_Target - p);}
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
	virtual ObjectType GetType()const override { return OT_CAMERA; }

	Matrix4 GetView()const;
	Matrix4 GetGLMView()const;
	Matrix4 GetProjection()const;

};

class PointLight : public Object
{
public:
	PointLight();
	PointLight(const std::string&);
	~PointLight();

	ObjectType GetType() const override;

	void SetPosition(const Vector& position)override;    // 设置位置
	Vector GetPosition() const;							 // 获取位置

	void SetLightColor(const Vector& lightColor);        // 设置颜色
	const Vector& GetLightColor() const;                 // 获取颜色

	void SetIntensity(float intensity);                  // 设置强度
	float GetIntensity() const;                          // 获取强度

	void SetRange(float range);                          // 设置灯半径
	float GetRange() const;                              // 获取灯半径

	void SetSoftShadow(float softShadow);                // 设置衰减系数
	float GetSoftShadow() const;                         // 获取衰减系数

private:
	Vector m_Position;                                   // 点光源位置
	Vector m_LightColor;                                 // 点光源颜色
	float m_Intensity;                                   // 点光源强度
	float m_Range;                                       // 点光源半径
	float m_SoftShadow;                                  // 点光源衰减系数
};


