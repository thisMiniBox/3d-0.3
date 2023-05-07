#pragma once
#include"×Ö·û×ª»».h"
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
