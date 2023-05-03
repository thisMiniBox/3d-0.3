#include "project.h"
Controller::Controller()
{
	m_RootFolder.SetName("��Ŀ¼");
	m_FileLoad = false;
	FILEWND = nullptr;
	TEXTWND = nullptr;
	MAINWND = new GDIWND;
	DETAWND = new DetaileWind;
	hWnd = nullptr;
	m_hInst = nullptr;
	m_rect = { 0, 0, 800, 500 };
	view = nullptr;
	Model_att = 0;
}
HWND Controller::CreateWind(HINSTANCE hInst)
{
	TEXTWND = new TextOutWind(hInst);
	FILEWND = new FileWind(hInst);
	WNDCLASSEX wcex = { 0 };
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.hInstance = hInst;
	wcex.hIcon = LoadIcon(hInst, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszClassName = L"szWindowClass";
	if (!RegisterClassEx(&wcex))
	{
		MessageBox(nullptr,
			L"������ע��ʧ�ܣ�",
			L"����",
			MB_ICONERROR);
	}

	// ִ��Ӧ�ó����ʼ��:
	hWnd = CreateWindowW(
		L"szWindowClass",
		L"win32ģ�Ϳ��",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		0,
		1200,
		720,
		nullptr,
		nullptr,
		hInst,
		nullptr);
	m_hInst = hInst;
	TEXTWND->CreateWind(hWnd);
	FILEWND->CreateWind(hWnd);
	Folder* a = new Folder("�½���Ŀ");
	view = new Camera(
		"�½������", Vector(0, 0, 3), Vector(0, 0, 0), Vector(0, 1, 0), GetRect().right / GetRect().bottom);
	AddObject(a);
	AddObject(view);
	MAINWND->CreateWind(hWnd);
	DETAWND->CreateWind(hWnd);
	OutMessage("ʹ��GDI��ͼʱ���ȵ��һ����������ز�߿�ؼ��������޷�������Ⱦ���ڣ�");
	OutMessage("��Ҳ��֪��Ϊʲô����Ϣѭ����ͬһ����opengl��û����");
	RECT m_rect;
	GetClientRect(hWnd, &m_rect);
	int cxClient = m_rect.right - m_rect.left;  // ��ÿͻ������
	int cyClient = m_rect.bottom - m_rect.top;
	SETWND.hWnd = CreateWindowW( //�����༭��
		SETWND.className.c_str(),
		0,
		WS_CHILD | WS_BORDER | WS_VISIBLE | ES_MULTILINE,
		0, 0, cxClient, 50,
		hWnd,
		(HMENU)2,
		hInst,
		nullptr);
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);
	return hWnd;
}
Controller::~Controller()
{
	m_RootFolder.ClearFolder_����ļ���();
	if (MAINWND)delete MAINWND;
	if (DETAWND)delete DETAWND;
	if (FILEWND)delete FILEWND;
	if (TEXTWND)delete TEXTWND;
}
void Controller::SetFileName(Object* obj, const std::wstring& NewName)
{
	char szBuf[128];
	WideCharToMultiByte(CP_ACP, 0, NewName.c_str(), -1, szBuf, 128, NULL, NULL);
	m_RootFolder.SetFileName(obj, szBuf);
	//DETAWND->GetTarget()->SetName(szBuf);
	FILEWND->FixItemName(DETAWND->GetTree(), NewName.c_str());
}
HINSTANCE Controller::GethInstance()const
{
	return m_hInst;
}
Object* Controller::GetFocusObject()const
{
	return DETAWND->GetTarget();
}
void Controller::OutMessage(const std::string&str, const char& type)
{
	TEXTWND->OutMessage(str, type);
}
void Controller::OutMessage(const std::wstring& str, const char& type)
{
	TEXTWND->OutMessage(str, type);
}
void Controller::updateMsg(const HDC& hdc)
{
	TEXTWND->DrawWind(hdc);
}
HTREEITEM Controller::AddObject(Object* a, std::string address)
{
	m_Models.clear();
	m_RootFolder.AddFile_����ļ�(a);
	return FILEWND->AddItem(*a, address);
}
HTREEITEM Controller::AddObject(Object* a, HTREEITEM parent)
{
	m_Models.clear();
	m_RootFolder.AddFile_����ļ�(a);
	return FILEWND->AddItem(*a, parent);
}
Object* Controller::CreateObject(Folder* parent, std::string name, int type)
{
	Object* out = nullptr;
	if (!parent)
		out = m_RootFolder.CreateFile_�����ļ�(name, type);
	else
		out = parent->CreateFile_�����ļ�(name, type);
	HTREEITEM New = FILEWND->AddItem(*out, DETAWND->GetTree());
	DETAWND->SetView(out);
	DETAWND->SetTree(New);
	FILEWND->ExploreFolder(New);
	//FILEWND->ShowFolder(m_RootFolder);
	return out;
}
void Controller::SetRect(RECT rect)
{
	m_rect = rect;
}
RECT Controller::GetRect()const
{
	return m_rect;
}
std::vector<Model*>& Controller::GetModels()
{
	if (!m_Models.empty())return m_Models;
	m_Models = m_RootFolder.GetAllModleFile_�ҵ�����ģ��();
	return m_Models;
}
std::vector<Model*>& Controller::UpdateModels()
{
	m_Models.clear();
	return GetModels();
}

