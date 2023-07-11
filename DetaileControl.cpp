#include "DetaileControl.h"
FileContentView::FileContentView(HINSTANCE hInst,HWND parent, int x, int y, int w,Folder* f)
{
    m_folder = f;
	m_hWnd = CreateDialog(hInst, MAKEINTRESOURCE(IDD_FILE_VIEW), parent, Dlgproc);
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
FileContentView::~FileContentView()
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
    SendMessage(m_hWnd, UM_UPDATE, 0, 0);
}

void PictureDataToBitmap(const PictureData& pictureData, HBITMAP& hBitmap)
{
    // 创建 BITMAPINFOHEADER 结构体
    BITMAPINFOHEADER bmpInfoHeader = { 0 };
    bmpInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmpInfoHeader.biWidth = pictureData.m_Width;
    bmpInfoHeader.biHeight = pictureData.m_Height;
    bmpInfoHeader.biPlanes = 1;
    bmpInfoHeader.biBitCount = 24; // 每个像素使用 24 位表示
    bmpInfoHeader.biCompression = BI_RGB;

    // 计算像素数据所需的字节数
    int stride = ((bmpInfoHeader.biWidth * bmpInfoHeader.biBitCount + 31) & ~31) / 8;
    int bufferSize = stride * bmpInfoHeader.biHeight;

    // 分配内存并填充像素数据
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

    // 创建 HBITMAP
    HDC hdc = GetDC(NULL);
    hBitmap = CreateDIBitmap(hdc, &bmpInfoHeader, CBM_INIT, buffer, (BITMAPINFO*)&bmpInfoHeader, DIB_RGB_COLORS);

    // 释放内存
    delete[] buffer;
    ReleaseDC(NULL, hdc);
}
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")

