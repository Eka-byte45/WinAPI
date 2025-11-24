#include<Windows.h>
#include"resource.h"

CONST CHAR* g_sz_VALUES[] = { "This","is","my","first","List","Box" };
BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgProcAdd(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgProcChange(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG_MAIN), NULL, (DLGPROC)DlgProc, 0);

	return 0;
}

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HWND hListBox; 

	switch (uMsg)
	{
	case WM_INITDIALOG:
		hListBox = GetDlgItem(hwnd, IDC_LIST1); 
		SetFocus(hListBox);
		for (int i = 0; i < sizeof(g_sz_VALUES) / sizeof(g_sz_VALUES[0]); ++i)
			SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)g_sz_VALUES[i]);
		break;

	case WM_KEYDOWN:
		if (wParam == VK_RETURN)
		{
			MessageBox(hwnd, "Вы нажали Enter", "Сообщение", MB_OK);
			hListBox = GetDlgItem(hwnd, IDC_LIST1); 
			int selIndex = SendMessage(hListBox, LB_GETCURSEL, 0, 0);
			if (selIndex >= 0)
			DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG_ADD), hwnd, (DLGPROC)DlgProcChange, 0);
		}
		break;
	
	case WM_COMMAND:

		switch (LOWORD(wParam))
		{
		case IDC_LIST1:
		{
			if (HIWORD(wParam) == LBN_DBLCLK)
			{
				CONST INT SIZE = 256;
				CHAR sz_buffer[SIZE];

				HWND hList = (HWND)lParam;
				INT i = SendMessage(hList, LB_GETCURSEL, 0, 0);
				SendMessage(hList, LB_GETTEXT, i, (LPARAM)sz_buffer);

				DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG_ADD), hwnd, (DLGPROC)DlgProcChange, 0);
			}

		}
		break;
		
		case IDC_BUTTON_DELETE:
		{
			HWND hListBox = GetDlgItem(hwnd, IDC_LIST1);
			INT i = SendMessage(hListBox, LB_GETCURSEL, 0, 0);
			SendMessage(hListBox, LB_DELETESTRING, i, 0);
		}
		break;
		case IDC_BUTTON_ADD:
		{
			DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG_ADD), hwnd, (DLGPROC)DlgProcAdd, 0);
		}
		break;
		case IDOK:
			break;
		case IDCANCEL:
			EndDialog(hwnd, 0);
			break;
		case WM_CLOSE:
			EndDialog(hwnd, 0);
			break;
		}
		
	}
	return FALSE;
}

BOOL CALLBACK DlgProcAdd(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		SetFocus(GetDlgItem(hwnd, IDC_EDIT));
	}
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
		{
			CONST INT SIZE = 256;
			CHAR sz_buffer[SIZE] = {};
			HWND hEdit = GetDlgItem(hwnd, IDC_EDIT);
			SendMessage(hEdit, WM_GETTEXT, SIZE, (LPARAM)sz_buffer);

			HWND hParent = GetParent(hwnd);
			HWND hList = GetDlgItem(hParent, IDC_LIST1);
			if (SendMessage(hList, LB_FINDSTRINGEXACT, 0, (LPARAM)sz_buffer) == LB_ERR)
				SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)sz_buffer);
			else
			{
				MessageBox(hwnd, "Такой элемент уже существует", "Warning", MB_OK | MB_ICONWARNING);
				break;
			}
				
		}	
		case IDCANCEL:
			EndDialog(hwnd, 0);
		}
		break;
	
	case WM_CLOSE:
		EndDialog(hwnd, 0);
	}
	return FALSE;
}

BOOL CALLBACK DlgProcChange(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		SendMessage(hwnd, WM_SETTEXT, 0, (LPARAM)"Изменить");
		SendMessage(GetDlgItem(hwnd, IDOK), WM_SETTEXT, 0, (LPARAM)"Сохранить");

		HWND hParent = GetParent(hwnd);
		HWND hListBox = GetDlgItem(hParent, IDC_LIST1);
		HWND hEdit = GetDlgItem(hwnd, IDC_EDIT);

		CONST INT SIZE = 256;
		CHAR sz_buffer[SIZE];

		INT i = SendMessage(hListBox, LB_GETCURSEL, 0, 0);
		SendMessage(hListBox, LB_GETTEXT, i, (LPARAM)sz_buffer);
		SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)sz_buffer);

		SetFocus(hEdit);
	}
	break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
		{
			/*HWND hEdit = GetDlgItem(hwnd, IDC_EDIT);
			HWND hParent = GetParent(hwnd);
			HWND hListBox = GetDlgItem(hParent, IDC_LIST1);

			CONST INT SIZE = 256;
			CHAR sz_buffer[SIZE];
			SendMessage(hEdit, WM_GETTEXT, SIZE, (LPARAM)sz_buffer);

			INT i = SendMessage(hListBox, LB_GETCURSEL, 0, 0);

			if (SendMessage(hListBox, LB_FINDSTRING, -1, (LPARAM)sz_buffer) != i)
			{
				SendMessage(hListBox, LB_DELETESTRING, i, 0);
				SendMessage(hListBox, LB_INSERTSTRING, i, (LPARAM)sz_buffer);
				EndDialog(hwnd, 0);
			}
			else
				MessageBox(hwnd, "Значение уже присутствует в списке", "Info", MB_OK | MB_ICONINFORMATION);*/

			HWND hEdit = GetDlgItem(hwnd, IDC_EDIT);
			HWND hParent = GetParent(hwnd);
			HWND hListBox = GetDlgItem(hParent, IDC_LIST1);

			CONST INT SIZE = 256;
			CHAR sz_buffer[SIZE];
			SendMessage(hEdit, WM_GETTEXT, SIZE, (LPARAM)sz_buffer);

			// Сохраняем индекс текущего элемента для последующего обновления
			INT currentIndex = SendMessage(hListBox, LB_GETCURSEL, 0, 0);

			// Ручной перебор всех элементов списка для точного сравнения строк
			for (INT j = 0; j < SendMessage(hListBox, LB_GETCOUNT, 0, 0); j++)
			{
				CHAR tempBuffer[SIZE];
				SendMessage(hListBox, LB_GETTEXT, j, (LPARAM)tempBuffer);

				// Регистронезависимый поиск совпадений
				if (_stricmp(tempBuffer, sz_buffer) == 0)
				{
					// Совпадение найдено
					MessageBox(hwnd, "Значение уже присутствует в списке", "Info", MB_OK | MB_ICONINFORMATION);
					return FALSE; // Выходим из процедуры без дальнейших действий
				}
			}

			// Если цикл завершился без нахождения совпадений, обновляем элемент
			SendMessage(hListBox, LB_DELETESTRING, currentIndex, 0);
			SendMessage(hListBox, LB_INSERTSTRING, currentIndex, (LPARAM)sz_buffer);
			EndDialog(hwnd, 0);
		}
		break;

		case IDCANCEL:
			EndDialog(hwnd, IDCANCEL);
			break;
		}
		break;

	case WM_CLOSE:
		EndDialog(hwnd, IDCANCEL);
		break;
	}
	return FALSE;
}