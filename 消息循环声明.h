#pragma once
#include <chrono>
#include <ctime>
#include"project.h"
#include"Object.h"
#include <commctrl.h>
#include<Windows.h>
#include<d3d11.h>
#pragma comment(lib, "d3d11.lib")

LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    cMainWndProc(HWND, UINT, WPARAM, LPARAM,project*);
LRESULT CALLBACK    cFileWndProc(HWND, UINT, WPARAM, LPARAM,project*);
LRESULT CALLBACK    cTextWndProc(HWND, UINT, WPARAM, LPARAM,project*);
LRESULT CALLBACK    cDetaileWndProc(HWND, UINT, WPARAM, LPARAM, project*);
LRESULT CALLBACK    cD3D11WndProc(HWND, UINT, WPARAM, LPARAM, project*);