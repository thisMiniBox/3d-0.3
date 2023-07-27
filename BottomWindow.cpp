#include "BottomWindow.h"
BottomWindow::BottomWindow(HINSTANCE hInst, HWND p) :m_ChildWindNumber(0), m_Foucs(nullptr)
{
	WNDCLASSEX wcex = { 0 };
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.hInstance = hInst;
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszClassName = L"BottomWindows";
	// 注册窗口类
	RegisterClassEx(&wcex);
	m_hInst = hInst;
	CreateWind(p);
}
HWND BottomWindow::GethWnd()const
{
	return m_hWnd;
}
BottomWindow::~BottomWindow()
{
	// 销毁窗口
	DestroyWindow(m_hWnd);
	// 注销窗口类
	UnregisterClass(L"BottomWindows", m_hInst);
}
HWND BottomWindow::CreateWind(HWND parent)
{
	m_hWnd = CreateWindow(
		L"BottomWindows", NULL,
		WS_VISIBLE | WS_CHILD,
		0, 0, 0, 0,
		parent, NULL,
		m_hInst, NULL);
	return m_hWnd;
}
int BottomWindow::AddWind(HWND hWnd, const std::wstring& text)
{
	HWND Foucs = CreateWindow(
		TEXT("button"), text.c_str(),
		WS_VISIBLE | WS_CHILD | WS_BORDER,
		0, 0, 100, 15,
		m_hWnd, (HMENU)m_ChildWindNumber,
		m_hInst, NULL);
	if (Foucs)
		m_Index[Foucs] = hWnd;
	m_ChildWindNumber++;
	ShowWindow(hWnd, SW_HIDE);
	return m_ChildWindNumber - 1;
}
HWND BottomWindow::Select(const std::wstring& windowTitle)
{
	HWND hWnd = FindValueByWindowTitle(windowTitle);
	SwitchViewWindow(hWnd);
	return hWnd;
}
HWND  BottomWindow::FindValueByWindowTitle(const std::wstring& windowTitle)
{
	for (const auto& pair : m_Index)
	{
		HWND key = pair.first;
		HWND value = pair.second;

		// 获取窗口标题
		const int titleLength = GetWindowTextLength(key) + 1;
		std::wstring title(titleLength, L'\0');
		GetWindowText(key, &title[0], titleLength);
		// 去除 title 末尾的多余 null 字符
		title.resize(titleLength - 1);

		// 比较窗口标题，如果匹配，则返回对应的值
		if (title == windowTitle)
		{
			return value;
		}
	}

	// 找不到匹配的窗口标题，返回特定的错误值或抛出异常
	return nullptr; // 或其他错误标识，如INVALID_HANDLE_VALUE等
}

