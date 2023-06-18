#include "project.h"
Controller::Controller()
{
	m_RootFolder.SetName("根目录");
	m_FocusFolder = &m_RootFolder;
	m_Focus = &m_RootFolder;
	m_Mode = RUNMODE::RM_EDIT;
	m_FileLoad = false;
	m_ImageList = nullptr;
	m_FileWind = nullptr;
	m_TextWind = nullptr;
	m_BottomWind = nullptr;
	m_MainWind = new GDIWND;
	m_EditWind = new DetaileWind;
	m_hWnd = nullptr;
	m_IOWind = nullptr;
	m_hInst = nullptr;
	view = nullptr;
	m_ActualMaxFPS = 0;
	m_SetFPS = 60;
	Model_att = 0;
	m_hDll = nullptr;
	MainWindUserCode = nullptr;
	m_KeyframeWind = nullptr;
	m_StartTime = 0;
	m_RunTime = 0;
}
HWND Controller::GetBottomWindhWnd()const
{
	return m_BottomWind->GethWnd();
}
void Controller::Size(int cxClient, int cyClient)
{
	MoveWindow(m_BottomWind->GethWnd(), 0, cyClient - 150, cxClient / 5 * 4, 150, true);
	MoveWindow(m_FileWind->GethWnd(), 0, 50, cxClient / 5, cyClient - 200, true);
	MoveWindow(m_EditWind->GethWnd(), cxClient / 5 * 4, 50, cxClient / 5, cyClient - 5, true);
	MoveWindow(m_MainWind->GethWnd(), cxClient / 5, 50, cxClient / 5 * 3, cyClient - 200, true);
	MoveWindow(m_MenuWind.hWnd, 0, 0, cxClient, 50, true);
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
	m_TextWind = new TextOutWind(hInst);
	m_FileWind = new FileWind(hInst);
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
			L"窗口类注册失败！",
			L"错误",
			MB_ICONERROR);
	}

	// 执行应用程序初始化:
	m_hWnd = CreateWindowW(
		L"szWindowClass",
		L"win32模型框架",
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

	//创建底部折叠窗口
	m_BottomWind = new BottomWindow(hInst, m_hWnd);
	//创建文本提示输出窗口
	m_TextWind->CreateWind(m_BottomWind->GethWnd());
	//创建控制台窗口
	m_IOWind = new InputOutput(hInst, m_BottomWind->GethWnd());
	//创建关键帧编辑窗口
	m_KeyframeWind = new KeyframeEdit(m_hInst, m_BottomWind->GethWnd());

	//添加控制台窗口到底部折叠窗口显示
	m_BottomWind->AddWind(m_IOWind->GethWnd(), L"控制台");
	//添加消息文本窗口到底部折叠窗口显示
	m_BottomWind->AddWind(m_TextWind->GethWnd(), L"消息窗口");
	//添加关键帧编辑窗口到底部折叠窗口显示
	m_BottomWind->AddWind(m_KeyframeWind->GethWnd(), L"关键帧");

	m_FileWind->CreateWind(m_hWnd);
	Folder* a = new Folder("新建项目");
	view = new Camera(
		"新建摄像机", Vector(0, 0, 3), Vector(0, 0, 0), Vector(0, 1, 0), GetRect().right / GetRect().bottom);
	AddObject(a);
	AddObject(view);
	m_MainWind->CreateWind(m_hWnd);
	m_EditWind->CreateWind(m_hWnd);
	RECT m_rect;
	GetClientRect(m_hWnd, &m_rect);
	int cxClient = m_rect.right - m_rect.left;  // 获得客户区宽度
	int cyClient = m_rect.bottom - m_rect.top;
	m_MenuWind.hWnd = CreateWindowW( //创建编辑框
		m_MenuWind.className.c_str(),
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
	SetFoucusObjcet(&m_RootFolder);

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
void Controller::SetRunMode(RUNMODE mode)
{
	m_Mode = mode;
}
RUNMODE Controller::GetRunMode()const
{
	return m_Mode;
}
int Controller::GetImageListIndex(int id)
{
	return m_ImageIndex[id];
}
Controller::~Controller()
{
	if (m_MainWind)delete m_MainWind;
	if (m_EditWind)delete m_EditWind;
	if (m_FileWind)delete m_FileWind;
	if (m_TextWind)delete m_TextWind;
	if (m_BottomWind)delete m_BottomWind;
	if (m_hDll)FreeLibrary(m_hDll);
	if (m_KeyframeWind)delete m_KeyframeWind;
	m_RootFolder.ClearFolder_清空文件夹();
}
void Controller::SetFileName(Object* obj, const std::wstring& NewName)
{
	m_FocusFolder->SetFileName(obj, wstr_str(NewName));
	m_FileWind->FixItemName(m_EditWind->GetTree(), NewName.c_str());
}
HINSTANCE Controller::GethInstance()const
{
	return m_hInst;
}
Object* Controller::GetFocusObject()const
{
	return m_Focus;
}
void Controller::OutMessage(const std::string&str, MSGtype type)
{
	m_TextWind->OutMessage(str, type);
}
void Controller::OutMessage(const std::wstring& str, MSGtype type)
{
	m_TextWind->OutMessage(str, type);
}
void Controller::updateMsg(const HDC& hdc)
{
	m_TextWind->DrawWind(hdc);
}
HTREEITEM Controller::AddObject(Object* a, std::string address)
{
	if (a->GetType() == OT_MODEL)
		m_Models.clear();
	m_RootFolder.AddFile_添加文件(a);
	return m_FileWind->AddItem(*a, address);
}
HTREEITEM Controller::AddObject(Object* a, HTREEITEM parent)
{
	m_Models.clear();
	m_RootFolder.AddFile_添加文件(a);
	return m_FileWind->AddItem(*a, parent);
}
Object* Controller::CreateObject(Folder* parent, std::string name, ObjectType type)
{
	Object* out = nullptr;
	switch (type)
	{
	case OT_FOLDER:
	{
		if (!parent)
			out = m_RootFolder.CreateFile_创建文件<Folder>(name);
		else
			out = parent->CreateFile_创建文件<Folder>(name);
	}
	break;

	case OT_MODEL:
	{
		if (!parent)
			out = m_RootFolder.CreateFile_创建文件<Model>(name);
		else
			out = parent->CreateFile_创建文件<Model>(name);
	}
	break;

	case OT_CAMERA:
	{
		if (!parent)
			out = m_RootFolder.CreateFile_创建文件<Camera>(name);
		else
			out = parent->CreateFile_创建文件<Camera>(name);
	}
	break;

	case OT_MESH:
	{
		if (!parent)
			out = m_RootFolder.CreateFile_创建文件<Mesh>(name);
		else
			out = parent->CreateFile_创建文件<Mesh>(name);
	}
	break;

	case OT_PICTURE:
	{
		if (!parent)
			out = m_RootFolder.CreateFile_创建文件<Picture>(name);
		else
			out = parent->CreateFile_创建文件<Picture>(name);
	}
	break;

	case OT_MATERIAL:
	{
		if (!parent)
			out = m_RootFolder.CreateFile_创建文件<Material>(name);
		else
			out = parent->CreateFile_创建文件<Material>(name);
	}
	break;

	case OT_POINT_LIGHT:
	{
		if (!parent)
			out = m_RootFolder.CreateFile_创建文件<PointLight>(name);
		else
			out = parent->CreateFile_创建文件<PointLight>(name);
	}
	break;

	case OT_DIRECTIONAL_LIGHT:
	{
		if (!parent)
			out = m_RootFolder.CreateFile_创建文件<DirectionalLight>(name);
		else
			out = parent->CreateFile_创建文件<DirectionalLight>(name);
	}
	break;
	case OT_KEYFRAME:
	{
		//if (!parent)
		//	out = m_RootFolder.CreateFile_创建文件<KeyFrame>(name);
		//else
		//	out = parent->CreateFile_创建文件<KeyFrame>(name);
	}
	break;

	default:
		return nullptr;
		break;
	}
	if (out != nullptr)
	{
		HTREEITEM New = m_FileWind->AddItem(*out, (m_EditWind->GetTree()));
		m_FileWind->ExploreFolder(New);
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
	m_Models = m_RootFolder.GetAllModleFile_找到所有模型();
	return m_Models;
}
std::vector<PointLight*>& Controller::GetAllPointLight()
{
	if (!m_PointLights.empty())return m_PointLights;
	m_PointLights = m_RootFolder.GetAllPointLightFile_找到所有光源文件();
	return m_PointLights;
}
std::vector<Model*>& Controller::UpdateModels()
{
	m_Models.clear();
	return GetModels();
}

void Controller::DeleteObject(Object* obj,HTREEITEM hTree)
{
	if (m_EditWind->GetTarget() == obj)
	{
		m_Focus = obj->GetParent();
		m_EditWind->SetView(m_Focus);
	}
	if (m_MainWind->GetType() == MOPENGL)
	{
		OpenGLWnd* glwind = dynamic_cast<OpenGLWnd*>(m_MainWind);
		Model* mod = dynamic_cast<Model*>(obj);
		if (mod)
			glwind->DeleteModelBuffer(mod);
	}
	m_Models.clear();
	m_FocusFolder->DeleteFile_删除文件(obj);
	if (hTree)
		m_FileWind->DeleteItem(hTree);
	InvalidateRect(m_MainWind->GethWnd(), NULL, true);
}

ReturnedOfLoadFile Controller::LoadFile(const std::wstring& path)
{
	OutMessage(L"开始加载文件" + path);
	std::wstring extension = path.substr(path.find_last_of('.'));
	ReturnedOfLoadFile error = ReturnedOfLoadFile::_Fail;
	if (extension == L".obj")
	{
		error |= LoadObj(wstrstr(path));
	}
	else if (this->isSupportedModelFile(wstrstr(extension)))
	{
		if (LoadModel(wstrstr(path)))
			error = _Succese;
		error= _Fail;
	}

	//else if (extension == L".xlsx")
	//{
	//	if (LoadXlsx(path))
	//		error = _Succese;
	//	else
	//		error = _Fail;
	//}
	else if (extension == L".xzcom")
	{
		error |= LoadCommand(path);
	}
	else if (extension == L".dll")
	{
		if (MessageBox(m_hWnd, L"是否链接消息处理程序？", L"", MB_OKCANCEL))
		{
			error |= LoadDLL(path);
		}
		error = _Succese;
	}
	else
		error = ReturnedOfLoadFile::_UnknownFormat;
	if ((error | 0xff00) == ReturnedOfLoadFile::_Succese)
	{
		m_Models.clear();
		m_FileWind->ShowFolder(m_RootFolder,GetFocusObject());
		UpdateFileView();
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

	// 设置当前环境使用本地区设置，以支持中文字符输出
	setlocale(LC_ALL, "");

	wchar_t Line[256];
	while (fgetws(Line, 256, comfile) != NULL)
	{

		if (!Command(Line))
		{
			OutMessage(L"运行错误！行：" + std::to_wstring(line), _Error);
		}
		line++;
	}

	fclose(comfile);
	return ReturnedOfLoadFile::_Succese;
}
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
	std::vector<FaceData_面信息>faceIndices;
	std::vector<Model*>Models;
	MatFileReader* mtl_材质 = new MatFileReader;
	Material* CurrentMaterial = nullptr;

	ReturnedOfLoadFile Error = ReturnedOfLoadFile::_ModelFail;
	char c = 0;
	size_t charLocate = 0;
	size_t m = 0;

	Model* NewModel = nullptr;
	Model* RootModel = new Model;
	RootModel->SetName(filePath.substr(pos + 1));
	Model* parentModel = RootModel;

	vec::Vector vector;
	vec::Vector2 vector2d;
	FaceData_面信息 Face;
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
				//获取了一行信息后进行解析
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
					if (FileData == "default")
						parentModel = RootModel;
					std::string ModelName = FileData + "网格模型";
					NewModel = new Model(ModelName);
					parentModel->addChildModel(NewModel);
					NewModel->SetMesh(new Mesh(FileData));
					NewModel->SetMaterial(CurrentMaterial);
					Models.push_back(NewModel);
				}
				else if (w == "usemtl")
				{
					if (!mtl_材质)
					{
						FileData.clear();
						continue;
					}
					if (!NewModel)return ReturnedOfLoadFile::_FailedToCreateFile;
					charLocate = (FileData = FileData.substr(charLocate)).find_first_not_of(' ');
					FileData = FileData.substr(charLocate);
					//NewModel->SetMaterial(mtl_材质->getMaterial(FileData));
					CurrentMaterial = mtl_材质->getMaterial(FileData);
					NewModel->SetMaterial(CurrentMaterial);
				}
				else if (w == "mtllib")
				{
					charLocate = (FileData = FileData.substr(charLocate)).find_first_not_of(' ');
					if (!mtl_材质->read(FolderPath.c_str() + FileData.substr(charLocate)))
					{
						delete mtl_材质;
						mtl_材质 = nullptr;
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
	std::vector<FaceData_面信息> child_face_data;
	std::map<int, int> vertex_index;
	std::map<int, int> normal_index;
	std::map<int, int> texcoord_index;

	// 遍历子模型
	for (auto child : Models)
	{
		vertex_index.clear();
		normal_index.clear();
		texcoord_index.clear();
		child_vertex_data.clear();
		child_normal_data.clear();
		child_texcoord_data.clear();
		child_face_data.clear();

		// 获取子模型需要的顶点、法向量和纹理坐标数据
		for (const auto& face : child->GetMesh()->m_FaceIndices)
		{
			FaceData_面信息 new_face = face;  // 复制原始三角面

			// 修改三角面中的索引，以反映新的数据位置
			for (int i = 0; i < 3; ++i)  // 顶点索引
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

			for (int i = 0; i < 3; ++i)  // 法向量索引
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

			for (int i = 0; i < 3; ++i)  // 纹理坐标索引
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

			// 将修改后的三角面添加到子模型的面数据中
			child_face_data.push_back(new_face);
		}

		// 将数据分配给子模型

		child->GetMesh()->m_VertexPosition = (child_vertex_data);
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
	m_RootFolder.AddFile_添加文件(MObj);
	Folder* ModelFolder = dynamic_cast<Folder*>(m_RootFolder.CreateFile_创建文件<Folder>(filePath.substr(pos + 1) + "资源"));
	Folder* ModelMeshFolder = dynamic_cast<Folder*>(ModelFolder->CreateFile_创建文件<Folder>("网格文件夹"));
	Folder* ModelMaterialFolder = dynamic_cast<Folder*>(ModelFolder->CreateFile_创建文件<Folder>("材质文件夹"));

	for (auto& child : Models)
	{
		MObj->addChildModel(child);
		ModelMeshFolder->AddFile_添加文件(child->GetMesh());
	}
	if (mtl_材质)
		for (auto& Mat : mtl_材质->m_Materials)
		{
			ModelMaterialFolder->AddFile_添加文件(Mat.second);
		}
	if (mtl_材质)
		for (auto& picture : mtl_材质->m_Picture)
		{
			ModelMaterialFolder->AddFile_添加文件(picture.second);
		}
	fclose(file);
	if (mtl_材质)
	{
		Error |= ReturnedOfLoadFile::_SuccessfullyLoadedMaterialFile;
		delete mtl_材质;
		mtl_材质 = nullptr;
	}
	return Error;
}
void Controller::UpdateFileView()const
{
	m_FileWind->ShowFolder(m_RootFolder, GetFocusObject());
}
void Controller::UpdateDetaileViev()const
{
	m_EditWind->UpDate();
}
void Controller::UpdateKeyframeView(ChildWindSign cws)const
{
	m_KeyframeWind->UpdateView(cws);
}
//void Controller::UpdateBottomView()const
//{
//	m_BottomWind->Update();
//}
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
void loadFileThread(HWND hWnd, Controller* current_project, std::wstring path)
{
	//rwlock.lock();
	current_project->m_FileLoad = true;
	unsigned int Error = current_project->LoadFile(path);
	current_project->Model_att = 0x01;

	if (Error == ReturnedOfLoadFile::_Succese)
	{
		current_project->OutMessage(L"加载完毕：" + path, _Message);
		current_project->Model_att = 0x01;
	}
	else if ((Error & 0xff) != 0)
	{
		switch (Error & 0xff)
		{
		case ReturnedOfLoadFile::_UnknownFormat:
			current_project->OutMessage(L"未知文件格式：" + path, _Error);
			break;
		case ReturnedOfLoadFile::_DataError:
			current_project->OutMessage(L"数据错误：" + path, _Error);
			break;
		case ReturnedOfLoadFile::_FailToOpenFile:
			current_project->OutMessage(L"无法打开文件：" + path, _Error);
			break;
		case ReturnedOfLoadFile::_FailedToCreateFile:
			current_project->OutMessage(L"文件创建失败：" + path, _Error);
			break;
		default:
			current_project->OutMessage(L"模型加载时发生未知错误：" + path, _Error);
			break;
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
			out = (searchP->FindFile_寻找文件(wstr_str(strand)));
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
		m_EditWind->SetView(aim);
		aim->Selected();
		UpdateKeyframeView();
		m_Focus = aim;
	}
}
std::map<int, int>& Controller::GetImageListIndex()
{
	return m_ImageIndex;
}
//设置帧数
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
//获取当前帧数
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
			OutMessage("链接成功");
		}
		else
			OutMessage("函数导出失败", _Error);
	}
	else
		OutMessage("DLL加载失败", _Error);
	return _Succese;
}
KeyframeEdit* Controller::GetKeyframeWind()const
{
	return m_KeyframeWind;
}
void Controller::GetTime(ULONG64* left, ULONG64* right)const
{
	m_KeyframeWind->GetTime(left, right);
}
ULONG64 Controller::GetStartTime()const
{
	return m_StartTime;
}
void Controller::Run()
{
	m_Mode = RUNMODE::RM_RUN;
	m_StartTime = GetTickCount64() - m_RunTime;
}
void Controller::Suspend()
{
	m_Mode = RUNMODE::RM_EDIT;
}
void Controller::Stop()
{
	m_Mode = RUNMODE::RM_EDIT;
	SetTime(0);
}
void Controller::SwitchRunMode()
{
	if (m_Mode == RUNMODE::RM_RUN)
		Suspend();
	else
		Run();
}
void Controller::Reset()
{
	m_Mode = RUNMODE::RM_EDIT;
	m_StartTime = 0;
	m_RunTime = 0;
}
void Controller::SetKeyframeLoop(bool b)
{
	if (m_Focus)
		m_Focus->SetKeyframeLoop(b);
}
void Controller::SwitchKeyframeLoop()
{
	if (m_Focus)
		m_Focus->SetKeyframeLoop(!m_Focus->GetKeyframeLoop());
}
bool Controller::GetKeyframeLoop()const
{
	if (m_Focus)
		return m_Focus->GetKeyframeLoop();
	return false;
}
int Controller::GetKeyframeEditY()const
{
	return m_KeyframeWind->GetY();
}
void Controller::MoveKeyframeEditY(int y)
{
	m_KeyframeWind->MoveY(y);
}
void Controller::MoveKeyframeEditTime(int x)
{
	m_KeyframeWind->MoveTime(x);
}
ULONG64 Controller::GetTime()
{
	if (m_Mode == RUNMODE::RM_EDIT)
		return m_RunTime;
	m_RunTime = GetTickCount64() - m_StartTime;
	//ULONG64 leftTime, rightTime;
	//GetTime(&leftTime, &rightTime);
	//if (m_RunTime < leftTime)
	//{
	//	int x = m_RunTime - leftTime - 500;
	//	MoveKeyframeEditTime(x);
	//}
	//else if (m_RunTime > rightTime)
	//{
	//	int x = m_RunTime - rightTime + 500;
	//	MoveKeyframeEditTime(x);
	//}
	return m_RunTime;
}
void Controller::SetTime(ULONG64 time)
{
	m_RunTime = time;
	m_StartTime = GetTickCount64() - time;
	ULONG64 leftTime, rightTime;
	GetTime(&leftTime, &rightTime);
	if (time < leftTime)
	{
		int x = time - leftTime - 500;
		MoveKeyframeEditTime(x);
	}
	else if (time > rightTime)
	{
		int x = time - rightTime + 500;
		MoveKeyframeEditTime(x);
	}
}
//bool Controller::LoadXlsx(const std::wstring& filePath)
//{
//	xlnt::workbook wb;
//	try
//	{
//		wb.load(filePath); // 文件名可以根据实际情况修改
//	}
//	catch (const std::exception& ex)
//	{
//		OutMessage(L"文件打开失败", _Error);
//		return false;
//	}
//
//	// 遍历所有工作表
//	for (const auto& ws : wb)
//	{
//		std::cout << "Sheet: " << ws.title() << std::endl;
//
//		// 遍历所有行和列，并输出每个单元格的值
//		for (const auto& row : ws.rows())
//		{
//			for (auto cell : row)
//			{
//				std::cout << cell.to_string() << "\t";
//			}
//			std::cout << std::endl;
//		}
//	}
//}
bool Controller::LoadModel(const std::string& path)
{
	// 读取模型文件到内存缓冲区
	std::ifstream file(path, std::ios::binary);
	if (!file) {
		OutMessage(L"无法打开文件", _Error);
		return false;
	}
	std::vector<char> buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	size_t pos = path.find_last_of('/');
	if (pos == -1)
	{
		pos = path.find_last_of('\\');
	}
	// 加载模型
	Assimp::Importer aimporter;
	const aiScene* scene = aimporter.ReadFileFromMemory(buffer.data(), buffer.size(),
		aiProcess_Triangulate | aiProcess_FlipUVs, nullptr);
	//const aiScene* scene = aimporter.ReadFile(path, aiProcess_Triangulate | aiProcess_GenNormals);
	std::string Output;
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		cout << "ERROR::ASSIMP::" << aimporter.GetErrorString() << endl;
		Output = "加载出错";
		Output += aimporter.GetErrorString();
		OutMessage(Output, _Error);
		aimporter.FreeScene();
		return false;
	}
	aiNode* node = scene->mRootNode;
	Output = path.substr(pos + 1);
	if(!node)
	{
		OutMessage(L"模型未包含节点", _Error);
		std::cout << "模型未包含节点" << std::endl;
		aimporter.FreeScene();
		return false;
	}
	Folder* folder = m_FocusFolder->CreateFile_创建文件<Folder>(Output);
	if (!folder)
	{
		OutMessage(L"文件夹创建失败", _Error);
		std::cout << "文件夹创建失败" << std::endl;
		aimporter.FreeScene();
		return false;
	}
	Model* parentModel = dynamic_cast<Model*>(this->CreateObject(folder, Output, OT_MODEL));
	if (!parentModel)
	{
		OutMessage(L"模型创建失败", _Error);
		std::cout << "模型创建失败" << std::endl;
		aimporter.FreeScene();
		return false;
	}
	if (!processModelNode(node, scene, parentModel, folder, path.substr(0, pos + 1)))
	{
		aimporter.FreeScene();
		return false;
	}
	aimporter.FreeScene();
	return true;
}
bool Controller::processModelNode(aiNode* node, const aiScene* scene, Model* parentModel, Folder* folder,const std::string& directory)
{
	Model* currentModel = parentModel;
	std::string Output = node->mName.C_Str();
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		currentModel = new Model(Output);
		if (!currentModel)
		{
			OutMessage(L"模型创建失败", _Error);
			std::cout << "模型创建失败" << std::endl;
			return false;
		}
		parentModel->addChildModel(currentModel);
		aiMesh* amesh = scene->mMeshes[node->mMeshes[i]];
		Mesh* mesh = dynamic_cast<Mesh*>(this->CreateObject(folder, Output + "网格", OT_MESH));
		if (!mesh)
		{
			OutMessage(L"网格创建失败", _Error);
			std::cout << "网格创建失败" << std::endl;
			return false;
		}
		mesh->processMesh(amesh, scene);
		currentModel->SetMesh(mesh);
		if (amesh->mMaterialIndex >= 0)
		{
			Material* material = dynamic_cast<Material*>(this->CreateObject(folder, Output + "材质", OT_MATERIAL));
			if (!material)
			{
				OutMessage(L"材质创建失败", _Error);
				std::cout << "材质创建失败" << std::endl;
				return false;
			}
			material->processMaterial(amesh, scene,folder, directory);
			currentModel->SetMaterial(material);
		}
	}
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		if (!processModelNode(node->mChildren[i], scene, currentModel, folder, directory))
		{
			return false;
		}
	}
}
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
bool Controller::isSupportedModelFile(const std::string& extension)
{
	static Assimp::Importer importer;
	return importer.IsExtensionSupported(extension.c_str());
}

