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
		WS_VISIBLE | WS_CHILD | WS_OVERLAPPEDWINDOW,
		0, 0, 0, 0,
		parent, NULL,
		m_hInst, NULL);
	return m_hWnd;
}
void BottomWindow::AddWind(HWND hWnd, const std::wstring& text)
{
	m_Foucs = CreateWindow(
		TEXT("button"), text.c_str(),
		WS_VISIBLE | WS_CHILD | WS_BORDER,
		0, 0, 100, 15,
		m_hWnd, (HMENU)m_ChildWindNumber,
		m_hInst, NULL);
	if (m_Foucs)
		m_Index[m_Foucs] = hWnd;
	m_ChildWindNumber++;
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
	for (auto& c : m_Index)
	{
		MoveWindow(c.first, 100 * (m_ChildWindNumber - 1), h - buttonH, 100, buttonH, true);
		MoveWindow(c.second, 0, 0, w, h-buttonH, true);
	}
}
HWND BottomWindow::Select(HWND hWnd)
{
	if (!hWnd)return nullptr;

	for (auto& c : m_Index)
	{
		// 获取当前位置和大小
		RECT rc;
		GetWindowRect(c.second, &rc);

		// 计算新位置
		rc.left += 50;
		rc.top += 50;
		rc.right = rc.left + 200;
		rc.bottom = rc.top + 100;

		// 调用 SetWindowPos 函数
		SetWindowPos(c.second, HWND_TOP, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top,
			SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOSENDCHANGING);

		// 隐藏子窗口
		ShowWindow(c.second, SW_HIDE);
		ShowWindow(c.second, SW_HIDE);
	}
	ShowWindow(m_Index[hWnd], SW_SHOW);
	//MoveWindow(m_Index[hWnd], 0, 0, m_w, m_h - buttonH, false);
	//Size(m_w, m_h);
	//InvalidateRect(m_hWnd, NULL, true);
	return m_Index[hWnd];
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
	lstrcpy(m_TextTypeFace.lfFaceName, TEXT("华文行楷"));  // 字体名称
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
	if (m_DisplayPosition.y < -(long long)(m_TextTypeFace.lfHeight * (m_MessageData.size() - 2)))
		m_DisplayPosition.y = -(long long)(m_TextTypeFace.lfHeight * (m_MessageData.size() - 2));
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
size_t TextOutWind::OutMessage(const std::string& str, const char& type)
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
size_t TextOutWind::OutMessage(const std::wstring& str, const char& type)
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