void BottomWindow::SwitchViewWindow(HWND hWnd)
{
	if (m_Foucs == hWnd)
	{
		printf_s("窗口相同");
		return;
	}
	if(m_Foucs)
		ShowWindow(m_Foucs, SW_HIDE);
	ShowWindow(hWnd, SW_SHOW);
	m_Foucs = hWnd;
}
void BottomWindow::DeleteWind(HWND hWnd)
{
	DestroyWindow(hWnd);
	m_ChildWindNumber--;
}
void BottomWindow::Size(int w, int h)
{
	m_w = w;
	m_h = h;
	UINT pos = 0;
	for (auto& c : m_Index)
	{
		MoveWindow(c.first, 100 * pos, h - buttonH, 100, buttonH, true);
		MoveWindow(c.second, 0, 0, w, h-buttonH, true);
		pos++;
	}
}
void KeyframeEdit::UpdateView(ChildWindSign cws)const
{
	switch (cws)
	{
	case ChildWindSign::KeyframeWind:
		InvalidateRect(m_hButten, nullptr, true);
		InvalidateRect(m_hFile, nullptr, true);
		InvalidateRect(m_hCanvas, nullptr, true);
		InvalidateRect(m_hTime, nullptr, true);
		break;
	case ChildWindSign::KF_TimeWind:
		InvalidateRect(m_hTime, nullptr, true);
		break;
	case ChildWindSign::KF_FileWind:
		InvalidateRect(m_hFile, nullptr, true);
		break;
	case ChildWindSign::KF_ButtenWind:
		InvalidateRect(m_hButten, nullptr, true);
		break;
	case ChildWindSign::KF_CanvasWind:
		InvalidateRect(m_hCanvas, nullptr, true);
		break;
	}
}
HWND BottomWindow::Select(HWND hWnd)
{
	if (!hWnd)return nullptr;
	HWND Foucs = m_Index[hWnd];
	SwitchViewWindow(Foucs);
	return Foucs;
}
TextOutWind::TextOutWind(HINSTANCE hInst)
{
	for (int i = 0; i < 4; i++)
		m_ChildControl[i] = nullptr;
	m_hWnd = nullptr;
	m_MessageSizeMaximum = 100;
	m_DisplayPosition = vec::Vector2(0, 0);
	m_DisplayMode = _ALL;
	m_hInst = hInst;
	WNDCLASSEX wcex = { 0 };
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = TextWndProc;
	wcex.hInstance = hInst;
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszClassName = L"TextWind";
	// 注册窗口类
	RegisterClassEx(&wcex);
	m_TextTypeFace.lfHeight = 20;  // 字体高度
	m_TextTypeFace.lfWidth = 0;
	m_TextTypeFace.lfEscapement = 0;
	m_TextTypeFace.lfOrientation = 0;
	m_TextTypeFace.lfWeight = FW_NORMAL;  // 字体粗细
	m_TextTypeFace.lfItalic = FALSE;     // 是否倾斜
	m_TextTypeFace.lfUnderline = FALSE;  // 是否下划线
	m_TextTypeFace.lfStrikeOut = FALSE;  // 是否删除线
	m_TextTypeFace.lfCharSet = DEFAULT_CHARSET;   // 字符集
	m_TextTypeFace.lfOutPrecision = OUT_DEFAULT_PRECIS;
	m_TextTypeFace.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	m_TextTypeFace.lfQuality = DEFAULT_QUALITY;
	m_TextTypeFace.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
	lstrcpy(m_TextTypeFace.lfFaceName, TEXT("仿宋"));  // 字体名称
}
void TextOutWind::SetFont(LOGFONT NewFont)
{
	m_TextTypeFace = NewFont;
}
HWND TextOutWind::CreateWind(HWND parent)
{
	m_hWnd = CreateWindowW(
		L"TextWind",
		0,
		WS_CHILD | WS_BORDER | WS_VISIBLE,
		0, 50, 100, 100,
		parent,
		(HMENU)ChildWindSign::MessageWind,
		m_hInst,
		nullptr);
	RECT m_rect;
	GetClientRect(m_hWnd, &m_rect);
	int cxClient = m_rect.right - m_rect.left;  // 获得客户区宽度
	m_ChildControl[0] = (CreateWindow(
		TEXT("button"),
		TEXT("消息"),
		WS_CHILD | WS_VISIBLE,
		cxClient - 50, 0, 50, 20,
		m_hWnd,
		(HMENU)13,
		m_hInst,
		NULL));
	m_ChildControl[1] = (CreateWindow(
		TEXT("button"),
		TEXT("警告"),
		WS_CHILD | WS_VISIBLE,
		cxClient - 100, 0, 50, 20,
		m_hWnd,
		(HMENU)12,
		m_hInst,
		NULL));
	m_ChildControl[2] = (CreateWindow(
		TEXT("button"),
		TEXT("错误"),
		WS_CHILD | WS_VISIBLE,
		cxClient - 150, 0, 50, 20,
		m_hWnd,
		(HMENU)11,
		m_hInst,
		NULL));
	m_ChildControl[3] = (CreateWindow(
		TEXT("button"),
		TEXT("所有"),
		WS_CHILD | WS_VISIBLE,
		cxClient - 150, 0, 50, 20,
		m_hWnd,
		(HMENU)10,
		m_hInst,
		NULL));
	return m_hWnd;
}
TextOutWind::~TextOutWind()
{
	// 销毁窗口
	DestroyWindow(m_hWnd);
	// 注销窗口类
	UnregisterClass(L"TextWind", m_hInst);
}
void TextOutWind::MoveText(const vec::Vector2& move)
{
	if (m_DisplayPosition.x >= 0)
		m_DisplayPosition.x += move.x;
	if (m_DisplayPosition.y <= 10)
		m_DisplayPosition.y += move.y;
	if (m_DisplayPosition.x < 0)
		m_DisplayPosition.x = 0;
	if (m_DisplayPosition.y < -(double)(m_TextTypeFace.lfHeight * (m_MessageData.size() - 2)))
		m_DisplayPosition.y = -(double)(m_TextTypeFace.lfHeight * (m_MessageData.size() - 2));
	else if (m_DisplayPosition.y > 0)
		m_DisplayPosition.y = 0;
}
void TextOutWind::SetTextMaximum(unsigned int max)
{
	m_MessageSizeMaximum = max;
}
void TextOutWind::SetOutMode(unsigned int mode)
{
	m_DisplayMode = mode;
}
HWND TextOutWind::GethWnd()
{
	return m_hWnd;
}
size_t TextOutWind::OutMessage(const std::string& str, MSGtype type)
{
	SYSTEMTIME sys;
	GetLocalTime(&sys);
	runMsg msg = { std::to_wstring(sys.wHour) + L':' + std::to_wstring(sys.wMinute) + L':' + std::to_wstring(sys.wSecond),str_wstr(str),type };
	m_MessageData.push_back(msg);
	InvalidateRect(m_hWnd, NULL, false);
	if (m_MessageData.size() > m_MessageSizeMaximum)m_MessageData.erase(m_MessageData.begin());
	if (size() > 5)MoveText(vec::Vector2(0, -m_TextTypeFace.lfHeight));
	else MoveText(vec::Vector2(0, -m_TextTypeFace.lfHeight / 6));
	return m_MessageData.size();
}
size_t TextOutWind::OutMessage(const std::wstring& str, MSGtype type)
{
	SYSTEMTIME sys;
	GetLocalTime(&sys);
	runMsg msg = { std::to_wstring(sys.wHour) + L':' + std::to_wstring(sys.wMinute) + L':' + std::to_wstring(sys.wSecond),str,type };
	m_MessageData.push_back(msg);
	InvalidateRect(m_hWnd, NULL, false);
	if (m_MessageData.size() > m_MessageSizeMaximum)m_MessageData.erase(m_MessageData.begin());
	if (size() > 5)MoveText(vec::Vector2(0, -m_TextTypeFace.lfHeight));
	else MoveText(vec::Vector2(0, -m_TextTypeFace.lfHeight / 6));
	return m_MessageData.size();
}
size_t TextOutWind::size()
{
	return m_MessageData.size();
}
void TextOutWind::SetTextPosition(const vec::Vector2& vec2)
{
	m_DisplayPosition = vec2;
}
void TextOutWind::DrawWind(HDC hdc)
{
	vec::Vector2 OutPosition = m_DisplayPosition;
	std::wstring out;
	RECT rect;
	GetClientRect(m_hWnd, &rect);
	Rectangle(hdc, 0, 0, rect.right, rect.bottom);
	HFONT hFont = CreateFontIndirect(&m_TextTypeFace);
	HFONT OldhFont = (HFONT)SelectObject(hdc, hFont);

	if (m_DisplayMode == _ALL)
	{
		for (const auto& MSG : m_MessageData)
		{
			out = L"";
			if (MSG.Type == _Warning)
			{
				out += L"警告 ";
				SetTextColor(hdc, WarningOutColor);
			}
			else if (MSG.Type == _Message)
			{
				out += L"消息 ";
				SetTextColor(hdc, RemindOutColor);
			}
			else
			{
				out += L"错误 ";
				SetTextColor(hdc, ErrorOutColor);
			}
			out += (MSG.Time);
			out += (MSG.Str);
			TextOut(hdc, OutPosition.x, OutPosition.y, out.c_str(), out.size());
			OutPosition.y += m_TextTypeFace.lfHeight;
		}
	}
	else if (m_DisplayMode == _ERROR)
	{
		SetTextColor(hdc, ErrorOutColor);
		for (const auto& MSG : m_MessageData)
		{
			if (MSG.Type == _Error)
			{
				out = L"";
				out += L"错误 ";
				out += (MSG.Time);
				out += (MSG.Str);
				TextOut(hdc, OutPosition.x, OutPosition.y, out.c_str(), out.size());
				OutPosition.y += m_TextTypeFace.lfHeight;
			}
		}
	}
	else if (m_DisplayMode == _WARNING)
	{
		SetTextColor(hdc, WarningOutColor);
		for (const auto& MSG : m_MessageData)
		{
			if (MSG.Type == _Warning)
			{
				out = L"";
				out += L"警告 ";
				out += (MSG.Time);
				out += (MSG.Str);
				TextOut(hdc, OutPosition.x, OutPosition.y, out.c_str(), out.size());
				OutPosition.y += m_TextTypeFace.lfHeight;
			}
		}
	}
	else if (m_DisplayMode == _REMIND)
	{
		SetTextColor(hdc, RemindOutColor);
		for (const auto& MSG : m_MessageData)
		{
			if (MSG.Type == _Message)
			{
				out = L"";
				out += L"消息 ";
				out += (MSG.Time);
				out += (MSG.Str);
				TextOut(hdc, OutPosition.x, OutPosition.y, out.c_str(), out.size());
				OutPosition.y += m_TextTypeFace.lfHeight;
			}
		}
	}
	SelectObject(hdc, OldhFont);
	DeleteObject(hFont);
	SetBkColor(hdc, RGB(10, 10, 10));
	switch (m_DisplayMode)
	{
	case MSGMode::_ALL:
	{
		TextOut(hdc, rect.right - 200 - 100, 3, TEXT("显示所有消息"), 6);
		break;
	}
	case MSGMode::_ERROR:
	{
		TextOut(hdc, rect.right - 200 - 100, 3, TEXT("显示错误消息"), 6);
		break;
	}
	case MSGMode::_WARNING:
	{
		TextOut(hdc, rect.right - 200 - 100, 3, TEXT("显示警告消息"), 6);
		break;
	}
	case MSGMode::_REMIND:
	{
		TextOut(hdc, rect.right - 200 - 100, 3, TEXT("显示普通消息"), 6);
		break;
	}
	}
}
void TextOutWind::UpdateWindowSize(int w, int h)
{
	for (int i = 0; i < 4; i++)
		MoveWindow(m_ChildControl[i], w - 50 * (i + 1), 1, 50, 30, true);
}
InputOutput::InputOutput(HINSTANCE hInst, HWND parent) :m_CurrentText(0), m_MaxText(128)
{
	m_hInst = hInst;
	m_hWnd = CreateDialog(hInst, MAKEINTRESOURCE(IDD_IOWND), parent, IOWndProc);
	m_Input = GetDlgItem(m_hWnd, IDC_IO_IN);
	m_Output = GetDlgItem(m_hWnd, IDC_IO_OUT);
	m_Enter = GetDlgItem(m_hWnd, IDC_IO_OK);
}
InputOutput::~InputOutput()
{
	DestroyWindow(m_hWnd);
}
void InputOutput::OutputString(const std::wstring& str)
{
	// 获取编辑框的当前文本长度
	int nTextLength = GetWindowTextLength(m_Output);

	// 将新文本追加到编辑框文本的末尾，并在末尾插入回车符
	SendMessage(m_Output, EM_SETSEL, (WPARAM)nTextLength, (LPARAM)nTextLength);
	if (nTextLength)
		SendMessage(m_Output, EM_REPLACESEL, 0, (LPARAM)L"\r\n");
	SendMessage(m_Output, EM_REPLACESEL, 0, (LPARAM)str.c_str());
	m_CurrentText++;
	if (m_CurrentText > m_MaxText)
	{
		DeleteLineText(0);
		m_CurrentText--;
	}
}
void InputOutput::DeleteLineText(int line)
{
	// 先获取当前编辑框的总行数
	int nLineCount = SendMessage(m_Output, EM_GETLINECOUNT, 0, 0);

	// 如果删除行号大于当前行数，则将删除行号设为最大行数
	if (line >= nLineCount) {
		line = nLineCount - 1;
	}

	// 获取当前编辑框中第一行的字符索引
	int nIndex = SendMessage(m_Output, EM_LINEINDEX, 0, 0);

	// 遍历每一行，直到找到要删除的行
	for (int i = 0; i < line && i < nLineCount; i++) {
		nIndex = SendMessage(m_Output, EM_LINEINDEX, nIndex + 1, 0);
	}

	// 获取要删除行的长度
	int nLineLen = SendMessage(m_Output, EM_LINELENGTH, nIndex, 0);

	// 将编辑框设置为可写模式
	SendMessage(m_Output, EM_SETREADONLY, FALSE, 0);

	// 如果要删除的不是最后一行，则将当前行的结尾换行符也删除
	if (line < nLineCount - 1) {
		nLineLen += 2; // 加上结尾的换行符长度
	}

	// 删除指定行的文本（包括结尾的换行符）
	SendMessage(m_Output, EM_SETSEL, nIndex, nIndex + nLineLen);
	SendMessage(m_Output, WM_CLEAR, 0, 0);

	// 将编辑框设置为只读模式
	SendMessage(m_Output, EM_SETREADONLY, TRUE, 0);
}

