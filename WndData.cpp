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