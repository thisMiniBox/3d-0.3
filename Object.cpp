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
bool ModelShader_模型着色器::read(const std::string& filename) {
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
	auto it = m_Child.find(Name);
	if (it != m_Child.end()) 
	{
		return it->second;
	}
	return nullptr;
}

void Folder::SetFileName(Object* obj, const std::string& NewName)
{
	if (!obj)
	{ 
		// 检查 obj 是否为空指针
		return;
	}
	auto it = m_Child.find(obj->GetName());
	if (it != m_Child.end() && it->second == obj)
	{
		// 如果找到了名称为 obj->GetName() 的元素且值为 obj，则更新名称
		m_Child.erase(it);
		m_Child.insert(std::make_pair(NewName, obj));
	}
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

Object* Folder::CreateFile_创建文件(std::string Name, int type)
{
	Object* out = nullptr;
	switch (type)
	{
	case OT_FOLDER:
	{
		Folder* a;
		if (Name.empty())
		{
			a = new Folder("新建文件夹");
			m_Child.insert(std::make_pair("新建文件夹", a));
		}
		else
		{
			a = new Folder(Name);
			m_Child.insert(std::make_pair(Name, a));
		}
		a->m_Parent = this;
		
		out = a;
		break;
	}
	case OT_MODEL:
	{
		//OldModelClass* a;
		//if (Name.empty())
		//{
		//	a = new OldModelClass("新建模型");
		//	m_Child.insert(std::make_pair("新建模型", a));
		//}
		//else
		//{
		//	a = new OldModelClass(Name);
		//	m_Child.insert(std::make_pair(Name, a));
		//}
		//out = a;
		break;
	}
	case OT_CAMERA:
	{
		Camera* a;
		if (Name.empty())
		{
			a = new Camera;
			m_Child.insert(std::make_pair("摄像头", a));
		}
		else
		{
			a = new Camera(Name, Vector(0, 0, 10), Vector(0, 0, 0), Vector(0, 1, 0), 1.0f);
			m_Child.insert(std::make_pair(Name, a));
		}
		out = a;
		break;
	}
	default:
		break;
	}
	return out;
}
ObjectType Folder::GetType()const
{
	return OT_FOLDER;
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
	: m_Name("")
{
}

// 构造函数，可指定物体名称
Object::Object(std::string nam)
	: m_Name(nam)
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
void Object::SetScale(Vector3)
{
}
Vector3 Model::GetScale()const
{
	return getScale();
}
void Model::SetScale(Vector3 scale)
{
	setScale(scale);
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
void Object::SetPosition(vec::Vector)
{
}
void Object::SetRotate(const Rotation&)
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
	m_MapKa = mapKa;
}

Picture* Material::getMapKd() const {
	return m_MapKd;
}

inline void Material::setMapKd(Picture* mapKd) {
	m_MapKd = mapKd;
}

Picture* Material::getMapKs() const {
	return m_MapKs;
}

inline void Material::setMapKs(Picture* mapKs) {
	m_MapKs = mapKs;
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
Material::~Material()
{

}
Model::Model()
	: m_Parent(nullptr), m_ModelMesh(nullptr), m_Material(nullptr),
	m_Position(Vector3(0.0f, 0.0f, 0.0f)), m_Scale(Vector3(1.0f, 1.0f, 1.0f)), m_Rotate(Rotation(0, Vector(0, 1, 0)))
{
	m_Name = "新建模型";
}
Model::Model(std::string& name)
	: m_Parent(nullptr), m_ModelMesh(nullptr), m_Material(nullptr),
	m_Position(Vector3(0.0f, 0.0f, 0.0f)), m_Scale(Vector3(1.0f, 1.0f, 1.0f)), m_Rotate(Rotation(0, Vector(0, 1, 0)))
{
	m_Name = name;
}
Model::~Model() 
{
	for (auto& p : m_ChildModel)
		if (p)
			delete p;
}

void Model::move(const Vector3& offset, bool add) {
	if (add) {
		m_Position += offset;
	}
	else {
		m_Position = offset;
	}
}

void Model::scale(const Vector3& scaling, bool multiply) {
	if (multiply) {
		m_Scale = m_Scale * scaling;
	}
	else {
		m_Scale = scaling;
	}
}

void Model::rotate(const Rotation& quaternion, bool multiply) {
	if (multiply) {
		m_Rotate = m_Rotate.compose(quaternion);
	}
	else {
		m_Rotate = quaternion;
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
}

Vector3 Model::getScale() const {
	return m_Scale;
}

void Model::setScale(const Vector3& scaling) {
	m_Scale = scaling;
}

Rotation Model::getRotation() const {
	return m_Rotate;
}

void Model::setRotation(const Rotation& quaternion) {
	m_Rotate = quaternion;
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
void Model::SetPosition(vec::Vector v)
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
			if (m_Material->getKd())
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
void Model::SetParent(Model* model)
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
	m_ModelMesh = mesh;
}
void Model::removeChildModel(Model* model) 
{
	auto it = std::find(m_ChildModel.begin(), m_ChildModel.end(), model);
	if (it != m_ChildModel.end()) {
		(*it)->m_Parent = nullptr;
		m_ChildModel.erase(it);
	}
}
Picture::Picture() :m_Data(nullptr), m_ID(0), m_Width(0), m_Height(0), m_NrComponents(0)
{
}
Picture::Picture(std::string& name) :m_Data(nullptr), m_ID(0), m_Width(0), m_Height(0), m_NrComponents(0)
{
	m_Name = name;
}
Picture::~Picture()
{
	if (m_Data)
		stbi_image_free(m_Data);
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
	m_Data = stbi_load(path.c_str(), &m_Width, &m_Height, &m_NrComponents, 0);
	if (m_Data)
	{
		return{ m_Data,m_Width,m_Height,m_NrComponents };
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(m_Data);
	}
	return{ m_Data,m_Width,m_Height,m_NrComponents };
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
Mesh::Mesh(std::string& Name)
{
	m_Name = Name;
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
ReturnedOfLoadFile operator|(ReturnedOfLoadFile a, ReturnedOfLoadFile b)
{
	return static_cast<ReturnedOfLoadFile>(static_cast<unsigned int>(a) | static_cast<unsigned int>(b));
}
ReturnedOfLoadFile& operator|=(ReturnedOfLoadFile& a, ReturnedOfLoadFile b)
{
	a = static_cast<ReturnedOfLoadFile>(static_cast<unsigned int>(a) | static_cast<unsigned int>(b));
	return a;
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
void Material::CleanUpOpenglImageCache()
{
	if (m_MapKa)
		m_MapKa->FreeOpenGL();
	if (m_MapKd)
		m_MapKd->FreeOpenGL();
	if (m_MapKs)
		m_MapKs->FreeOpenGL();
}
void Picture::FreeOpenGL()
{
	if (m_ID)
		glDeleteTextures(1, &m_ID);
	m_ID = 0;
}