std::wstring InputOutput::InputString()
{
	int nLineCount = SendMessage(m_Output, EM_GETLINECOUNT, 0, 0);
	// 获取最后一行的起始位置和长度
	int nLastLineStart = SendMessage(m_Output, EM_LINEINDEX, nLineCount - 1, 0);
	int nLastLineLength = SendMessage(m_Output, EM_LINELENGTH, nLastLineStart, 0);

	// 分配内存用于存储最后一行内容
	WCHAR* szLastLine = new WCHAR[nLastLineLength + 1];

	// 获取最后一行内容
	SendMessage(m_Output, EM_GETLINE, nLineCount - 1, (LPARAM)szLastLine);

	// 确保字符串以 '\0' 结尾
	szLastLine[nLastLineLength] = L'\0';
	std::wstring out = szLastLine;
	// 释放内存
	delete[] szLastLine;
	return out;
}
HWND InputOutput::GethWnd()const
{
	return m_hWnd;
}
void InputOutput::Size()
{
	RECT rect;
	GetClientRect(m_hWnd, &rect);
	MoveWindow(m_Output, 0, 0, rect.right, rect.bottom-buttonH, true);
	MoveWindow(m_Input, 0, rect.bottom - buttonH, rect.right - buttonH * 3, buttonH, true);
	MoveWindow(m_Enter, rect.right - buttonH * 3, rect.bottom - buttonH, buttonH * 3, buttonH, true);
}
void InputOutput::Clear()
{
	SetWindowText(m_Input, NULL);
}
HWND KeyframeEdit::GethWnd()const
{
	return m_hWnd;
}
KeyframeEdit::KeyframeEdit(HINSTANCE hInst, HWND parent)
{
	m_StartTime = 0;
	m_TimeRange = 3000;
	m_Y = 0;
	m_hInst = hInst;
	m_ClassName = L"KeyframeWind";
	m_TimeClassName = L"KeyframeWind_Time";
	m_ButtenClassName = L"KeyframeWind_Botten";
	m_CanvasClassName = L"KeyframeWind_Canvas";
	m_FileClassName = L"KeyframeWind_File";
	WNDCLASSEX wcex = { 0 };
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc = KeyframeWndProc;
	wcex.hInstance = hInst;
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszClassName = m_ClassName.c_str();
	// 注册窗口类
	if (!RegisterClassEx(&wcex))
	{
		DWORD error = GetLastError();
		std::cout << "窗口类注册失败，错误代码：" << error << std::endl;
		return;
	}

	wcex.lpszClassName = m_CanvasClassName.c_str();
	wcex.lpfnWndProc = KeyframeCanvasProc;
	if (!RegisterClassEx(&wcex))
	{
		DWORD error = GetLastError();
		std::cout << "画布窗口类注册失败，错误代码：" << error << std::endl;
		return;
	}

	wcex.lpszClassName = m_TimeClassName.c_str();
	wcex.lpfnWndProc = KeyframeTimeProc;
	if (!RegisterClassEx(&wcex))
	{
		DWORD error = GetLastError();
		std::cout << "时间窗口类注册失败，错误代码：" << error << std::endl;
		return;
	}

	wcex.lpszClassName = m_FileClassName.c_str();
	wcex.lpfnWndProc = KeyframeFileProc;
	if (!RegisterClassEx(&wcex))
	{
		DWORD error = GetLastError();
		std::cout << "文件窗口类注册失败，错误代码：" << error << std::endl;
		return;
	}

	wcex.lpszClassName = m_ButtenClassName.c_str();
	wcex.lpfnWndProc = KeyframeButtenProc;
	if (!RegisterClassEx(&wcex))
	{
		DWORD error = GetLastError();
		std::cout << "按钮窗口类注册失败，错误代码：" << error << std::endl;
		return;
	}

	m_hWnd = CreateWindowExW(
		WS_EX_WINDOWEDGE,
		m_ClassName.c_str(),
		L"KeyframeWindows",
		WS_CHILD | WS_VISIBLE,
		0, 0, 200, 100,
		parent, (HMENU)ChildWindSign::KeyframeWind, hInst, nullptr);
	if (!m_hWnd)
	{
		DWORD error = GetLastError();
		std::cout << "关键帧编辑窗口创建失败，错误代码：" << error << std::endl;
		return;
	}
	m_hFile = CreateWindowExW(
		0,
		m_FileClassName.c_str(),
		nullptr,
		WS_CHILD | WS_VISIBLE,
		0, 0, 200, 100,
		m_hWnd, (HMENU)ChildWindSign::KF_FileWind, hInst, nullptr);
	if (!m_hFile)
	{
		DWORD error = GetLastError();
		std::cout << "文件窗口创建失败，错误代码：" << error << std::endl;
		return;
	}
	m_hTime = CreateWindowExW(
		0,
		m_TimeClassName.c_str(),
		L"KeyframeWindows",
		WS_CHILD | WS_VISIBLE,
		0, 0, 200, 100,
		m_hWnd, (HMENU)ChildWindSign::KF_TimeWind, hInst, nullptr);
	if (!m_hTime)
	{
		DWORD error = GetLastError();
		std::cout << "时间窗口创建失败，错误代码：" << error << std::endl;
		return;
	}
	m_hCanvas = CreateWindowExW(
		0,
		m_CanvasClassName.c_str(),
		L"KeyframeWindows",
		WS_CHILD | WS_VISIBLE | CS_DBLCLKS,
		0, 0, 200, 100,
		m_hWnd, (HMENU)ChildWindSign::KF_CanvasWind, hInst, nullptr);
	if (!m_hCanvas)
	{
		DWORD error = GetLastError();
		std::cout << "画布窗口创建失败，错误代码：" << error << std::endl;
		return;
	}
	m_hButten = CreateWindowExW(
		0,
		m_ButtenClassName.c_str(),
		L"KeyframeWindows",
		WS_CHILD | WS_VISIBLE,
		0, 0, 200, 100,
		m_hWnd, (HMENU)ChildWindSign::KF_ButtenWind, hInst, nullptr);
	if (!m_hButten)
	{
		DWORD error = GetLastError();
		std::cout << "按钮窗口创建失败，错误代码：" << error << std::endl;
		return;
	}
}

