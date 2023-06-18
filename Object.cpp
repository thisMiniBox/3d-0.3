#include "Object.h"
inline FaceData_面信息 faceData(std::string cin) {
	FaceData_面信息 result;
	std::stringstream ss(cin);
	std::string token;
	int index = 0;
	while (std::getline(ss, token, ' ')) {
		std::stringstream ss2(token);
		std::string num;
		int sub_index = 0;
		while (std::getline(ss2, num, '/')) {
			result.a[index * 3 + sub_index] = std::stoi(num);
			sub_index++;
		}
		index++;
	}
	return result;
}
std::wstring ObjectTypeToWstr(ObjectType OT)
{
	switch (OT)
	{
	case OT_FOLDER:
		return L"文件夹";
		break;
	case OT_MODEL:
		return L"模型";
		break;
	case OT_CAMERA:
		return L"摄像机";
		break;
	case OT_MESH:
		return L"网格";
		break;
	case OT_PICTURE:
		return L"图片";
		break;
	case OT_MATERIAL:
		return L"材质";
		break;
	case OT_POINT_LIGHT:
		return L"点光源";
		break;
	case OT_DIRECTIONAL_LIGHT:
		return L"平行光源";
		break;
	case OT_KEYFRAME:
		return L"关键帧";
		break;
	default:
		return L"未知";
		break;
	}
}
std::string ObjectTypeTostr(ObjectType OT)
{
	switch (OT)
	{
	case OT_FOLDER:
		return "文件夹";
		break;
	case OT_MODEL:
		return "模型";
		break;
	case OT_CAMERA:
		return "摄像机";
		break;
	case OT_MESH:
		return "网格";
		break;
	case OT_PICTURE:
		return "图片";
		break;
	case OT_MATERIAL:
		return "材质";
		break;
	case OT_POINT_LIGHT:
		return "点光源";
		break;
	case OT_DIRECTIONAL_LIGHT:
		return "平行光源";
		break;
	case OT_KEYFRAME:
		return "关键帧";
		break;
	default:
		return "未知";
		break;
	}
}
ObjectType StrToObjectType(const std::string& str)
{
	if (str == "文件夹" || str == "Folder" || str == "folder") {
		return OT_FOLDER;
	}
	else if (str == "模型" || str == "Model" || str == "model") {
		return OT_MODEL;
	}
	else if (str == "摄像机" || str == "Camera" || str == "camera") {
		return OT_CAMERA;
	}
	else if (str == "网格" || str == "Mesh"||str == "mesh") {
		return OT_MESH;
	}
	else if (str == "图片" || str == "Picture"||str == "picture") {
		return OT_PICTURE;
	}
	else if (str == "材质" || str == "Material" || str == "material") {
		return OT_MATERIAL;
	}
	else if (str == "点光源" || str == "PointLight" || str == "pointlight") {
		return OT_POINT_LIGHT;
	}
	else if (str == "平行光源" || str == "ParallelLight" || str == "parallellight") {
		return OT_DIRECTIONAL_LIGHT;
	}
	else if (str == "关键帧" || str == "Keyframe" || str == "keyframe") {
		return OT_KEYFRAME;
	}
	else {
		return OT_UNKNOWN;
	}
}

ObjectType WStrToObjectType(const std::wstring& str)
{
	if (str == L"文件夹" || str == L"Folder" || str == L"folder") {
		return OT_FOLDER;
	}
	else if (str == L"模型" || str == L"Model" || str == L"model") {
		return OT_MODEL;
	}
	else if (str == L"摄像机" || str == L"Camera" || str == L"camera") {
		return OT_CAMERA;
	}
	else if (str == L"网格" || str == L"Mesh" || str == L"mesh") {
		return OT_MESH;
	}
	else if (str == L"图片" || str == L"Picture" || str == L"picture") {
		return OT_PICTURE;
	}
	else if (str == L"材质" || str == L"Material" || str == L"material") {
		return OT_MATERIAL;
	}
	else if (str == L"点光源" || str == L"PointLight" || str == L"pointlight") {
		return OT_POINT_LIGHT;
	}
	else if (str == L"平行光源" || str == L"ParallelLight" || str == L"parallellight") {
		return OT_DIRECTIONAL_LIGHT;
	}
	else if (str == L"关键帧" || str == L"Keyframe" || str == L"keyframe") {
		return OT_KEYFRAME;
	}
	else {
		return OT_UNKNOWN;
	}
}
bool ModelShader_模型着色器::read(const std::string& filename) 
{
	std::ifstream file(filename);
	if (!file.is_open()) {
		std::cerr << "Failed to open file: " << filename << std::endl;
		return false;
	}

	TMaterial current_material;
	std::string line;
	while (std::getline(file, line)) {
		std::istringstream iss(line);
		std::string token;
		iss >> token;

		if (token == "newmtl") {
			if (!current_material.name.empty()) {
				materials[current_material.name] = current_material;
			}
			iss >> current_material.name;
		}
		else if (token == "Ka") {
			iss >> current_material.Ka[0] >> current_material.Ka[1] >> current_material.Ka[2];
		}
		else if (token == "Kd") {
			iss >> current_material.Kd[0] >> current_material.Kd[1] >> current_material.Kd[2];
		}
		else if (token == "Ks") {
			iss >> current_material.Ks[0] >> current_material.Ks[1] >> current_material.Ks[2];
		}
		else if (token == "Ns") {
			iss >> current_material.Ns;
		}
		else if (token == "Ni") {
			iss >> current_material.Ni;
		}
		else if (token == "d") {
			iss >> current_material.Tr;
		}
		else if (token == "Tr") {
			iss >> current_material.Tr;
		}
		else if (token == "map_Kd") {
			iss >> current_material.map_Kd;
		}
		else if (token == "map_Ka") {
			iss >> current_material.map_Ka;
		}
		else if (token == "map_Ks") {
			iss >> current_material.map_Ks;
		}
	}
	materials[current_material.name] = current_material;
	return true;
}
Folder::Folder(std::string NAME)
{
	m_Name = NAME;
}
Object* Folder::FindFile_寻找文件(const std::string& Name) const {
	auto it = m_Child.find(Name.c_str());
	if (it != m_Child.end()) 
	{
		return it->second;
	}
	return nullptr;
}

void Folder::SetFileName(Object* obj, const std::string& NewName)
{
	if (!obj||NewName.empty()||NewName[0]=='\0')
	{ 
		return;
	}
	for (auto& f : m_Child)
	{
		if (f.second == obj)
		{
			obj->SetName(NewName);
			m_Child.erase(f.first);
			m_Child.insert(std::make_pair(NewName.c_str(), obj));
			return;
		}
	}
}
INT_PTR Object::ListControlView(const HWND hWndList, HIMAGELIST, std::map<int,int>& index)
{
	return false;
}
bool Object::IsStatic()const
{
	return true;
}
std::vector<Object*> Folder::GetTheCurrentDirectoryFile()const
{
	std::vector<Object*> out;
	for (const auto& kv : m_Child)
		if (kv.second)
			out.push_back(kv.second);
	return out;
}
void Folder::DeleteChildObject()
{
	ClearFolder_清空文件夹();
}
void Folder::ClearFolder_清空文件夹()
{
	for (const auto& i : m_Child)
	{
		if(i.second)
		{
			delete i.second;
		}
	}
	m_Child.clear();
}
void Folder::AddFile_添加文件(Object* obj)
{
	if (!obj)return;
	obj->SetParent(this);
	std::string name = obj->GetName();
	if (m_Child.find(name) == m_Child.end())
	{
		m_Child.insert(std::make_pair(name, obj));
		return;
	}

	// 名称已经存在，添加数字序号
	int i = 1;
	do {
		name = obj->GetName() + "_" + std::to_string(i);
		i++;
	} while (m_Child.find(name) != m_Child.end());

	obj->SetName(name);
	m_Child.insert(std::make_pair(name, obj));
}
Folder* Folder::GetParent()const
{
	return m_ParentFolder;
}
std::vector<Object*> Folder::GetFileContent()const
{
	std::vector<Object*> out;
	for (const auto& c : m_Child)
		if (c.second)
			out.push_back(c.second);
	return out;
}
//template<typename T>
//T* Folder::CreateFile_创建文件(std::string Name)
//{
//	T* a;
//	if (Name.empty() || Name == "")
//	{
//		a = new T;
//		AddFile_添加文件(a);
//	}
//	else
//	{
//		a = new T(Name);
//		AddFile_添加文件(a);
//	}
//	a->SetParent(this);
//	return a;
//}
void Object::SetParent(Folder* p)
{
	m_ParentFolder = p;
}
Folder* Object::GetParent()const
{
	return m_ParentFolder;
}
ObjectType Folder::GetType()const
{
	return OT_FOLDER;
}
void Folder::DeleteIndex(Object* child)
{
	if (!child)
		return;
	for (auto& i : m_Child)
	{
		if (i.second == child)
		{
			m_Child.erase(i.first);
			return;
		}
	}
}
void Folder::DeleteFile_删除文件(Object* obj)
{
	if (!obj)
	{ // 检查 obj 是否为空指针
		return;
	}
	for (auto& i : m_Child)
	{
		if (i.second == obj)
		{
			delete i.second;
			m_Child.erase(i.first);
			obj = nullptr;
			return;
		}
		if (i.second->GetType() == ObjectType::OT_FOLDER)
		{
			Folder* folder = dynamic_cast<Folder*>(i.second);
			folder->DeleteFile_删除文件(obj);
			if (!obj)return;
		}
	}
}
// 默认构造函数
Object::Object()
	: m_Name("新建物品"), m_Selected(false), m_ParentFolder(nullptr)
{
}

