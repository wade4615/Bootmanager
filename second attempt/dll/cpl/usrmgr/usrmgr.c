/*
 * COPYRIGHT:       See COPYING in the top level directory
 * PROJECT:         ReactOS User Manager Control Panel
 * FILE:            dll/cpl/usrmgr/usrmgr.c
 * PURPOSE:         Main functions
 *
 * PROGRAMMERS:     Eric Kohl
 */

#include "usrmgr.h"

#define NUM_APPLETS 1

static LONG APIENTRY UsrmgrApplet(HWND hwnd, UINT uMsg, LPARAM wParam, LPARAM lParam);

HINSTANCE hApplet = 0;

/* Applets */
APPLET Applets[NUM_APPLETS] =
{
    {
        IDI_USRMGR_ICON,
        IDS_CPLNAME,
        IDS_CPLDESCRIPTION,
        UsrmgrApplet
    }
};


static VOID
InitPropSheetPage(PROPSHEETPAGE *psp, WORD idDlg, DLGPROC DlgProc)
{
    ZeroMemory(psp, sizeof(PROPSHEETPAGE));
    psp->dwSize = sizeof(PROPSHEETPAGE);
    psp->dwFlags = PSP_DEFAULT;
    psp->hInstance = hApplet;
    psp->pszTemplate = MAKEINTRESOURCE(idDlg);
    psp->pfnDlgProc = DlgProc;
}

static int CALLBACK
PropSheetProc(HWND hwndDlg, UINT uMsg, LPARAM lParam)
{
    // NOTE: This callback is needed to set large icon correctly.
    HICON hIcon;
    switch (uMsg)
    {
        case PSCB_INITIALIZED:
        {
            hIcon = LoadIconW(hApplet, MAKEINTRESOURCEW(IDI_USRMGR_ICON));
            SendMessageW(hwndDlg, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
            break;
        }
    }
    return 0;
}

/* Display Applet */
static LONG APIENTRY
UsrmgrApplet(HWND hwnd, UINT uMsg, LPARAM wParam, LPARAM lParam)
{
    PROPSHEETPAGE psp[2];
    PROPSHEETHEADER psh;
    TCHAR Caption[1024];

    UNREFERENCED_PARAMETER(lParam);
    UNREFERENCED_PARAMETER(wParam);
    UNREFERENCED_PARAMETER(uMsg);

    LoadString(hApplet, IDS_CPLNAME, Caption, sizeof(Caption) / sizeof(TCHAR));

    ZeroMemory(&psh, sizeof(PROPSHEETHEADER));
    psh.dwSize = sizeof(PROPSHEETHEADER);
    psh.dwFlags =  PSH_PROPSHEETPAGE | PSH_USEICONID | PSH_USECALLBACK;
    psh.hwndParent = hwnd;
    psh.hInstance = hApplet;
    psh.pszIcon = MAKEINTRESOURCEW(IDI_USRMGR_ICON);
    psh.pszCaption = Caption;
    psh.nPages = sizeof(psp) / sizeof(PROPSHEETPAGE);
    psh.nStartPage = 0;
    psh.ppsp = psp;
    psh.pfnCallback = PropSheetProc;

    InitPropSheetPage(&psp[0], IDD_USERS, UsersPageProc);
    InitPropSheetPage(&psp[1], IDD_GROUPS, GroupsPageProc);
    /* InitPropSheetPage(&psp[2], IDD_EXTRA, ExtraPageProc); */

    return (LONG)(PropertySheet(&psh) != -1);
}


/* Control Panel Callback */
LONG CALLBACK
CPlApplet(HWND hwndCPl, UINT uMsg, LPARAM lParam1, LPARAM lParam2)
{
    int i = (int)lParam1;

    switch (uMsg)
    {
        case CPL_INIT:
            return TRUE;

        case CPL_GETCOUNT:
            return NUM_APPLETS;

        case CPL_INQUIRE:
            {
                CPLINFO *CPlInfo = (CPLINFO*)lParam2;
                CPlInfo->lData = 0;
                CPlInfo->idIcon = Applets[i].idIcon;
                CPlInfo->idName = Applets[i].idName;
                CPlInfo->idInfo = Applets[i].idDescription;
            }
            break;

        case CPL_DBLCLK:
            Applets[i].AppletProc(hwndCPl, uMsg, lParam1, lParam2);
            break;
    }

    return FALSE;
}


BOOL WINAPI
DllMain(HINSTANCE hinstDLL, DWORD dwReason, LPVOID lpvReserved)
{
    UNREFERENCED_PARAMETER(lpvReserved);

    switch (dwReason)
    {
        case DLL_PROCESS_ATTACH:
            hApplet = hinstDLL;
            break;
    }

    return TRUE;
}
