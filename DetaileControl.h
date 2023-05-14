#pragma once
#include<windows.h>
#include<string>
#include"vector_向量.h"
#include"resource.h"
#include"Object.h"

std::wstring NumberToWString(const double value);
void PictureDataToBitmap(const PictureData& pictureData, HBITMAP& hBitmap);
bool ScaleBitmap(HBITMAP srcBitmap, int destWidth, int destHeight, HBITMAP& destBitmap);

enum _ControlType
{
	CT_NAME,
	CT_FILEVIEW,
	CT_ROTATE,
	CT_TRANSFORM,
	CT_PICTURE,
};
class DetaileControl_细节菜单控件组合
{
protected:
	HWND m_hWnd;
public:
	HWND GethWnd()const { return m_hWnd; }
	void Hide()const { ShowWindow(m_hWnd, SW_HIDE); }
	bool IsVisible() const { return IsWindowVisible(m_hWnd); }
	DetaileControl_细节菜单控件组合() :m_hWnd(nullptr) {}
	virtual ~DetaileControl_细节菜单控件组合();
	virtual int MoveWind_移动窗口(int x, int y, int w);
	int GetHeight();
	static bool IsNumeric(const std::wstring& str);

	virtual void updateControl() {};
	virtual _ControlType GetType()const = 0;
};
class FileContentView :public DetaileControl_细节菜单控件组合
{
public:
	FileContentView(HINSTANCE, HWND parent, int x, int y, int w, Folder*);
	~FileContentView();
	virtual _ControlType GetType()const override { return CT_FILEVIEW; }
	static INT_PTR Dlgproc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
private:
	Folder* m_folder;
};
class Name_对象名称控件 : public DetaileControl_细节菜单控件组合
{
public:
	static INT_PTR Dlgproc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
	Name_对象名称控件(HINSTANCE hIns, HWND parent, int x, int y, int w);
	virtual _ControlType GetType()const override { return CT_NAME; }
};
class Rotation_旋转控件 :public DetaileControl_细节菜单控件组合
{
public:
	Rotation_旋转控件(HINSTANCE, HWND parent, int x, int y, int w);
	virtual _ControlType GetType()const override { return CT_ROTATE; }
	static INT_PTR Dlgproc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
};
class TransForm_变换控件 :public DetaileControl_细节菜单控件组合
{
public:
	TransForm_变换控件(HINSTANCE, HWND parent, int x, int y, int w);
	virtual _ControlType GetType()const override { return CT_TRANSFORM; }
	static INT_PTR Dlgproc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void updateControl()override;
};
class PictureControl : public DetaileControl_细节菜单控件组合 {
public:
    PictureControl(HINSTANCE hInstance, HWND parent, int x, int y, int w, Picture* pPicture);
	~PictureControl();
    virtual _ControlType GetType() const override { return CT_PICTURE; }
	
private:
	HWND m_hStatic;
    Picture* m_pPicture; // 成员变量，指向要显示的 Picture 对象
    float m_AspectRatio; // 缩放比例，单位百分比
	void ScalePicture(int w);
	int MoveWind_移动窗口(int x, int y, int w)override;
};