KeyframeEdit::~KeyframeEdit()
{
	// 销毁窗口
	DestroyWindow(m_hWnd);
	// 注销窗口类
	UnregisterClass(m_ClassName.c_str(), m_hInst);
	UnregisterClass(m_CanvasClassName.c_str(), m_hInst);
	UnregisterClass(m_FileClassName.c_str(), m_hInst);
	UnregisterClass(m_TimeClassName.c_str(), m_hInst);
	UnregisterClass(m_ButtenClassName.c_str(), m_hInst);
}
void KeyframeEdit::MoveSize(int w, int h)
{
	m_Y = 0;
	float width = w;
	float height = h;
	if (m_hButten)
		MoveWindow(m_hButten, 0, 0, WL_KeyframeBotten_Width, WL_KeyframeBotten_Height, true);
	if (m_hFile)
		MoveWindow(m_hFile, 0, WL_KeyframeBotten_Height, WL_KeyframeBotten_Width, (int)(height - WL_KeyframeBotten_Height), true);
	if (m_hTime)
		MoveWindow(m_hTime, WL_KeyframeBotten_Width, 0, (int)(width - WL_KeyframeBotten_Width), WL_KeyframeTime_Height, true);
	if (m_hCanvas)
		MoveWindow(m_hCanvas, WL_KeyframeBotten_Width, WL_KeyframeTime_Height, (int)(width - WL_KeyframeBotten_Width), (int)(height - WL_KeyframeTime_Height), true);
	UpdateStepSize();
}
int KeyframeEdit::GetY()const
{
	return m_Y;
}
void KeyframeEdit::MoveY(int y)
{
	if (m_Y >= 0 && y > 0)
		return;
	m_Y += y;
}
void KeyframeEdit::MoveTime(int x)
{
	LONG64 left = m_StartTime;
	if (left + x * m_StepSize <= 0)
	{
		left = m_TimeRange;
		m_StartTime = 0;
		m_TimeRange = left;
		return;
	}
	m_StartTime += (x * m_StepSize);
}
void KeyframeEdit::GetTime(ULONG64* start, ULONG64* range)const
{
	*start = m_StartTime;
	*range = m_TimeRange;
}
void KeyframeEdit::ScaleTime(int x)
{
	int minTimeView = 20;
	int step = m_StepSize / 10;
	if (-x * step >= m_TimeRange)
	{
		m_TimeRange = minTimeView;
	}
	else if (m_TimeRange + x > 500000000000)
	{
		m_TimeRange = 500000000000;
	}
	else
	{
		m_TimeRange += x < 0 ? (x * step) : (x * m_StepSize);
	}
	if (m_TimeRange < minTimeView)
		m_TimeRange = minTimeView;
	UpdateStepSize();
}

ULONG64 KeyframeEdit::GetStepSize()const
{
	return m_StepSize;
}
void KeyframeEdit::UpdateStepSize()
{
	// 获取窗口大小
	RECT rect;
	GetClientRect(m_hTime, &rect);
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;

	// 计算比例尺
	double scale = (double)(width - 20) / (m_TimeRange);

	// 计算最小步长
	int minStepSize = 1;
	minStepSize = pow(10, floor(log10(m_TimeRange)) - 1);
	// 计算步长倍数
	int multiplier = 1;
	while (multiplier * minStepSize * scale < 10) {
		multiplier++;
	}
	// 计算实际步长
	m_StepSize = minStepSize * multiplier;
}