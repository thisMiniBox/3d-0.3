// header.h: 标准系统包含文件的包含文件，
// 或特定于项目的包含文件
//

#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // 从 Windows 头文件中排除极少使用的内容
// Windows 头文件
#include <windows.h>
#include <commctrl.h>
#include <d2d1.h>
#pragma comment(lib, "d2d1")
#pragma comment (lib,"Comctl32.lib")
// C 运行时头文件
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include<thread>
//功能头文件
#include"vector_向量.h"
#include"消息循环声明.h"
#include"Menu.h"
#include"Object.h"
#include"project.h"
#include"LargeNumber.h"