#include "TextOutWind.h"
#include"�ַ�ת��.h"
TextOutWind::TextOutWind(HINSTANCE hInst)
{
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
    // ע�ᴰ����
    RegisterClassEx(&wcex);
	m_TextTypeFace.lfHeight = 20;  // ����߶�
	m_TextTypeFace.lfWidth = 0;
	m_TextTypeFace.lfEscapement = 0;
	m_TextTypeFace.lfOrientation = 0;
	m_TextTypeFace.lfWeight = FW_NORMAL;  // �����ϸ
	m_TextTypeFace.lfItalic = FALSE;     // �Ƿ���б
	m_TextTypeFace.lfUnderline = FALSE;  // �Ƿ��»���
	m_TextTypeFace.lfStrikeOut = FALSE;  // �Ƿ�ɾ����
	m_TextTypeFace.lfCharSet = DEFAULT_CHARSET;   // �ַ���
	m_TextTypeFace.lfOutPrecision = OUT_DEFAULT_PRECIS;
	m_TextTypeFace.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	m_TextTypeFace.lfQuality = DEFAULT_QUALITY;
	m_TextTypeFace.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
	lstrcpy(m_TextTypeFace.lfFaceName, TEXT("�����п�"));  // ��������
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
        (HMENU)1,
        m_hInst,
        nullptr);
	RECT m_rect;
	GetClientRect(m_hWnd, &m_rect);
	int cxClient = m_rect.right - m_rect.left;  // ��ÿͻ������
	m_ChildControl.push_back(CreateWindow(
		TEXT("button"),
		TEXT("��Ϣ"),
		WS_CHILD | WS_VISIBLE,
		cxClient - 50, 0, 50, 20,
		m_hWnd,
		(HMENU)13,
		m_hInst,
		NULL));
	m_ChildControl.push_back(CreateWindow(
		TEXT("button"),
		TEXT("����"),
		WS_CHILD | WS_VISIBLE,
		cxClient - 100, 0, 50, 20,
		m_hWnd,
		(HMENU)12,
		m_hInst,
		NULL));
	m_ChildControl.push_back(CreateWindow(
		TEXT("button"),
		TEXT("����"),
		WS_CHILD | WS_VISIBLE,
		cxClient - 150, 0, 50, 20,
		m_hWnd,
		(HMENU)11,
		m_hInst,
		NULL));
	m_ChildControl.push_back(CreateWindow(
		TEXT("button"),
		TEXT("����"),
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
    // ���ٴ���
    DestroyWindow(m_hWnd);
    // ע��������
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
				out += L"���� ";
				SetTextColor(hdc, WarningOutColor);
			}
			else if (MSG.Type == _Message)
			{
				out += L"��Ϣ ";
				SetTextColor(hdc, RemindOutColor);
			}
			else
			{
				out += L"���� ";
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
				out += L"���� ";
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
				out += L"���� ";
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
				out += L"��Ϣ ";
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
		TextOut(hdc, rect.right - 200 - 100, 3, TEXT("��ʾ������Ϣ"), 6);
		break;
	}
	case MSGMode::_ERROR:
	{
		TextOut(hdc, rect.right - 200 - 100, 3, TEXT("��ʾ������Ϣ"), 6);
		break;
	}
	case MSGMode::_WARNING:
	{
		TextOut(hdc, rect.right - 200 - 100, 3, TEXT("��ʾ������Ϣ"), 6);
		break;
	}
	case MSGMode::_REMIND:
	{
		TextOut(hdc, rect.right - 200 - 100, 3, TEXT("��ʾ��ͨ��Ϣ"), 6);
		break;
	}
	}
}
void TextOutWind::UpdateWindowSize(int w, int h)
{
	for (int i = 0; i < m_ChildControl.size(); i++)
		MoveWindow(m_ChildControl[i], w - 50 * (i + 1), 1, 50, 30, true);
}