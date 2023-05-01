 #include"��Ϣѭ������.h"
LRESULT CALLBACK cFileWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, Controller* current_project)
{
    switch (message)
    {
    case WM_NOTIFY:
    {
        LPNMHDR lphr = (LPNMHDR)lParam;
        if (lphr->hwndFrom == current_project->FILEWND->GetTree())//�ж��Ƿ������οؼ���������Ϣ
        {
            switch (lphr->code)
            {
            case NM_CLICK://��굥��
            {
                Object* o = current_project->FILEWND->GetMousePositionItemData();
                if (!o)break;
                current_project->DETAWND->SetView(o);
                current_project->DETAWND->SetTree(current_project->FILEWND->GetMousePositionItem());
            }
            break;
            }
            break;
        }
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
            if (current_project->m_FileLoad)
            {
                current_project->OutMessage("���ڼ����ļ����޷�����ɾ������", _Warning);
                break;
            }
            Object* ta = current_project->FILEWND->GetSelectedItemData();
            switch (ta->GetType())
            {
            case OT_CAMERA:
            {
                Camera* v = dynamic_cast<Camera*>(ta);
                if (v == current_project->view)
                {
                    current_project->OutMessage("����ɾ����ǰ�ӽ������", _Error);
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
        case ID_FM_CREATEFOLDER:
        {
            Object* par = current_project->DETAWND->GetTarget();
            if (par && par->GetType() == OT_FOLDER)
                current_project->CreateObject(dynamic_cast<Folder*>(par));
            else
                current_project->CreateObject();
            break;
        }
        default:
            break;
        }
        // ���ٲ˵�
        DestroyMenu(hMenu);
        break;
    }
    case WM_SIZE:
    {
        int cxClient = LOWORD(lParam);  // ��ÿͻ������
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