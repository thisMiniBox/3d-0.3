#pragma once
#include<vector>
#include<string>
#include <iostream>
#include <fstream>
#include <array>
#include <set>
#include<d3d11.h>
#include<sstream>
#include <map>
#include<algorithm>
#include"vector_向量.h"
#include"字符转换.h"
#include"WndData.h"
#include<glm/gtc/matrix_transform.hpp>
//#include<xlnt/xlnt.hpp>
#include<assimp/scene.h>
//声明类（方便定位）
class Object;
class Folder;
class Mesh;
class Picture;
class Material;
class Model;
class Camera;

class PointLight;
class DirectionalLight;

class Project;
class Room;
class Button;
class FixedUI;
template<typename OBJ>
class Keyframe;

class ModelShader_模型着色器;

struct _Material;
struct _OutPoint3;
struct _fm_面信息;
struct _face_完整的三角面数据;
struct Vertex;
struct _PictureData;

void OutMessage_g(const std::string&, MSGtype type = _Message);
std::vector<PointLight*>& GetAllPointLight();
RUNMODE GetRunMode_g();
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
	virtual void ToggleSelection();
	virtual void Selected();
	virtual void Unselected();
	//获取选中状态
	bool IsSelected();
	
	//资源文件的引用状态切换
	//设置引用（引用资源时调用，资源记录引用地址）
	virtual void Reference(Object*);
	virtual const std::set<Object*>* GetAllReference()const;
	//解除引用
	virtual void Dereference(Object*);
	//判断引用
	virtual bool IsReference(Object*)const;
	//从调用者解引用
	virtual void DeleteReferenceP(Object*);
	//由被引用的资源调用，删除被引用的指针
	virtual void InitReferenceP(Object*);
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
	//属性窗口内容
	virtual _ControlType SetDetaileView()const;
	//删除关联物体
	virtual void DeleteChildObject();
	//list控件内容展示
	virtual INT_PTR ListControlView(const HWND hWndList, HIMAGELIST, std::map<int, int>& index);

	virtual bool IsStatic()const;

	virtual bool SaveFile(const std::wstring path, SaveMode = SM_BINARY)const;

	virtual void SetKeyframeLoop(bool);
	virtual bool GetKeyframeLoop()const;
	//virtual void UpdateClassInfo(const ClassInfo&) = 0;
	//virtual ClassInfo* GainClassInfo() = 0;
};
template<typename OBJ>
class Keyframe :public Object
{
	std::vector<std::pair<ULONG64, OBJ>>m_keyframe;
	OBJ m_TemporaryStorage;
	bool m_loop;
public:
	Keyframe(const std::string& name = "新建关键帧");
	const std::vector<std::pair<ULONG64, OBJ>>& GetData()const;
	void SetKeyframe(ULONG64 time, OBJ key);
	void DeleteKeyframe(ULONG64 time);
	OBJ* GetKeyframe(ULONG64 time);
	void SetLoop(bool loop);
	bool IsLoop()const;
	ObjectType GetType()const override;
	void SetKeyframeLoop(bool) override;
	bool GetKeyframeLoop()const override;
	bool SaveFile(const std::wstring path, SaveMode)const override;
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
	std::map<std::string, TMaterial> materials;
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
	LONG64 a[9];
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
#include"stb_image.h"

class Mesh :public Object
{
	std::vector<Vertex>m_Data;
	std::vector<UINT>m_Indices;
	std::set<Object*>m_Reference;
public:
	Mesh() {}
	Mesh(std::string&);
	~Mesh();
	void Reference(Object*);
	const std::set<Object*>* GetAllReference()const override;
	void Dereference(Object*)override;
	bool IsReference(Object*)const override;
	void processMesh(aiMesh* mesh, const aiScene* scene);

	std::vector<vec::Vector> m_VertexPosition;  // 顶点坐标数据
	std::vector<vec::Vector> m_Normal;  // 法向量数据
	std::vector<vec::Vector2> m_TexCoords;  // 贴图坐标数据
	std::vector<FaceData_面信息> m_FaceIndices;  // 面信息


	const std::vector<Vertex>& GetVertexData();
	const std::vector<UINT>& GetIndices();
	INT_PTR ListControlView(const HWND hWndList, HIMAGELIST, std::map<int, int>& index)override;
	_ControlType SetDetaileView()const override { return CT_NAME | CT_FILEVIEW; }
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
	std::set<Object*>m_Reference;
	unsigned char* m_Data;
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

