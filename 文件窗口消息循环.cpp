 #include"��Ϣѭ������.h"
LRESULT CALLBACK cFileWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, Controller* Central_control)
{
    static bool gDragging = false;
    static HTREEITEM gStartItem = nullptr;
    switch (message)
    {
    case WM_NOTIFY:
    {
        LPNMHDR lphr = (LPNMHDR)lParam;
        switch (lphr->code)
        {
        case NM_CLICK://��굥��
        {
            Object* o = Central_control->m_FileWind->GetMousePositionItemData();
            if (!o)break;
            Central_control->SetFoucusObjcet(o);
            break;
        }
        case TVN_BEGINDRAG:
        {
            gStartItem = Central_control->m_FileWind->GetMousePositionItem();
            SetCapture(hWnd);
            gDragging = true;
            break;
        }
        break;
        }
        break;
    }

    case WM_CONTEXTMENU:
    {
        // ��ȡ��굱ǰλ��
        POINT pt;
        GetCursorPos(&pt);

        // ���ز˵���Դ
        HMENU hMenu = LoadMenu(GetModuleHandle(NULL), MAKEINTRESOURCE(IDR_FILERIGHT));
        HMENU hPopup = GetSubMenu(hMenu, 0);

        // ��ʾ�˵����ȴ��û�ѡ��
        int r = TrackPopupMenuEx(hPopup, TPM_LEFTALIGN | TPM_TOPALIGN | TPM_RETURNCMD, pt.x, pt.y, hWnd, NULL);
        switch (r)
        {
        case ID_FM_DELETE:
        {
            if (Central_control->m_FileLoad)
            {
                Central_control->OutMessage("���ڼ����ļ����޷�����ɾ������", _Warning);
                break;
            }
            Object* ta = Central_control->m_FileWind->GetSelectedItemData();
            if (!ta)
                break;
            switch (ta->GetType())
            {
            case OT_CAMERA:
            {
                Camera* v = dynamic_cast<Camera*>(ta);
                if (v == Central_control->view)
                {
                    Central_control->OutMessage("����ɾ����ǰ�ӽ������", _Error);
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
                Central_control->CreateObject(dynamic_cast<Folder*>(par), "�½������", OT_CAMERA);
            else
                Central_control->CreateObject(par->GetParent(), "�½������", OT_CAMERA);
            break;
        }
        case ID_FM_CREATEMODEL:
        {
            Object* par = Central_control->GetFocusObject();
            if (par->GetType() == OT_FOLDER)
                Central_control->CreateObject(dynamic_cast<Folder*>(par), "�½�ģ��", OT_MODEL);
            else
                Central_control->CreateObject(par->GetParent(), "�½�ģ��", OT_MODEL);
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
        // ���ٲ˵�
        DestroyMenu(hMenu);
        break;
    }
    case WM_LBUTTONUP:
    {
        if (gDragging)
        {
            FileWind* filewind = Central_control->m_FileWind;
            HTREEITEM gAimItem = Central_control->m_FileWind->GetMousePositionItem();
            if (gAimItem == gStartItem)
                break;
            if (Central_control->MoveFile_(filewind->GetItemObject(gStartItem), filewind->GetItemObject(gAimItem)))
                filewind->MoveItem(gStartItem, gAimItem);
            gDragging = false;
            ReleaseCapture();
        }

        break;
    }
    case WM_SIZE:
    {
        int cxClient = LOWORD(lParam);  // ��ÿͻ������
        int cyClient = HIWORD(lParam);
        if (Central_control->m_FileWind)
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