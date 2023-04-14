#pragma once
#include"EditControl.h"
#include<windows.h>
#include"vector_����.h"
#include"resource.h"
enum _ControlType
{
	CT_NAME,
	CT_POSITION,
	CT_ROTATE
};
class DetaileControl_ϸ�ڲ˵��ؼ����
{
protected:
	HWND m_hWnd;
public:
	HWND GethWnd()const { return m_hWnd; }
	void Hide()const { ShowWindow(m_hWnd, SW_HIDE); }
	bool IsVisible() const { return IsWindowVisible(m_hWnd); }
	DetaileControl_ϸ�ڲ˵��ؼ����() :m_hWnd(nullptr) {}
	virtual ~DetaileControl_ϸ�ڲ˵��ؼ����();
	int MoveWind_�ƶ�����(int x, int y, int w);
	int GetHeight();
	static bool IsNumeric(const std::wstring& str);
};
class Position_λ�ÿؼ� :public DetaileControl_ϸ�ڲ˵��ؼ����
{
public:
	Position_λ�ÿؼ�(HINSTANCE,HWND parent, int x, int y, int w);
	~Position_λ�ÿؼ�();
	static INT_PTR Dlgproc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
private:
	vec::Vector3 m_Position;
};
class Name_�������ƿؼ� : public DetaileControl_ϸ�ڲ˵��ؼ����
{
public:
	static INT_PTR Dlgproc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
	Name_�������ƿؼ�(HINSTANCE hIns, HWND parent, int x, int y, int w);
};
class Rotation_��ת�ؼ� :public DetaileControl_ϸ�ڲ˵��ؼ����
{
public:
	Rotation_��ת�ؼ�(HINSTANCE, HWND parent, int x, int y, int w);
	static INT_PTR Dlgproc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
};