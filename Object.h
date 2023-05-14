#pragma once
#include<vector>
#include<string>
#include <iostream>
#include <fstream>
#include <array>
#include<d3d11.h>
#include<sstream>
#include <unordered_map>
#include"vector_����.h"
#include"�ַ�ת��.h"
#include<glm/gtc/matrix_transform.hpp>
//�����ࣨ���㶨λ��
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
	OT_FOLDER,				//�ļ���
	OT_MODEL,				//ģ��
	OT_CAMERA,				//�����
	OT_MESH,				//����
	OT_PICTURE,				//ͼƬ
	OT_MATERIAL,			//����
	OT_POINTLIGHT,			//���Դ
	OT_PARALLELLIGHT,		//ƽ�й��Դ
	OT_KEYFRAME,			//�ؼ�֡
};
using namespace vec;
std::wstring ObjectTypeToWstr(ObjectType);
std::string ObjectTypeTostr(ObjectType);
ObjectType  StrToObjectType(const std::string&);
ObjectType  WStrToObjectType(const std::wstring&);
// Object �࣬��ʾ�����е��������
class Object
{
protected:
	bool m_Selected;
	Folder* m_ParentFolder;
	std::string m_Name;  // ��������
public:
	// Ĭ�Ϲ��캯��
	Object();
	// ���캯������ָ����������
	Object(std::string nam);
	// ��������
	virtual ~Object();
	
	void SetParent(Folder*); 
	Folder* GetParent()const;
	// ��ȡ��������
	const std::string GetName() const;
	// ������������
	void SetName(std::string NewName);
	//�л�ѡ��״̬
	void ToggleSelection();
	void Selected();
	void Unselected();
	//��ȡѡ��״̬
	bool IsSelected();
	// ��ȡ�������ͣ����麯��
	virtual ObjectType GetType()const = 0;
	// ��ȡ����λ�ã�����Ĭ��ֵ
	virtual Vector GetPosition() const;
	// ��ȡ����
	virtual Vector GetScale() const;
	// ��ȡ�����������꣬����Ĭ��ֵ
	virtual Vector GetWorldPosition() const;
	// ��ȡ��ת
	virtual Rotation GetRotate()const;
	// �趨��ת
	virtual void SetRotate(const Rotation&);
	//��ת
	virtual void Rotate(const Rotation&);
	// ��������λ�ã���ʵ��
	virtual void SetPosition(const vec::Vector&);
	// ��������
	virtual void SetScale(const Vector3&);
	//����
	virtual void Scale(const Vector3&);
	// �ƶ����壬��ʵ��
	virtual void Move(const Vector3&);
	//ɾ����������
	virtual void DeleteChildObject();

	//virtual void UpdateClassInfo(const ClassInfo&) = 0;
	//virtual ClassInfo* GainClassInfo() = 0;
};
//������Ϣ�ṹ��
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

//����Objģ�ͼ��ز���
class ModelShader_ģ����ɫ�� {
public:
	std::string file;
	ModelShader_ģ����ɫ��() {}
	~ModelShader_ģ����ɫ��() {}
	bool read(const std::string& filename);

	TMaterial get_materials(std::string name) const {
		return materials.at(name);
	}

private:
	std::unordered_map<std::string, TMaterial> materials;
};
//GDI���������
typedef struct _OutPoint3
{
	POINT point[3];
	COLORREF color;
	float distance;
	_OutPoint3():point(),distance(0),color(RGB(0,0,0)){}
}Outface;
//���ȡ��Ϣ
typedef struct _fm_����Ϣ
{
	size_t a[9];
	_fm_����Ϣ() :a() {}
}FaceData_����Ϣ;
//����������
typedef struct _face_����������������
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
	_face_����������������() :color(RGB(155, 155, 155)) {}
	_face_����������������(Vector3 A, Vector3 B, Vector3 C,
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
			// ��������������Ч���򷵻�������
			return Vector3(0, 0, 0);
		}
	}
}FACE;
using namespace DirectX;
struct Vertex {
	vec::Vector position;    // ��������
	vec::Vector normal;      // ���㷨��
	vec::Vector2 texCoord;   // ��������
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
	std::vector<vec::Vector> m_VertexPosition;  // ������������
	std::vector<vec::Vector> m_Normal;  // ����������
	std::vector<vec::Vector2> m_TexCoords;  // ��ͼ��������
	std::vector<FaceData_����Ϣ> m_FaceIndices;  // ����Ϣ
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
	//����ͼƬ
	PictureData LoadPicture(const std::string& path);
	PictureData GetPicture()const;
	//����ͼƬ�����뵽OpenGl
	unsigned int loadTexture();
	unsigned int GetID() { return m_ID; }

	int GetHeight() { return m_Height; }
	int GetWidth() { return m_Width; }
	//�ͷ�ͼƬ
	void FreePictureData();
	void FreeOpenGL();
	virtual ObjectType GetType() const override { return OT_PICTURE; }
};
class Material :public Object
{
public:
	// ���캯��
	Material();
	Material(std::string& name) { m_Name = name; }
	// ��������
	~Material();

	// �������������������û��ȡ˽�г�Ա������ֵ
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
	// ˽�г�Ա����
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
//���ڶ�ȡ����ʱ�洢����
class MatFileReader {
public:
	// ���캯��
	MatFileReader() {}
	//��������
	~MatFileReader() {}

	//��ȡmat�ļ�
	bool read(const std::string& filename);

