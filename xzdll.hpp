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
enum class CommandAct
{
    UNKNOWN,
    OPEN_DIRECTORY,
    LOAD_FILE,
    LIST_FOLDER,
    CREATE_FOLDER,
    CREATE_FILE,
    DELETE_FILE,
    OPEN_FILE,
    MOVE_FILE,
    LIST_SUPPORTED_FILE_TYPES,
    MOVE,
    SCALE,
    ROTATE,
    MOVETO,
    SCALETO,
    ROTATIONTO,
};

typedef struct CommandData
{
    CommandAct Act;
    std::wstring Message;
    CommandData():Act(CommandAct::UNKNOWN){}
}ComData;
