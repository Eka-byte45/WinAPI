#include<Windows.h>
#include "resource.h"

CONST CHAR* g_sz_VALUES[] = { "This","is","my","first","List","Box" };

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);   //прототип процедуры окна
BOOL CALLBACK DlgProcAdd(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
//BOOL CALLBACK DlgProcChange(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

HINSTANCE hInstanceG;

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	hInstanceG = hInstance;
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL,(DLGPROC)DlgProc, 0);
	
	return 0;
}

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		HWND hList = GetDlgItem(hwnd, IDC_LIST1);

		for (int i = 0; i < sizeof(g_sz_VALUES) / sizeof(g_sz_VALUES[0]); i++)
		{
			SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)g_sz_VALUES[i]);
		}
	}
	break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{

		case IDC_BUTTON2:
		{
			DialogBoxParam(hInstanceG, MAKEINTRESOURCE(IDD_DIALOG2), hwnd, (DLGPROC)DlgProcAdd, 0);
		}

		break;
		case IDC_BUTTON3:
			HWND hList = GetDlgItem(hwnd, IDC_LIST1);
			int index = SendMessage(hList, LB_GETCURSEL, 0, 0);
			if (index != LB_ERR)
			{
				SendMessage(hList, LB_DELETESTRING, index, 0);
			}
			break;
		}
		break;

	case WM_CLOSE:
		EndDialog(hwnd, 0);	
	}
	return FALSE;
}

BOOL CALLBACK DlgProcAdd(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
		SetFocus(GetDlgItem(hwnd, IDC_EDIT1));
		break;
	
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
		{
			CONST INT SIZE = 256;
			CHAR sz_buffer[SIZE]{};
			SendMessage(GetDlgItem(hwnd, IDC_EDIT1), WM_GETTEXT, SIZE, (LPARAM)sz_buffer);
			HWND hParent = GetParent(hwnd);
			HWND hListBox = GetDlgItem(hParent, IDC_LIST1);
			if (SendMessage(hListBox, LB_FINDSTRING, -1, (LPARAM)sz_buffer) == LB_ERR)
			{
				SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)sz_buffer);
				EndDialog(hwnd, 0);
			}
			else
			{
				MessageBox(hwnd, "Такое значение уже существует в списке", "Info", MB_OK | MB_ICONINFORMATION);
			}
		}
		break;
		case IDCANCEL:
			EndDialog(hwnd, 0);
			break;
		}
		break;
	case WM_CLOSE:
		EndDialog(hwnd, 0);	
	}
	return FALSE;
}

