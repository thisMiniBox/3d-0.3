#pragma once
#include"×Ö·û×ª»».h"
std::string wstr_str(const std::wstring& str)
{
	int cLen = WideCharToMultiByte(CP_ACP, 0, str.c_str(), -1, NULL, 0, 0, NULL);
	std::string newBuf;
	newBuf.resize(cLen + 1);
	WideCharToMultiByte(CP_ACP, 0, str.c_str(), -1, (char*)newBuf.c_str(), cLen, 0, NULL);
	return std::move(newBuf);
}
//std::wstring str_wstr(const std::string& s)
//{
//	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
//	return converter.from_bytes(s);
//}
std::wstring str_wstr(const std::string& s)
{
	std::string curlLocale = setlocale(LC_ALL, NULL);
	setlocale(LC_ALL, "chs");
	const char* _Source = s.c_str();
	size_t _Dsize = s.size() + 1;
	wchar_t* _Dest = new wchar_t[_Dsize];
	size_t i;
	mbstowcs_s(&i, _Dest, _Dsize, _Source, s.size());
	std::wstring result = _Dest;
	delete[] _Dest;
	setlocale(LC_ALL, curlLocale.c_str());
	return result;
}