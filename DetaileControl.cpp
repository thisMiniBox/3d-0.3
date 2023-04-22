#include "DetaileControl.h"
Position_λ�ÿؼ�::Position_λ�ÿؼ�(HINSTANCE hIns,HWND parent, int x, int y, int w)
{
	m_hWnd = CreateDialog(hIns, MAKEINTRESOURCE(IDD_POSITION), parent, Dlgproc);
	MoveWindow(m_hWnd, x, y, w, GetHeight(), true);
}
int DetaileControl_ϸ�ڲ˵��ؼ����::GetHeight()
{
	RECT m_rect = {};
	GetClientRect(m_hWnd, &m_rect);
	return m_rect.bottom - m_rect.top;
}
int DetaileControl_ϸ�ڲ˵��ؼ����::MoveWind_�ƶ�����(int x, int y, int w)
{
	if (IsWindowVisible(m_hWnd))
	{
		int h = GetHeight();
		MoveWindow(m_hWnd, x, y, w, h, true);
		return h;
	}
	return 0;
}
Position_λ�ÿؼ�::~Position_λ�ÿؼ�()
{
	DestroyWindow(m_hWnd);
}
DetaileControl_ϸ�ڲ˵��ؼ����::~DetaileControl_ϸ�ڲ˵��ؼ����()
{
	DestroyWindow(m_hWnd);
}
bool DetaileControl_ϸ�ڲ˵��ؼ����::IsNumeric(const std::wstring& str)
{
	if (str.empty())
	{
		return false;
	}
	try
	{
		// ʹ�� stod ��������ת�������ת���ɹ���˵�����ַ�������ֵ����
		double a = std::stod(std::wstring(str.begin(), str.end()));
		return true;
	}
	catch (const std::exception&)
	{
		return false; // ת��ʧ�ܣ��ַ���������ֵ����
	}
}
#include <tchar.h> 
Name_�������ƿؼ�::Name_�������ƿؼ�(HINSTANCE hIns, HWND parent, int x, int y, int w)
{
	m_hWnd = CreateDialog(hIns, MAKEINTRESOURCE(IDD_CNAME), parent, Dlgproc);
#ifdef _DEBUG
	if (m_hWnd == NULL)
	{
		DWORD dwErrorCode = GetLastError();
		TCHAR szBuf[100];
		_stprintf_s(szBuf, _countof(szBuf), TEXT("CreateDialog�������� %d"), dwErrorCode);
		OutputDebugString(szBuf);
		std::cout << dwErrorCode << std::endl;
	}
#endif // DEBUG
	MoveWindow(m_hWnd, x, y, w, GetHeight(), true);
}
Rotation_��ת�ؼ�::Rotation_��ת�ؼ�(HINSTANCE hIns, HWND parent, int x, int y, int w)
{
	m_hWnd = CreateDialog(hIns, MAKEINTRESOURCE(IDD_ROTATE), parent, Dlgproc);
	MoveWindow(m_hWnd, x, y, w, GetHeight(), true);
}
ScaleControl_���ſؼ�::ScaleControl_���ſؼ�(HINSTANCE hIns, HWND parent, int x, int y, int w)
{
	m_hWnd = CreateDialog(hIns, MAKEINTRESOURCE(IDD_POSITION), parent, Dlgproc);
	MoveWindow(m_hWnd, x, y, w, GetHeight(), true);
}
