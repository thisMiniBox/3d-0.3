 #include"消息循环声明.h"
LRESULT CALLBACK cFileWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, Controller* Central_control)
{
    switch (message)
    {
    case WM_NOTIFY:
    {
        LPNMHDR lphr = (LPNMHDR)lParam;
        if (lphr->hwndFrom == Central_control->m_FileWind->GetTree())//判断是否是树形控件发来的消息
        {
            switch (lphr->code)
            {
            case NM_CLICK://鼠标单击
            {
                Object* o = Central_control->m_FileWind->GetMousePositionItemData();
                if (!o)break;
                Central_control->SetFoucusObjcet(o);
                Central_control->m_EditWind->SetTree(Central_control->m_FileWind->GetMousePositionItem());
            }
            break;
            }
            break;
        }
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
            if (Central_control->m_FileLoad)
            {
                Central_control->OutMessage("正在加载文件，无法进行删除操作", _Warning);
                break;
            }
            Object* ta = Central_control->m_FileWind->GetSelectedItemData();
            switch (ta->GetType())
            {
            case OT_CAMERA:
            {
                Camera* v = dynamic_cast<Camera*>(ta);
                if (v == Central_control->view)
                {
                    Central_control->OutMessage("不能删除当前视角摄像机", _Error);
                    break;
                }
            }
            default:
            {
                Central_control->DeleteObject(ta, Central_control->m_FileWind->GetSelectedItem());
            }
            break;
            }
            break;
        }
        case ID_FM_CREATECAMERA:
        {
            Object* par = Central_control->GetFocusObject();
            if (par->GetType() == OT_FOLDER)
                Central_control->CreateObject(dynamic_cast<Folder*>(par), "新建摄像机", OT_CAMERA);
            else
                Central_control->CreateObject(par->GetParent(), "新建摄像机", OT_CAMERA);
            break;
        }
        case ID_FM_CREATEMODEL:
        {
            Object* par = Central_control->GetFocusObject();
            if (par->GetType() == OT_FOLDER)
                Central_control->CreateObject(dynamic_cast<Folder*>(par), "新建模型", OT_MODEL);
            else
                Central_control->CreateObject(par->GetParent(), "新建模型", OT_MODEL);
            break;
        }
        case ID_FM_CREATEFOLDER:
        {
            Object* par = Central_control->m_EditWind->GetTarget();
            if (par && par->GetType() == OT_FOLDER)
                Central_control->CreateObject(dynamic_cast<Folder*>(par));
            else
                Central_control->CreateObject();
            break;
        }
        case ID_FM_UPDATE:
        {
            Central_control->UpdateFileView();
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
        Central_control->m_FileWind->MoveTree(0, 0, cxClient, cyClient);
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