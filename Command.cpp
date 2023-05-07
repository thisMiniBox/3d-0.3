#include"project.h"

bool Controller::Command(const std::wstring& command)
{
	if (!m_FocusFolder)return false;
	if (command.empty())
	{
		std::wstring wstr;
		std::wstring cd;
		Folder* fp = m_FocusFolder;
		while (fp)
		{
			cd.clear();
			cd += L'/';
			cd += str_wstr(fp->GetName());
			wstr.insert(wstr.begin(), cd.begin(), cd.end());
			fp = fp->GetParent();
		}
		m_CurrentPath = wstr;
		wstr += L'>';
		Print(wstr);
		return true;
	}
	std::wstring OutPut = m_CurrentPath.c_str();
	OutPut += L'>';
	OutPut += command;
	Print(OutPut);
	ComData com = ParseCommand(command.c_str());
	switch (com.Act)
	{
	case CommandAct::OPEN_DIRECTORY:
	{
		Folder* searchP = nullptr;
		if (com.Message[0] == L':')
		{
			searchP = &m_RootFolder;
			com.Message = com.Message.substr(1);
		}
		else
			searchP = m_FocusFolder;
		if (com.Message[0] == L'/')
			com.Message = com.Message.substr(1);
		if (com.Message.back() == L'\0')
			com.Message = com.Message.substr(0, com.Message.size() - 1);
		if (com.Message.back() != L'/')
			com.Message += L'/';
		std::wstring strand;
		while (!com.Message.empty())
		{
			strand = com.Message.substr(0, com.Message.find_first_of(L'/'));
			if (strand == L"..")
				searchP = searchP->GetParent();
			else
				searchP = dynamic_cast<Folder*>(searchP->FindFile_寻找文件(wstr_str(strand).c_str()));
			if (!searchP)
			{
				Print(L"未找到文件夹");
				return false;
			}
			com.Message = com.Message.substr(com.Message.find_first_of(L'/') + 1);
		}
		m_FocusFolder = searchP;
		break;
	}
		break;
	case CommandAct::LOAD_FILE:
		break;
	case CommandAct::LIST_FOLDER:
	{
		for (auto cf : m_FocusFolder->GetFileContent())
		{
			OutPut = strwstr(cf->GetName());
			OutPut += L"    ";
			OutPut += ObjectTypeToWstr(cf->GetType());
			Print(OutPut);
		}
	}
		break;
	case CommandAct::CREATE_FOLDER:
		break;
	case CommandAct::CREATE_FILE:
		break;
	case CommandAct::DELETE_FILE:
		break;
	case CommandAct::OPEN_FILE:
		break;
	case CommandAct::MOVE_FILE:
		break;
	case CommandAct::LIST_SUPPORTED_FILE_TYPES:
		break;
	case CommandAct::MOVE:
		break;
	case CommandAct::SCALE:
		break;
	case CommandAct::ROTATE:
		break;
	case CommandAct::MOVETO:
		break;
	case CommandAct::SCALETO:
		break;
	case CommandAct::ROTATIONTO:
		break;
	case CommandAct::UNKNOWN:
	{
		Print(L"未知指令");
		return false;
	}
	default:
		Print(L"未知指令");
		return false;
		break;
	}
	Command(L"");
	return true;
}
CommandData Controller::ParseCommand(const wchar_t* command)
{
	std::wstringstream wss(command);
	std::wstring cmd, arg1, arg2;

	wss >> cmd;
	CommandData comData;

	if (cmd == L"changedirectory" || cmd == L"cd" || cmd == L"opendirectory" || cmd == L"od") {
		comData.Act = CommandAct::OPEN_DIRECTORY;
		wss >> arg1;
		comData.Message = arg1;
	}
	else if (cmd == L"loadfile") {
		comData.Act = CommandAct::LOAD_FILE;
		wss >> arg1;
		comData.Message = arg1;
	}
	else if (cmd == L"ls" || cmd == L"list") {
		comData.Act = CommandAct::LIST_FOLDER;
		wss >> arg1;
		comData.Message = arg1;
	}
	else if (cmd == L"mkdir" || cmd == L"create") {
		comData.Act = CommandAct::CREATE_FOLDER;
		wss >> arg1;
		comData.Message = arg1;
	}
	else if (cmd == L"ct") {
		comData.Act = CommandAct::CREATE_FILE;
		wss >> arg1 >> arg2;
		comData.Message = arg1 + L" " + arg2;
	}
	else if (cmd == L"rm" || cmd == L"remove" || cmd == L"delete" || cmd == L"dlt") {
		comData.Act = CommandAct::DELETE_FILE;
		wss >> arg1;
		comData.Message = arg1;
	}
	else if (cmd == L"mv" || cmd == L"move") {
		comData.Act = CommandAct::MOVE_FILE;
		wss >> arg1 >> arg2;
		comData.Message = arg1 + L" " + arg2;
	}
	else if (cmd == L"of" || cmd == L"openfile") {
		comData.Act = CommandAct::OPEN_FILE;
		wss >> arg1;
		comData.Message = arg1;
	}
	else if (cmd == L"fmv" || cmd == L"fmove") {
		comData.Act = CommandAct::MOVE;
		std::wstring x, y, z;
		std::getline(wss >> std::ws, x, L',');
		std::getline(wss >> std::ws, y, L',');
		std::getline(wss >> std::ws, z);
		comData.Message = x + L" " + y + L" " + z;
	}
	else if (cmd == L"fscale" || cmd == L"scl" || cmd == L"scale") {
		comData.Act = CommandAct::SCALE;
		std::wstring x, y, z;
		std::getline(wss >> std::ws, x, L',');
		std::getline(wss >> std::ws, y, L',');
		std::getline(wss >> std::ws, z);
		comData.Message = x + L" " + y + L" " + z;
	}
	else if (cmd == L"frotate" || cmd == L"rte" || cmd == L"rotate") {
		comData.Act = CommandAct::ROTATE;
		std::wstring angle, x, y, z;
		wss >> angle;
		std::getline(wss >> std::ws, x, L',');
		std::getline(wss >> std::ws, y, L',');
		std::getline(wss >> std::ws, z);
		comData.Message = angle + L" " + x + L" " + y + L" " + z;
	}
	else if (cmd == L"mvt" || cmd == L"moveto") {
		comData.Act = CommandAct::MOVETO;
		std::wstring x, y, z;
		wss >> x >> y >> z;
		comData.Message = x + L" " + y + L" " + z;
	}
	else if (cmd == L"sclto" || cmd == L"scaleto") {
		comData.Act = CommandAct::SCALETO;
		std::wstring x, y, z;
		wss >> x >> y >> z;
		comData.Message = x + L" " + y + L" " + z;
	}
	else if (cmd == L"rteto" || cmd == L"rotationto") {
		comData.Act = CommandAct::ROTATIONTO;
		std::wstring angle, x, y, z;
		wss >> angle >> x >> y >> z;
		comData.Message = angle + L" " + x + L" " + y + L" " + z;
	}
	else if (cmd == L"lsftp") {
		comData.Act = CommandAct::LIST_SUPPORTED_FILE_TYPES;
	}
	else {
		comData.Act = CommandAct::UNKNOWN;
	}

	return comData;
}