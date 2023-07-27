#include"WndData.h"
int getBit(int num, int pos)
{
    return (num >> pos) & 1;
}
ReturnedOfLoadFile operator|(ReturnedOfLoadFile a, ReturnedOfLoadFile b)
{
    return static_cast<ReturnedOfLoadFile>(static_cast<unsigned int>(a) | static_cast<unsigned int>(b));
}
ReturnedOfLoadFile& operator|=(ReturnedOfLoadFile& a, ReturnedOfLoadFile b)
{
    a = static_cast<ReturnedOfLoadFile>(static_cast<unsigned int>(a) | static_cast<unsigned int>(b));
    return a;
}
void ReplaceChar(std::string& str, char from, char to)
{
	for (auto& c : str)
	{
		if (c == from) c = to;
	}
}
std::string GetFileExtension(const std::string& path)
{
    size_t pos = path.rfind('.');
    if (pos == std::string::npos) return std::string(); // 如果没有找到点号，说明没有扩展名，则返回空字符串

    return path.substr(pos); // 返回点号及其后面的字符串，即扩展名
}
std::wstring GetFileExtension(const std::wstring& path)
{
    size_t pos = path.rfind('.');
    if (pos == std::wstring::npos) return std::wstring(); // 如果没有找到点号，说明没有扩展名，则返回空字符串

    return path.substr(pos); // 返回点号及其后面的字符串，即扩展名
}
bool RenameFile(const std::string& originalFilename, const std::string& newFilename) {
	if (rename(originalFilename.c_str(), newFilename.c_str()) == 0) {
		std::cout << "文件名已修改为 " << newFilename << std::endl;
		return true;
	}
	else {
		std::cerr << "修改文件名失败" << std::endl;
		return false;
	}
}
bool RenameFile(const std::wstring& originalFilename, const std::wstring& newFilename) {
    if (MoveFile(originalFilename.c_str(), newFilename.c_str()) != 0) {
        std::wcout << L"文件名已修改为 " << newFilename << std::endl;
        return true;
    }
    else {
        DWORD error = GetLastError();
        LPWSTR errorMsg = nullptr;
        FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, error,
            0, reinterpret_cast<LPWSTR>(&errorMsg), 0, nullptr);
        std::wcerr << L"修改文件名失败: " << errorMsg << std::endl;
        LocalFree(errorMsg);
        return false;
    }
}