void Controller::DeleteObject(Object* obj,HTREEITEM hTree)
{
	if (DETAWND->GetTarget() == obj)
		DETAWND->SetView(nullptr);
	if (MAINWND->GetType() == MOPENGL && obj->GetType() == OT_MODEL)
	{
		OpenGLWnd* glwind = dynamic_cast<OpenGLWnd*>(MAINWND);
		Model* mod = dynamic_cast<Model*>(obj);
		glwind->DeleteModelBuffer(mod);
	}
	m_RootFolder.DeleteFile_ɾ���ļ�(obj);
	UpdateModels();
	if (hTree)
		FILEWND->DeleteItem(hTree);
	m_Models.clear();
	InvalidateRect(MAINWND->GethWnd(), NULL, true);
}

ReturnedOfLoadFile Controller::LoadFile(const std::wstring& path)
{
	OutMessage(L"��ʼ�����ļ�");
	std::wstring extension = path.substr(path.find_last_of('.') + 1);
	ReturnedOfLoadFile error = ReturnedOfLoadFile::_Fail;
	if (extension == L"obj")
	{
		char narrowPath[MAX_PATH];
		int pathLength = WideCharToMultiByte(CP_ACP, 0, path.c_str(), -1, narrowPath, MAX_PATH, nullptr, nullptr);
		if (pathLength == 0) {
			std::cout << "�ַ���ת������" << std::endl;
			return ReturnedOfLoadFile::_FailToOpenFile;
		}
		error |= LoadObj(narrowPath);
	}
	else
		return ReturnedOfLoadFile::_UnknownFormat;
	if ((error | 0xff00) == ReturnedOfLoadFile::_Succese)
	{
		m_Models.clear();
		FILEWND->ShowFolder(m_RootFolder);
	}
	return error;
	
}

//int project::loadModel(const std::wstring& path)
//{
//	OutMessage("��ʼ����ģ��");
//	Model* model = new Model;
//	int error = model->loadModelFile_����ģ���ļ�(path);
//	if ((error | 0xff00) == ReturnedOfLoadFile::_Succese)
//	{
//		m_RootFolder.AddFile_����ļ�(model);
//		FILEWND->AddItem(*model);
//		m_Models.clear();
//	}
//	else
//	{
//		delete model;
//	}
//	return error;
//}
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

