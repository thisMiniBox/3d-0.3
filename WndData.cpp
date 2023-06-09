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
    if (pos == std::string::npos) return std::string(); // ���û���ҵ���ţ�˵��û����չ�����򷵻ؿ��ַ���

    return path.substr(pos); // ���ص�ż��������ַ���������չ��
}
std::wstring GetFileExtension(const std::wstring& path)
{
    size_t pos = path.rfind('.');
    if (pos == std::wstring::npos) return std::wstring(); // ���û���ҵ���ţ�˵��û����չ�����򷵻ؿ��ַ���

    return path.substr(pos); // ���ص�ż��������ַ���������չ��
}