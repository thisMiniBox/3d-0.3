#include "project.h"
project::project()
{
	m_FileLoad = false;
	FILEWND = nullptr;
	MAINWND = new GDIWND;
	DETAWND = new DetaileWind;
	hWnd = nullptr;
	m_hInst = nullptr;
	m_rect = { 0, 0, 800, 500 };
	view = nullptr;
	Model_att = 0;
	MSG_att = 0;
	MAX_runMSG = 100;
}
HWND project::CreateWind(HINSTANCE hInst)
{
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
		800,
		500,
		nullptr,
		nullptr,
		hInst,
		nullptr);
	m_hInst = hInst;
	FILEWND->CreateWind(hWnd);
	Folder* a = new Folder("�½���Ŀ");
	view = new Camera(
		"�½������", Vector(0, 0, 3), Vector(0, 0, 0), Vector(0, 1, 0), GetRect().right / GetRect().bottom);
	AddObject(a);
	AddObject(view);
	MAINWND->CreateWind(hWnd);
	DETAWND->CreateWind(hWnd);
	upMsg("ʹ��GDI��ͼʱ���ȵ��һ����������ز�߿�ؼ��������޷�������Ⱦ���ڣ�");
	upMsg("��Ҳ��֪��Ϊʲô����Ϣѭ����ͬһ����opengl��û����");
	RECT m_rect;
	GetClientRect(hWnd, &m_rect);
	int cxClient = m_rect.right - m_rect.left;  // ��ÿͻ������
	int cyClient = m_rect.bottom - m_rect.top;
	TEXTWND.hWnd = CreateWindow( //�����༭��
		TEXTWND.className.c_str(),
		TEXT("��Ϣ"),
		WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_AUTOVSCROLL | ES_MULTILINE,
		0, cyClient - 150, cxClient / 5 * 4, 150,
		hWnd,
		(HMENU)1,
		hInst,
		nullptr);
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
project::~project()
{
	m_RootFolder.ClearFolder_����ļ���();
	if (MAINWND)delete MAINWND;
	if (DETAWND)delete DETAWND;
	if (FILEWND)delete FILEWND;
}
void project::SetFileName(Object* obj, const std::wstring& NewName)
{
	char szBuf[128];
	WideCharToMultiByte(CP_ACP, 0, NewName.c_str(), -1, szBuf, 128, NULL, NULL);
	m_RootFolder.SetFileName(obj, szBuf);
	//DETAWND->GetTarget()->SetName(szBuf);
	FILEWND->FixItemName(DETAWND->GetTree(), NewName.c_str());
}
HINSTANCE project::GethInstance()const
{
	return m_hInst;
}
void project::upMsg(const std::string&str, const char& type)
{
	SYSTEMTIME sys;
	GetLocalTime(&sys);
	runMsg msg = {std::to_wstring(sys.wHour)+L':'+ std::to_wstring(sys.wMinute)+L':'+ std::to_wstring(sys.wSecond),str_wstr(str),type};
	runMSG.AddNode(msg);
	InvalidateRect(TEXTWND.hWnd, NULL, false);
}
void project::upMsg(const std::wstring& str, const char& type)
{
	SYSTEMTIME sys;
	GetLocalTime(&sys);
	runMsg msg = { std::to_wstring(sys.wHour) + L':' + std::to_wstring(sys.wMinute) + L':' + std::to_wstring(sys.wSecond),str,type };
	runMSG.AddNode(msg);
	InvalidateRect(TEXTWND.hWnd, NULL, false);
}
void project::updateMsg(const HDC& hdc)
{

	int msgLong = -TEXTWND.m_rect.top;
	std::wstring out;
	LinkList<runMsg>* p = runMSG.above;
	Rectangle(hdc, 0, 0, 1000, 1000);
	if(MSG_att==0)
	{
		for (int i = 0; i < MAX_runMSG; i++)
		{
			out = L"";
			if (p == &runMSG)break;
			if (p->data.Type == 2)out += L"���� ";
			else if (p->data.Type == 1)out += L"��Ϣ ";
			else out += L"���� ";
			out += (p->data.Time);
			out += (p->data.Str);
			p = p->above;
			msgLong++;
			TextOut(hdc, 0, msgLong * 20, out.c_str(), out.size());
		}
		if (p != &runMSG)runMSG.DeleteNode(1);
	}
	else if (MSG_att == 1)
	{
		for (int i = 0; i < MAX_runMSG; i++)
		{
			out = L"";
			if (p == &runMSG)break;
			if (p->data.Type == 3)out += L"���� ";
			else
			{
				p = p->above;
				continue;
			}
			out += (p->data.Time);
			out += (p->data.Str);
			p = p->above;
			msgLong++;
			TextOut(hdc, 0, msgLong * 20, out.c_str(), out.size());
		}
		if (p != &runMSG)runMSG.DeleteNode(1);
	}
	else if (MSG_att == 2)
	{
		for (int i = 0; i < MAX_runMSG; i++)
		{
			out = L"";
			if (p == &runMSG)break;
			if (p->data.Type == 2)out += L"���� ";
			else
			{
				p = p->above;
				continue;
			}
			out += (p->data.Time);
			out += (p->data.Str);
			p = p->above;
			msgLong++;
			TextOut(hdc, 0, msgLong * 20, out.c_str(), out.size());
		}
		if (p != &runMSG)runMSG.DeleteNode(1);
	}
	else if (MSG_att == 3)
	{
		for (int i = 0; i < MAX_runMSG; i++)
		{
			out = L"";
			if (p == &runMSG)break;
			if (p->data.Type == 1)out += L"��Ϣ ";
			else
			{
				p = p->above;
				continue;
			}
			out += (p->data.Time);
			out += (p->data.Str);
			p = p->above;
			msgLong++;
			TextOut(hdc, 0, msgLong * 20, out.c_str(), out.size());
		}
		if (p != &runMSG)runMSG.DeleteNode(1);
	}
}
HTREEITEM project::AddObject(Object* a, std::string address)
{
	m_Models.clear();
	m_RootFolder.AddFile_����ļ�(a);
	return FILEWND->AddItem(*a, address);
}
HTREEITEM project::AddObject(Object* a, HTREEITEM parent)
{
	m_Models.clear();
	m_RootFolder.AddFile_����ļ�(a);
	return FILEWND->AddItem(*a, parent);
}
Object* project::CreateObject(Folder* parent, std::string name, int type)
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
void project::SetRect(RECT rect)
{
	m_rect = rect;
}
RECT project::GetRect()const
{
	return m_rect;
}
std::vector<Model*>& project::GetModels()
{
	if (!m_Models.empty())return m_Models;
	m_Models = m_RootFolder.GetAllModleFile_�ҵ�����ģ��();
	return m_Models;
}
std::vector<Model*>& project::UpdateModels()
{
	m_Models.clear();
	return GetModels();
}

void project::DeleteObject(Object* obj,HTREEITEM hTree)
{
	if (DETAWND->GetTarget() == obj)
		DETAWND->SetView(nullptr);
	m_RootFolder.DeleteFile_ɾ���ļ�(obj);
	UpdateModels();
	if (hTree)
		FILEWND->DeleteItem(hTree);
	InvalidateRect(MAINWND->GethWnd(), NULL, true);
}

int project::loadModel(const std::wstring& path)
{
	Model* model = new Model;
	int error = model->loadModelFile_����ģ���ļ�(path);
	m_RootFolder.AddFile_����ļ�(model);
	FILEWND->AddItem(*model);
	return error;
}