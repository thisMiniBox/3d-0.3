#include"字符转换.h"
std::string wstr_str(const std::wstring& str)
{
    std::string newbuf;
    int len = WideCharToMultiByte(CP_ACP, 0, str.data(), -1, nullptr, 0, 0, NULL);
    newbuf.resize(len);
    WideCharToMultiByte(CP_ACP, 0, str.data(), -1, &newbuf[0], len, 0, NULL);
    return std::move(newbuf);
}
std::wstring str_wstr(const std::string& s)
{
    int len = MultiByteToWideChar(CP_ACP, 0, s.data(), -1, nullptr, 0);
    std::wstring newbuf(len-1, L'\0');
    MultiByteToWideChar(CP_ACP, 0, s.data(), -1, &newbuf[0], len-1);
    return std::move(newbuf);
}
int LoadPngToList(int Index, HIMAGELIST hImage, HINSTANCE hInst)
{
    // 加载 PNG 图片资源
    HRSRC hResource = FindResource(hInst, MAKEINTRESOURCE(Index), L"PNG");
    if (hResource == nullptr) {
        return -1;
    }
    HGLOBAL hData = LoadResource(hInst, hResource);
    if (hData == nullptr) {
        return -1;
    }
    LPBYTE pData = (LPBYTE)LockResource(hData);
    DWORD dwSize = SizeofResource(hInst, hResource);
    if (pData == nullptr || dwSize == 0) {
        return -1;
    }

    // 解码 PNG 图片数据
    int width, height, channels;
    unsigned char* image_data = stbi_load_from_memory(pData, dwSize, &width, &height, &channels, STBI_rgb_alpha);
    FreeResource(hData);
    if (image_data == nullptr) {
        return -1;
    }

    // 创建 DIB 对象
    BITMAPINFO bmi;
    ZeroMemory(&bmi, sizeof(bmi));
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = width;
    bmi.bmiHeader.biHeight = -height; // 指定负数即可让像素从上到下排列
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;
    void* pBits = nullptr;
    HBITMAP hBitmap = CreateDIBSection(nullptr, &bmi, DIB_RGB_COLORS, &pBits, nullptr, 0);
    if (hBitmap == nullptr || pBits == nullptr) {
        stbi_image_free(image_data);
        return -1;
    }

    // 将像素数据复制到 DIB 对象中
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int index = y * width + x;
            unsigned char r = image_data[index * 4 + 0];
            unsigned char g = image_data[index * 4 + 1];
            unsigned char b = image_data[index * 4 + 2];
            unsigned char a = image_data[index * 4 + 3];
            ((BYTE*)pBits)[index * 4 + 0] = b;
            ((BYTE*)pBits)[index * 4 + 1] = g;
            ((BYTE*)pBits)[index * 4 + 2] = r;
            ((BYTE*)pBits)[index * 4 + 3] = a;
        }
    }

    // 释放 PNG 图片数据内存
    stbi_image_free(image_data);

    int out = ImageList_AddMasked(hImage, hBitmap, RGB(0, 0, 0));
    DeleteObject(hBitmap);
    return out;
}