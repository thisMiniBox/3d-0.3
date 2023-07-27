#include "project.h"
Controller::Controller()
{
	m_RootFolder.SetName("��Ŀ¼");
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
	m_HighLightColor = Vector{ 0.7, 0.7, 0.4 };
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
HWND Controller::InitWindow(HINSTANCE hInst)
{
	m_TextWind = new TextOutWind(hInst);

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
	m_FileWind = new FileWind(m_hWnd, hInst);
	//�����ײ��۵�����
	m_BottomWind = new BottomWindow(hInst, m_hWnd);
	//�����ı���ʾ�������
	m_TextWind->CreateWind(m_BottomWind->GethWnd());
	//��������̨����
	m_IOWind = new InputOutput(hInst, m_BottomWind->GethWnd());
	//�����ؼ�֡�༭����
	m_KeyframeWind = new KeyframeEdit(m_hInst, m_BottomWind->GethWnd());

	//��ӿ���̨���ڵ��ײ��۵�������ʾ
	m_BottomWind->AddWind(m_IOWind->GethWnd(), L"����̨");
	//�����Ϣ�ı����ڵ��ײ��۵�������ʾ
	m_BottomWind->AddWind(m_TextWind->GethWnd(), L"��Ϣ����");
	//��ӹؼ�֡�༭���ڵ��ײ��۵�������ʾ
	m_BottomWind->AddWind(m_KeyframeWind->GethWnd(), L"�ؼ�֡");

	m_BottomWind->Select(L"��Ϣ����");
	Folder* a = new Folder("�½���Ŀ");
	view = new Camera(
		"�½������", Vector(0, 0, 3), Vector(0, 0, 0), Vector(0, 1, 0), GetRect().right / GetRect().bottom);
	AddObject(a);
	AddObject(view);
	m_MainWind->CreateWind(m_hWnd);
	m_EditWind->CreateWind(m_hWnd);
	RECT m_rect;
	GetClientRect(m_hWnd, &m_rect);
	int cxClient = m_rect.right - m_rect.left;  // ��ÿͻ������
	int cyClient = m_rect.bottom - m_rect.top;
	m_MenuWind.hWnd = CreateWindowW( //�����༭��
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

	SetHighLightColor(m_HighLightColor);
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
	m_RootFolder.ClearFolder_����ļ���();
}
void Controller::SetFileName(Object* obj, const std::wstring& NewName)
{
	m_FocusFolder->SetFileName(obj, wstr_str(NewName));
	m_FileWind->FixItemName(obj, NewName.c_str());
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
HTREEITEM Controller::AddObject(Object* a, HTREEITEM parent)
{
	if (!a) {
		std::cout << "����" << "�������Ϊ��" << std::endl;
		OutMessage(L"�������Ϊ��", _Error);
		return nullptr;
	}
	else if (a->GetName().empty()) {
		std::cout << "����" << "�����������Ϊ��" << std::endl;
		OutMessage(L"�����������Ϊ��", _Error);
		return nullptr;
	}
	m_Models.clear();
	m_RootFolder.AddFile_����ļ�(a);
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

	case OT_POINT_LIGHT:
	{
		if (!parent)
			out = m_RootFolder.CreateFile_�����ļ�<PointLight>(name);
		else
			out = parent->CreateFile_�����ļ�<PointLight>(name);
	}
	break;

	case OT_DIRECTIONAL_LIGHT:
	{
		if (!parent)
			out = m_RootFolder.CreateFile_�����ļ�<DirectionalLight>(name);
		else
			out = parent->CreateFile_�����ļ�<DirectionalLight>(name);
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
std::vector<PointLight*>& Controller::GetAllPointLight()
{
	if (!m_PointLights.empty())return m_PointLights;
	m_PointLights = m_RootFolder.GetAllPointLightFile_�ҵ����й�Դ�ļ�();
	return m_PointLights;
}
std::vector<Model*>& Controller::UpdateModels()
{
	m_Models.clear();
	return GetModels();
}

void Controller::DeleteObject(Object* obj,HTREEITEM hTree)
{
	if (!obj)
		return;
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
	m_FocusFolder->DeleteFile_ɾ���ļ�(obj);
	if (hTree)
		m_FileWind->DeleteItem(hTree);
	InvalidateRect(m_MainWind->GethWnd(), NULL, true);
}

ReturnedOfLoadFile Controller::LoadFile(const std::wstring& path)
{
	std::filesystem::path filepath(path);
	OutMessage(L"��ʼ�����ļ�" + path);
	std::wstring extension = filepath.extension();
	ReturnedOfLoadFile error = ReturnedOfLoadFile::_Fail;
	if (extension == L".obj")
	{
		error |= LoadObj(wstrstr(path));
	}
	else if (this->isSupportedModelFile(wstrstr(extension)))
	{
		if (LoadModel(wstrstr(path)))
			error = _Succese;
		else
			error = _Fail;
	}
	else if (extension == L".TfKeyframe")
	{
		Keyframe<TransForm>* tk = new Keyframe<TransForm>(filepath.filename().string());
		tk->LoadFile(path);
		AddObject(tk);
	}
	else if (extension == L".xzcom")
	{
		error |= LoadCommand(path);
	}
	else if (extension == L".dll")
	{
		if (MessageBox(m_hWnd, L"�Ƿ�������Ϣ�������", L"", MB_OKCANCEL))
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
	ReturnedOfLoadFile Error = ReturnedOfLoadFile::_ModelFail;

	// ��obj�ļ�
	std::ifstream objFile(filePath);
	if (!objFile.is_open())
	{
		std::cout << "Failed to open obj file: " << filePath << std::endl;
		return ReturnedOfLoadFile::_FailToOpenFile;
	}

	// ������ʱ�ļ���·��
	std::string tempFolderPath = "./temporary";
	std::filesystem::create_directory(tempFolderPath);

	// ��ȡobj�ļ���
	std::filesystem::path objPath(filePath);
	std::string objFileName = objPath.filename().generic_string();

	// ������ʱ�ļ�·��
	std::string tempObjFilePath = tempFolderPath + "/" + objFileName;

	// ����obj�ļ�����ʱ�ļ���
	std::filesystem::copy_file(filePath, tempObjFilePath, std::filesystem::copy_options::overwrite_existing);

	// ����obj�ļ����ݣ�����mtllibָ��
	std::string mtlFileName;
	std::string line;
	while (std::getline(objFile, line))
	{
		if (line.substr(0, 7) == "mtllib ")
		{
			mtlFileName = line.substr(7);
			break;
		}
	}

	objFile.close();

	// ����ҵ��˹�����mtl�ļ�
	if (!mtlFileName.empty())
	{
		// ����mtl�ļ�������·��
		std::filesystem::path mtlPath = objPath.parent_path() / mtlFileName;
		std::string tempMtlFilePath = tempFolderPath + "/" + mtlFileName;

		// ����mtl�ļ�����ʱ�ļ���
		std::filesystem::copy_file(mtlPath, tempMtlFilePath, std::filesystem::copy_options::overwrite_existing);
	}

	// ʹ��Assimp�����obj�ļ�
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(tempObjFilePath, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "Failed to load obj model: " << importer.GetErrorString() << std::endl;
		return ReturnedOfLoadFile::_DataError;
	}

	// ��������н�һ���Ĵ���Ͳ������������ģ�����ݡ���Ⱦ��

	std::string Output;
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		cout << "ERROR::ASSIMP::" << importer.GetErrorString() << endl;
		Output = "���س���";
		Output += importer.GetErrorString();
		OutMessage(Output, _Error);
		importer.FreeScene();
		return ReturnedOfLoadFile::_DataError;
	}
	aiNode* node = scene->mRootNode;
	Output = objFileName;
	if (!node)
	{
		OutMessage(L"ģ��δ�����ڵ�", _Error);
		std::cout << "ģ��δ�����ڵ�" << std::endl;
		importer.FreeScene();
		return ReturnedOfLoadFile::_DataError;
	}
	Folder* folder = m_FocusFolder->CreateFile_�����ļ�<Folder>(Output);
	if (!folder)
	{
		OutMessage(L"�ļ��д���ʧ��", _Error);
		std::cout << "�ļ��д���ʧ��" << std::endl;
		importer.FreeScene();
		return ReturnedOfLoadFile::_FailedToCreateFile;
	}
	Model* parentModel = dynamic_cast<Model*>(this->CreateObject(folder, Output, OT_MODEL));
	if (!parentModel)
	{
		OutMessage(L"ģ�ʹ���ʧ��", _Error);
		std::cout << "ģ�ʹ���ʧ��" << std::endl;
		importer.FreeScene();
		return ReturnedOfLoadFile::_FailedToCreateFile;
	}
	if (!processModelNode(node, scene, parentModel, folder, objPath.parent_path().string() + '\\'))
	{
		importer.FreeScene();
		return ReturnedOfLoadFile::_FailedToCreateFile;
	}
	importer.FreeScene();
	this->UpdateModels();


	// ������ɺ�ɾ����ʱ�ļ�
	std::filesystem::remove(tempObjFilePath);
	if (!mtlFileName.empty())
	{
		std::filesystem::path tempMtlPath = tempFolderPath + "/" + mtlFileName;
		std::filesystem::remove(tempMtlPath);
	}
	importer.FreeScene();
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
bool Controller::MoveFile_(Object* aim, Object* parent)
{
	if (!aim)
		return false;
	if (!parent)
		parent = &m_RootFolder;
	switch (parent->GetType())
	{
	case OT_FOLDER:
	{
		Folder* folder = aim->GetParent();
		if (folder)
		{
			folder->DeleteIndex(aim);
		}
		folder = dynamic_cast<Folder*>(parent);
		folder->AddFile_����ļ�(aim);
		break;
	}
	case OT_MODEL:
	{
		switch (aim->GetType())
		{
		case OT_MODEL:
		{
			Model* pmodel = dynamic_cast<Model*>(parent);
			Model* model = dynamic_cast<Model*>(aim);
			model->SetModelParent(pmodel);
			break;
		}
		case OT_KEYFRAME:
		{
			Keyframe<TransForm>* key = dynamic_cast<Keyframe<TransForm>*>(aim);
			if (!key)
				return false;
			Model* pmodel = dynamic_cast<Model*>(parent);
			pmodel->SetKeyframe(key);
			return false;
			break;
		}
		default:
			return false;
			break;
		}
		break;
	}
	default:
		return false;
		break;
	}
	return true;
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
KeyframeEdit* Controller::GetKeyframeWind()const
{
	return m_KeyframeWind;
}
void Controller::GetTime(ULONG64* start, ULONG64* range)const
{
	m_KeyframeWind->GetTime(start, range);
}
ULONG64 Controller::GetKeyframeEditStepSize()const
{
	return m_KeyframeWind->GetStepSize();
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
	if (m_Mode == RUNMODE::RM_EDIT || m_Mode == RUNMODE::RM_VIEW)
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
	m_StartTime = 0;
	ULONG64 startTime, runTime;
	GetTime(&startTime, &runTime);
	if (time < startTime)
	{
		int x = time - startTime - 500;
		MoveKeyframeEditTime(x);
	}
	else if (time > runTime + startTime)
	{
		int x = time - runTime + startTime + 500;
		MoveKeyframeEditTime(x);
	}
}
//assimp������ת��
Vector3 aiVector3dToVector3(const aiVector3D& av3)
{
	return Vector3(av3.x, av3.y, av3.z);
}
Vector3 aiVector3dToVector3(const aiColor3D& av3)
{
	return Vector3(av3.r, av3.g, av3.b);
}
bool Controller::LoadModel(const std::string& path)
{
	// ��ȡģ���ļ����ڴ滺����
	std::ifstream file(path, std::ios::binary);
	if (!file) {
		OutMessage(L"�޷����ļ�", _Error);
		return false;
	}
	std::vector<char> buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	size_t pos = path.find_last_of('/');
	if (pos == -1)
	{
		pos = path.find_last_of('\\');
	}
	// ����ģ��
	Assimp::Importer aimporter;
	const aiScene* scene = aimporter.ReadFileFromMemory(buffer.data(), buffer.size(),
		aiProcess_Triangulate | aiProcess_FlipUVs, nullptr);
	//const aiScene* scene = aimporter.ReadFile(path, aiProcess_Triangulate | aiProcess_GenNormals);
	std::string Output;
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		cout << "ERROR::ASSIMP::" << aimporter.GetErrorString() << endl;
		Output = "���س���";
		Output += aimporter.GetErrorString();
		OutMessage(Output, _Error);
		aimporter.FreeScene();
		return false;
	}
	aiNode* node = scene->mRootNode;
	Output = path.substr(pos + 1);
	if(!node)
	{
		OutMessage(L"ģ��δ�����ڵ�", _Error);
		std::cout << "ģ��δ�����ڵ�" << std::endl;
		aimporter.FreeScene();
		return false;
	}
	Folder* folder = m_FocusFolder->CreateFile_�����ļ�<Folder>(Output);
	if (!folder)
	{
		OutMessage(L"�ļ��д���ʧ��", _Error);
		std::cout << "�ļ��д���ʧ��" << std::endl;
		aimporter.FreeScene();
		return false;
	}
	Model* parentModel = dynamic_cast<Model*>(this->CreateObject(folder, Output, OT_MODEL));
	if (!parentModel)
	{
		OutMessage(L"ģ�ʹ���ʧ��", _Error);
		std::cout << "ģ�ʹ���ʧ��" << std::endl;
		aimporter.FreeScene();
		return false;
	}
	if (!processModelNode(node, scene, parentModel, folder->CreateFile_�����ļ�(Folder("���������")), path.substr(0, pos + 1)))
	{
		aimporter.FreeScene();
		return false;
	}
	Folder* amiFolder = folder->CreateFile_�����ļ�(Folder("����"));
	
	UINT AnimationsSize = scene->mNumAnimations;
	for(UINT animationIndex =0; animationIndex <AnimationsSize;++animationIndex)
	{
		aiAnimation* animation = scene->mAnimations[animationIndex]; // ����������ȡ�ض��Ķ���
		unsigned int numChannels = animation->mNumChannels; // ����ͨ����

		for (unsigned int i = 0; i < numChannels; i++) {
			aiNodeAnim* channel = animation->mChannels[i]; // ��ȡÿ��ͨ��

			std::string nodeName = channel->mNodeName.data; // �ڵ�����
			unsigned int numPositionKeys = channel->mNumPositionKeys; // λ�ùؼ�֡����
			unsigned int numRotationKeys = channel->mNumRotationKeys; // ��ת�ؼ�֡����
			unsigned int numScalingKeys = channel->mNumScalingKeys; // ���Źؼ�֡����
			Keyframe<TransFrame>* keyData = new Keyframe<TransFrame>(nodeName);
			amiFolder->AddFile_����ļ�(keyData);
			TransForm key;
			std::set<float>times;
			std::vector<std::pair<float, Vector3>>Positions;
			std::vector<std::pair<float, Vector3>>Scales;
			std::vector<std::pair<float, Quaternion>>Rotates;
			for (unsigned int j = 0; j < numPositionKeys; j++) {
				aiVector3D position = channel->mPositionKeys[j].mValue; // �ؼ�֡λ��
				float timeInSeconds = channel->mPositionKeys[j].mTime / animation->mTicksPerSecond * 1000; // �ؼ�֡ʱ�䣨�룩
				Vector3 pos(position.x, position.y, position.z);
				Positions.push_back(std::pair(timeInSeconds, pos));
				times.insert(timeInSeconds);
			}
			for (unsigned int j = 0; j < numScalingKeys; j++) {
				aiVector3D scale = channel->mScalingKeys[j].mValue;
				float timeInSeconds = channel->mScalingKeys[j].mTime / animation->mTicksPerSecond * 1000;
				Vector3 sca(scale.x, scale.y, scale.z);
				Scales.push_back(std::pair(timeInSeconds, sca));
				times.insert(timeInSeconds);
			}
			for (UINT j = 0; j < numRotationKeys; j++) {
				aiQuaternion rotation = channel->mRotationKeys[j].mValue;
				Quaternion internalQuaternion(rotation.w, rotation.x, rotation.y, rotation.z);
				float timeInSeconds = channel->mRotationKeys[j].mTime / animation->mTicksPerSecond * 1000;
				Rotates.push_back(std::pair(timeInSeconds, internalQuaternion));
				times.insert(timeInSeconds);
			}
			for (auto keytime : times)
			{
				key.Position = extractTransform(Positions, keytime);
				key.Scale = extractTransform(Scales, keytime);
				Quaternion q = extractTransform(Rotates, keytime);
				key.Rotate = Rotation(q.getAngle(), q.GetAxis());
				keyData->SetKeyframe(keytime, key);
			}
		}
	}

	// ��鳡���Ƿ�Ϊ���Լ��Ƿ���������
	if (scene && scene->HasCameras())
	{
		// �������е������
		for (unsigned int i = 0; i < scene->mNumCameras; ++i)
		{
			aiCamera* camera = scene->mCameras[i];
			Camera* camera_ = new Camera(camera->mName.C_Str(),
				aiVector3dToVector3(camera->mPosition), aiVector3dToVector3(camera->mPosition + camera->mLookAt),
				aiVector3dToVector3(camera->mUp), 
				camera->mAspect, camera->mHorizontalFOV, camera->mClipPlaneNear, camera->mClipPlaneFar);
			AddObject(camera_);
		}
	}

	if (scene && scene->HasLights())
	{
		for (UINT i = 0; i < scene->mNumLights; i++)
		{
			aiLight* aLight = scene->mLights[i];
			auto type = aLight->mType;
			switch (type)
			{
			case aiLightSource_UNDEFINED:
				// ����δ�������͵Ĺ�Դ
				break;
			case aiLightSource_DIRECTIONAL:
				// �������Դ
				break;
			case aiLightSource_POINT:
			{
				// ������Դ
				PointLight* ptLnt = new PointLight(aLight->mName.C_Str(), aiVector3dToVector3(aLight->mPosition),
					aiVector3dToVector3(aLight->mColorAmbient), aiVector3dToVector3(aLight->mColorDiffuse), aiVector3dToVector3(aLight->mColorSpecular),
					aLight->mAttenuationConstant, aLight->mAttenuationLinear, aLight->mAttenuationQuadratic);
				folder->AddFile_����ļ�(ptLnt);
			}
				break;
			case aiLightSource_SPOT:
				// ����۹�ƹ�Դ
				break;
			case aiLightSource_AMBIENT:
				// ��������Դ
				break;
			case aiLightSource_AREA:
				// ���������Դ
				break;
			case _aiLightSource_Force32Bit:
				// ����32λǿ�����ͣ������Ҫ��
				break;
			default:
				// ��������δ֪���͵Ĺ�Դ
				break;
			}
		}
	}
	aimporter.FreeScene();
	this->UpdateModels();
	return true;
}



bool Controller::processModelNode(aiNode* node, const aiScene* scene, Model* parentModel, Folder* folder,const std::string& directory)
{
	Model* currentModel = parentModel;
	std::string Output = node->mName.C_Str();

	aiMatrix4x4 transformation = node->mTransformation;

	// ת�ñ任����
	transformation.Transpose();

	// ��Assimp�ı任�����Ƶ�OpenGL�ľ���
	glm::mat4 modelMatrix;
	memcpy(&modelMatrix[0][0], &transformation.a1, sizeof(float) * 16);

	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		currentModel = new Model(Output);
		if (!currentModel)
		{
			OutMessage(L"ģ�ʹ���ʧ��", _Error);
			std::cout << "ģ�ʹ���ʧ��" << std::endl;
			return false;
		}
		parentModel->addChildModel(currentModel);
		aiMesh* amesh = scene->mMeshes[node->mMeshes[i]];
		Mesh* mesh = dynamic_cast<Mesh*>(this->CreateObject(folder, Output + "����", OT_MESH));
		if (!mesh)
		{
			OutMessage(L"���񴴽�ʧ��", _Error);
			std::cout << "���񴴽�ʧ��" << std::endl;
			return false;
		}
		mesh->processMesh(amesh, scene);
		currentModel->SetMesh(mesh);
		if (amesh->mMaterialIndex >= 0)
		{
			Material* material = dynamic_cast<Material*>(this->CreateObject(folder, Output + "����", OT_MATERIAL));
			if (!material)
			{
				OutMessage(L"���ʴ���ʧ��", _Error);
				std::cout << "���ʴ���ʧ��" << std::endl;
				return false;
			}
			material->processMaterial(amesh, scene,folder, directory);
			currentModel->SetMaterial(material);
		}
		currentModel->SetTransform(modelMatrix);
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

void Controller::ScaleKeyframeEditTime(int scale)
{
	m_KeyframeWind->ScaleTime(scale);
}
Vector Controller::GetHighLightColor()const
{
	return m_HighLightColor;
}
void Controller::SetHighLightColor(const Vector& color)
{
	Vector positiveColor = color;
	positiveColor.x = std::abs(positiveColor.x);
	positiveColor.y = std::abs(positiveColor.y);
	positiveColor.z = std::abs(positiveColor.z);

	float maxComponent = std::max(positiveColor.x, std::max(positiveColor.y, positiveColor.z));
	float scaleFactor = 1.0f;

	if (maxComponent > 1.0f)
	{
		scaleFactor = 1.0f / maxComponent;
	}

	Vector scaledColor = positiveColor * scaleFactor;

	m_HighLightColor = scaledColor;
	m_MainWind->SetHighLightColor(scaledColor);
}
// ��ȡ��Ӧʱ��ı任
template <typename T>
T Controller::extractTransform(const std::vector<std::pair<float, T>>& transforms, float time) {
	auto compare = [](const std::pair<float, T>& a, const std::pair<float, T>& b) {
		return a.first < b.first;
	};
	auto it = std::lower_bound(transforms.begin(), transforms.end(), std::pair<float, T>(time, T()), compare);

	if (it == transforms.end()) {
		return transforms.back().second;
	}

	return it->second;
}