#include"消息循环声明.h"
// 对话框过程函数
//LRESULT CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
//{
//	switch (uMsg)
//	{
//	case WM_COMMAND:
//	{
//		// 处理控件消息
//		switch (LOWORD(wParam))
//		{
//		case IDCANCEL:
//			EndDialog(hwndDlg, 0);
//			return TRUE;
//		}
//		break;
//	}
//
//	case WM_DESTROY:
//	{
//		// 释放资源
//		PostQuitMessage(0);
//		return TRUE;
//	}
//	default:
//		return DefWindowProc(hwndDlg, uMsg, wParam, lParam);
//	}
//	return 0;
//}

LRESULT CALLBACK  cDetaileWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, project* current_project)
{
	switch (message)
	{
	case WM_SIZE:
	{
		RECT m_rect;
		GetClientRect(hWnd, &m_rect);
		current_project->DETAWND->SetRect(m_rect);
		break;
	}
	case WM_MOUSEWHEEL:
	{
		int zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
		if (zDelta != 0)
		{
			if (current_project->DETAWND->GetControlPos() < 0 || zDelta < 0)
			current_project->DETAWND->ControlPosMove(zDelta/10);
		}
		break;
	}
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}