// 构造函数，可指定物体名称
Object::Object(std::string nam)
	: m_Name(nam), m_Selected(false), m_ParentFolder(nullptr)
{
}

// 析构函数
Object::~Object()
{
}

// 获取物体名称
const std::string Object::GetName() const
{
	return m_Name;
}

// 设置物体名称
void Object::SetName(std::string NewName)
{
	m_Name = NewName;
}

// 获取物体位置，返回默认值
Vector Object::GetPosition() const
{
	return Vector(0, 0, 0);
}

Vector Object::GetScale() const
{
	return Vector(1, 1, 1);
}
void Object::SetScale(const Vector3&)
{
}
void Object::Scale(const Vector3&)
{
}
Vector3 Model::GetScale()const
{
	return getScale();
}
void Model::SetScale(const Vector3& scale)
{
	setScale(scale);
}
void Model::Scale(const Vector3& sca)
{
	scale(sca);
}
// 获取物体世界坐标，返回默认值
Vector Object::GetWorldPosition() const
{
	return Vector(0, 0, 0);
}
Rotation Object::GetRotate()const
{
	return Rotation();
}
void Object::SetPosition(const vec::Vector&)
{
}
void Object::SetRotate(const Rotation&)
{
}
void Object::Rotate(const Rotation&)
{
}
// 移动物体，空实现
void Object::Move(const Vector3&)
{
}

void Object::DeleteChildObject()
{
}
using namespace vec;
Camera::Camera() {
	m_Name = "摄像头";
	m_Position = vec::Vector3(0, 0, 3);
	m_Target = vec::Vector(0, 0, 0);
	m_Direction = (m_Target-m_Position).Normalize();
	m_Up = vec::Vector3(0, 1, 0);
	m_CameraUp = vec::Vector(0, 1, 0);
	m_Far = 150.0f;
	m_Near = 0.01f;
	m_Field = 45.0f;
	m_Ratio = (float)4 / 3;
}
Camera::Camera(std::string name,
	Vector3 Position, Vector3 Target, Vector3 Cameraup,
	float aspect_ratio, float field_of_view, float near_clip_plane, float far_clip_plane)
	:m_Position(Position), m_Target(Target), m_CameraUp(Cameraup), m_Ratio(aspect_ratio),
	m_Field(field_of_view),m_Near(near_clip_plane),m_Far(far_clip_plane)
{
	m_Name = name;
	SetDirection(m_Target - m_Position);
}
void Camera::SetDirection(const Vector& dir)
{
	m_Direction = dir.Normalize();
	m_Target = m_Position + m_Direction;
	m_Right = -m_Direction.Cross(m_CameraUp).Normalize();
	m_Up = m_Direction.Cross(m_Right).Normalize();
}

Matrix4 Camera::GetView()const
{
	return Matrix4(
		m_Right.x, m_Up.x, -m_Direction.x, 0,
		m_Right.y, m_Up.y, -m_Direction.y, 0,
		m_Right.z, m_Up.z, -m_Direction.z, 0,
		-Vector::dot(m_Right, m_Position), -Vector::dot(m_Up, m_Position), Vector::dot(m_Direction, m_Position), 1);
}
bool Camera::IsStatic()const
{
	return false;
}
void Camera::Move(const Vector3& v1)
{
	m_Position += v1;
	m_Target += v1;
}
Matrix4 Camera::GetProjection() const
{
	// Calculate and return the perspective projection matrix
	float tan_fov = tan(vec::DegToRad(m_Field) / 2.0f);
	float range = m_Far - m_Near;

	return Matrix4(
		1.0f / (tan_fov * m_Ratio), 0, 0, 0,
		0, 1.0f / tan_fov, 0, 0,
		0, 0, -(m_Far + m_Near) / range, -1,
		0, 0, (-2.0f * m_Far * m_Near) / range, 0
	) * Matrix4(
		Vector4(1.0f, 0.0f, 0.0f, 0.0f),
		Vector4(0.0f, 1.0f, 0.0f, 0.0f),
		Vector4(0.0f, 0.0f, -1.0f, -1.0f),
		Vector4(0.0f, 0.0f, -2.0f * m_Far * m_Near / range, 0.0f)
	);
}
Rotation Camera::GetRotate()const
{
	
	return Rotation(vec::DegToRad(m_Field), m_Direction);
}
void Camera::SetRotate(const Rotation& rot)
{
	SetDirection(rot.axis);
	this->SetFieldOfView(vec::RadToDeg(rot.angle));
}
void Camera::SetTarget(const Vector3& Target)
{
	m_Target = Target;
	SetDirection(m_Target - m_Position);
}
void Camera::SetCameraUP(const Vector3& Cup)
{
	m_CameraUp = Cup;
	SetDirection(m_Target - m_Position);
}
void Camera::SetFieldOfView(float FieldOfView)
{ 
	if (FieldOfView < 1 || FieldOfView >= 180)
		return;
	m_Field = FieldOfView; 
}
Matrix4 Camera::GetGLMView()const
{
	return Matrix4(
		m_Right.x, m_Right.y, m_Right.z, 0,
		m_Up.x, m_Up.y, m_Up.z, 0,
		-m_Direction.x, -m_Direction.y, -m_Direction.z, 0,
		0, 0, 0, 1
	) * Matrix4(
		1, 0, 0, -m_Position.x,
		0, 1, 0, -m_Position.y,
		0, 0, 1, -m_Position.z,
		0, 0, 0, 1
	);
}
float Material::getNs() const {
	return m_Ns;
}

void Material::setNs(float ns) {
	m_Ns = ns;
}

float Material::getNi() const {
	return m_Ni;
}

void Material::setNi(float ni) {
	m_Ni = ni;
}

float Material::getTr() const {
	return m_Tr;
}

void Material::setTr(float tr) {
	m_Tr = tr;
}

const vec::Vector3& Material::getKa()const
{
	return m_Ka;
}
void Material::setKa(const vec::Vector3& ka)
{
	m_Ka = ka;
}
const vec::Vector3& Material::getKd()const
{
	return m_Kd;
}
void Material::setKd(const vec::Vector3& kd)
{
	m_Kd = kd;
}
const vec::Vector3& Material::getKs()const
{
	return m_Ks;
}
void Material::setKs(const vec::Vector3& ks)
{
	m_Ks = ks;
}


Picture* Material::getMapKa() const {
	return m_MapKa;
}

inline void Material::setMapKa(Picture* mapKa) {
	if (m_MapKa)
		m_MapKa->Dereference(this);
	m_MapKa = mapKa;
	if (m_MapKa)
		m_MapKa->Reference(this);
}

Picture* Material::getMapKd() const {
	return m_MapKd;
}

inline void Material::setMapKd(Picture* mapKd) {
	if (m_MapKd)
		m_MapKd->Dereference(this);
	m_MapKd = mapKd;
	if (m_MapKd)
		m_MapKd->Reference(this);
}

