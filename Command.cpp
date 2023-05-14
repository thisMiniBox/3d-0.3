#include"project.h"

bool Controller::Command(const std::wstring& command, bool ignoreOutput)
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
		if (!ignoreOutput) Print(wstr);
		return true;
	}
	std::wstring OutPut = m_CurrentPath.c_str();
	OutPut += L'>';
	OutPut += command;
	if (!ignoreOutput) Print(OutPut);
	ComData com = ParseCommand(command.c_str());
	switch (com.Act)
	{
	case CommandAct::OPEN_DIRECTORY:
	{
		Folder* searchP = dynamic_cast<Folder*>(SearchObject(com.Parameter));
		if (!searchP)
		{
			if (!ignoreOutput) Print(L"未找到文件夹");
			return false;
		}
		m_FocusFolder = searchP;

		break;
	}
	break;
	case CommandAct::LOAD_FILE:
	{
		loadModelThread(nullptr, this, com.Parameter);
		break;
	}
	break;
	case CommandAct::LIST_FOLDER:
	{
		for (auto cf : m_FocusFolder->GetFileContent())
		{
			OutPut = strwstr(cf->GetName());
			OutPut += L"    ";
			OutPut += ObjectTypeToWstr(cf->GetType());
			if (!ignoreOutput) Print(OutPut);
		}
	}
	break;
	case CommandAct::CREATE_FOLDER:
	{
		if (!CreateObject(m_FocusFolder, wstrstr(com.Parameter).c_str()))
		{
			if (!ignoreOutput) Print(L"创建失败");
			return false;
		}
	}
	break;
	case CommandAct::CREATE_FILE:
	{
		if (!CreateObject(m_FocusFolder, wstrstr(com.Parameter.substr(com.Parameter.find_first_of(L' ') + 1)).c_str(), WStrToObjectType(com.Parameter.substr(0, com.Parameter.find_first_of(L' ')))))
		{
			if (!ignoreOutput) Print(L"创建失败");
			return false;
		}
	}
	break;
	case CommandAct::DELETE_FILE:
	{
		Object* file = SearchObject(com.Parameter);
		if (!file)
		{
			if (!ignoreOutput) Print(L"未找到文件");
			return false;
		}
		DeleteObject(file);
		UpdateFileView();
	}
	break;
	case CommandAct::OPEN_FILE:
	{
		Object* file = SearchObject(com.Parameter);
		if (!file)
		{
			if (!ignoreOutput) Print(L"未找到文件");
			return false;
		}
		SetFoucusObjcet(file);
	}
	break;
	case CommandAct::MOVE_FILE:
	{
		Object* aim = SearchObject(com.Parameter.substr(0, com.Parameter.find_first_of(L' ')));
		if (!aim)
		{
			if (!ignoreOutput) Print(L"未找到文件");
			return false;
		}
		aim->GetParent()->DeleteIndex(aim);
		com.Parameter = com.Parameter.substr(com.Parameter.find_first_of(L' ') + 1);
		if (com.Parameter.find_first_of(L'/') == -1)
		{
			aim->SetName(wstrstr(com.Parameter));
			m_FocusFolder->AddFile_添加文件(aim);
			UpdateFileView();
			break;
		}
		std::string NewName = wstrstr(com.Parameter.substr(com.Parameter.find_first_of(L'/') + 1));
		Folder* NewFolder = nullptr;
		NewFolder = dynamic_cast<Folder*>(SearchObject(com.Parameter.substr(0, com.Parameter.find_first_of(L'/'))));
		if (!NewFolder)
		{
			if (!ignoreOutput) Print(L"未找到文件");
			return false;
		}
		aim->SetName(NewName);
		NewFolder->AddFile_添加文件(aim);
		UpdateFileView();
	}
	break;
	case CommandAct::LIST_SUPPORTED_FILE_TYPES:
	{
		for (int i = 1; i <= ObjectType::OT_KEYFRAME; i++)
		{
			if (!ignoreOutput) Print(ObjectTypeToWstr((ObjectType)i));
		}
	}
	break;
	case CommandAct::MOVE:
	{
		GetFocusObject()->Move(vec::str_vector(com.Parameter));
	}
	break;
	case CommandAct::SCALE:
	{
		GetFocusObject()->SetScale(vec::str_vector(com.Parameter));
	}
	break;
	case CommandAct::ROTATE:
	{
		Rotation rot;
		int pos = com.Parameter.find_first_of(' ');
		if (!pos)
		{
			if (!ignoreOutput) Print(L"格式不正确");
			return false;
		}
		rot.angle = std::stod((com.Parameter.substr(0, pos)).c_str());
		rot.axis = str_vector(com.Parameter.substr(pos + 1));
		GetFocusObject()->Rotate(rot);
	}
	break;
	case CommandAct::MOVETO:
	{
		GetFocusObject()->SetPosition(vec::str_vector(com.Parameter));
	}
	break;
	case CommandAct::SCALETO:
	{
		GetFocusObject()->SetScale(vec::str_vector(com.Parameter));
	}
	break;
	case CommandAct::ROTATIONTO:
	{
		Rotation rot;
		int pos = com.Parameter.find_first_of(' ');
		if (!pos)
		{
			if (!ignoreOutput) Print(L"格式不正确");
			return false;
		}
		rot.angle = std::stod((com.Parameter.substr(0, pos)).c_str());
		rot.axis = str_vector(com.Parameter.substr(pos + 1));
		GetFocusObject()->SetRotate(rot);
	}
	break;
	case CommandAct::OUTCONTROL:
	{

		break;
	}
	case CommandAct::UPDATE:
	{

		break;
	}
	case CommandAct::SLEEP:
	{
		LONG64 time = std::stoll(com.Parameter);
		if (time > 0)
			Sleep(time);
		break;
	}
	default:
		if (!ignoreOutput) Print(L"未知指令");
		return false;
		break;
	}
	Command(L"", ignoreOutput);
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
		comData.Parameter = arg1;
	}
	else if (cmd == L"loadfile") {
		comData.Act = CommandAct::LOAD_FILE;
		arg1 = wss.str();
		arg1 = arg1.substr(arg1.find_first_of(L' ') + 1);
		if (arg1.back() == L'\n')
		{
			arg1.erase(arg1.end()-1);
		}
		comData.Parameter = arg1;
	}
	else if (cmd == L"ls" || cmd == L"list") {
		comData.Act = CommandAct::LIST_FOLDER;
		wss >> arg1;
		comData.Parameter = arg1;
	}
	else if (cmd == L"mkdir") {
		comData.Act = CommandAct::CREATE_FOLDER;
		wss >> arg1;
		comData.Parameter = arg1;
	}
	else if (cmd == L"ct" || cmd == L"create") {
		comData.Act = CommandAct::CREATE_FILE;
		wss >> arg1 >> arg2;
		comData.Parameter = arg1 + L" " + arg2;
	}
	else if (cmd == L"rm" || cmd == L"remove" || cmd == L"delete" || cmd == L"dlt") {
		comData.Act = CommandAct::DELETE_FILE;
		wss >> arg1;
		comData.Parameter = arg1;
	}
	else if (cmd == L"fmv" || cmd == L"fmove") {
		comData.Act = CommandAct::MOVE_FILE;
		wss >> arg1 >> arg2;
		comData.Parameter = arg1 + L" " + arg2;
	}
	else if (cmd == L"of" || cmd == L"openfile") {
		comData.Act = CommandAct::OPEN_FILE;
		wss >> arg1;
		comData.Parameter = arg1;
	}
	else if (cmd == L"mv" || cmd == L"move") {
		comData.Act = CommandAct::MOVE;
		std::wstring x, y, z;
		std::getline(wss >> std::ws, x, L',');
		std::getline(wss >> std::ws, y, L',');
		std::getline(wss >> std::ws, z);
		comData.Parameter = x + L" " + y + L" " + z;
	}
	else if (cmd == L"fscale" || cmd == L"scl" || cmd == L"scale") {
		comData.Act = CommandAct::SCALE;
		std::wstring x, y, z;
		std::getline(wss >> std::ws, x, L',');
		std::getline(wss >> std::ws, y, L',');
		std::getline(wss >> std::ws, z);
		comData.Parameter = x + L" " + y + L" " + z;
	}
	else if (cmd == L"frotate" || cmd == L"rte" || cmd == L"rotate") {
		comData.Act = CommandAct::ROTATE;
		std::wstring angle, x, y, z;
		wss >> angle;
		std::getline(wss >> std::ws, x, L',');
		std::getline(wss >> std::ws, y, L',');
		std::getline(wss >> std::ws, z);
		comData.Parameter = angle + L" " + x + L" " + y + L" " + z;
	}
	else if (cmd == L"mvt" || cmd == L"moveto") {
		comData.Act = CommandAct::MOVETO;
		std::wstring x, y, z;
		wss >> x >> y >> z;
		comData.Parameter = x + L" " + y + L" " + z;
	}
	else if (cmd == L"sclto" || cmd == L"scaleto") {
		comData.Act = CommandAct::SCALETO;
		std::wstring x, y, z;
		wss >> x >> y >> z;
		comData.Parameter = x + L" " + y + L" " + z;
	}
	else if (cmd == L"rteto" || cmd == L"rotationto") {
		comData.Act = CommandAct::ROTATIONTO;
		std::wstring angle, x, y, z;
		wss >> angle >> x >> y >> z;
		comData.Parameter = angle + L" " + x + L" " + y + L" " + z;
	}
	else if (cmd == L"lsftp") {
		comData.Act = CommandAct::LIST_SUPPORTED_FILE_TYPES;
	}
	else if (cmd == L"out")
	{
		comData.Act = CommandAct::OUTCONTROL;
		std::wstring arg3;
		wss >> arg1 >> arg2>>arg3;
		comData.Parameter = arg1 + L' ' + arg2 + L' ' + arg3;
	}
	else if (cmd == L"update")
	{
		comData.Act = CommandAct::UPDATE;
		std::wstring arg3;
		wss >> arg1 >> arg2 >> arg3;
		comData.Parameter = arg1 + L' ' + arg2 + L' ' + arg3;
	}
	else if (cmd == L"sleep")
	{
		comData.Act = CommandAct::SLEEP;
		wss >> arg1;
		comData.Parameter = arg1;
	}
	else {
		comData.Act = CommandAct::UNKNOWN;
	}

	return comData;
}