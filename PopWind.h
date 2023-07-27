#pragma once
#include"resource.h"
#include"WndData.h"
#include"vector_向量.h"
#include<thread>

using namespace vec;
class PopWind
{
};
vec::Vector3 GetVectorByPopWind(vec::Vector3 DefaultVector,const std::wstring& text = L"获取向量", const std::wstring& Xtext = L"X",
	const std::wstring& Ytext = L"Y", const std::wstring& Ztext = L"Z", HWND parent = nullptr);