	//��ȡ����
	Material* getMaterial(const std::string& name) const;

	std::unordered_map<std::string, Material*> m_Materials;
	std::unordered_map<std::string, Picture*> m_Picture;
};

class Model : public Object {
public:
	Model();
	Model(std::string& name);
	~Model();

	// ��Ե�ǰģ�ͽ��б任
	void move(const Vector3& offset, bool add = true);
	void scale(const Vector3& scaling, bool multiply = true);
	void rotate(const Rotation& quaternion, bool multiply = true);

	// ��Ե�ǰģ�ͼ���������ģ�ͽ��б任
	void moveAll(const Vector3& offset, bool add = true);
	void scaleAll(const Vector3& scaling, bool multiply = true);
	void rotateAll(const Rotation& quaternion, bool multiply = true);

	// ��ȡ����������ڸ�ģ�͵ı任����
	Vector3 getPosition() const;
	void setPosition(const Vector3& position);
	Vector3 getScale() const;
	void setScale(const Vector3& scaling);
	Rotation getRotation() const;
	void setRotation(const Rotation& quaternion);

	// ��ȡ�������������������ϵ�ı任����
	Vector3 getWorldPosition() const;
	void setWorldPosition(const Vector3& position);
	Vector3 getWorldScale() const;
	void setWorldScale(const Vector3& scaling);
	Rotation getWorldRotation() const;
	void setWorldRotation(const Rotation& quaternion);

	// ��ӻ�ɾ����ģ��
	void addChildModel(Model* model);
	void removeChildModel(Model* model);

	//�޸ĸ�ģ��
	void SetModelParent(Model* model);
	Model* GetModelParent() const{ return m_Parent; }
	//����
	void SetMaterial(Material* material) { m_Material = material; }
	Material* GetMaterial() const{ return m_Material; }
	//����
	Mesh* GetMesh()const;
	void SetMesh(Mesh*);
	const std::vector<Model*>& GetChildModel()const { return m_ChildModel; }
	virtual ObjectType GetType() const override { return OT_MODEL; }

	glm::mat4 GetGLTransform()const;

	virtual void Move(const Vector3&)override;
	// ��ȡ����λ�ã�����Ĭ��ֵ
	virtual Vector GetPosition() const override;
	// ��ȡ�����������꣬����Ĭ��ֵ
	virtual Vector GetWorldPosition() const override;
	// ��ȡ��ת
	virtual Rotation GetRotate()const override;
	// �趨��ת
	virtual void SetRotate(const Rotation&)override;
	//��ת
	virtual void Rotate(const Rotation&)override;
	// ��������λ��
	virtual void SetPosition(const vec::Vector&)override;
	//ɾ����������
	virtual void DeleteChildObject()override;
	// ��ȡ����
	virtual Vector3 GetScale()const override;
	//��������
	virtual void SetScale(const Vector3&)override;
	//����
	virtual void Scale(const Vector3&)override;
	//GDI��Ⱦ
	const std::vector<FACE>& GetTriFace();
private:
	// ��Ա������������ģ��ָ�롢��ģ������������ָ�롢����ָ�롢λ�á����ź���ת��
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
//�����ļ�/ռλ����
class Folder : public Object
{
	std::unordered_map<std::string,Object*> m_Child;
public:
	Folder() { m_Name = "�½��ļ���"; }
	Folder(std::string NAME);
	~Folder();
	Folder* GetParent()const;
	template<typename T>
	T* CreateFile_�����ļ�(std::string Name)
	{
		T* a;
		if (Name.empty() || Name == "")
		{
			a = new T;
			AddFile_����ļ�(a);
		}
		else
		{
			a = new T(Name);
			AddFile_����ļ�(a);
		}
		a->SetParent(this);
		return a;
	}
	std::vector<Object*> GetFileContent()const;
	void AddFile_����ļ�(Object*);
	//��Ѱ�ҵ�ǰĿ¼
	Object* FindFile_Ѱ���ļ�(const std::string& Name)const;
	void SetFileName(Object*, const std::string& NewName);
	std::vector<Object*> GetTheCurrentDirectoryFile()const;
	void ClearFolder_����ļ���();
	void DeleteFile_ɾ���ļ�(Object*);
	void DeleteIndex(Object*);
	std::vector<Model*> GetAllModleFile_�ҵ�����ģ��()const;
 	virtual ObjectType GetType()const override;
	virtual Vector GetPosition()const override { return Vector(0, 0, 0); }
	virtual void DeleteChildObject()override;
};
//�������������
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

	void SetPosition(const Vector& position)override;    // ����λ��
	Vector GetPosition() const;							 // ��ȡλ��

	void SetLightColor(const Vector& lightColor);        // ������ɫ
	const Vector& GetLightColor() const;                 // ��ȡ��ɫ

	void SetIntensity(float intensity);                  // ����ǿ��
	float GetIntensity() const;                          // ��ȡǿ��

	void SetRange(float range);                          // ���õư뾶
	float GetRange() const;                              // ��ȡ�ư뾶

	void SetSoftShadow(float softShadow);                // ����˥��ϵ��
	float GetSoftShadow() const;                         // ��ȡ˥��ϵ��

private:
	Vector m_Position;                                   // ���Դλ��
	Vector m_LightColor;                                 // ���Դ��ɫ
	float m_Intensity;                                   // ���Դǿ��
	float m_Range;                                       // ���Դ�뾶
	float m_SoftShadow;                                  // ���Դ˥��ϵ��
};


