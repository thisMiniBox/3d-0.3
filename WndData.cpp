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
bool RenameFile(const std::string& originalFilename, const std::string& newFilename) {
	if (rename(originalFilename.c_str(), newFilename.c_str()) == 0) {
		std::cout << "�ļ������޸�Ϊ " << newFilename << std::endl;
		return true;
	}
	else {
		std::cerr << "�޸��ļ���ʧ��" << std::endl;
		return false;
	}
}
bool RenameFile(const std::wstring& originalFilename, const std::wstring& newFilename) {
    if (MoveFile(originalFilename.c_str(), newFilename.c_str()) != 0) {
        std::wcout << L"�ļ������޸�Ϊ " << newFilename << std::endl;
        return true;
    }
    else {
        DWORD error = GetLastError();
        LPWSTR errorMsg = nullptr;
        FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, error,
            0, reinterpret_cast<LPWSTR>(&errorMsg), 0, nullptr);
        std::wcerr << L"�޸��ļ���ʧ��: " << errorMsg << std::endl;
        LocalFree(errorMsg);
        return false;
    }
}