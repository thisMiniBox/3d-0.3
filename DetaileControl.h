#pragma once
#include<windows.h>
#include<string>
#include"vector_����.h"
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
	virtual int MoveWind_�ƶ�����(int x, int y, int w);
	int GetHeight();
	static bool IsNumeric(const std::wstring& str);

	virtual void updateControl() {};
	virtual _ControlType GetType()const = 0;
};
class FileContentView :public DetaileControl_ϸ�ڲ˵��ؼ����
{
public:
	FileContentView(HINSTANCE, HWND parent, int x, int y, int w, Folder*);
	~FileContentView();
	virtual _ControlType GetType()const override { return CT_FILEVIEW; }
	static INT_PTR Dlgproc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
private:
	Folder* m_folder;
};
class Name_�������ƿؼ� : public DetaileControl_ϸ�ڲ˵��ؼ����
{
public:
	static INT_PTR Dlgproc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
	Name_�������ƿؼ�(HINSTANCE hIns, HWND parent, int x, int y, int w);
	virtual _ControlType GetType()const override { return CT_NAME; }
};
class Rotation_��ת�ؼ� :public DetaileControl_ϸ�ڲ˵��ؼ����
{
public:
	Rotation_��ת�ؼ�(HINSTANCE, HWND parent, int x, int y, int w);
	virtual _ControlType GetType()const override { return CT_ROTATE; }
	static INT_PTR Dlgproc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
};
class TransForm_�任�ؼ� :public DetaileControl_ϸ�ڲ˵��ؼ����
{
public:
	TransForm_�任�ؼ�(HINSTANCE, HWND parent, int x, int y, int w);
	virtual _ControlType GetType()const override { return CT_TRANSFORM; }
	static INT_PTR Dlgproc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void updateControl()override;
};
class PictureControl : public DetaileControl_ϸ�ڲ˵��ؼ���� {
public:
    PictureControl(HINSTANCE hInstance, HWND parent, int x, int y, int w, Picture* pPicture);
	~PictureControl();
    virtual _ControlType GetType() const override { return CT_PICTURE; }
	
private:
	HWND m_hStatic;
    Picture* m_pPicture; // ��Ա������ָ��Ҫ��ʾ�� Picture ����
    float m_AspectRatio; // ���ű�������λ�ٷֱ�
	void ScalePicture(int w);
	int MoveWind_�ƶ�����(int x, int y, int w)override;
};
