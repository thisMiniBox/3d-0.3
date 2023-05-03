#include "project.h"
Controller::Controller()
{
	m_RootFolder.SetName("根目录");
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
			L"窗口类注册失败！",
			L"错误",
			MB_ICONERROR);
	}

	// 执行应用程序初始化:
	hWnd = CreateWindowW(
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
	TEXTWND->CreateWind(hWnd);
	FILEWND->CreateWind(hWnd);
	Folder* a = new Folder("新建项目");
	view = new Camera(
		"新建摄像机", Vector(0, 0, 3), Vector(0, 0, 0), Vector(0, 1, 0), GetRect().right / GetRect().bottom);
	AddObject(a);
	AddObject(view);
	MAINWND->CreateWind(hWnd);
	DETAWND->CreateWind(hWnd);
	OutMessage("使用GDI绘图时，先点击一下摄像机加载侧边框控件，否则无法正常渲染窗口，");
	OutMessage("我也不知道为什么，消息循环是同一个，opengl就没问题");
	RECT m_rect;
	GetClientRect(hWnd, &m_rect);
	int cxClient = m_rect.right - m_rect.left;  // 获得客户区宽度
	int cyClient = m_rect.bottom - m_rect.top;
	SETWND.hWnd = CreateWindowW( //创建编辑框
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
	m_RootFolder.ClearFolder_清空文件夹();
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
	m_RootFolder.AddFile_添加文件(a);
	return FILEWND->AddItem(*a, address);
}
HTREEITEM Controller::AddObject(Object* a, HTREEITEM parent)
{
	m_Models.clear();
	m_RootFolder.AddFile_添加文件(a);
	return FILEWND->AddItem(*a, parent);
}
Object* Controller::CreateObject(Folder* parent, std::string name, int type)
{
	Object* out = nullptr;
	if (!parent)
		out = m_RootFolder.CreateFile_创建文件(name, type);
	else
		out = parent->CreateFile_创建文件(name, type);
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
	m_Models = m_RootFolder.GetAllModleFile_找到所有模型();
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
	m_RootFolder.DeleteFile_删除文件(obj);
	UpdateModels();
	if (hTree)
		FILEWND->DeleteItem(hTree);
	m_Models.clear();
	InvalidateRect(MAINWND->GethWnd(), NULL, true);
}

ReturnedOfLoadFile Controller::LoadFile(const std::wstring& path)
{
	OutMessage(L"开始加载文件");
	std::wstring extension = path.substr(path.find_last_of('.') + 1);
	ReturnedOfLoadFile error = ReturnedOfLoadFile::_Fail;
	if (extension == L"obj")
	{
		char narrowPath[MAX_PATH];
		int pathLength = WideCharToMultiByte(CP_ACP, 0, path.c_str(), -1, narrowPath, MAX_PATH, nullptr, nullptr);
		if (pathLength == 0) {
			std::cout << "字符串转换错误" << std::endl;
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
//	OutMessage("开始加载模型");
//	Model* model = new Model;
//	int error = model->loadModelFile_加载模型文件(path);
//	if ((error | 0xff00) == ReturnedOfLoadFile::_Succese)
//	{
//		m_RootFolder.AddFile_添加文件(model);
//		FILEWND->AddItem(*model);
//		m_Models.clear();
//	}
//	else
//	{
//		delete model;
//	}
//	return error;
//}
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

	ReturnedOfLoadFile Error =ReturnedOfLoadFile::_Fail;
	char c = 0;
	size_t charLocate = 0;
	size_t m = 0;

	Model* NewModel = nullptr;

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
					charLocate = (FileData = FileData.substr(charLocate)).find_first_of(' ');
					NewModel->GetMesh()->m_FaceIndices.push_back(faceData(FileData));
				}
				else if (w == "g" || w == "o" || w == "mg")
				{
					charLocate = (FileData = FileData.substr(charLocate)).find_first_not_of(' ');
					FileData = FileData.substr(charLocate);
					std::string ModelName = FileData + "网格模型";
					NewModel = new Model(ModelName);
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
	std::unordered_map<int, int> vertex_index;
	std::unordered_map<int, int> normal_index;
	std::unordered_map<int, int> texcoord_index;

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
	m_RootFolder.AddFile_添加文件(MObj);
	Folder* ModelFolder = dynamic_cast<Folder*>(m_RootFolder.CreateFile_创建文件(filePath.substr(pos + 1) + "资源", OT_FOLDER));
	Folder* ModelMeshFolder = dynamic_cast<Folder*>(ModelFolder->CreateFile_创建文件("网格文件夹", OT_FOLDER));
	Folder* ModelMaterialFolder = dynamic_cast<Folder*>(ModelFolder->CreateFile_创建文件("材质文件夹", OT_FOLDER));
	MObj->SetName(filePath.substr(pos + 1));
	for (auto& child : Models)
	{
		MObj->addChildModel(child);
		ModelMeshFolder->AddFile_添加文件(child->GetMesh());
	}
	for (auto& Mat : mtl_材质->m_Materials)
	{
		ModelMaterialFolder->AddFile_添加文件(Mat.second);
	}
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
void Controller::UpdateRightWind()
{
	
}