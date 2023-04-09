 #include"消息循环声明.h"
LRESULT CALLBACK cFileWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, project* current_project)
{
    switch (message)
    {
    case WM_NOTIFY:
    {
        LPNMHDR lphr = (LPNMHDR)lParam;
        if (lphr->hwndFrom == current_project->FILEWND->GetTree())//判断是否是树形控件发来的消息
        {
            switch (lphr->code)
            {
            case NM_CLICK://鼠标单击
            {
                Object* o = current_project->FILEWND->GetMousePositionItemData();
                if (!o)break;
                current_project->DETAWND->SetView(o);
                current_project->DETAWND->SetTree(current_project->FILEWND->GetMousePositionItem());
            }
            break;
            //case NM_DBLCLK:
            //{
            //    Object* o = current_project->FileTree_文件树.GetOption_获取被选中节点();
            //    if (!o)break;
            //    
            //    break;
            //}
            }
        }
        break;
    }
    case WM_CONTEXTMENU: 
    {
        // 获取鼠标当前位置
        POINT pt;
        GetCursorPos(&pt);

        // 加载菜单资源
        HMENU hMenu = LoadMenu(GetModuleHandle(NULL), MAKEINTRESOURCE(IDR_FILERIGHT));
        HMENU hPopup = GetSubMenu(hMenu, 0);

        // 显示菜单并等待用户选择
        int r = TrackPopupMenuEx(hPopup, TPM_LEFTALIGN | TPM_TOPALIGN | TPM_RETURNCMD, pt.x, pt.y, hWnd, NULL);
        switch (r)
        {
        case ID_FM_DELETE:
        {
            Object* ta = current_project->FILEWND->GetSelectedItemData();
            switch (ta->GetType())
            {
            case OT_CAMERA:
            {
                Camera* v = dynamic_cast<Camera*>(ta);
                if (v == current_project->view)
                {
                    current_project->upMsg("不能删除当前视角摄像机",_Error);
                    break;
                }    
            }
            default:
            {
                current_project->DeleteObject(ta, current_project->FILEWND->GetSelectedItem());
            }
                break;
            }
            break;
        }
        default:
            break;
        }
        // 销毁菜单
        DestroyMenu(hMenu);
        break;
    }
    case WM_SIZE:
    {
        int cxClient = LOWORD(lParam);  // 获得客户区宽度
        int cyClient = HIWORD(lParam);
        current_project->FILEWND->MoveTree(0, 0, cxClient, cyClient);
        break;
    }
    case WM_CREATE:
    {
        break;
    }
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}