bool ScaleBitmap(HBITMAP srcBitmap, int destWidth, int destHeight, HBITMAP& destBitmap)
{
    // 加载 Bitmap 对象
    Gdiplus::Bitmap srcGdipBitmap(srcBitmap, nullptr);

    // 创建目标 Bitmap 对象
    Gdiplus::Bitmap* pDestGdipBitmap = new Gdiplus::Bitmap(destWidth, destHeight, srcGdipBitmap.GetPixelFormat());

    // 创建 Graphics 对象
    Gdiplus::Graphics destGraphics(pDestGdipBitmap);

    // 绘制原始图像到目标图像
    destGraphics.DrawImage(&srcGdipBitmap, 0, 0, destWidth, destHeight);

    // 获取目标图像的 HBITMAP 句柄
    HBITMAP hDestBitmap = nullptr;
    pDestGdipBitmap->GetHBITMAP(Gdiplus::Color(), &hDestBitmap);
    destBitmap = hDestBitmap;

    // 释放资源
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
int PictureControl::MoveWind_移动窗口(int x, int y, int w)
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
IndexControl::IndexControl(HINSTANCE hInstance, HWND parent, int x, int y, int w)
{
    WndClassName = L"IndexControl";
    WNDCLASSEX wcex = { 0 };
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszClassName = WndClassName.c_str();
    if (!RegisterClassEx(&wcex))
    {
        MessageBox(nullptr,
            L"窗口类注册失败！",
            L"错误",
            MB_ICONERROR);
    }
     
    // 执行应用程序初始化:
    m_hWnd = CreateWindowW(
        WndClassName.c_str(),
        WndClassName.c_str(),
        WS_CHILD | WS_BORDER,
        x,
        y,
        w,
        100,
        nullptr,
        nullptr,
        hInstance,
        nullptr);
}
TableControl::TableControl(HINSTANCE hInstance, HWND parent, int x, int y, int w)
{
    m_WndClassName = L"TableControl";
    WNDCLASSEX wcex = { 0 };
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszClassName = m_WndClassName.c_str();
    if (!RegisterClassEx(&wcex))
    {
        MessageBox(nullptr,
            L"窗口类注册失败！",
            L"错误",
            MB_ICONERROR);
    }

    // 执行应用程序初始化:
    m_hWnd = CreateWindowW(
        m_WndClassName.c_str(),
        m_WndClassName.c_str(),
        WS_CHILD | WS_BORDER,
        x,
        y,
        w,
        100,
        nullptr,
        nullptr,
        hInstance,
        nullptr);
    m_hInst = hInstance;
}

TableControl::~TableControl()
{
    // 销毁表格控件的窗口对象
    DestroyWindow(m_hWnd);
    UnregisterClass(L"TableControl", m_hInst);
}

int TableControl::AddColumn(const std::wstring& name,TableControlDataType dataType)
{
    switch (dataType)
    {
    case TableControlDataType::NUMBER:
    {
        std::vector<std::variant<std::wstring, int>> rowData;
        rowData.push_back('i');
        m_tableData.push_back(rowData);
    }
        break;
    case TableControlDataType::CHAR:
    {
        std::vector<std::variant<std::wstring, int>> rowData;
        rowData.push_back('c');
        m_tableData.push_back(rowData);
    }
        break;
    default:
        return 0;
        break;
    }
    int columnIndex = m_tableData.size();
    m_ColumnNameIndex.insert(std::make_pair(name, columnIndex));
    return columnIndex;
}

bool TableControl::DeleteColumn(const std::wstring& name)
{
    auto p = m_ColumnNameIndex.find(name);
    if (p == m_ColumnNameIndex.end())
        return false;
    int index = (*p).second;
    auto t = m_tableData.begin() + index - 1;
    m_tableData.erase(t);
    m_ColumnNameIndex.erase(p);
    return true;
}

bool TableControl::DeleteRow(int id)
{
    if (id > m_tableData.size())
        return false;
    auto t = m_tableData.begin() + id - 1;
    m_tableData.erase(t);

    m_ColumnNameIndex.erase(p);
    return true;
}

bool TableControl::DeleteColumn(int id)
{
    if (id < 0 || id >= static_cast<int>(m_tableData[0].size()))
        return false;

    // 删除对应的列数据
    for (auto& row : m_tableData)
    {
        row.erase(row.begin() + id);
    }

    // 更新列索引
    std::wstring columnName;
    for (auto& pair : m_ColumnNameIndex)
    {
        if (pair.second == id)
        {
            columnName = pair.first;
            break;
        }
    }
    m_ColumnNameIndex.erase(columnName);
    for (auto& pair : m_ColumnNameIndex)
    {
        if (pair.second > id)
        {
            pair.second--;
        }
    }

    // 在表格控件中删除指定的列
    // ...

    return true;
}

bool TableControl::MoveRow(const std::wstring& name, int id)
{
    if (id < 0 || id >= static_cast<int>(m_tableData.size()))
        return false;

    // 移动行数据到指定位置
    std::vector<std::variant<std::wstring, int>> row = m_tableData[id];
    m_tableData.erase(m_tableData.begin() + id);
    m_tableData.insert(m_tableData.begin() + id, row);

    // 在表格控件中更新行的展示顺序
    // ...

    return true;
}

bool TableControl::MoveColumn(const std::wstring& name, int id)
{
    if (m_ColumnNameIndex.count(name) == 0 || id < 0 || id >= static_cast<int>(m_tableData[0].size()))
        return false;

    int columnIndex = m_ColumnNameIndex[name];

    // 更新列索引，交换列的位置
    for (auto& pair : m_ColumnNameIndex)
    {
        if (pair.second == id)
        {
            pair.second = columnIndex;
        }
        else if (pair.second == columnIndex)
        {
            pair.second = id;
        }
    }

    // 更新每行数据中对应列的值
    for (auto& row : m_tableData)
    {
        std::swap(row[columnIndex], row[id]);
    }

    // 在表格控件中更新列的展示顺序
    // ...

    return true;
}

bool TableControl::Set(int x, int y, const std::wstring& content)
{
    if (x < 0 || x >= static_cast<int>(m_tableData.size()) || y < 0 || y >= static_cast<int>(m_tableData[0].size()))
        return false;

    // 设置指定位置的单元格内容
    m_tableData[x][y] = content;

    // 在表格控件中更新指定位置的单元格展示内容
    // ...

    return true;
}

bool TableControl::Push(const std::wstring& content)
{
    // 将一行数据添加到表格的末尾
    std::vector<std::variant<std::wstring, int>> row;
    for (int i = 0; i < static_cast<int>(m_tableData[0].size()); i++)
    {
        row.emplace_back(L"");
    }
    row[0] = content;

    m_tableData.push_back(row);

    // 在表格控件中添加新的一行
    // ...

    return true;
}