Picture* Material::getMapKs() const {
	return m_MapKs;
}

inline void Material::setMapKs(Picture* mapKs) {
	if (m_MapKs)
		m_MapKs->Dereference(this);
	m_MapKs = mapKs;
	if (m_MapKs)
		m_MapKs->Reference(this);
}

Material::Material(std::string& name)
	: m_Ns(0.0f),
	m_Ni(0.0f),
	m_Tr(0.0f),
	m_MapKa(nullptr),
	m_MapKd(nullptr),
	m_MapKs(nullptr)
{ 
	m_Name = name; 
}
Material::Material()
	: m_Ns(0.0f),
	m_Ni(0.0f),
	m_Tr(0.0f),
	m_MapKa(nullptr),
	m_MapKd(nullptr),
	m_MapKs(nullptr)
{
	m_Name = "新建材质";
}
void Object::DeleteReferenceP(Object*){}
void Object::InitReferenceP(Object*){}
Material::~Material()
{
	auto ls = m_Reference;
	for (auto& re : ls)
	{
		re->DeleteReferenceP(this);
	}
	if (m_MapKa)
		m_MapKa->Dereference(this);
	if (m_MapKd)
		m_MapKd->Dereference(this);
	if (m_MapKs)
		m_MapKs->Dereference(this);
}
Model::Model()
	: m_Parent(nullptr), m_ModelMesh(nullptr), m_Material(nullptr),
	m_Position(Vector3(0.0f, 0.0f, 0.0f)), m_Scale(Vector3(1.0f, 1.0f, 1.0f)), m_Rotate(Rotation(0, Vector(0, 1, 0)))
{
	m_Transform = glm::mat4(1.0f);
	m_Transform = glm::scale(m_Transform, (glm::vec3)m_Scale);
	m_Transform = glm::rotate(m_Transform, (float)m_Rotate.angle, (glm::vec3)m_Rotate.axis);
	m_Transform = glm::translate(m_Transform, (glm::vec3)m_Position);
	m_Name = "新建模型";
	m_Mode = ModelMode::MM_EDIT;
}
Model::Model(std::string& name)
	: m_Parent(nullptr), m_ModelMesh(nullptr), m_Material(nullptr),
	m_Position(Vector3(0.0f, 0.0f, 0.0f)), m_Scale(Vector3(1.0f, 1.0f, 1.0f)), m_Rotate(Rotation(0, Vector(0, 1, 0)))
{
	m_Name = name;
	m_Transform = glm::mat4(1.0f);
	m_Transform = glm::scale(m_Transform, (glm::vec3)m_Scale);
	m_Transform = glm::rotate(m_Transform, (float)m_Rotate.angle, (glm::vec3)m_Rotate.axis);
	m_Transform = glm::translate(m_Transform, (glm::vec3)m_Position);
}
Model::~Model() 
{
	for (auto& p : m_ChildModel)
		if (p)
			delete p;
	if (m_ModelMesh)
		m_ModelMesh->Dereference(this);
	if (m_Material)
		m_Material->Dereference(this);
}

void Model::move(const Vector3& offset, bool add) {
	if (add) {
		m_Position += offset;
		m_Transform = glm::translate(m_Transform, (glm::vec3)offset);
	}
	else {
		m_Position = offset;
		m_Transform = glm::mat4(1.0f);
		m_Transform = glm::scale(m_Transform, (glm::vec3)m_Scale);
		m_Transform = glm::rotate(m_Transform, (float)m_Rotate.angle, (glm::vec3)m_Rotate.axis);
		m_Transform = glm::translate(m_Transform, (glm::vec3)m_Position);
	}
}

void Model::scale(const Vector3& scaling, bool multiply) {
	if (multiply) {
		m_Scale = m_Scale * scaling;
		m_Transform = glm::scale(m_Transform, (glm::vec3)scaling);
	}
	else {
		m_Scale = scaling;
		m_Transform = glm::mat4(1.0f);
		m_Transform = glm::scale(m_Transform, (glm::vec3)m_Scale);
		m_Transform = glm::rotate(m_Transform, (float)m_Rotate.angle, (glm::vec3)m_Rotate.axis);
		m_Transform = glm::translate(m_Transform, (glm::vec3)m_Position);
	}
}

void Model::rotate(const Rotation& quaternion, bool multiply) {
	if (multiply) {
		m_Rotate = m_Rotate.compose(quaternion);
		m_Transform = glm::rotate(m_Transform, (float)quaternion.angle, (glm::vec3)quaternion.axis);
	}
	else {
		m_Rotate = quaternion;
		m_Transform = glm::mat4(1.0f);
		m_Transform = glm::scale(m_Transform, (glm::vec3)m_Scale);
		m_Transform = glm::rotate(m_Transform, (float)m_Rotate.angle, (glm::vec3)m_Rotate.axis);
		m_Transform = glm::translate(m_Transform, (glm::vec3)m_Position);
	}
}

void Model::moveAll(const Vector3& offset, bool add) {
	move(offset, add);
	for (auto child : m_ChildModel) {
		child->moveAll(offset, add);
	}
}

void Model::scaleAll(const Vector3& scaling, bool multiply) {
	scale(scaling, multiply);
	for (auto child : m_ChildModel) {
		child->scaleAll(scaling, multiply);
	}
}

void Model::rotateAll(const Rotation& quaternion, bool multiply) {
	rotate(quaternion, multiply);
	for (auto child : m_ChildModel) {
		child->rotateAll(quaternion, multiply);
	}
}

Vector3 Model::getPosition() const {
	return m_Position;
}

void Model::setPosition(const Vector3& position) {
	m_Position = position;
	m_Transform = glm::mat4(1.0f);
	m_Transform = glm::scale(m_Transform, (glm::vec3)m_Scale);
	m_Transform = glm::rotate(m_Transform, (float)m_Rotate.angle, (glm::vec3)m_Rotate.axis);
	m_Transform = glm::translate(m_Transform, (glm::vec3)m_Position);
}

Vector3 Model::getScale() const {
	return m_Scale;
}

void Model::setScale(const Vector3& scaling) {
	m_Scale = scaling;
	m_Transform = glm::mat4(1.0f);
	m_Transform = glm::scale(m_Transform, (glm::vec3)m_Scale);
	m_Transform = glm::rotate(m_Transform, (float)m_Rotate.angle, (glm::vec3)m_Rotate.axis);
	m_Transform = glm::translate(m_Transform, (glm::vec3)m_Position);
}

Rotation Model::getRotation() const {
	return m_Rotate;
}

void Model::setRotation(const Rotation& quaternion) {
	m_Rotate = quaternion;
	m_Transform = glm::mat4(1.0f);
	m_Transform = glm::scale(m_Transform, (glm::vec3)m_Scale);
	m_Transform = glm::rotate(m_Transform, (float)m_Rotate.angle, (glm::vec3)m_Rotate.axis);
	m_Transform = glm::translate(m_Transform, (glm::vec3)m_Position);
}

Vector3 Model::getWorldPosition() const {
	if (m_Parent) {
		return m_Position + m_Parent->getWorldPosition();
	}
	else {
		return m_Position;
	}
}

void Model::setWorldPosition(const Vector3& position) {
	if (m_Parent) {
		m_Position = position - m_Parent->getWorldPosition();
	}
	else {
		m_Position = position;
	}
}

