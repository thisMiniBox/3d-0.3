#pragma once
#include<Windows.h>
#include<string>
typedef struct _DllMessageOut
{
    char command[128];
    _DllMessageOut()
    {
        std::fill(command, command + 128, 0);
    }
    _DllMessageOut(const char* com)
    {
        if (std::strlen(com) < 128)
            strcpy_s(command, com);
        else
            std::fill(command, command + 128, 0);
    }
}DllMessageOut;
typedef DllMessageOut(*ProcessMessagesFunc)(UINT, WPARAM, LPARAM);