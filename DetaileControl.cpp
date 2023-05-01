#include "DetaileControl.h"
Position_位置控件::Position_位置控件(HINSTANCE hIns,HWND parent, int x, int y, int w)
{
	m_hWnd = CreateDialog(hIns, MAKEINTRESOURCE(IDD_POSITION), parent, Dlgproc);
	MoveWindow(m_hWnd, x, y, w, GetHeight(), true);
}
int DetaileControl_细节菜单控件组合::GetHeight()
{
	RECT m_rect = {};
	GetClientRect(m_hWnd, &m_rect);
	return m_rect.bottom - m_rect.top;
}
int DetaileControl_细节菜单控件组合::MoveWind_移动窗口(int x, int y, int w)
{
	if (IsWindowVisible(m_hWnd))
	{
		int h = GetHeight();
		MoveWindow(m_hWnd, x, y, w, h, true);
		return h;
	}
	return 0;
}
Position_位置控件::~Position_位置控件()
{
	DestroyWindow(m_hWnd);
}
DetaileControl_细节菜单控件组合::~DetaileControl_细节菜单控件组合()
{
	DestroyWindow(m_hWnd);
}
bool DetaileControl_细节菜单控件组合::IsNumeric(const std::wstring& str)
{
	if (str.empty())
	{
		return false;
	}
	try
	{
		// 使用 stod 函数进行转换，如果转换成功则说明该字符串是数值类型
		double a = std::stod(std::wstring(str.begin(), str.end()));
		return true;
	}
	catch (const std::exception&)
	{
		return false; // 转换失败，字符串不是数值类型
	}
}
#include <tchar.h> 
Name_对象名称控件::Name_对象名称控件(HINSTANCE hIns, HWND parent, int x, int y, int w)
{
	m_hWnd = CreateDialog(hIns, MAKEINTRESOURCE(IDD_CNAME), parent, Dlgproc);
#ifdef _DEBUG
	if (m_hWnd == NULL)
	{
		DWORD dwErrorCode = GetLastError();
		TCHAR szBuf[100];
		_stprintf_s(szBuf, _countof(szBuf), TEXT("CreateDialog函数错误 %d"), dwErrorCode);
		OutputDebugString(szBuf);
		std::cout << dwErrorCode << std::endl;
	}
#endif // DEBUG
	MoveWindow(m_hWnd, x, y, w, GetHeight(), true);
}
Rotation_旋转控件::Rotation_旋转控件(HINSTANCE hIns, HWND parent, int x, int y, int w)
{
	m_hWnd = CreateDialog(hIns, MAKEINTRESOURCE(IDD_ROTATE), parent, Dlgproc);
	MoveWindow(m_hWnd, x, y, w, GetHeight(), true);
}
ScaleControl_缩放控件::ScaleControl_缩放控件(HINSTANCE hIns, HWND parent, int x, int y, int w)
{
	m_hWnd = CreateDialog(hIns, MAKEINTRESOURCE(IDD_POSITION), parent, Dlgproc);
	MoveWindow(m_hWnd, x, y, w, GetHeight(), true);
}
TransForm_变换控件::TransForm_变换控件(HINSTANCE hIns, HWND parent, int x, int y, int w)
{
	m_hWnd = CreateDialog(hIns, MAKEINTRESOURCE(IDD_TRANSFORM), parent, Dlgproc);
	MoveWindow(m_hWnd, x, y, w, GetHeight(), true);
}
std::wstring NumberToWString(const double value) {
	std::wstring wstr = std::to_wstring(value);
	size_t dot_pos = wstr.find(L'.');
	if (dot_pos != std::wstring::npos) {
		size_t last_nonzero_pos = wstr.find_last_not_of(L'0');
		if (last_nonzero_pos == dot_pos) {
			return wstr.substr(0, dot_pos);
		}
		else if (last_nonzero_pos != std::wstring::npos) {
			return wstr.substr(0, last_nonzero_pos + 1);
		}
	}
	return wstr;
}

void TransForm_变换控件::updateControl()
{

}