	void Reference(Object*);
	const std::set<Object*>* GetAllReference()const;
	void Dereference(Object*);
	bool IsReference(Object*)const;
	int GetHeight() { return m_Height; }
	int GetWidth() { return m_Width; }
	//释放图片
	void FreePictureData();
	_ControlType SetDetaileView()const override { return CT_NAME | CT_PICTURE; }
	virtual ObjectType GetType() const override { return OT_PICTURE; }
};
class Material :public Object
{
public:
	// 构造函数
	Material();
	Material(std::string& name);
	// 析构函数
	~Material();
	void Reference(Object*);
	const std::set<Object*>* GetAllReference()const override;
	void Dereference(Object*);
	bool IsReference(Object*)const;
	// 访问器方法，用于设置或读取私有成员变量的值
	float getNs() const;
	void setNs(float ns);
	float getNi() const;
	void setNi(float ni);
	float getTr() const;
	void setTr(float tr);

	const vec::Vector3& getKa() const;
	void setKa(const vec::Vector3& ka);

	const vec::Vector3& getKd() const;
	void setKd(const vec::Vector3& kd);

	const vec::Vector3& getKs() const;
	void setKs(const vec::Vector3& ks);

	Picture* getMapKa() const;
	void setMapKa(Picture* mapKa);
	Picture* getMapKd() const;
	void setMapKd(Picture* mapKd);
	Picture* getMapKs() const;
	void setMapKs(Picture* mapKs);
	INT_PTR ListControlView(const HWND hWndList, HIMAGELIST, std::map<int, int>& index)override;
	_ControlType SetDetaileView()const override { return CT_NAME | CT_FILEVIEW; }
	virtual ObjectType GetType() const override { return OT_MATERIAL; }
	void DeleteReferenceP(Object* obj)override;
	void InitReferenceP(Object*)override;
	void processMaterial(aiMesh* mesh, const aiScene* scene, Folder* folder, const std::string& directory);
private:
	// 私有成员变量
	float m_Ns;
	float m_Ni;
	float m_Tr;
	vec::Vector3 m_Ka;
	vec::Vector3 m_Kd;
	vec::Vector3 m_Ks;
	Picture* m_MapKa;
	Picture* m_MapKd;
	Picture* m_MapKs;
	std::set<Object*>m_Reference;
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

	std::map<std::string, Material*> m_Materials;
	std::map<std::string, Picture*> m_Picture;
};

class Model : public Object 
{
public:
	Model();
	Model(std::string& name);
	~Model();

	void SetKeyframe(Keyframe<TransFrame>*);
	bool SetKeyframe(ULONG64 time);
	static Keyframe<TransForm>* CreateKryframe();

	//void SetMode(ModelMode);
	//ModelMode GetMode()const;
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
	//移除引用
	void RemoveChildModel(Model* model);
	//删除模型
	void DeleteChildModel(Model* model);

	//修改父模型
	void SetModelParent(Model* model);
	Model* GetModelParent() const{ return m_Parent; }
	//材质
	void SetMaterial(Material* material);
	Material* GetMaterial() const{ return m_Material; }
	//网格
	Mesh* GetMesh()const;
	void SetMesh(Mesh*);
	const std::vector<Model*>& GetChildModel()const { return m_ChildModel; }
	virtual ObjectType GetType() const override { return OT_MODEL; }

	glm::mat4 GetGLTransform()const;
	void DeleteReferenceP(Object* obj)override;
	void InitReferenceP(Object* obj)override;
	inline void updateTransform();
	// 切换选中状态
	void ToggleSelection() override;
	// 物体被选中时调用
	void Selected() override;
	// 物体取消选中时调用
	void Unselected() override;
	// 移动物体并更新变换矩阵
	void Move(const Vector3&) override;
	// 获取物体位置，返回默认值
	// 如果子类未重写该函数，则返回默认值
	Vector GetPosition() const override;
	// 获取物体世界坐标，返回默认值
	// 如果子类未重写该函数，则返回默认值
	Vector GetWorldPosition() const override;
	// 获取物体旋转角度
	// 如果子类未重写该函数，则返回默认值
	Rotation GetRotate() const override;
	// 设定物体的旋转角度
	void SetRotate(const Rotation&) override;
	// 对物体进行旋转操作并更新变换矩阵
	void Rotate(const Rotation&) override;
	// 设置物体的位置并更新变换矩阵
	void SetPosition(const Vector&) override;
	// 删除子物体
	void DeleteChildObject() override;
	// 获取物体缩放比例，返回默认值
	// 如果子类未重写该函数，则返回默认值
	Vector3 GetScale() const override;
	// 设置物体缩放比例并更新变换矩阵
	void SetScale(const Vector3&) override;
	// 对物体进行缩放操作并更新变换矩阵
	void Scale(const Vector3&) override;
	//GDI渲染
	const std::vector<FACE>& GetTriFace();
	_ControlType SetDetaileView()const override { return CT_NAME | CT_TRANSFORM | CT_FILEVIEW; }

