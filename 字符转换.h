#pragma once
#include<string>
#include <codecvt>
#include <locale>
#include<Windows.h>
#include<iostream>
std::string wstr_str(const std::wstring& ws);
std::wstring str_wstr(const std::string& s);