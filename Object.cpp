#include "Object.h"
Model::Model()
{
	m_Scale = Vector(1.0, 1.0, 1.0);
	parent_��ģ��ָ�� = nullptr;
	mtl_���� = nullptr;
}
Model::~Model()
{
	vertex_������������.~vector();
	normal_����������.~vector();
	texCoords_��ͼ��������.~vector();
	for (const auto& i : child_��ģ��ָ��)
	{
		i->parent_��ģ��ָ�� = parent_��ģ��ָ��;
	}
	if (mtl_����)delete mtl_����;
	child_��ģ��ָ��.~vector();
	face_��Ķ�ȡλ��.~vector();
}
inline FaceData_����Ϣ faceData(std::string cin) {
	FaceData_����Ϣ result;
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
int Model::loadobj(const std::wstring& path, FILE* file)
{
	char c = 0;
	size_t n = 0;
	size_t m = 0;
	Model* NewModel = nullptr;
	vec::Vector vector;
	vec::Vector2 vector2d;
	FaceData_����Ϣ Face;
	std::string FileData;
	std::string w;
	while (1)
	{
		c = fgetc(file);
		if (c == EOF)break;
		if (c == '\n')
		{
			if (FileData.size() > 0)
			{
				//��ȡ��һ����Ϣ����н���
				n = FileData.find_first_of(' ');
				if (n == -1)
				{
					FileData.clear();
					continue;
				}
				w = FileData.substr(0, n);
				if (w == "v")
				{
					n = (FileData = FileData.substr(n)).find_first_not_of(' ');
					n = (FileData = FileData.substr(n)).find_first_of(' ');
					vector.SetX(std::stod((w = FileData.substr(0, n)).c_str()));
					n = (FileData = FileData.substr(n + 1)).find_first_of(' ');
					vector.SetY(std::stod((w = FileData.substr(0, n)).c_str()));
					n = (FileData = FileData.substr(n + 1)).find_first_of(' ');
					vector.SetZ(std::stod((w = FileData.substr(0, n)).c_str()));
					vertex_������������.push_back(vector);
				}
				else if (w == "vn")
				{
					n = (FileData = FileData.substr(n)).find_first_not_of(' ');
					n = (FileData = FileData.substr(n)).find_first_of(' ');
					vector.SetX(std::stod((w = FileData.substr(0, n)).c_str()));
					n = (FileData = FileData.substr(n + 1)).find_first_of(' ');
					vector.SetY(std::stod((w = FileData.substr(0, n)).c_str()));
					n = (FileData = FileData.substr(n + 1)).find_first_of(' ');
					vector.SetZ(std::stod((w = FileData.substr(0, n)).c_str()));
					normal_����������.push_back(vector);
				}
				else if (w == "vt")
				{
					n = (FileData = FileData.substr(n)).find_first_not_of(' ');
					n = (FileData = FileData.substr(n)).find_first_of(' ');
					vector2d.setX(std::stod((w = FileData.substr(0, n)).c_str()));
					n = (FileData = FileData.substr(n + 1)).find_first_of(' ');
					vector2d.setY(std::stod((w = FileData.substr(0, n)).c_str()));
					texCoords_��ͼ��������.push_back(vector2d);
				}
				else if (w == "f")
				{
					if (!NewModel)return -1;
					n = (FileData = FileData.substr(n)).find_first_not_of(' ');
					n = (FileData = FileData.substr(n)).find_first_of(' ');
					NewModel->face_��Ķ�ȡλ��.push_back(faceData(FileData));
				}
				else if (w == "g" || w == "o" || w == "mg")
				{
					n = (FileData = FileData.substr(n)).find_first_not_of(' ');
					FileData = FileData.substr(n);
					NewModel = CreateChildModel(FileData);
					child_��ģ��ָ��.push_back(NewModel);
				}
				else if (w == "usemtl")
				{
					if (!mtl_����)
					{
						FileData.clear();
						continue;
					}
					if (!NewModel)return -1;
					n = (FileData = FileData.substr(n)).find_first_not_of(' ');
					FileData = FileData.substr(n);
					NewModel->mtl = mtl_����->get_materials(FileData);
				}
				else if (w == "mtllib")
				{
					n = (FileData = FileData.substr(n)).find_first_not_of(' ');
					if (mtl_����)
						if (!mtl_����->read(mtl_����->file.c_str() + FileData.substr(n)))
						{
							delete mtl_����;
							mtl_���� = nullptr;
						}
				}
				FileData.clear();
			}
			continue;
		}
		FileData.push_back(c);
	}
	if (child_��ģ��ָ��.size() == 0)return -3;
	return 1;
}
int Model::loadModelFile_����ģ���ļ�(const std::wstring& filename)
{
	FILE* file = nullptr;
	int error = -2;
	mtl_���� = new ModelShader_ģ����ɫ��;
	_wfopen_s(&file, filename.c_str(), L"r");
	if (!file)return 0;
	std::wstring suffix;
	int meshNum = filename.find_last_of(L"/");
	if (meshNum == -1)meshNum = filename.find_last_of(L"\\");
	m_Name = wstr_str(filename.substr(meshNum + 1, filename.find_last_of(L'.') - meshNum - 1));
	mtl_����->file = wstr_str(filename.substr(0, meshNum + 1));
	fileAddress = mtl_����->file;
	meshNum = filename.find_last_of(L'.');
	if (meshNum >= 0)suffix = filename.substr(meshNum);
	if (suffix == L".obj")error = loadobj(filename, file);
	fclose(file);
	if (error)DistributeDataToChildModels();
	return error;
}
bool ModelShader_ģ����ɫ��::read(const std::string& filename) {
	std::ifstream file(filename);
	if (!file.is_open()) {
		std::cerr << "Failed to open file: " << filename << std::endl;
		return false;
	}

	Material current_material;
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
int Model::GetType()
{
	return OT_MODEL;
}
Folder::Folder(std::string NAME)
{
	m_Name = NAME;
}
auto Folder::FindFile_Ѱ���ļ�(const std::string& Name)const
{
	return m_Child.equal_range(Name);
}
std::vector<Object*> Folder::GetTheCurrentDirectoryFile()const
{
	std::vector<Object*> out;
	for (const auto& kv : m_Child)
	{
		out.push_back(kv.second);
	}
	return out;
}
void Folder::DeleteChildObject()
{
	ClearFolder_����ļ���();
}
void Folder::ClearFolder_����ļ���()
{
	for (const auto& i : m_Child)
	{
		i.second->DeleteChildObject();
		delete i.second;
	}
	m_Child.clear();
}
std::vector<Model*> Folder::GetAllModleFile_�ҵ�����ģ��()const
{
	std::vector<Model*> out;
	
	for (const auto& kv : m_Child) 
	{
		switch (kv.second->GetType())
		{
		case OT_MODEL:
		{
			Model* mod = dynamic_cast<Model*>(kv.second);
			out.push_back(mod);
			out.insert(out.end(), mod->GetChildModel().begin(), mod->GetChildModel().end());
			break;
		}
		case OT_FOLDER:
		{
			Folder* fod= dynamic_cast<Folder*>(kv.second);
			std::vector<Model*> fc = fod->GetAllModleFile_�ҵ�����ģ��();
			out.insert(out.end(), fc.begin(), fc.end());
		}
		default:
			break;
		}
	}
	return out;
}
void Folder::AddFile_����ļ�(Object* obj)
{
	m_Child.insert(std::make_pair(obj->GetName(), obj));
}
void Folder::CreateFile_�����ļ�(std::string Name, int type)
{
	switch (type)
	{
	case OT_FOLDER:
	{
		Folder* a = new Folder(Name);
		a->m_Parent = this;
		m_Child.insert(std::make_pair(Name, a));
		break;
	}
	case OT_MODEL:
	{
		m_Child.insert(std::make_pair(Name, new Model(Name)));
		break;
	}
	case OT_CAMERA:
	{
		m_Child.insert(std::make_pair(Name, new Camera(Name, Vector(0, 0, 10), Vector(0, 0, 0), Vector(0, 1, 0), 1.0f)));
		break;
	}
	default:
		break;
	}
}
int Folder::GetType()
{
	return OT_FOLDER;
}
void Folder::DeleteFile_ɾ���ļ�(Object* obj)
{
	auto range = FindFile_Ѱ���ļ�(obj->GetName());
	for (auto it = range.first; it != range.second; ++it) {
		if ((*it).second == obj)
		{
			obj->DeleteChildObject();
			delete obj;
			m_Child.erase(it);
			return;
		}
	}

}
Model::Model(std::string NAME)
{
	m_Name = NAME;
	parent_��ģ��ָ�� = nullptr;
	mtl_���� = nullptr;
}
Model* Model::CreateChildModel(std::string NAME)
{
	Model* child = new Model;
	child->m_Name = NAME;
	child->parent_��ģ��ָ�� = this;
	return child;
}
// Ĭ�Ϲ��캯��
Object::Object()
	: m_Name("")
{
}

// ���캯������ָ����������
Object::Object(std::string nam)
	: m_Name(nam)
{
}

// ��������
Object::~Object()
{
}

// ��ȡ��������
const std::string Object::GetName() const
{
	return m_Name;
}

// ������������
void Object::SetName(std::string NewName)
{
	m_Name = NewName;
}

// ��ȡ����λ�ã�����Ĭ��ֵ
Vector Object::GetPosition() const
{
	return Vector(0, 0, 0);
}

// ��ȡ�����������꣬����Ĭ��ֵ
Vector Object::GetWorldPosition() const
{
	return Vector(0, 0, 0);
}

// ��������λ�ã���ʵ��
void Object::SetPosition(vec::Vector)
{
}

// �ƶ����壬��ʵ��
void Object::Move(const Vector3&)
{
}

void Object::DeleteChildObject()
{
}
Vector Model::GetWorldPosition()const
{
	if (parent_��ģ��ָ��)
		return m_Position + parent_��ģ��ָ��->GetWorldPosition();
	return m_Position;
}
void Model::SetPosition(vec::Vector p)
{
	m_Position = p;
	for (const auto& i : child_��ģ��ָ��)
		i->UpdateFACEdata();

}
const std::vector<FACE>& Model::GetTriFace()
{
	if (!ModelVertex.empty())ModelVertex.clear();
	if (face_��Ķ�ȡλ��.empty())return TriFace;
	if (TriFace.empty())
	{
		FACE outf;
		for (const FaceData_����Ϣ& fi : face_��Ķ�ȡλ��)
		{
			outf.vertexA = vertex_������������[fi.a[0] - 1] + GetWorldPosition();
			outf.vertexB = vertex_������������[fi.a[3] - 1] + GetWorldPosition();
			outf.vertexC = vertex_������������[fi.a[6] - 1] + GetWorldPosition();
			outf.texCoordA = texCoords_��ͼ��������[fi.a[1]-1];
			outf.texCoordB = texCoords_��ͼ��������[fi.a[4]-1];
			outf.texCoordC = texCoords_��ͼ��������[fi.a[7]-1];
			outf.normalA = normal_����������[fi.a[2]-1];
			outf.normalB = normal_����������[fi.a[5]-1];
			outf.normalC = normal_����������[fi.a[8]-1];
			if (mtl.Kd)
				outf.color = RGB(mtl.Kd[0] * 255, mtl.Kd[1] * 255, mtl.Kd[2] * 255);
			else
				outf.color = RGB(200, 200, 200);
			TriFace.push_back(outf);
		}
	}
	return TriFace;
}
using namespace vec;
void Model::DistributeDataToChildModels()
{
	std::vector<vec::Vector> child_vertex_data;
	std::vector<vec::Vector> child_normal_data;
	std::vector<vec::Vector2> child_texcoord_data;
	std::vector<FaceData_����Ϣ> child_face_data;
	std::unordered_map<int, int> vertex_index;
	std::unordered_map<int, int> normal_index;
	std::unordered_map<int, int> texcoord_index;

	// ������ģ��
	for (auto child : child_��ģ��ָ��)
	{
		child->fileAddress = fileAddress;
		vertex_index.clear();
		normal_index.clear();
		texcoord_index.clear();
		child_vertex_data.clear();
		child_normal_data.clear();
		child_texcoord_data.clear();
		child_face_data.clear();

		// ��ȡ��ģ����Ҫ�Ķ��㡢��������������������
		for (const auto& face : child->GetFaceData())
		{
			FaceData_����Ϣ new_face = face;  // ����ԭʼ������

			// �޸��������е��������Է�ӳ�µ�����λ��
			for (int i = 0; i < 3; ++i)  // ��������
			{
				int index = new_face.a[3 * i];
				if (0 != vertex_index[index])
				{
					new_face.a[3 * i] = vertex_index[index];
					continue;
				}
				child_vertex_data.push_back(vertex_������������[index-1]);
				new_face.a[3 * i] = (int)child_vertex_data.size();
				vertex_index[index] = new_face.a[3 * i];
			}

			for (int i = 0; i < 3; ++i)  // ����������
			{
				int index = new_face.a[3 * i + 2];
				if (0 != normal_index[index])
				{
					new_face.a[3 * i + 2] = normal_index[index];
					continue;
				}
				child_normal_data.push_back(normal_����������[index-1]);
				new_face.a[3 * i + 2] = (int)child_normal_data.size();
				normal_index[index] = new_face.a[3 * i + 2];
			}

			for (int i = 0; i < 3; ++i)  // ������������
			{
				int index = new_face.a[3 * i + 1];
				if (0 != texcoord_index[index])
				{
					new_face.a[3 * i + 1] = texcoord_index[index];
					continue;
				}
				child_texcoord_data.push_back(texCoords_��ͼ��������[index-1]);
				new_face.a[3 * i + 1] = (int)child_texcoord_data.size();
				texcoord_index[index] = new_face.a[3 * i + 1];
			}

			// ���޸ĺ����������ӵ���ģ�͵���������
			child_face_data.push_back(new_face);
		}

		// �����ݷ������ģ��
		child->vertex_������������ = (child_vertex_data);
		child->normal_���������� = (child_normal_data);
		child->texCoords_��ͼ�������� = (child_texcoord_data);
		child->face_��Ķ�ȡλ�� = (child_face_data);
	}
	vertex_������������.clear();
	normal_����������.clear();
	texCoords_��ͼ��������.clear();
	face_��Ķ�ȡλ��.clear();
}
const std::vector<Vertex>& Model::GetVertices()
{
	if (!TriFace.empty())TriFace.clear();
	if (face_��Ķ�ȡλ��.empty())return ModelVertex;
	if (ModelVertex.empty())
	{
		Vertex v0, v1, v2;
		for (const FaceData_����Ϣ& fi : face_��Ķ�ȡλ��)
		{
			v0.position = vertex_������������[fi.a[0] - 1];
			v1.position = vertex_������������[fi.a[3] - 1];
			v2.position = vertex_������������[fi.a[6] - 1];
			v0.texCoord = texCoords_��ͼ��������[fi.a[1] - 1];
			v1.texCoord = texCoords_��ͼ��������[fi.a[4] - 1];
			v2.texCoord = texCoords_��ͼ��������[fi.a[7] - 1];
			v0.normal = normal_����������[fi.a[2] - 1];
			v1.normal = normal_����������[fi.a[5] - 1];
			v2.normal = normal_����������[fi.a[8] - 1];
			ModelVertex.push_back(v0);
			ModelVertex.push_back(v1);
			ModelVertex.push_back(v2);
		}
	}
	return ModelVertex;
}
Camera::Camera() {
	m_Name = "����ͷ";
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
	m_Right = -m_Direction.cross(m_CameraUp).Normalize();
	m_Up = m_Direction.cross(m_Right).Normalize();
}

Matrix4x4 Camera::GetView()const
{
	return Matrix4x4(
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
Matrix4x4 Camera::GetProjection() const
{
	// Calculate and return the perspective projection matrix
	float tan_fov = tan(vec::DegToRad(m_Field) / 2.0f);
	float range = m_Far - m_Near;

	return Matrix4x4(
		1.0f / (tan_fov * m_Ratio), 0, 0, 0,
		0, 1.0f / tan_fov, 0, 0,
		0, 0, -(m_Far + m_Near) / range, -1,
		0, 0, (-2.0f * m_Far * m_Near) / range, 0
	) * Matrix4x4(
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
Matrix4x4 Camera::GetGLMView()const
{
	return Matrix4x4(
		m_Right.x, m_Right.y, m_Right.z, 0,
		m_Up.x, m_Up.y, m_Up.z, 0,
		-m_Direction.x, -m_Direction.y, -m_Direction.z, 0,
		0, 0, 0, 1
	) * Matrix4x4(
		1, 0, 0, -m_Position.x,
		0, 1, 0, -m_Position.y,
		0, 0, 1, -m_Position.z,
		0, 0, 0, 1
	);
}