ReturnedOfLoadFile Controller::LoadObj(const std::string& filePath)
{
	FILE* file = nullptr;
	fopen_s(&file, filePath.c_str(), "r");
	if (!file)
		return ReturnedOfLoadFile::_FailToOpenFile;
	int pos = filePath.find_last_of('\\');
	if (pos == -1)
		pos = filePath.find_last_of('/');
	std::string FolderPath = filePath.substr(0, pos + 1);
	std::vector<Vector3>vertexs;
	std::vector<Vector3>normals;
	std::vector<Vector2>texCoords;
	std::vector<FaceData_����Ϣ>faceIndices;
	std::vector<Model*>Models;
	MatFileReader* mtl_���� = new MatFileReader;
	Material* CurrentMaterial = nullptr;

	ReturnedOfLoadFile Error =ReturnedOfLoadFile::_Fail;
	char c = 0;
	size_t charLocate = 0;
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
				charLocate = FileData.find_first_of(' ');
				if (charLocate == -1)
				{
					FileData.clear();
					continue;
				}
				w = FileData.substr(0, charLocate);
				if (w == "v")
				{
					charLocate = (FileData = FileData.substr(charLocate)).find_first_not_of(' ');
					charLocate = (FileData = FileData.substr(charLocate)).find_first_of(' ');
					vector.SetX(std::stod((w = FileData.substr(0, charLocate)).c_str()));
					charLocate = (FileData = FileData.substr(charLocate + 1)).find_first_of(' ');
					vector.SetY(std::stod((w = FileData.substr(0, charLocate)).c_str()));
					charLocate = (FileData = FileData.substr(charLocate + 1)).find_first_of(' ');
					vector.SetZ(std::stod((w = FileData.substr(0, charLocate)).c_str()));
					vertexs.push_back(vector);
				}
				else if (w == "vn")
				{
					charLocate = (FileData = FileData.substr(charLocate)).find_first_not_of(' ');
					charLocate = (FileData = FileData.substr(charLocate)).find_first_of(' ');
					vector.SetX(std::stod((w = FileData.substr(0, charLocate)).c_str()));
					charLocate = (FileData = FileData.substr(charLocate + 1)).find_first_of(' ');
					vector.SetY(std::stod((w = FileData.substr(0, charLocate)).c_str()));
					charLocate = (FileData = FileData.substr(charLocate + 1)).find_first_of(' ');
					vector.SetZ(std::stod((w = FileData.substr(0, charLocate)).c_str()));
					normals.push_back(vector);
				}
				else if (w == "vt")
				{
					charLocate = (FileData = FileData.substr(charLocate)).find_first_not_of(' ');
					charLocate = (FileData = FileData.substr(charLocate)).find_first_of(' ');
					vector2d.setX(std::stod((w = FileData.substr(0, charLocate)).c_str()));
					charLocate = (FileData = FileData.substr(charLocate + 1)).find_first_of(' ');
					vector2d.setY(std::stod((w = FileData.substr(0, charLocate)).c_str()));
					texCoords.push_back(vector2d);
				}
				else if (w == "f")
				{
					if (!NewModel)return ReturnedOfLoadFile::_DataError;
					charLocate = (FileData = FileData.substr(charLocate)).find_first_not_of(' ');
					charLocate = (FileData = FileData.substr(charLocate)).find_first_of(' ');
					NewModel->GetMesh()->m_FaceIndices.push_back(faceData(FileData));
				}
				else if (w == "g" || w == "o" || w == "mg")
				{
					charLocate = (FileData = FileData.substr(charLocate)).find_first_not_of(' ');
					FileData = FileData.substr(charLocate);
					std::string ModelName = FileData + "����ģ��";
					NewModel = new Model(ModelName);
					NewModel->SetMesh(new Mesh(FileData));
					NewModel->SetMaterial(CurrentMaterial);
					Models.push_back(NewModel);
				}
				else if (w == "usemtl")
				{
					if (!mtl_����)
					{
						FileData.clear();
						continue;
					}
					if (!NewModel)return ReturnedOfLoadFile::_FailedToCreateFile;
					charLocate = (FileData = FileData.substr(charLocate)).find_first_not_of(' ');
					FileData = FileData.substr(charLocate);
					//NewModel->SetMaterial(mtl_����->getMaterial(FileData));
					CurrentMaterial = mtl_����->getMaterial(FileData);
					NewModel->SetMaterial(CurrentMaterial);
				}
				else if (w == "mtllib")
				{
					charLocate = (FileData = FileData.substr(charLocate)).find_first_not_of(' ');
					if (!mtl_����->read(FolderPath.c_str() + FileData.substr(charLocate)))
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
	if (Models.size() != 0)
		Error |= ReturnedOfLoadFile::_SuccessfullyLoadedVertex;
	int result = 0;
	bool map = false;
	for (const auto& i : Models)
	{
		if (!i->GetMaterial())
			continue;
		if (i->GetMaterial()->getMapKa())
		if (i->GetMaterial()->getMapKa()->GetPicture().m_Data)
		{
			result++;
			map = true;
		}
		if (i->GetMaterial()->getMapKd())
		if (i->GetMaterial()->getMapKd()->GetPicture().m_Data)
		{
			result++;
			map = true;
		}
		if (i->GetMaterial()->getMapKs())
		if (i->GetMaterial()->getMapKs()->GetPicture().m_Data)
		{
			result++;
			map = true;
		}
	}
	if (!map || result > 0)
	{
		Error |= ReturnedOfLoadFile::_SuccessfullyLoadedMaterialMaps;
	}


	std::vector<vec::Vector> child_vertex_data;
	std::vector<vec::Vector> child_normal_data;
	std::vector<vec::Vector2> child_texcoord_data;
	std::vector<FaceData_����Ϣ> child_face_data;
	std::unordered_map<int, int> vertex_index;
	std::unordered_map<int, int> normal_index;
	std::unordered_map<int, int> texcoord_index;

	// ������ģ��
	for (auto child : Models)
	{
		vertex_index.clear();
		normal_index.clear();
		texcoord_index.clear();
		child_vertex_data.clear();
		child_normal_data.clear();
		child_texcoord_data.clear();
		child_face_data.clear();

		// ��ȡ��ģ����Ҫ�Ķ��㡢��������������������
		for (const auto& face : child->GetMesh()->m_FaceIndices)
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
				child_vertex_data.push_back(vertexs[index - 1]);
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
				child_normal_data.push_back(normals[index - 1]);
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
				child_texcoord_data.push_back(texCoords[index - 1]);
				new_face.a[3 * i + 1] = (int)child_texcoord_data.size();
				texcoord_index[index] = new_face.a[3 * i + 1];
			}

			// ���޸ĺ����������ӵ���ģ�͵���������
			child_face_data.push_back(new_face);
		}

		// �����ݷ������ģ��
		
		child->GetMesh()->m_VertexPosition= (child_vertex_data);
		child->GetMesh()->m_Normal = (child_normal_data);
		child->GetMesh()->m_TexCoords = (child_texcoord_data);
		child->GetMesh()->m_FaceIndices.clear();
		child->GetMesh()->m_FaceIndices = (child_face_data);
	}
	vertexs.clear();
	normals.clear();
	texCoords.clear();
	faceIndices.clear();

	Model* MObj = new Model();
	m_RootFolder.AddFile_����ļ�(MObj);
	Folder* ModelFolder = dynamic_cast<Folder*>(m_RootFolder.CreateFile_�����ļ�(filePath.substr(pos + 1) + "��Դ", OT_FOLDER));
	Folder* ModelMeshFolder = dynamic_cast<Folder*>(ModelFolder->CreateFile_�����ļ�("�����ļ���", OT_FOLDER));
	Folder* ModelMaterialFolder = dynamic_cast<Folder*>(ModelFolder->CreateFile_�����ļ�("�����ļ���", OT_FOLDER));
	MObj->SetName(filePath.substr(pos + 1));
	for (auto& child : Models)
	{
		MObj->addChildModel(child);
		ModelMeshFolder->AddFile_����ļ�(child->GetMesh());
	}
	for (auto& Mat : mtl_����->m_Materials)
	{
		ModelMaterialFolder->AddFile_����ļ�(Mat.second);
	}
	for (auto& picture : mtl_����->m_Picture)
	{
		ModelMaterialFolder->AddFile_����ļ�(picture.second);
	}
	fclose(file);
	if (mtl_����)
	{
		Error |= ReturnedOfLoadFile::_SuccessfullyLoadedMaterialFile;
		delete mtl_����;
		mtl_���� = nullptr;
	}
	return Error;
}
void Controller::UpdateRightWind()
{
	
}