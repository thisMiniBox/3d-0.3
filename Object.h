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
enum ObjectType
{
	OT_FOLDER,
	OT_MODEL,
	OT_CAMERA
};
using namespace vec;

// Object �࣬��ʾ�����е��������
class Object
{
protected:
	std::string m_Name;  // ��������
public:
	// Ĭ�Ϲ��캯��
	Object();
	// ���캯������ָ����������
	Object(std::string nam);
	// ��������
	virtual ~Object();
	// ��ȡ��������
	const std::string GetName() const;
	// ������������
	void SetName(std::string NewName);
	// ��ȡ�������ͣ����麯��
	virtual int GetType() = 0;
	// ��ȡ����λ�ã�����Ĭ��ֵ
	virtual Vector GetPosition() const;
	// ��ȡ�����������꣬����Ĭ��ֵ
	virtual Vector GetWorldPosition() const;
	// ��ȡ��ת
	virtual Rotation GetRotate()const;
	// �趨��ת
	virtual void SetRotate(const Rotation&);
	// ��������λ�ã���ʵ��
	virtual void SetPosition(vec::Vector);
	// �ƶ����壬��ʵ��
	virtual void Move(const Vector3&);
	//ɾ����������
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
}Material;
//����Objģ�ͼ��ز���
class ModelShader_ģ����ɫ�� {
public:
	std::string file;
	ModelShader_ģ����ɫ��() {}
	~ModelShader_ģ����ɫ��() {}
	bool read(const std::string& filename);

	Material get_materials(std::string name) const {
		return materials.at(name);
	}

private:
	std::unordered_map<std::string, Material> materials;
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
	//Ĭ�ϳɹ���Ϣ
	_Succese = 0xffffff00,
	//����
	_UnknownFormat = 0b0001,
	_DataError = 0b0010,
	_FailToOpenFile = 0b0100,
	_FailedToCreateFile = 0b1000,
	//ģ�ͼ���ϸ��
	_SuccessfullyLoadedVertex = 0x0100,
	_SuccessfullyLoadedMaterialFile = 0x0200,
	_SuccessfullyLoadedMaterialMaps = 0x0400,

};
#include"stb_image.h"
class Model : public Object
{
public:
	// Ĭ�Ϲ��캯��
	Model();
	// ���캯������ָ����������
	Model(std::string NAME);
	// ��������
	~Model();

	// ������ģ�ͣ�������ģ��ָ��
	Model* CreateChildModel(std::string Name);

	// ��ȡ������������
	const std::vector<vec::Vector>& GetVertexData() const { return vertex_������������; }
	// ��ȡ����������
	const std::vector<vec::Vector>& GetNormalData() const { return normal_����������; }
	// ��ȡ��ͼ��������
	const std::vector<vec::Vector2>& GetTexCoordData() const { return texCoords_��ͼ��������; }
	// ��ȡ������Ϣ
	const Material& GetMaterial() const { return mtl; }
	// ��ȡģ���ļ�·��
	std::string GetFileAddress()const { return fileAddress; }
	// ��ȡ����Ϣ
	const std::vector<FaceData_����Ϣ>& GetFaceData() const { return face_��Ķ�ȡλ��; }
	// ��ȡ��ģ��ָ���б�
	const std::vector<Model*>& GetChildModel()const { return child_��ģ��ָ��; }
	// ��ȡ��ģ��ָ��
	const Model* GetParentModel()const { return parent_��ģ��ָ��; }

	// ����ģ���ļ�
	int loadModelFile_����ģ���ļ�(const std::wstring& filename);
	// ��ȡ��������Ƭ����
	const std::vector<FACE>& GetTriFace();
	// ��ȡ��������
	const std::vector<Vertex>& GetVertices();

	// �������ű���
	void SetScale(Vector s) { m_Scale = s; }
	// ������������Ƭ����
	void UpdateFACEdata() { TriFace.clear(); }

	// ��ȡ���ű���
	Vector GetScale()const { return m_Scale; }

	// ��ȡ����λ��
	virtual Vector GetPosition()const override { return m_Position; }
	// ��ȡ������������
	virtual Vector GetWorldPosition() const override;
	// ��ȡ��ת
	virtual Rotation GetRotate()const override;
	//�趨��ת��Ԫ��
	virtual void SetRotate(const Rotation&) override;
	// ��������λ��
	virtual void SetPosition(vec::Vector p)override;
	// �ƶ�����
	virtual void Move(const Vector3& v) override { m_Position += v; if (!TriFace.empty())TriFace.clear(); }
	// ��ȡ��������
	virtual int GetType()override;

private:
	void DistributeDataToChildModels();  // ��ģ�����ݷ������ģ��
	int loadobj(const std::wstring& path, FILE* file);  // ���� obj �ļ�
	std::vector<vec::Vector> vertex_������������;  // ������������
	std::vector<vec::Vector> normal_����������;  // ����������
	std::vector<vec::Vector2> texCoords_��ͼ��������;  // ��ͼ��������
	std::vector<FACE> TriFace;  // ��������Ƭ����
	std::vector<Vertex> ModelVertex;  // ��������
	Material mtl;  // ������Ϣ
	std::vector<FaceData_����Ϣ> face_��Ķ�ȡλ��;  // ����Ϣ
	Model* parent_��ģ��ָ��;  // ��ģ��ָ��
	ModelShader_ģ����ɫ��* mtl_����;  // ģ����ɫ��
	std::vector<Model*>child_��ģ��ָ��;  // ��ģ��ָ���б�
	std::string fileAddress;  // ģ���ļ�·��

	Vector m_Position;  // ����λ��
	Vector m_Scale;  // ���ű���
	Rotation m_Rotate;//��ת
};
#include<unordered_map>
//�����ļ�/ռλ����
class Folder : public Object
{
	std::unordered_map<std::string,Object*> m_Child;
	Folder* m_Parent;
public:
	Folder() { m_Name = "�½��ļ���"; }
	Folder(std::string NAME);
	Object* CreateFile_�����ļ�(std::string Name, int type = OT_FOLDER);
	void AddFile_����ļ�(Object*);
	//��Ѱ�ҵ�ǰĿ¼
	Object* FindFile_Ѱ���ļ�(const std::string& Name)const;
	void SetFileName(Object*, const std::string& NewName);
	std::vector<Object*> GetTheCurrentDirectoryFile()const;
	void ClearFolder_����ļ���();
	void DeleteFile_ɾ���ļ�(Object*);
	std::vector<Model*> GetAllModleFile_�ҵ�����ģ��()const;
	int GetType();
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
