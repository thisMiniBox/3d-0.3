#pragma once
#include<Windows.h>
#include<string>
//°æ±¾0.1
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
    OUTCONTROL,
    UPDATE,
    SLEEP,
};
typedef struct _DllMessageOut
{
    CommandAct act;
    char command[128];
    _DllMessageOut()
    {
        act = CommandAct::UNKNOWN;
        std::fill(command, command + 128, 0);
    }
    _DllMessageOut(CommandAct Act, const char* com) :act(Act)
    {
        if (std::strlen(com) < 128)
            strcpy_s(command, com);
        else
            std::fill(command, command + 128, 0);
    }
}DllMessageOut;


typedef struct CommandData
{
    CommandAct Act;
    std::wstring Parameter;
    CommandData():Act(CommandAct::UNKNOWN){}
}ComData;
