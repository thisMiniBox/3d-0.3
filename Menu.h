#pragma once
#include"project.h"
#include<shobjidl.h>
#include<iostream>
#include<string>
#include<Windows.h>
#include<thread>
#include<string>
#include <thread>
#include <mutex>
#include <queue>


//菜单消息处理
LRESULT __stdcall Menu(HINSTANCE, HWND, UINT, WPARAM, LPARAM, project*);
//“关于”窗口消息循环
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);
//调整大小窗口
INT_PTR CALLBACK SetSize(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
//打开文件，返回地址
std::wstring MenuGetPath(void);