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
	MoveWindow(m_hWnd, x, y, w, GetHeight(), true);
}
Rotation_��ת�ؼ�::Rotation_��ת�ؼ�(HINSTANCE hIns, HWND parent, int x, int y, int w)
{
	m_hWnd = CreateDialog(hIns, MAKEINTRESOURCE(IDD_ROTATE), parent, Dlgproc);
	MoveWindow(m_hWnd, x, y, w, GetHeight(), true);
}
TransForm_�任�ؼ�::TransForm_�任�ؼ�(HINSTANCE hIns, HWND parent, int x, int y, int w)
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

void TransForm_�任�ؼ�::updateControl()
{

}

void PictureDataToBitmap(const PictureData& pictureData, HBITMAP& hBitmap)
{
    // ���� BITMAPINFOHEADER �ṹ��
    BITMAPINFOHEADER bmpInfoHeader = { 0 };
    bmpInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmpInfoHeader.biWidth = pictureData.m_Width;
    bmpInfoHeader.biHeight = pictureData.m_Height;
    bmpInfoHeader.biPlanes = 1;
    bmpInfoHeader.biBitCount = 24; // ÿ������ʹ�� 24 λ��ʾ
    bmpInfoHeader.biCompression = BI_RGB;

    // ������������������ֽ���
    int stride = ((bmpInfoHeader.biWidth * bmpInfoHeader.biBitCount + 31) & ~31) / 8;
    int bufferSize = stride * bmpInfoHeader.biHeight;

    // �����ڴ沢�����������
    unsigned char* buffer = new unsigned char[bufferSize];
    memset(buffer, 0, bufferSize);
    unsigned char* dst = buffer;
    for (int y = 0; y < pictureData.m_Height; ++y)
    {
        for (int x = 0; x < pictureData.m_Width; ++x)
        {
            int srcIdx = (y * pictureData.m_Width + x) * pictureData.m_NrComponents;
            *(dst++) = pictureData.m_Data[srcIdx + 2]; // R
            *(dst++) = pictureData.m_Data[srcIdx + 1]; // G
            *(dst++) = pictureData.m_Data[srcIdx + 0]; // B
        }
        dst += (stride - pictureData.m_Width * 3);
    }

    // ���� HBITMAP
    HDC hdc = GetDC(NULL);
    hBitmap = CreateDIBitmap(hdc, &bmpInfoHeader, CBM_INIT, buffer, (BITMAPINFO*)&bmpInfoHeader, DIB_RGB_COLORS);

    // �ͷ��ڴ�
    delete[] buffer;
    ReleaseDC(NULL, hdc);
}
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")

bool ScaleBitmap(HBITMAP srcBitmap, int destWidth, int destHeight, HBITMAP& destBitmap)
{
    // ���� Bitmap ����
    Gdiplus::Bitmap srcGdipBitmap(srcBitmap, nullptr);

    // ����Ŀ�� Bitmap ����
    Gdiplus::Bitmap* pDestGdipBitmap = new Gdiplus::Bitmap(destWidth, destHeight, srcGdipBitmap.GetPixelFormat());

    // ���� Graphics ����
    Gdiplus::Graphics destGraphics(pDestGdipBitmap);

    // ����ԭʼͼ��Ŀ��ͼ��
    destGraphics.DrawImage(&srcGdipBitmap, 0, 0, destWidth, destHeight);

    // ��ȡĿ��ͼ��� HBITMAP ���
    HBITMAP hDestBitmap = nullptr;
    pDestGdipBitmap->GetHBITMAP(Gdiplus::Color(), &hDestBitmap);
    destBitmap = hDestBitmap;

    // �ͷ���Դ
    delete pDestGdipBitmap;
    return true;
}
PictureControl::PictureControl(HINSTANCE hIns, HWND parent, int x, int y, int w, Picture* picture) :m_pPicture(picture)
{
    PictureData pictureData = picture->GetPicture();
    m_AspectRatio = (float)pictureData.m_Width / pictureData.m_Height;
    m_hWnd = m_hWnd = CreateWindowEx(0, L"pictureproc", nullptr, WS_CHILD | WS_VISIBLE,
        x, y, w, (float)w / m_AspectRatio, parent, nullptr, hIns, nullptr);
    MoveWindow(m_hWnd, x, y, w, (float)w / m_AspectRatio, true);
    m_hStatic = CreateWindowEx(0, L"STATIC", nullptr, WS_CHILD | WS_VISIBLE | SS_BITMAP,
        0, 0, w, (float)w / m_AspectRatio, m_hWnd, nullptr, hIns, nullptr);
    ScalePicture(w);
}
void PictureControl::ScalePicture(int w)
{
    PictureData pictureData = m_pPicture->GetPicture();
    HBITMAP hBitmap;
    PictureDataToBitmap(pictureData, hBitmap);
    HBITMAP Scaled;
    ScaleBitmap(hBitmap, w, (float)w / m_AspectRatio, Scaled);
    SendMessage(m_hStatic, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)Scaled); 
    DeleteObject(hBitmap);
    DeleteObject(Scaled);

}
int PictureControl::MoveWind_�ƶ�����(int x, int y, int w)
{
    if (IsWindowVisible(m_hWnd))
    {
        int h = (float)w / m_AspectRatio;
        MoveWindow(m_hWnd, x, y, w, h, true);
        MoveWindow(m_hStatic, 0, 0, w, h, true);
        ScalePicture(w);
        InvalidateRect(m_hWnd, NULL, true);
        return h;
    }
    return 0;
}
PictureControl::~PictureControl()
{
    SendMessage(m_hWnd, WM_CLOSE, 0, 0);
}