Vector3 Model::getWorldScale() const {
	if (m_Parent) {
		Vector out;
		Vector ParentSanle = m_Parent->getWorldScale();
		out.x = m_Scale.x * ParentSanle.x;
		out.y = m_Scale.y * ParentSanle.y;
		out.z = m_Scale.z * ParentSanle.z;
		return out;
	}
	else {
		return m_Scale;
	}
}
Vector Model::GetPosition() const
{
	return m_Position;
}
Vector Model::GetWorldPosition() const
{
	return getWorldPosition();
}
Rotation Model::GetRotate()const
{
	return getRotation();
}
void Model::SetRotate(const Rotation& r)
{
	setRotation(r);
}
void Model::Rotate(const Rotation& r)
{
	rotate(r);
}
void Model::SetPosition(const vec::Vector& v)
{
	setPosition(v);
}
void Model::DeleteChildObject()
{
	for (auto& i : m_ChildModel)
		delete i;
	m_ChildModel.clear();
}
const std::vector<FACE>& Model::GetTriFace()
{
	if (m_GDI_TriFaceData.empty() && m_ModelMesh)
	{
		FACE outf;
		for (const FaceData_面信息& fi : m_ModelMesh->m_FaceIndices)
		{
			outf.vertexA = m_ModelMesh->m_VertexPosition[fi.a[0] - 1] + GetWorldPosition();
			outf.vertexB = m_ModelMesh->m_VertexPosition[fi.a[3] - 1] + GetWorldPosition();
			outf.vertexC = m_ModelMesh->m_VertexPosition[fi.a[6] - 1] + GetWorldPosition();
			outf.texCoordA = m_ModelMesh->m_TexCoords[fi.a[1] - 1];
			outf.texCoordB = m_ModelMesh->m_TexCoords[fi.a[4] - 1];
			outf.texCoordC = m_ModelMesh->m_TexCoords[fi.a[7] - 1];
			outf.normalA = m_ModelMesh->m_Normal[fi.a[2] - 1];
			outf.normalB = m_ModelMesh->m_Normal[fi.a[5] - 1];
			outf.normalC = m_ModelMesh->m_Normal[fi.a[8] - 1];
			outf.color = RGB(m_Material->getKd().x * 255, m_Material->getKd().y * 255, m_Material->getKd().z * 255);
			m_GDI_TriFaceData.push_back(outf);
		}
	}
	return m_GDI_TriFaceData;
}
void Model::setWorldScale(const Vector3& scaling) {
	if (m_Parent) {
		Vector ParentSanle = m_Parent->getWorldScale();
		m_Scale.x = scaling.x / ParentSanle.x;
		m_Scale.y = scaling.y / ParentSanle.y;
		m_Scale.z = scaling.z / ParentSanle.z;
	}
	else {
		m_Scale = scaling;
	}
}

Rotation Model::getWorldRotation() const {
	if (m_Parent) {
		return m_Parent->getWorldRotation().compose(m_Rotate);
	}
	else {
		return m_Rotate;
	}
}

void Model::setWorldRotation(const Rotation& quaternion) {
	if (m_Parent) {
		m_Rotate = m_Rotate.getRelativeRotation(quaternion, m_Parent->getWorldRotation());
	}
	else {
		m_Rotate = quaternion;
	}
}
void Model::Move(const Vector3& m)
{
	move(m);
}
// 切换选中状态
void Model::ToggleSelection()
{
	m_Selected = !m_Selected;
	for (auto c : m_ChildModel)
		c->ToggleSelection();
}
// 物体被选中时调用
void Model::Selected()
{
	m_Selected = true;
	for (auto c : m_ChildModel)
		c->Selected();
}
// 物体取消选中时调用
void Model::Unselected()
{
	m_Selected = false;
	for (auto c : m_ChildModel)
		c->Unselected();
}
void Model::addChildModel(Model* model) 
{
	if (!model) {
		return;
	}
	if (model->m_Parent) 
	{
		model->m_Parent->removeChildModel(model);
	}
	model->m_Parent = this;
	m_ChildModel.push_back(model);
}
void Model::SetModelParent(Model* model)
{
	m_Parent = model;
}
Mesh* Model::GetMesh()const
{
	return m_ModelMesh;
}
const std::vector<UINT>& Mesh::GetIndices()
{
	return m_Indices;
}
const std::vector<Vertex>& Mesh::GetVertexData()
{
	if (!m_Data.empty())return m_Data;
	Vertex v0, v1, v2;
	for (const FaceData_面信息& fi : m_FaceIndices)
	{
		v0.position = m_VertexPosition[fi.a[0] - 1];
		v1.position = m_VertexPosition[fi.a[3] - 1];
		v2.position = m_VertexPosition[fi.a[6] - 1];
		v0.texCoord = m_TexCoords[fi.a[1] - 1];
		v1.texCoord = m_TexCoords[fi.a[4] - 1];
		v2.texCoord = m_TexCoords[fi.a[7] - 1];
		v0.normal = m_Normal[fi.a[2] - 1];
		v1.normal = m_Normal[fi.a[5] - 1];
		v2.normal = m_Normal[fi.a[8] - 1];
		m_Data.push_back(v0);
		m_Data.push_back(v1);
		m_Data.push_back(v2);

		UINT indexSize = static_cast<UINT>(m_Data.size()) - 3;
		m_Indices.push_back(indexSize);
		m_Indices.push_back(indexSize + 1);
		m_Indices.push_back(indexSize + 2);
	}
	return m_Data;
}
void Model::SetMesh(Mesh* mesh)
{
	if (m_ModelMesh)
		m_ModelMesh->Dereference(this);
	m_ModelMesh = mesh;
	if (m_ModelMesh)
		m_ModelMesh->Reference(this);
}
glm::mat4 Model::GetGLTransform()const
{
	if (m_Parent)
		return m_Parent->GetGLTransform() * m_Transform;
	return m_Transform;
}
void Model::InitReferenceP(Object* obj)
{
	if (!obj)return;
	if (obj == m_ModelMesh)
	{
		m_ModelMesh = nullptr;
	}
	else if (obj == m_Material)
	{
		m_Material = nullptr;
	}
}
void Model::DeleteReferenceP(Object* obj)
{
	if (!obj)return;
	if (obj == m_ModelMesh)
	{
		m_ModelMesh->Dereference(this);
		m_ModelMesh = nullptr;
	}
	else if (obj == m_Material)
	{
		m_Material->Dereference(this);
		m_Material = nullptr;
	}
}

void Model::removeChildModel(Model* model) 
{
	auto it = std::find(m_ChildModel.begin(), m_ChildModel.end(), model);
	if (it != m_ChildModel.end()) {
		(*it)->m_Parent = nullptr;
		m_ChildModel.erase(it);
	}
}
Picture::Picture() :m_Data(nullptr), m_Width(0), m_Height(0), m_NrComponents(0)
{
}
Picture::Picture(std::string& name) :m_Data(nullptr), m_Width(0), m_Height(0), m_NrComponents(0)
{
	m_Name = name;
}
Picture::~Picture()
{
	if (m_Data)
		stbi_image_free(m_Data);
	auto ls = m_Reference;
	for (auto& re : ls)
	{
		re->DeleteReferenceP(this);
	}
}
void Picture::FreePictureData()
{
	if (m_Data)
		stbi_image_free(m_Data);
	m_Data = nullptr;
}
PictureData Picture::GetPicture()const
{
	return{ m_Data,m_Width,m_Height,m_NrComponents };
}
PictureData Picture::LoadPicture(const std::string& path)
{
	stbi_set_flip_vertically_on_load(true);
	m_Data = stbi_load(path.c_str(), &m_Width, &m_Height, &m_NrComponents, 0);
	if (m_Data)
	{
		return{ m_Data,m_Width,m_Height,m_NrComponents };
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path.c_str() << std::endl;
		stbi_image_free(m_Data);
	}
	return{ m_Data,m_Width,m_Height,m_NrComponents };
}
Mesh::Mesh(std::string& Name)
{
	m_Name = Name;
}
Mesh::~Mesh()
{
	auto ls = m_Reference;
	for (auto& re : ls)
	{
		re->DeleteReferenceP(this);
	}
}
bool MatFileReader::read(const std::string& filename) 
{
	std::ifstream file(filename);
	if (!file.is_open()) {
		std::cerr << "Failed to open file: " << filename << std::endl;
		return false;
	}
	int pos = filename.find_last_of('\\');
	if (pos == -1)
		pos = filename.find_last_of('/');
	std::string FolderPath = filename.substr(0, pos + 1);
	std::string line;
	Material* current_material = nullptr;
	while (std::getline(file, line)) 
	{
		std::istringstream iss(line);
		std::string token;
		iss >> token;

		if (token == "newmtl") {
			if (current_material != nullptr) {
				m_Materials[current_material->GetName()] = current_material;
			}
			current_material = new Material();
			iss >> token;
			current_material->SetName(token.c_str());
		}
		else if (token == "Ka") {
			vec::Vector3 ka;
			iss >> ka.x >> ka.y >> ka.z;
			current_material->setKa(ka);
		}
		else if (token == "Kd") {
			vec::Vector3 kd;
			iss >> kd.x >> kd.y >> kd.z;
			current_material->setKd(kd);
		}
		else if (token == "Ks") {
			vec::Vector3 ks;
			iss >> ks.x >> ks.y >> ks.z;
			current_material->setKs(ks);
		}
		else if (token == "Ns") {
			float ns;
			iss >> ns;
			current_material->setNs(ns);
		}
		else if (token == "Ni") {
			float ni;
			iss >> ni;
			current_material->setNi(ni);
		}
		else if (token == "d" || token == "Tr") {
			float tr;
			iss >> tr;
			current_material->setTr(tr);
		}
		else if (token == "map_Ka") {
			std::string filename;
			iss >> filename;
			if (!m_Picture[filename])
			{
				m_Picture[filename] = new Picture(filename);
				m_Picture[filename]->LoadPicture((FolderPath + filename).c_str());
			}
			current_material->setMapKa(m_Picture[filename]);
		}
		else if (token == "map_Kd") {
			std::string filename;
			iss >> filename;
			if (!m_Picture[filename])
			{
				m_Picture[filename] = new Picture(filename);
				m_Picture[filename]->LoadPicture((FolderPath + filename).c_str());
			}
			current_material->setMapKd(m_Picture[filename]);
		}
		else if (token == "map_Ks") {
			std::string filename;
			iss >> filename;
			if (!m_Picture[filename])
			{
				m_Picture[filename] = new Picture(filename);
				m_Picture[filename]->LoadPicture((FolderPath + filename).c_str());
			}
			current_material->setMapKs(m_Picture[filename]);
		}
	}

	if (current_material != nullptr)
	{
		m_Materials[current_material->GetName()] = current_material;
	}

	return true;
}

