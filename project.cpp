#include "project.h"
Controller::Controller()
{
	m_RootFolder.SetName("��Ŀ¼");
	m_FocusFolder = &m_RootFolder;
	m_FileLoad = false;
	m_ImageList = nullptr;
	FILEWND = nullptr;
	TEXTWND = nullptr;
	m_BottomWind = nullptr;
	m_MainWind = new GDIWND;
	DETAWND = new DetaileWind;
	m_hWnd = nullptr;
	m_IOWind = nullptr;
	m_hInst = nullptr;
	view = nullptr;
	m_ActualMaxFPS = 0;
	m_SetFPS = 60;
	Model_att = 0;
	m_hDll = nullptr;
	MainWindUserCode = nullptr;
}
HWND Controller::GetBottomWindhWnd()const
{
	return m_BottomWind->GethWnd();
}
void Controller::Size(int cxClient, int cyClient)
{
	MoveWindow(m_BottomWind->GethWnd(), 0, cyClient - 150, cxClient / 5 * 4, 150, true);
	MoveWindow(FILEWND->GethWnd(), 0, 50, cxClient / 5, cyClient - 200, true);
	MoveWindow(DETAWND->GethWnd(), cxClient / 5 * 4, 50, cxClient / 5, cyClient - 5, true);
	MoveWindow(m_MainWind->GethWnd(), cxClient / 5, 50, cxClient / 5 * 3, cyClient - 200, true);
	MoveWindow(SETWND.hWnd, 0, 0, cxClient, 50, true);
}
void Controller::AddShader(int ID, std::string vsPath, std::string fsPath)
{
	m_ShaderIDPath.insert(std::make_pair(ID, ShaderPath(vsPath,fsPath)));
}
BottomWindow* Controller::GetBottom()
{
	return m_BottomWind;
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
	m_hWnd = CreateWindowW(
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

	m_BottomWind = new BottomWindow(hInst, m_hWnd);
	TEXTWND->CreateWind(m_BottomWind->GethWnd());
	m_BottomWind->AddWind(TEXTWND->GethWnd(), L"��Ϣ����");
	m_IOWind = new InputOutput(hInst, m_BottomWind->GethWnd());
	m_BottomWind->AddWind(m_IOWind->GethWnd(), L"����̨");

	FILEWND->CreateWind(m_hWnd);
	Folder* a = new Folder("�½���Ŀ");
	view = new Camera(
		"�½������", Vector(0, 0, 3), Vector(0, 0, 0), Vector(0, 1, 0), GetRect().right / GetRect().bottom);
	AddObject(a);
	AddObject(view);
	m_MainWind->CreateWind(m_hWnd);
	DETAWND->CreateWind(m_hWnd);
	RECT m_rect;
	GetClientRect(m_hWnd, &m_rect);
	int cxClient = m_rect.right - m_rect.left;  // ��ÿͻ������
	int cyClient = m_rect.bottom - m_rect.top;
	SETWND.hWnd = CreateWindowW( //�����༭��
		SETWND.className.c_str(),
		0,
		WS_CHILD | WS_BORDER | WS_VISIBLE | ES_MULTILINE,
		0, 0, cxClient, 50,
		m_hWnd,
		(HMENU)2,
		hInst,
		nullptr);
	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);
	Command(L"");
	DETAWND->SetView(&m_RootFolder);

	m_ImageList = ImageList_Create(64, 64, ILC_COLOR32, 7, 0);
	LoadPngFromResources(IDB_UNKNOWN64);
	LoadPngFromResources(IDB_FOLDER64);
	LoadPngFromResources(IDB_MODEL64);
	LoadPngFromResources(IDB_CAMERA64);
	LoadPngFromResources(IDB_MESH64);
	LoadPngFromResources(IDB_PICTURE64);
	LoadPngFromResources(IDB_MATERIAL64);
	return m_hWnd;
}
void Controller::LoadPngFromResources(int png)
{
	m_ImageIndex[png] = LoadPngToList(png, m_ImageList, m_hInst);
}
HIMAGELIST Controller::GetImageList()const
{
	return m_ImageList;
}
int Controller::GetImageListIndex(int id)
{
	return m_ImageIndex[id];
}
Controller::~Controller()
{
	if (m_MainWind)delete m_MainWind;
	if (DETAWND)delete DETAWND;
	if (FILEWND)delete FILEWND;
	if (TEXTWND)delete TEXTWND;
	if (m_BottomWind)delete m_BottomWind;
	if (m_hDll)FreeLibrary(m_hDll);
	m_RootFolder.ClearFolder_����ļ���();
}
void Controller::SetFileName(Object* obj, const std::wstring& NewName)
{
	m_FocusFolder->SetFileName(obj, wstr_str(NewName));
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
Object* Controller::CreateObject(Folder* parent, std::string name, ObjectType type)
{
	Object* out = nullptr;
	switch (type)
	{
	case OT_FOLDER:
	{
		if (!parent)
			out = m_RootFolder.CreateFile_�����ļ�<Folder>(name);
		else
			out = parent->CreateFile_�����ļ�<Folder>(name);
	}
	break;

	case OT_MODEL:
	{
		if (!parent)
			out = m_RootFolder.CreateFile_�����ļ�<Model>(name);
		else
			out = parent->CreateFile_�����ļ�<Model>(name);
	}
	break;

	case OT_CAMERA:
	{
		if (!parent)
			out = m_RootFolder.CreateFile_�����ļ�<Camera>(name);
		else
			out = parent->CreateFile_�����ļ�<Camera>(name);
	}
	break;

	case OT_MESH:
	{
		if (!parent)
			out = m_RootFolder.CreateFile_�����ļ�<Mesh>(name);
		else
			out = parent->CreateFile_�����ļ�<Mesh>(name);
	}
	break;

	case OT_PICTURE:
	{
		if (!parent)
			out = m_RootFolder.CreateFile_�����ļ�<Picture>(name);
		else
			out = parent->CreateFile_�����ļ�<Picture>(name);
	}
	break;

	case OT_MATERIAL:
	{
		if (!parent)
			out = m_RootFolder.CreateFile_�����ļ�<Material>(name);
		else
			out = parent->CreateFile_�����ļ�<Material>(name);
	}
	break;

	case OT_POINTLIGHT:
	{
		if (!parent)
			out = m_RootFolder.CreateFile_�����ļ�<PointLight>(name);
		else
			out = parent->CreateFile_�����ļ�<PointLight>(name);
	}
	break;

	case OT_PARALLELLIGHT:
	{
		//if (!parent)
		//	out = m_RootFolder.CreateFile_�����ļ�<ParallelLight>(name);
		//else
		//	out = parent->CreateFile_�����ļ�<ParallelLight>(name);
	}
	break;
	case OT_KEYFRAME:
	{
		//if (!parent)
		//	out = m_RootFolder.CreateFile_�����ļ�<KeyFrame>(name);
		//else
		//	out = parent->CreateFile_�����ļ�<KeyFrame>(name);
	}
	break;

	default:
		return nullptr;
		break;
	}
	if (out != nullptr)
	{
		HTREEITEM New = FILEWND->AddItem(*out, (DETAWND->GetTree()));
		DETAWND->SetView(out);
		DETAWND->SetTree(New);
		FILEWND->ExploreFolder(New);
	}

	return out;
}

RECT Controller::GetRect()const
{
	RECT rect;
	GetClientRect(m_hWnd, &rect);
	return rect;
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
	if (m_MainWind->GetType() == MOPENGL && obj->GetType() == OT_MODEL)
	{
		OpenGLWnd* glwind = dynamic_cast<OpenGLWnd*>(m_MainWind);
		Model* mod = dynamic_cast<Model*>(obj);
		glwind->DeleteModelBuffer(mod);
	}
	m_FocusFolder->DeleteFile_ɾ���ļ�(obj);
	UpdateModels();
	if (hTree)
		FILEWND->DeleteItem(hTree);
	m_Models.clear();
	InvalidateRect(m_MainWind->GethWnd(), NULL, true);
}

ReturnedOfLoadFile Controller::LoadFile(const std::wstring& path)
{
	OutMessage(L"��ʼ�����ļ�" + path);
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
	else if (extension == L"xzcom")
	{
		error |= LoadCommand(path);
	}
	else if (extension == L"dll")
	{
		if (MessageBox(m_hWnd, L"�Ƿ�������Ϣ�������", L"", MB_OKCANCEL))
		{
			error |= LoadDLL(path);
		}
		return _Succese;
	}
	else
		return ReturnedOfLoadFile::_UnknownFormat;
	if ((error | 0xff00) == ReturnedOfLoadFile::_Succese)
	{
		m_Models.clear();
		FILEWND->ShowFolder(m_RootFolder,GetFocusObject());
	}
	return error;
	
}
ReturnedOfLoadFile Controller::LoadCommand(const std::wstring& filePath)
{
	size_t line = 1;
	FILE* comfile;
	_wfopen_s(&comfile, filePath.c_str(), L"r, ccs=UTF-8");
	if (!comfile)
		return ReturnedOfLoadFile::_FailToOpenFile;

	// ���õ�ǰ����ʹ�ñ��������ã���֧�������ַ����
	setlocale(LC_ALL, "");

	wchar_t Line[256];
	while (fgetws(Line, 256, comfile) != NULL)
	{

		if (!Command(Line))
		{
			OutMessage(L"���д����У�" + std::to_wstring(line), _Error);
		}
		line++;
	}

	fclose(comfile);
	return ReturnedOfLoadFile::_Succese;
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

	ReturnedOfLoadFile Error =ReturnedOfLoadFile::_ModelFail;
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
					charLocate = (FileData = FileData.substr(charLocate)).find_last_not_of(' ');
					FileData = FileData.substr(0, charLocate + 1); 
					FileData += ' ';
					std::vector<std::string>vertexIncludeInLine;
					while (1)
					{
						charLocate = FileData.find_first_of(' ');
						if (charLocate == 0xFFFFFFFFFFFFFFFF)break;
						vertexIncludeInLine.push_back(FileData.substr(0, charLocate));
						FileData = FileData.substr(charLocate + 1);
					}
					for (int i = 2; i < vertexIncludeInLine.size(); i++)
					{
						NewModel->GetMesh()->m_FaceIndices.push_back(faceData(vertexIncludeInLine[0] + ' ' + vertexIncludeInLine[i - 1] + ' ' + vertexIncludeInLine[i]));
					}
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
				if (index < 0)
					index += (vertexs.size() + 1);
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
				if (index < 0)
					index += (normals.size() + 1);
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
				if (index < 0)
					index += (texCoords.size() + 1);
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
	MObj->SetName(filePath.substr(pos + 1));
	m_RootFolder.AddFile_����ļ�(MObj);
	Folder* ModelFolder = dynamic_cast<Folder*>(m_RootFolder.CreateFile_�����ļ�<Folder>(filePath.substr(pos + 1) + "��Դ"));
	Folder* ModelMeshFolder = dynamic_cast<Folder*>(ModelFolder->CreateFile_�����ļ�<Folder>("�����ļ���"));
	Folder* ModelMaterialFolder = dynamic_cast<Folder*>(ModelFolder->CreateFile_�����ļ�<Folder>("�����ļ���"));
	
	for (auto& child : Models)
	{
		MObj->addChildModel(child);
		ModelMeshFolder->AddFile_����ļ�(child->GetMesh());
	}
	if (mtl_����)
		for (auto& Mat : mtl_����->m_Materials)
		{
			ModelMaterialFolder->AddFile_����ļ�(Mat.second);
		}
	if (mtl_����)
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
void Controller::UpdateFileView()const
{
	FILEWND->ShowFolder(m_RootFolder, GetFocusObject());
}
void Controller::UpdateDetaileViev()const
{
	DETAWND->UpDate();
}
InputOutput* Controller::GetIOWind()
{
	return m_IOWind;
}
void Print(const std::string&);
void Controller::Print(const std::wstring& wstr)
{
	m_IOWind->OutputString(wstr);
	m_IOWind->Clear();
}
void loadModelThread(HWND hWnd, Controller* current_project, std::wstring path)
{
	//rwlock.lock();
	current_project->m_FileLoad = true;
	unsigned int Error = current_project->LoadFile(path);
	if (Error == ReturnedOfLoadFile::_Succese)
	{
		current_project->OutMessage(L"������ϣ�" + path, _Message);
		current_project->Model_att = 0x01;
	}
	else if ((Error & 0xff) != 0)
	{
		switch (Error & 0xff)
		{
		case ReturnedOfLoadFile::_UnknownFormat:
			current_project->OutMessage(L"δ֪�ļ���ʽ��" + path, _Error);
			break;

		case ReturnedOfLoadFile::_DataError:
			current_project->OutMessage(L"���ݴ���" + path, _Error);
			break;

		case ReturnedOfLoadFile::_FailToOpenFile:
			current_project->OutMessage(L"�޷����ļ���" + path, _Error);
			break;

		case ReturnedOfLoadFile::_FailedToCreateFile:
			current_project->OutMessage(L"�ļ�����ʧ�ܣ�" + path, _Error);
			break;
		default:
			current_project->OutMessage(L"ģ�ͼ���ʱ����δ֪����" + path, _Error);
			break;
		}
	}
	else
	{
		if ((Error & ReturnedOfLoadFile::_SuccessfullyLoadedVertex) != ReturnedOfLoadFile::_SuccessfullyLoadedVertex)
		{
			current_project->OutMessage(L"���ض�������ʧ�ܣ�" + path, _Warning);
		}
		if ((Error & ReturnedOfLoadFile::_SuccessfullyLoadedMaterialFile) != ReturnedOfLoadFile::_SuccessfullyLoadedMaterialFile)
		{
			current_project->OutMessage(L"���ز����ļ�ʧ�ܣ�" + path, _Warning);
		}
		if ((Error & ReturnedOfLoadFile::_SuccessfullyLoadedMaterialMaps) != ReturnedOfLoadFile::_SuccessfullyLoadedMaterialMaps)
		{
			current_project->OutMessage(L"���ز�����ͼʧ�ܣ�" + path, _Warning);
		}
	}
	current_project->m_FileLoad = false;
	//rwlock.unlock();
	return;
}
Object* Controller::SearchObject(std::wstring filename)
{
	if (filename.empty())return nullptr;
	Folder* searchP = nullptr;
	if (filename[0] == L':')
	{
		searchP = &m_RootFolder;
		filename = filename.substr(1);
	}
	else
		searchP = m_FocusFolder;
	if (filename[0] == L'/')
		filename = filename.substr(1);
	if (filename.back() == L'\0')
		filename = filename.substr(0, filename.size() - 1);
	if (filename.back() != L'/')
		filename += L'/';
	std::wstring strand;
	Object* out = nullptr;
	while (!filename.empty())
	{
		if (!searchP)return nullptr;
		strand = filename.substr(0, filename.find_first_of(L'/'));
		if (strand == L"..")
			out = searchP->GetParent();
		else
			out = (searchP->FindFile_Ѱ���ļ�(wstr_str(strand)));
		if (!out)
		{
			return nullptr;
		}
		searchP = dynamic_cast<Folder*>(out);
		filename = filename.substr(filename.find_first_of(L'/') + 1);
	}
	return out;
}

void Controller::SetFoucusObjcet(Object* aim)
{
	if (aim)
	{
		Object* olda = GetFocusObject();
		if (olda)
			olda->Unselected();
		if (aim->GetParent())
			m_FocusFolder = aim->GetParent();
		else
			m_FocusFolder = &m_RootFolder;
		DETAWND->SetView(aim);
		aim->Selected();
	}
}
std::map<int, int>& Controller::GetImageListIndex()
{
	return m_ImageIndex;
}
//����֡��
void Controller::SetFPS(float fps)
{
	m_SetFPS = fps;
	SendMessage(m_MainWind->GethWnd(), UM_CREATE_TIMER, fps, 0);
}
float FPSNumber = 0;
void Controller::__UpdateFPS(float fps)
{
	m_ActualMaxFPS = fps;
	FPSNumber++;
}
//��ȡ��ǰ֡��
float Controller::GetMaxFPS()const
{
	return m_ActualMaxFPS;
}
float Controller::GetSetFPS()const
{
	return m_SetFPS;
}
MainWind* Controller::GetMainWind()
{
	return m_MainWind;
}
ReturnedOfLoadFile Controller::LoadDLL(const std::wstring& filePath)
{
	if (m_hDll)FreeLibrary(m_hDll);
	m_hDll = LoadLibrary(filePath.c_str());
	if (m_hDll != NULL)
	{
		MainWindUserCode = (UserCode)GetProcAddress(m_hDll, "UserMessage");
		if (MainWindUserCode != NULL)
		{
			OutMessage("���ӳɹ�");
		}
		else
			OutMessage("��������ʧ��", _Error);
	}
	else
		OutMessage("DLL����ʧ��", _Error);
	return _Succese;
}