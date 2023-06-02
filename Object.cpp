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
	case OT_POINTLIGHT:
		return L"点光源";
		break;
	case OT_PARALLELLIGHT:
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
	case OT_POINTLIGHT:
		return "点光源";
		break;
	case OT_PARALLELLIGHT:
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
		return OT_POINTLIGHT;
	}
	else if (str == "平行光源" || str == "ParallelLight" || str == "parallellight") {
		return OT_PARALLELLIGHT;
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
		return OT_POINTLIGHT;
	}
	else if (str == L"平行光源" || str == L"ParallelLight" || str == L"parallellight") {
		return OT_PARALLELLIGHT;
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
	Folder* fold = dynamic_cast<Folder*>(obj);
	if (fold)
		fold->SetParent(this);
	std::string name = obj->GetName();
	if (m_Child.find(name) == m_Child.end()) {
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
PointLight::PointLight(const std::string& name)
{
	m_Intensity = 100;
	m_LightColor = Vector(255, 255, 255);
	m_Position = Vector(0, 0, 0);
	m_Range = 0.1;
	m_SoftShadow = 0.01;
	m_Name = name;
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
inline float Material::getNs() const {
	return m_Ns;
}

inline void Material::setNs(float ns) {
	m_Ns = ns;
}

inline float Material::getNi() const {
	return m_Ni;
}

inline void Material::setNi(float ni) {
	m_Ni = ni;
}

inline float Material::getTr() const {
	return m_Tr;
}

inline void Material::setTr(float tr) {
	m_Tr = tr;
}

inline const float* Material::getKa() const {
	return m_Ka;
}

inline void Material::setKa(const float ka[3]) {
	memcpy(m_Ka, ka, sizeof(float) * 3);
}

inline const float* Material::getKd() const {
	return m_Kd;
}

inline void Material::setKd(const float kd[3]) {
	memcpy(m_Kd, kd, sizeof(float) * 3);
}

inline const float* Material::getKs() const {
	return m_Ks;
}

inline void Material::setKs(const float ks[3]) {
	memcpy(m_Ks, ks, sizeof(float) * 3);
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


Material::Material()
	: m_Ns(0.0f),
	m_Ni(0.0f),
	m_Tr(0.0f),
	m_MapKa(nullptr),
	m_MapKd(nullptr),
	m_MapKs(nullptr)
{
	memset(m_Ka, 0, sizeof(float) * 3);
	memset(m_Kd, 0, sizeof(float) * 3);
	memset(m_Ks, 0, sizeof(float) * 3);
}
void Object::DeleteReferenceP(Object*)
{}
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
			if (m_Material && m_Material->getKd())
				outf.color = RGB(m_Material->getKd()[0] * 255, m_Material->getKd()[1] * 255, m_Material->getKd()[2] * 255);
			else
				outf.color = RGB(200, 200, 200);
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
void Model::addChildModel(Model* model) {
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
	m_ID = 0;
}
Picture::Picture(std::string& name) :m_Data(nullptr), m_Width(0), m_Height(0), m_NrComponents(0)
{
	m_ID = 0;
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
	m_ID = 0;
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
			float ka[3];
			iss >> ka[0] >> ka[1] >> ka[2];
			current_material->setKa(ka);
		}
		else if (token == "Kd") {
			float kd[3];
			iss >> kd[0] >> kd[1] >> kd[2];
			current_material->setKd(kd);
		}
		else if (token == "Ks") {
			float ks[3];
			iss >> ks[0] >> ks[1] >> ks[2];
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
	return OT_POINTLIGHT;
}
PointLight::PointLight()
{
	m_Intensity = 100;
	m_LightColor = Vector(255, 255, 255);
	m_Name = "新建点光源";
	m_Position = Vector(0, 0, 0);
	m_Range = 0.1;
	m_SoftShadow = 0.01;
}
PointLight::~PointLight()
{
}
void PointLight::SetPosition(const Vector& position)
{
	m_Position = position;
}
Vector PointLight::GetPosition() const
{
	return m_Position;
}

void PointLight::SetLightColor(const Vector& lightColor)
{
	m_LightColor = lightColor;
}
const Vector& PointLight::GetLightColor() const
{
	return m_LightColor;
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
void PointLight::SetSoftShadow(float softShadow)
{
	m_SoftShadow = softShadow;
}
float PointLight::GetSoftShadow() const
{
	return m_SoftShadow;
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
	m_Reference.erase(obj);
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
	m_Reference.erase(obj);
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
	m_Reference.erase(obj);
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
	if (m_Ka)
	{
		name = L"环境光";
		lvItem.mask = LVIF_TEXT;
		lvItem.iItem = item;
		lvItem.iSubItem = 0;
		lvItem.pszText = (LPWSTR)name.c_str();
		ListView_InsertItem(hWndList, &lvItem);
		name = L"颜色";
		ListView_SetItemText(hWndList, item, 1, (LPWSTR)name.c_str());
		name = std::to_wstring(m_Ka[0]);
		ListView_SetItemText(hWndList, item, 2, (LPWSTR)name.c_str());
		name = std::to_wstring(m_Ka[1]);
		ListView_SetItemText(hWndList, item, 3, (LPWSTR)name.c_str());
		name = std::to_wstring(m_Ka[2]);
		ListView_SetItemText(hWndList, item, 4, (LPWSTR)name.c_str());
		item++;
	}
	if (m_Kd)
	{
		name = L"漫反射";
		lvItem.mask = LVIF_TEXT;
		lvItem.iItem = item;
		lvItem.iSubItem = 0;
		lvItem.pszText = (LPWSTR)name.c_str();
		ListView_InsertItem(hWndList, &lvItem);
		name = L"颜色";
		ListView_SetItemText(hWndList, item, 1, (LPWSTR)name.c_str());
		name = std::to_wstring(m_Kd[0]);
		ListView_SetItemText(hWndList, item, 2, (LPWSTR)name.c_str());
		name = std::to_wstring(m_Kd[1]);
		ListView_SetItemText(hWndList, item, 3, (LPWSTR)name.c_str());
		name = std::to_wstring(m_Kd[2]);
		ListView_SetItemText(hWndList, item, 4, (LPWSTR)name.c_str());
		item++;
	}
	if (m_Ks)
	{
		name = L"镜面反射";
		lvItem.mask = LVIF_TEXT;
		lvItem.iItem = item;
		lvItem.iSubItem = 0;
		lvItem.pszText = (LPWSTR)name.c_str();
		ListView_InsertItem(hWndList, &lvItem);
		name = L"颜色";
		ListView_SetItemText(hWndList, item, 1, (LPWSTR)name.c_str());
		name = std::to_wstring(m_Ks[0]);
		ListView_SetItemText(hWndList, item, 2, (LPWSTR)name.c_str());
		name = std::to_wstring(m_Ks[1]);
		ListView_SetItemText(hWndList, item, 3, (LPWSTR)name.c_str());
		name = std::to_wstring(m_Ks[2]);
		ListView_SetItemText(hWndList, item, 4, (LPWSTR)name.c_str());
		item++;
	}
	return 0;
}
unsigned int Picture::loadTexture()
{
	if (m_ID != 0)return m_ID;
	glGenTextures(1, &m_ID);
	if (m_Data)
	{
		GLenum format = GL_RGB;
		if (m_NrComponents == 1)
			format = GL_RED;
		else if (m_NrComponents == 3)
			format = GL_RGB;
		else if (m_NrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, m_ID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, m_Width, m_Height, 0, format, GL_UNSIGNED_BYTE, m_Data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		return m_ID;
	}
	return 0;
}
void Picture::FreeOpenGL()
{
	if (m_ID)
		glDeleteTextures(1, &m_ID);
	m_ID = 0;
}
template<typename OBJ>
Keyframe<OBJ>::Keyframe()
{
	// 构造函数
}

template<typename OBJ>
void Keyframe<OBJ>::SetKeyframe(ULONG64 time, OBJ key)
{
	// 将时间和关键帧添加到 m_keyframe 中，如果该时间点已经存在，则更新对应关键帧
	for (auto& kf : m_keyframe)
	{
		if (kf.first == time)
		{
			kf.second = key;
			return;
		}
	}
	m_keyframe.push_back(std::make_pair(time, key));
}

template<typename OBJ>
void Keyframe<OBJ>::DeleteKeyframe(ULONG64 time)
{
	// 根据时间删除对应的关键帧
	for (auto iter = m_keyframe.begin(); iter != m_keyframe.end(); ++iter)
	{
		if (iter->first == time)
		{
			m_keyframe.erase(iter);
			return;
		}
	}
}

template<typename OBJ>
OBJ* Keyframe<OBJ>::GetKeyframe(ULONG64 time)
{
	// 如果关键帧序列为空，则返回空指针
	if (m_keyframe.empty())
	{
		return nullptr;
	}

	ULONG64 lastTime = m_keyframe.back().first;
	// 如果指定的时间点比最后一个关键帧的时间还大，则返回最后一个关键帧
	if (time >= lastTime)
	{
		return &m_keyframe.back().second;
	}

	for (size_t i = 0; i < m_keyframe.size(); ++i)
	{
		ULONG64 currentTime = m_keyframe[i].first;
		OBJ* currentKeyframe = &m_keyframe[i].second;

		// 如果当前关键帧刚好是指定时间，则返回该关键帧
		if (currentTime == time)
		{
			return currentKeyframe;
		}
		// 如果当前关键帧的时间晚于指定时间，则需要查找前一帧和后一帧并计算中间帧
		else if (currentTime > time)
		{
			// 如果当前关键帧是第一帧，则返回空指针
			if (i == 0)
			{
				return nullptr;
			}

			ULONG64 prevTime = m_keyframe[i - 1].first;
			OBJ* prevKeyframe = &m_keyframe[i - 1].second;

			// 计算插值因子 t，使得 time = prevTime + t * (currentTime - prevTime)
			double t = static_cast<double>(time - prevTime) / static_cast<double>(currentTime - prevTime);
			// 根据插值因子 t 计算中间帧
			return prevKeyframe->GetKeyframe(currentKeyframe, t);
		}
	}
	// 如果关键帧序列不为空且指定时间点在第一帧和最后一帧之间，则返回空指针
	return nullptr;
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
Model* Model::GetKeyframe(Model* next, float shifting)
{
	m_Position += (next->getPosition() - m_Position) * shifting;
	m_Scale += (next->getScale() - m_Scale) * shifting;
	m_Rotate = m_Rotate.getRotationTo(next->GetRotate(), shifting);
	updateTransform();
	return this;
}
bool Model::SetKeyframe(ULONG64 time)
{
	if (!m_keyframe)
		return false;
	m_keyframe->SetKeyframe(time,*this);
	return true;
}
bool Model::CreateKryframe()
{
	if (m_keyframe)
		delete m_keyframe;
	m_keyframe = new Keyframe<Model>;
	if (m_keyframe)
		return true;
	return false;
}

void Model::SetMode(ModelMode Mode)
{
	m_Mode = Mode;
}
bool Model::IsStatic()const
{
	return false;
}