Material* MatFileReader::getMaterial(const std::string& name) const {
	auto it = m_Materials.find(name);
	if (it != m_Materials.end()) {
		return it->second;
	}
	return nullptr;
}

std::vector<Model*> Folder::GetAllModleFile_找到所有模型()const
{
	std::vector<Model*> out;
	for (const auto& c : m_Child)
	{
		switch (c.second->GetType())
		{
		case ObjectType::OT_MODEL:
		{
			out.push_back(dynamic_cast<Model*>(c.second));
			break;
		}
		case ObjectType::OT_FOLDER:
		{
			std::vector<Model*> f = dynamic_cast<Folder*>(c.second)->GetAllModleFile_找到所有模型();
			out.insert(out.end(), f.begin(), f.end());
			break;
		}
		default:
			break;
		}	
	}
	return out;
}
std::vector<PointLight*> Folder::GetAllPointLightFile_找到所有光源文件()const
{
	std::vector<PointLight*> out;
	for (const auto& c : m_Child)
	{
		switch (c.second->GetType())
		{
		case ObjectType::OT_POINT_LIGHT:
		{
			out.push_back(dynamic_cast<PointLight*>(c.second));
			break;
		}
		case ObjectType::OT_FOLDER:
		{
			Folder* folder = dynamic_cast<Folder*>(c.second);
			std::vector<PointLight*> f = (folder)->GetAllPointLightFile_找到所有光源文件();
			out.insert(out.end(), f.begin(), f.end());
			break;
		}
		default:
			break;
		}
	}
	return out;
}
Folder::~Folder()
{
	for (auto& c : m_Child)
	{
 		delete c.second;
		c.second = nullptr;
	}
	m_Child.clear();
}
ObjectType PointLight::GetType()const
{
	return OT_POINT_LIGHT;
}
PointLight::~PointLight()
{
}
void PointLight::SetPosition(const Vector& position)
{
	m_Position = position;
}
void PointLight::Move(const Vector& move)
{
	m_Position += move;
}
Vector PointLight::GetPosition() const
{
	return m_Position;
}

void PointLight::SetLightColor(const Vector& lightColor)
{
	m_Color = lightColor;
}
const Vector& PointLight::GetLightColor() const
{
	return m_Color;
}
//设置颜色
void PointLight::SetScale(const Vector& color)
{
	SetLightColor(color);
}
//获取颜色
Vector PointLight::GetScale() const
{
	return GetLightColor();
}
Rotation PointLight::GetRotate()const
{
	return Rotation(0, vec::Vector3(m_Intensity, 0, m_Range));
}
void PointLight::SetRotate(const Rotation& rot)
{
	SetIntensity(rot.axis.x);
	SetRange(rot.axis.z);
}
void PointLight::SetIntensity(float intensity)
{
	m_Intensity = intensity;
}
float PointLight::GetIntensity() const
{
	return m_Intensity;
}

void PointLight::SetRange(float range)
{
	m_Range = range;
}
float PointLight::GetRange() const
{
	return m_Range;
}
void Object::ToggleSelection()
{
	m_Selected = !m_Selected;
}
bool Object::IsSelected()
{
	return m_Selected;
}
void Object::Selected()
{
	m_Selected = true;
}
void Object::Unselected()
{
	m_Selected = false;
}
ObjectType Room::GetType()const
{
	return ObjectType::OT_ROOM;
}

