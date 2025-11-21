#include<Windows.h>
#include"resource.h"

CONST CHAR LOGIN[] = "Введите имя пользователя";

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	/*MessageBox
	(
		NULL,
		"Hello WinAPI!", 
		"MessageBox",
		MB_YESNOCANCEL | MB_ICONERROR |MB_HELP|MB_DEFBUTTON3|MB_SYSTEMMODAL|MB_RIGHT
	);*/
	//MB_MessageBox
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG_MAIN), NULL, (DLGPROC)DlgProc, 0);

	return 0;
}

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static BOOL isEmptyLoginField = TRUE;
	switch (uMsg)
	{
	case WM_INITDIALOG://Выполняется один раз при запуске окна
		
	{ 
		HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));
		SendMessage(hwnd, WM_SETICON, 0, (LPARAM)hIcon);

		HWND hEditLogin = GetDlgItem(hwnd, IDC_EDIT_LOGIN);
		SendMessage(hEditLogin, WM_SETTEXT, 0, (LPARAM)LOGIN);

		//SetFocus(GetDlgItem(hwnd, IDC_EDIT_LOGIN));
		//SendMessage(GetDlgItem(hwnd, IDC_EDIT_LOGIN), WN_SETFOCUS, GetDlgItem(hwnd, IDOK), NULL);
	}
		break;
	case WM_COMMAND: //обрабатывает комманды с клавиатуры и мыши
		switch (LOWORD(wParam))
		{
			
		case IDC_BUTTON_COPY:
		{
			CONST INT SIZE = 256;
			CHAR sz_buffer[SIZE] = {};
			HWND hEditLogin = GetDlgItem(hwnd, IDC_EDIT_LOGIN);
			HWND hEditPassword = GetDlgItem(hwnd,IDC_EDIT_PASSWORD);
			SendMessage(hEditLogin, WM_GETTEXT, SIZE, (LPARAM)sz_buffer);
			SendMessage(hEditPassword, WM_SETTEXT, 0, (LPARAM)sz_buffer);

		}
			
		break;
		case IDC_EDIT_LOGIN:
		{
			CONST INT SIZE = 256;
			CHAR textBuffer[SIZE] = {};
			HWND hEditLogin = GetDlgItem(hwnd, IDC_EDIT_LOGIN);

			if (HIWORD(wParam) == EN_SETFOCUS)
			{
				SendMessage(hEditLogin, WM_GETTEXT, SIZE, (LPARAM)textBuffer);
				if (!strcmp(textBuffer, LOGIN))
				{
					SendMessage(hEditLogin, WM_SETTEXT, 0, (LPARAM)"");
					isEmptyLoginField = FALSE;
				}
				
			}
			else if (HIWORD(wParam) == EN_KILLFOCUS)
			{
				SendMessage(hEditLogin, WM_GETTEXT, SIZE, (LPARAM)textBuffer);
				if (*textBuffer == '\0')
				{
					SendMessage(hEditLogin, WM_SETTEXT, 0, (LPARAM)LOGIN);
					isEmptyLoginField = TRUE;
				}
				
			}
		}
		break;
		case IDOK:
			MessageBox(NULL, "Была нажата кнопка 'ОК' ", "Info", MB_OK | MB_ICONINFORMATION);
			break;
		case IDCANCEL:
			EndDialog(hwnd, 0);
			break;
		}
		               
		break;
	case WM_CLOSE:			//выполняется при нажатии кнопки 'X'
		EndDialog(hwnd, 0);
	}
	return FALSE;
}
