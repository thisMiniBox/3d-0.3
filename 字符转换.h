#pragma once
#include<string>
#include <codecvt>
#include <locale>
#include<Windows.h>
#include<commctrl.h>
#include<iostream>
#include"stb_image.h"
std::string wstr_str(const std::wstring& ws);
std::wstring str_wstr(const std::string& s);
int LoadPngToList(int, HIMAGELIST, HINSTANCE hInst = GetModuleHandle(NULL));
#define wstrtostr wstr_str
#define WstrToStr wstr_str
#define strtowstr str_wstr
#define StrToWstr str_wstr
#define wstrstr wstr_str
#define strwstr str_wstr