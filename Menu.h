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
#include"PopWind.h"

//�˵���Ϣ����
LRESULT __stdcall Menu(HINSTANCE, HWND, UINT, WPARAM, LPARAM, Controller*);
//�����ڡ�������Ϣѭ��
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);
//������С����
INT_PTR CALLBACK SetSize(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
//���ļ������ص�ַ
std::wstring MenuGetPath(void);