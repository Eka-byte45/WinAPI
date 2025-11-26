#define _CRT_SECURE_NO_WARNINGS
#include<Windows.h>
#include"resource.h"

CONST CHAR* g_sz_VALUES[] = { "This","is","my","first","List","Box" };
BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgProcAdd(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgProcChange(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
VOID SaveList(HWND hwnd, CONST CHAR filename[]);


INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	//hInstance - это экземпляр исполняемго файа программы, загруженного в память(.exe)
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
	
	case WM_COMMAND:

		switch (LOWORD(wParam))
		{
		case IDC_LIST1:
		{
			if (HIWORD(wParam) == LBN_DBLCLK)
			{
				CONST INT SIZE = 256;
				CHAR sz_buffer[SIZE];

				HWND hList = (HWND)lParam;//получаем дескриптор через lParam текущего активного списка
				INT i = SendMessage(hList, LB_GETCURSEL, 0, 0);//получаем индекс 
				SendMessage(hList, LB_GETTEXT, i, (LPARAM)sz_buffer);//запрашиваем содержимое выбранного элемента
				//GetModuleHandle(NULL) - возвращает 'hInstance' нашей программы
				DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG_ADD), hwnd, (DLGPROC)DlgProcChange, 0);//открываем диалоговое окно
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
			SaveList(hwnd, "list.txt");
			EndDialog(hwnd, 0);
			//break;
		case WM_KEYUP:
		{
			switch (LOWORD(wParam))
			{
			case VK_SPACE:
			{
				HWND hList = GetDlgItem(hwnd, IDC_LIST1);
				if (GetFocus() == hList)
				{
					DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG_ADD), hwnd, (DLGPROC)DlgProcChange, 0);
				}
			}
			break;
			}
		}
		break;
		case WM_CLOSE:
			SaveList(hwnd, "list.txt");
			EndDialog(hwnd, 0);
			//break;
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

		HWND hParent = GetParent(hwnd);//дескриптор родительского окна
		HWND hListBox = GetDlgItem(hParent, IDC_LIST1);//получ дескриптора упэранта ищем лист бокс
		HWND hEdit = GetDlgItem(hwnd, IDC_EDIT);//получение дескриптора текстового поля

		CONST INT SIZE = 256;
		CHAR sz_buffer[SIZE];

		INT i = SendMessage(hListBox, LB_GETCURSEL, 0, 0);
		SendMessage(hListBox, LB_GETTEXT, i, (LPARAM)sz_buffer);
		SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)sz_buffer);

		SetFocus(hEdit);
		SendMessage(hEdit, EM_SETSEL, SIZE, -1);
	}
	break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
		{
			HWND hEdit = GetDlgItem(hwnd, IDC_EDIT);
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
				MessageBox(hwnd, "Значение уже присутствует в списке", "Info", MB_OK | MB_ICONINFORMATION);

			//HWND hEdit = GetDlgItem(hwnd, IDC_EDIT);
			//HWND hParent = GetParent(hwnd);
			//HWND hListBox = GetDlgItem(hParent, IDC_LIST1);

			//CONST INT SIZE = 256;
			//CHAR sz_buffer[SIZE];
			//SendMessage(hEdit, WM_GETTEXT, SIZE, (LPARAM)sz_buffer);

			//// Сохраняем индекс текущего элемента для последующего обновления
			//INT currentIndex = SendMessage(hListBox, LB_GETCURSEL, 0, 0);

			//// Ручной перебор всех элементов списка для точного сравнения строк
			//for (INT j = 0; j < SendMessage(hListBox, LB_GETCOUNT, 0, 0); j++)
			//{
			//	CHAR tempBuffer[SIZE];
			//	SendMessage(hListBox, LB_GETTEXT, j, (LPARAM)tempBuffer);

			//	// Регистронезависимый поиск совпадений
			//	if (_stricmp(tempBuffer, sz_buffer) == 0)
			//	{
			//		// Совпадение найдено
			//		MessageBox(hwnd, "Значение уже присутствует в списке", "Info", MB_OK | MB_ICONINFORMATION);
			//		return FALSE; // Выходим из процедуры без дальнейших действий
			//	}
			//}

			//// Если цикл завершился без нахождения совпадений, обновляем элемент
			//SendMessage(hListBox, LB_DELETESTRING, currentIndex, 0);
			//SendMessage(hListBox, LB_INSERTSTRING, currentIndex, (LPARAM)sz_buffer);
			EndDialog(hwnd, 0);
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
VOID SaveList(HWND hwnd, CONST CHAR filename[])
{
	CONST INT SIZE = 32768;
	CHAR sz_buffer[SIZE] = {};

	HWND hList = GetDlgItem(hwnd, IDC_LIST1);
	INT n = SendMessage(hList, LB_GETCOUNT, 0, 0);

	for (int i = 0; i < n; ++i)
	{
		CHAR sz_item[256] = {};
		SendMessage(hList, LB_GETTEXT, i, (LPARAM)sz_item);
		strcat(sz_buffer, sz_item);
		strcat(sz_buffer, "\n");
		/*
		lstrcat(sz_buffer, sz_item);
		lstrcat(sz_buffer, "\n");
		*/
		
	}
	HANDLE hFile = CreateFile(filename,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	DWORD dwBytesWriten = 0;
	WriteFile(hFile, sz_buffer, strlen(sz_buffer) + 1, &dwBytesWriten, NULL);
	CloseHandle(hFile);

}