	INT_PTR ListControlView(const HWND hWndList, HIMAGELIST, std::map<int, int>& index)override;

	bool IsStatic()const override;
	void SetKeyframeLoop(bool) override;
	bool GetKeyframeLoop()const override;
	const std::vector<std::pair<ULONG64, TransForm>>* GetKeyframeData()const;
	glm::mat4 GetTransform(ULONG64 time);

	//bool processNode(aiNode* node, const aiScene* scene);
private:
	TransForm GetTransForm()const;
	ModelMode m_Mode;
	Model* m_Parent;
	std::vector<Model*> m_ChildModel;
	Mesh* m_ModelMesh;
	Material* m_Material;
	Vector3 m_Position;
	Vector3 m_Scale;
	Rotation m_Rotate;
	Keyframe<TransForm>* m_keyframe;
	int m_ShaderID;
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
	std::vector<PointLight*> GetAllPointLightFile_找到所有光源文件()const;
	_ControlType SetDetaileView()const override { return CT_NAME | CT_FILEVIEW; }
	INT_PTR ListControlView(const HWND hWndList, HIMAGELIST, std::map<int, int>& index)override;
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
	void SetFieldOfView(float FieldOfView);
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
	ObjectType GetType()const override { return OT_CAMERA; }
	bool IsStatic()const override;

	Matrix4 GetView()const;
	Matrix4 GetGLMView()const;
	Matrix4 GetProjection()const;
	_ControlType SetDetaileView()const override { return CT_NAME | CT_TRANSFORM; }
	Rotation GetRotate()const override;
	void SetRotate(const Rotation&)override;
};

class PointLight : public Object
{
public:
	PointLight(const std::string& name = "点光源", const vec::Vector& pos = vec::Vector3(0.f), const vec::Vector& col = vec::Vector(1.f), float intens = 100, float rng = 100)
		: m_Position(pos), m_Color(col), m_Intensity(intens), m_Range(rng)
	{
		m_Name = name;
	}
	~PointLight();

	ObjectType GetType() const override;

	void SetPosition(const Vector& position)override;    // 设置位置
	Vector GetPosition() const override;				 // 获取位置
	void Move(const Vector&)override;

	void SetLightColor(const Vector& lightColor);        // 设置颜色
	const Vector& GetLightColor() const;                 // 获取颜色

	//设置颜色
	void SetScale(const Vector& position)override;
	//获取颜色
	Vector GetScale() const override;
	//获取光照信息
	Rotation GetRotate()const override;
	//设置光照信息
	void SetRotate(const Rotation&)override;

	void SetIntensity(float intensity);                  // 设置强度
	float GetIntensity() const;                          // 获取强度

	void SetRange(float range);                          // 设置灯半径
	float GetRange() const;                              // 获取灯半径

	_ControlType SetDetaileView()const override { return CT_NAME | CT_TRANSFORM | CT_FILEVIEW; }
private:
	Vector m_Position;                                   // 点光源位置
	Vector m_Color;										 // 点光源颜色
	float m_Intensity;                                   // 点光源强度
	float m_Range;                                       // 点光照半径
};

class Room :public Object
{
public:
	Room(const std::string& name = "新键场景");
	ObjectType GetType()const override;
private:
	Folder m_RoomContent;

};
class Project :public Object
{
private:
	std::string m_Notes;
	std::vector<Room*>m_Sence;
	Folder m_ResourceFolder;
public:
	Project(std::string name);
	~Project();
	ObjectType GetType()const override;
	Room* AddRoom(std::string roomName);
	Room* GetRoom(UINT ID)const;
	void DeleteRoom(Room*);
	void DeleteRoom(UINT id);
	bool SaveProject(const std::string& path)const;
	bool LoadProject(const std::string& path);
};
class DirectionalLight : public Object {
public:
	DirectionalLight(const std::string& name="平行光", const Vector& dir = vec::Vector3(-1.f), const Vector& col = vec::Vector3(1.f), float intens = 100.f)
		: m_Direction(dir), m_Color(col), m_Intensity(intens) 
	{
		m_Name = name;
	}
	~DirectionalLight();

	ObjectType GetType() const override;

	void SetDirection(const Vector& direction);          // 设置光源方向
	Vector GetDirection() const;                         // 获取光源方向

	void SetLightColor(const Vector& lightColor);         // 设置光源颜色
	const Vector& GetLightColor() const;                  // 获取光源颜色

	void SetIntensity(float intensity);                   // 设置光源强度
	float GetIntensity() const;                           // 获取光源强度

	_ControlType SetDetaileView() const override { return CT_NAME | CT_TRANSFORM | CT_FILEVIEW; }

private:
	Vector m_Direction;                                  // 光源方向
	Vector m_Color;                                      // 光源颜色
	float m_Intensity;                                   // 光源强度
};