#include"消息循环声明.h"

LRESULT CALLBACK  cDetaileWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, Controller* current_project)
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