INT_PTR Folder::ListControlView(const HWND hWndList,HIMAGELIST hImageList, std::map<int, int>& index)
{
	while (ListView_DeleteColumn(hWndList, 0));
	ListView_DeleteAllItems(hWndList);
	int item = 0;

	LVCOLUMN lvColumn = { 0 };
	lvColumn.mask = LVCF_TEXT | LVCF_WIDTH;
	lvColumn.cx = 100;
	lvColumn.pszText = (LPWSTR)L"文件名";
	ListView_InsertColumn(hWndList, 0, &lvColumn);
	lvColumn.pszText = (LPWSTR)L"文件类型";
	ListView_InsertColumn(hWndList, 1, &lvColumn);
	LVITEM lvItem = { 0 };
	lvItem.mask = LVIF_TEXT | LVIF_PARAM;
	std::wstring name;
	for (auto& c : m_Child)
	{
		name = strwstr(c.second->GetName());
		lvItem.lParam = (LPARAM)c.second;
		lvItem.iItem = item;
		lvItem.iSubItem = 0;
		lvItem.pszText = (LPWSTR)name.c_str();
		ListView_InsertItem(hWndList, &lvItem);

		name = ObjectTypeToWstr(c.second->GetType());
		ListView_SetItemText(hWndList, item, 1, (LPWSTR)name.c_str());
		item++;
	}
	return TRUE;
}
void Mesh::Reference(Object* obj)
{
	m_Reference.emplace(obj);
}
const std::set<Object*>* Mesh::GetAllReference()const
{
	return &m_Reference;
}
void Mesh::Dereference(Object* obj)
{
	auto it = m_Reference.find(obj);
	if (it != m_Reference.end())
	{
		m_Reference.erase(obj);
		obj->InitReferenceP(this);
	}
}
bool Mesh::IsReference(Object* obj)const
{
	if (m_Reference.find(obj) == m_Reference.end())
		return false;
	return true;
}
void Picture::Reference(Object* obj)
{
	m_Reference.emplace(obj);
}
const std::set<Object*>* Picture::GetAllReference()const
{
	return &m_Reference;
}
void Picture::Dereference(Object* obj)
{
	auto it = m_Reference.find(obj);
	if (it != m_Reference.end())
	{
		m_Reference.erase(obj);
		obj->InitReferenceP(this);
	}
}
bool Picture::IsReference(Object* obj)const
{
	if (m_Reference.find(obj) == m_Reference.end())
		return false;
	return true;
}
void Material::Reference(Object* obj)
{
	m_Reference.emplace(obj);
}
const std::set<Object*>* Material::GetAllReference()const
{
	return &m_Reference;
}
void Material::Dereference(Object* obj)
{
	auto it = m_Reference.find(obj);
	if (it != m_Reference.end())
	{
		m_Reference.erase(obj);
		obj->InitReferenceP(this);
	}
}
bool Material::IsReference(Object* obj)const
{
	if (m_Reference.find(obj) == m_Reference.end())
		return false;
	return true;
}
void Object::Reference(Object* obj){}
const std::set<Object*>* Object::GetAllReference()const
{
	return nullptr;
}
void Object::Dereference(Object* obj){}
bool Object::IsReference(Object* obj)const
{
	return true;
}
INT_PTR Model::ListControlView(const HWND hWndList, HIMAGELIST hImageList, std::map<int, int>& index)
{
	while (ListView_DeleteColumn(hWndList, 0));
	ListView_DeleteAllItems(hWndList);
	int item = 0;

	LVCOLUMN lvColumn = { 0 };
	lvColumn.mask = LVCF_TEXT | LVCF_WIDTH;
	lvColumn.cx = 100;
	lvColumn.pszText = (LPWSTR)L"名称";
	ListView_InsertColumn(hWndList, 0, &lvColumn);
	lvColumn.pszText = (LPWSTR)L"类型";
	ListView_InsertColumn(hWndList, 1, &lvColumn);
	LVITEM lvItem = { 0 };
	lvItem.mask = LVIF_TEXT | LVIF_PARAM;
	std::wstring name;
	if (m_Material)
	{
		name = strwstr(m_Material->GetName());
		lvItem.lParam = (LPARAM)(Object*)m_Material;
		lvItem.iItem = item;
		lvItem.iSubItem = 0;
		lvItem.pszText = (LPWSTR)name.c_str();
		ListView_InsertItem(hWndList, &lvItem);
		ListView_SetItemText(hWndList, item, 1, (LPWSTR)L"材质");
		item++;
	}
	if (m_ModelMesh)
	{
		name = strwstr(m_ModelMesh->GetName());
		lvItem.lParam = (LPARAM)(Object*)m_ModelMesh;
		lvItem.iItem = item;
		lvItem.iSubItem = 0;
		lvItem.pszText = (LPWSTR)name.c_str();
		ListView_InsertItem(hWndList, &lvItem);
		ListView_SetItemText(hWndList, item, 1, (LPWSTR)L"网格");
		item++;
	}
	for (auto& c : m_ChildModel)
	{
		name = strwstr(c->GetName());
		lvItem.lParam = (LPARAM)(Object*)c;
		lvItem.iItem = item;
		lvItem.iSubItem = 0;
		lvItem.pszText = (LPWSTR)name.c_str();
		ListView_InsertItem(hWndList, &lvItem);
		ListView_SetItemText(hWndList, item, 1, (LPWSTR)L"子模型");
		item++;
	}
	return TRUE;
}
_ControlType Object::SetDetaileView()const
{
	return CT_NAME;
}
void Model::SetMaterial(Material * material)
{ 
	if (m_Material)
		m_Material->Dereference(this);
	m_Material = material;
	if (m_Material)
		m_Material->Reference(this);
}
INT_PTR Mesh::ListControlView(const HWND hWndList, HIMAGELIST, std::map<int, int>& index)
{
	while (ListView_DeleteColumn(hWndList, 0));
	ListView_DeleteAllItems(hWndList);
	ListView_SetView(hWndList, LV_VIEW_TILE);
	int item = 0;

	LVCOLUMN lvColumn = { 0 };
	lvColumn.mask = LVCF_TEXT | LVCF_WIDTH;
	lvColumn.cx = 70;
	lvColumn.pszText = (LPWSTR)L"顶点位置x";
	ListView_InsertColumn(hWndList, 0, &lvColumn);
	lvColumn.pszText = (LPWSTR)L"顶点位置y";
	ListView_InsertColumn(hWndList, 1, &lvColumn);
	lvColumn.pszText = (LPWSTR)L"顶点位置z";
	ListView_InsertColumn(hWndList, 2, &lvColumn);
	lvColumn.pszText = (LPWSTR)L"法向量x";
	ListView_InsertColumn(hWndList, 3, &lvColumn);
	lvColumn.pszText = (LPWSTR)L"法向量y";
	ListView_InsertColumn(hWndList, 4, &lvColumn);
	lvColumn.pszText = (LPWSTR)L"法向量z";
	ListView_InsertColumn(hWndList, 5, &lvColumn);
	lvColumn.pszText = (LPWSTR)L"贴图x";
	ListView_InsertColumn(hWndList, 6, &lvColumn);
	lvColumn.pszText = (LPWSTR)L"贴图y";
	ListView_InsertColumn(hWndList, 7, &lvColumn);
	LVITEM lvItem = { 0 };
	std::wstring name;
	for (auto& c : m_Data)
	{
		if (item > 100)break;
		name = std::to_wstring(c.position.x);
		lvItem.mask = LVIF_TEXT;
		lvItem.iItem = item;
		lvItem.iSubItem = 0;
		lvItem.pszText = (LPWSTR)name.c_str();
		ListView_InsertItem(hWndList, &lvItem);
		name = std::to_wstring(c.position.y);
		ListView_SetItemText(hWndList, item, 1, (LPWSTR)name.c_str());
		name = std::to_wstring(c.position.z);
		ListView_SetItemText(hWndList, item, 2, (LPWSTR)name.c_str());
		name = std::to_wstring(c.normal.x);
		ListView_SetItemText(hWndList, item, 3, (LPWSTR)name.c_str());
		name = std::to_wstring(c.normal.y);
		ListView_SetItemText(hWndList, item, 4, (LPWSTR)name.c_str());
		name = std::to_wstring(c.normal.z);
		ListView_SetItemText(hWndList, item, 5, (LPWSTR)name.c_str());
		name = std::to_wstring(c.texCoord.x);
		ListView_SetItemText(hWndList, item, 6, (LPWSTR)name.c_str());
		name = std::to_wstring(c.texCoord.y);
		ListView_SetItemText(hWndList, item, 7, (LPWSTR)name.c_str());
		item++;
	}
	return 0;
}
inline void uplistToListControl(int& list,int x, HWND hList, Object* content, std::wstring& name)
{
	if (content)
	{
		LVITEM lvItem = { 0 };
		name = strwstr(content->GetName());
		lvItem.mask = LVIF_TEXT;
		lvItem.iItem = list;
		lvItem.iSubItem = 0;
		lvItem.pszText = (LPWSTR)name.c_str();
		ListView_InsertItem(hList, &lvItem);
		ListView_SetItemText(hList, list, x, (LPWSTR)L"材质");
		list++;
	}
}
INT_PTR Material::ListControlView(const HWND hWndList, HIMAGELIST, std::map<int, int>& index)
{
	while (ListView_DeleteColumn(hWndList, 0));
	ListView_DeleteAllItems(hWndList);
	int item = 0;

	LVCOLUMN lvColumn = { 0 };
	lvColumn.mask = LVCF_TEXT | LVCF_WIDTH;
	lvColumn.cx = 100;
	lvColumn.pszText = (LPWSTR)L"名称";
	ListView_InsertColumn(hWndList, 0, &lvColumn);
	lvColumn.cx = 70;
	lvColumn.pszText = (LPWSTR)L"类型";
	ListView_InsertColumn(hWndList, 1, &lvColumn);
	lvColumn.pszText = (LPWSTR)L"R";
	ListView_InsertColumn(hWndList, 2, &lvColumn);
	lvColumn.pszText = (LPWSTR)L"G";
	ListView_InsertColumn(hWndList, 3, &lvColumn);
	lvColumn.pszText = (LPWSTR)L"B";
	ListView_InsertColumn(hWndList, 4, &lvColumn);
	LVITEM lvItem = { 0 };
	std::wstring name;
	lvItem.mask = LVIF_TEXT | LVIF_PARAM;
	if (m_MapKa)
	{
		name = strwstr(m_MapKa->GetName());
		lvItem.lParam = (LPARAM)(Object*)m_MapKa;
		lvItem.iItem = item;
		lvItem.iSubItem = 0;
		lvItem.pszText = (LPWSTR)name.c_str();
		ListView_InsertItem(hWndList, &lvItem);
		name = ObjectTypeToWstr(m_MapKa->GetType());
		ListView_SetItemText(hWndList, item, 1, (LPWSTR)name.c_str());
		name = L"环境光";
		ListView_SetItemText(hWndList, item, 2, (LPWSTR)name.c_str());
		item++;
	}
	if (m_MapKd)
	{
		name = strwstr(m_MapKd->GetName());
		lvItem.lParam = (LPARAM)(Object*)m_MapKd;
		lvItem.iItem = item;
		lvItem.iSubItem = 0;
		lvItem.pszText = (LPWSTR)name.c_str();
		ListView_InsertItem(hWndList, &lvItem);
		name = ObjectTypeToWstr(m_MapKd->GetType());
		ListView_SetItemText(hWndList, item, 1, (LPWSTR)name.c_str());
		name = L"漫反射";
		ListView_SetItemText(hWndList, item, 2, (LPWSTR)name.c_str());
		item++;
	}
	if (m_MapKs)
	{
		name = strwstr(m_MapKs->GetName());
		lvItem.lParam = (LPARAM)(Object*)m_MapKs;
		lvItem.iItem = item;
		lvItem.iSubItem = 0;
		lvItem.pszText = (LPWSTR)name.c_str();
		ListView_InsertItem(hWndList, &lvItem);
		name = ObjectTypeToWstr(m_MapKs->GetType());
		ListView_SetItemText(hWndList, item, 1, (LPWSTR)name.c_str());
		name = L"镜面反射";
		ListView_SetItemText(hWndList, item, 2, (LPWSTR)name.c_str());
		item++;
	}
	name = L"环境光";
	lvItem.mask = LVIF_TEXT;
	lvItem.iItem = item;
	lvItem.iSubItem = 0;
	lvItem.pszText = (LPWSTR)name.c_str();
	ListView_InsertItem(hWndList, &lvItem);
	name = L"颜色";
	ListView_SetItemText(hWndList, item, 1, (LPWSTR)name.c_str());
	name = std::to_wstring(m_Ka.x);
	ListView_SetItemText(hWndList, item, 2, (LPWSTR)name.c_str());
	name = std::to_wstring(m_Ka.y);
	ListView_SetItemText(hWndList, item, 3, (LPWSTR)name.c_str());
	name = std::to_wstring(m_Ka.z);
	ListView_SetItemText(hWndList, item, 4, (LPWSTR)name.c_str());
	item++;
	name = L"漫反射";
	lvItem.mask = LVIF_TEXT;
	lvItem.iItem = item;
	lvItem.iSubItem = 0;
	lvItem.pszText = (LPWSTR)name.c_str();
	ListView_InsertItem(hWndList, &lvItem);
	name = L"颜色";
	ListView_SetItemText(hWndList, item, 1, (LPWSTR)name.c_str());
	name = std::to_wstring(m_Kd.x);
	ListView_SetItemText(hWndList, item, 2, (LPWSTR)name.c_str());
	name = std::to_wstring(m_Kd.y);
	ListView_SetItemText(hWndList, item, 3, (LPWSTR)name.c_str());
	name = std::to_wstring(m_Kd.z);
	ListView_SetItemText(hWndList, item, 4, (LPWSTR)name.c_str());
	item++;
	name = L"镜面反射";
	lvItem.mask = LVIF_TEXT;
	lvItem.iItem = item;
	lvItem.iSubItem = 0;
	lvItem.pszText = (LPWSTR)name.c_str();
	ListView_InsertItem(hWndList, &lvItem);
	name = L"颜色";
	ListView_SetItemText(hWndList, item, 1, (LPWSTR)name.c_str());
	name = std::to_wstring(m_Ks.x);
	ListView_SetItemText(hWndList, item, 2, (LPWSTR)name.c_str());
	name = std::to_wstring(m_Ks.y);
	ListView_SetItemText(hWndList, item, 3, (LPWSTR)name.c_str());
	name = std::to_wstring(m_Ks.z);
	ListView_SetItemText(hWndList, item, 4, (LPWSTR)name.c_str());
	item++;
	return 0;
}
unsigned int Picture::loadTexture()
{
	UINT ID;
	glGenTextures(1, &ID);
	if (m_Data)
	{
		GLenum format = GL_RGB;
		if (m_NrComponents == 1)
			format = GL_RED;
		else if (m_NrComponents == 3)
			format = GL_RGB;
		else if (m_NrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, ID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, m_Width, m_Height, 0, format, GL_UNSIGNED_BYTE, m_Data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		return ID;
	}
	return 0;
}
template<typename OBJ>
Keyframe<OBJ>::Keyframe(const std::string& name)
{
	m_loop = false;
	m_Name = name;
}

template<typename OBJ>
void Keyframe<OBJ>::SetKeyframe(ULONG64 time, OBJ key)
{
	// 使用二分法查找该时间点是否已存在关键帧
	bool found = false;
	auto it = std::lower_bound(m_keyframe.begin(), m_keyframe.end(), std::make_pair(time, key), [](const std::pair<ULONG64, OBJ>& a, const std::pair<ULONG64, OBJ>& b) { return a.first < b.first; });
	if (it != m_keyframe.end() && it->first == time)
	{
		m_keyframe.erase(it);
		return;
	}
	// 如果该时间点不存在关键帧，则添加一个新的关键帧
	if (!found)
		m_keyframe.insert(it, std::make_pair(time, key));

	// 根据关键帧的时间戳从小到大对 vector 进行排序
	std::sort(m_keyframe.begin(), m_keyframe.end(), [](const std::pair<ULONG64, OBJ>& a, const std::pair<ULONG64, OBJ>& b) { return a.first < b.first; });
}
template<typename OBJ>
void Keyframe<OBJ>::DeleteKeyframe(ULONG64 time)
{
	for (auto it = m_keyframe.begin(); it != m_keyframe.end(); ++it)
	{
		if (it->first == time)
		{
			m_keyframe.erase(it);
			return;
		}
	}
}

template<typename OBJ>
const std::vector<std::pair<ULONG64, OBJ>>& Keyframe<OBJ>::GetData()const
{
	return m_keyframe;
}
template<typename OBJ>
void Keyframe<OBJ>::SetLoop(bool loop)
{
	m_loop = loop;
}
template<typename OBJ>
bool Keyframe<OBJ>::IsLoop()const
{
	return m_loop;
}
template<typename OBJ>
OBJ* Keyframe<OBJ>::GetKeyframe(ULONG64 time)
{
	if (m_keyframe.empty() || time < m_keyframe.front().first)
		return nullptr;
	if (time > m_keyframe.back().first)
		if (m_loop)
			return GetKeyframe(time % m_keyframe.back().first);
		else
			return &m_keyframe.back().second;
	auto iter = std::lower_bound(m_keyframe.begin(), m_keyframe.end(), std::make_pair(time, OBJ()), [](const std::pair<ULONG64, OBJ>& a, const std::pair<ULONG64, OBJ>& b) { return a.first < b.first; });
	if (iter != m_keyframe.end() && iter->first == time)
		return &iter->second;
	else
	{
		OBJ prev, next;
		ULONG64 prevTime, nextTime;

		// 找到前后两个关键帧
		auto prevIter = iter - 1;
		auto nextIter = iter;

		// 如果前一个关键帧不存在，则使用第一个关键帧
		if (prevIter == m_keyframe.end())
			prevIter = m_keyframe.begin();

		// 如果下一个关键帧不存在，则使用最后一个关键帧
		if (nextIter == m_keyframe.end())
			nextIter = --m_keyframe.end();

		prev = prevIter->second;
		prevTime = prevIter->first;

		next = nextIter->second;
		nextTime = nextIter->first;

		// 计算前后两个关键帧之间的时间比值
		double ratio = static_cast<double>(time - prevTime) / (nextTime - prevTime);

		// 调用 OBJ 类型的 GetKeyframe 函数获取中间状态并返回
		m_TemporaryStorage = prev.GetKeyframe(next, ratio);
		return &m_TemporaryStorage;
	}
}


template<typename OBJ>
ObjectType Keyframe<OBJ>::GetType()const
{
	return ObjectType::OT_KEYFRAME;
}
void Model::updateTransform()
{
	m_Transform = glm::mat4(1.0f);
	m_Transform = glm::scale(m_Transform, (glm::vec3)m_Scale);
	m_Transform = glm::rotate(m_Transform, (float)m_Rotate.angle, (glm::vec3)m_Rotate.axis);
	m_Transform = glm::translate(m_Transform, (glm::vec3)m_Position);
}
bool Model::SetKeyframe(ULONG64 time)
{
	if (!m_keyframe)
	{
		return false;
	}
	m_keyframe->SetKeyframe(time,GetTransForm());
	return true;
}

void Model::SetKeyframe(Keyframe<TransFrame>* NewKeyframe)
{
	m_keyframe = NewKeyframe;
	
}
Keyframe<TransForm>* Model::CreateKryframe()
{
	return new Keyframe<TransForm>();
}
//void Model::SetMode(ModelMode Mode)
//{
//	m_Mode = Mode;
//}
//ModelMode Model::GetMode()const
//{
//	return m_Mode;
//}
bool Model::IsStatic()const
{
	if (m_keyframe)
		return false;
	return true;
}
void Model::SetKeyframeLoop(bool b)
{
	m_keyframe->SetLoop(b);
}
bool Model::GetKeyframeLoop()const
{
	if (m_keyframe)
		return m_keyframe->IsLoop();
	return false;
}
TransForm Model::GetTransForm()const
{
	return TransForm(m_Position, m_Scale, m_Rotate);
}
glm::mat4 Model::GetTransform(ULONG64 time)
{

	if (m_Parent)
		if (m_keyframe)
		{
			RUNMODE rm = GetRunMode_g();
			if (rm == RUNMODE::RM_RUN)
			{
				TransForm* tf = m_keyframe->GetKeyframe(time);
				if (tf)
				{
					m_Position = tf->Position;
					m_Scale = tf->Scale;
					m_Rotate = tf->Rotate;
					updateTransform();
					return m_Parent->GetTransform(time) * m_Transform;
				}
			}
			return m_Parent->GetTransform(time) * m_Transform;
		}
		else
			return m_Parent->GetTransform(time) * m_Transform;
	else
	{
		if (!m_keyframe)
			return m_Transform;
		else
		{
			RUNMODE rm = GetRunMode_g();
			if (rm == RUNMODE::RM_RUN)
			{
				TransForm* tf = m_keyframe->GetKeyframe(time);
				if (tf)
				{
					m_Position = tf->Position;
					m_Scale = tf->Scale;
					m_Rotate = tf->Rotate;
					updateTransform();
					return m_Transform;
				}
			}
			return m_Transform;
		}
	}
	//TransForm tf = m_keyframe->GetKeyframe(time);
	//if (tf.Scale == TransForm().Scale)
	//	return m_Transform;
	//return tf.GetOpenGLMat();
}
const std::vector<std::pair<ULONG64, TransForm>>* Model::GetKeyframeData()const
{
	if (m_keyframe)
		return &m_keyframe->GetData();
	return nullptr;
}
bool Object::SaveFile(const std::wstring path, SaveMode sm)const
{
	return false;
}
void Object::SetKeyframeLoop(bool b)
{
	return;
}
bool Object::GetKeyframeLoop()const
{
	return false;
}
template<typename OBJ>
void Keyframe<OBJ>::SetKeyframeLoop(bool b)
{
	SetLoop(b);
}
template<typename OBJ>
bool Keyframe<OBJ>::GetKeyframeLoop()const
{
	return IsLoop();
}
template<typename OBJ>
bool Keyframe<OBJ>::SaveFile(const std::wstring path, SaveMode mode) const
{
	switch (mode)
	{
	//case SM_XLSX:
	//{
	//	xlnt::workbook wb;
	//	xlnt::worksheet ws = wb.active_sheet();

	//	ws.cell("A1").value(OBJ::GetStrType());
	//	std::stringstream ss;
	//	ss << OBJ::GetDataType();
	//	std::string line;
	//	int column = 2;
	//	while (std::getline(ss, line))
	//	{
	//		ws.cell(column, 1).value(line);
	//		column++;
	//	}
	//	int row_num = 2;
	//	for (const auto& frame : m_keyframe) {
	//		ws.cell(xlnt::cell_reference(1, row_num)).value(frame.first);
	//		ss.clear();
	//		ss << frame.second.GetStrData();
	//		column = 2;
	//		while (std::getline(ss, line))
	//		{
	//			ws.cell(column, row_num).value(line);
	//			column++;
	//		}
	//		row_num++;
	//	}
	//	wb.save(path);
	//	return true;
	//	break;
	//}
	case SM_TEXT:
	{
		std::ofstream fout(path);
		if (!fout.is_open())
		{
			OutMessage_g("文件打开失败", _Error);
			return false;
		}
		std::string str = OBJ::GetStrType() + ' ' + OBJ::GetDataType();
		ReplaceChar(str, '\n', ' ');
		fout << str << std::endl;
		for (const auto& data : m_keyframe)
		{
			str = data.second.GetStrData();
			ReplaceChar(str, '\n', ' ');
			fout << data.first << ' ' << str << std::endl;
		}
		fout.close();
		return true;
		break;
	}
	case SM_BINARY:
		break;
	default:
		return false;
		break;
	}
	return false;
}
void Material::InitReferenceP(Object* obj)
{
	if (!obj)return;
	if (obj == m_MapKa)
	{
		m_MapKa = nullptr;
		return;
	}
	if (obj == m_MapKd)
	{
		m_MapKd = nullptr;
		return;
	}
	if (obj == m_MapKs)
	{
		m_MapKs = nullptr;
		return;
	}
}
void Material::DeleteReferenceP(Object* obj)
{
	if (!obj)return;
	if (obj == m_MapKa)
	{
		m_MapKa->Dereference(this);
		m_MapKa = nullptr;
		return;
	}
	if (obj == m_MapKd)
	{
		m_MapKd->Dereference(this);
		m_MapKd = nullptr;
		return;
	}
	if (obj == m_MapKs)
	{
		m_MapKs->Dereference(this);
		m_MapKs = nullptr;
		return;
	}
}
void Mesh::processMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		Vector3 vector;
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.position = vector;
		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.normal = vector;
		if (mesh->mTextureCoords[0]) // 网格是否有纹理坐标？
		{
			Vector2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.texCoord = vec;
		}
		else
			vertex.texCoord = Vector2(0.0f, 0.0f);
		vertices.push_back(vertex);
	}
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
	m_Data = vertices;
	m_Indices = indices;
}
void Material::processMaterial(aiMesh* mesh, const aiScene* scene, Folder* folder,const std::string& directory)
{
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		// 提取漫反射系数
		aiColor4D diffuse;
		if (AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &diffuse))
		{
			m_Kd.x = diffuse.r;
			m_Kd.y = diffuse.g;
			m_Kd.z = diffuse.b;
		}

		// 提取镜面反射系数
		aiColor4D specular;
		if (AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &specular))
		{
			m_Ks.x = specular.r;
			m_Ks.y = specular.g;
			m_Ks.z = specular.b;
		}

		// 提取环境光反射系数
		aiColor4D ambient;
		if (AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_AMBIENT, &ambient))
		{
			m_Ka.x = ambient.r;
			m_Ka.y = ambient.g;
			m_Ka.z = ambient.b;
		}

		if (material->GetTextureCount(aiTextureType_AMBIENT) > 0)
		{
			aiString path;
			material->GetTexture(aiTextureType_AMBIENT, 0, &path);
			m_MapKa = folder->CreateFile_创建文件<Picture>(material->GetName().C_Str());
			m_MapKa->LoadPicture(directory + path.C_Str());
			m_MapKa->Reference(this);
		}

		if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
		{
			aiString path;
			material->GetTexture(aiTextureType_DIFFUSE, 0, &path);
			m_MapKd = folder->CreateFile_创建文件<Picture>(material->GetName().C_Str());
			m_MapKd->LoadPicture(directory + path.C_Str());
			m_MapKd->Reference(this);
		}

		if (material->GetTextureCount(aiTextureType_SPECULAR) > 0)
		{
			aiString path;
			material->GetTexture(aiTextureType_SPECULAR, 0, &path);
			m_MapKs = folder->CreateFile_创建文件<Picture>(material->GetName().C_Str());
			m_MapKs->LoadPicture(directory + path.C_Str());
			m_MapKs->Reference(this);
		}
	}
}

DirectionalLight::~DirectionalLight() 
{
	// 析构函数
}

ObjectType DirectionalLight::GetType() const 
{
	return ObjectType::OT_DIRECTIONAL_LIGHT;
}

void DirectionalLight::SetDirection(const Vector& direction) 
{
	m_Direction = direction.Normalize();
}

Vector DirectionalLight::GetDirection() const {
	return m_Direction;
}

void DirectionalLight::SetLightColor(const Vector& lightColor) {
	m_Color = lightColor;
}

const Vector& DirectionalLight::GetLightColor() const {
	return m_Color;
}

void DirectionalLight::SetIntensity(float intensity) {
	m_Intensity = intensity;
}

float DirectionalLight::GetIntensity() const {
	return m_Intensity;
}