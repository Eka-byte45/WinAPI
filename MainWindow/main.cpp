#define _CRT_SECURE_NO_WARNINGS
#include<Windows.h>
#include"resource1.h"
#include<stdio.h>

CONST CHAR g_sz_WINDOW_CLASS[] = "My first window";

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	//1)Регистарция класса окна. На этом этапе класс нашего окна регистрируется в ОС.
	//Класс окна это не тип данных, как в ООП, класс любого окна представляет собой
	//самую обычную строку с именем класса.
	WNDCLASSEX wClass;
	ZeroMemory(&wClass, sizeof(wClass));

	//Инициализируем размеры и стиль:
	wClass.style = NULL;
	wClass.cbSize = sizeof(wClass); //cb_ - Count Bytes
	wClass.cbClsExtra = 0;
	wClass.cbWndExtra = 0;

	//Инициализируем внешний вид окон:
	//wClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON_BITCOIN));
	//wClass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON_PALM));
	
	wClass.hIcon =(HICON) LoadImage(NULL,"bitcoin.ico",IMAGE_ICON,LR_DEFAULTSIZE,LR_DEFAULTSIZE,LR_LOADFROMFILE);
	wClass.hIconSm = (HICON)LoadImage(NULL, "palm.ico",IMAGE_ICON,LR_DEFAULTSIZE,LR_DEFAULTSIZE,LR_LOADFROMFILE);

	//wClass.hCursor = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR1));
	wClass.hCursor = (HCURSOR)LoadImage
	(   hInstance,
		"starcraft-original\\Working In Background.ani",
		IMAGE_CURSOR,
		LR_DEFAULTSIZE,
		LR_DEFAULTSIZE,
		LR_LOADFROMFILE
	);
	wClass.hbrBackground =(HBRUSH)(COLOR_WINDOW + 1);

	//Инициализация системных переменных:
	wClass.hInstance = hInstance;
	wClass.lpszClassName = g_sz_WINDOW_CLASS;
	wClass.lpszMenuName = NULL;
	wClass.lpfnWndProc = WndProc;

	if (RegisterClassEx(&wClass) == NULL)
	{
		MessageBox(NULL, "Class registration failed",NULL, MB_OK | MB_ICONERROR);
		return 0;
	}

	//2)Создание окна:
	int width = GetSystemMetrics(SM_CXSCREEN);
	int heigth = GetSystemMetrics(SM_CYSCREEN);
	int windowWidth = 0.75f * width;
	int windowHeigth = 0.75f * heigth;

	int centerX = (width - windowWidth) / 2;
	int centerY = (heigth - windowHeigth) / 2;

	HWND hwnd = CreateWindowEx
	(
		NULL,//exStyle
		g_sz_WINDOW_CLASS, //имя класса окна
		g_sz_WINDOW_CLASS,//Заголовок окна
		WS_OVERLAPPEDWINDOW,//Стиль окна. Стили всегда зависят от класса окна.WS_OVERLAPPEDWINDOW - Означает главное окно
		//CW_USEDEFAULT, CW_USEDEFAULT,//Position
		centerX, centerY,
		//CW_USEDEFAULT, CW_USEDEFAULT,//Размер окна
		//640,480,
		windowWidth,windowHeigth,
		NULL,
		NULL,//Для главного окна это ResourceID главного меню, для дочернего окна (Control) - ResourceID дочернего окна(IDC_BUTTON_COPY)
		hInstance,
		NULL

	);
	if (hwnd == NULL)
	{
		MessageBox(NULL, "Windows creation failed", NULL, MB_OK | MB_ICONERROR);
		return 0;
	}ShowWindow(hwnd, nCmdShow);//Задает режим отображения окна - Развернуто на весь экран, Свернуто в окно,Свернуто на панель задач
	UpdateWindow(hwnd);//Обновляет клиентскую область указанного окна, отправляя сообщение WM_PAINT,если клиентская область окна не пустая

	//3)Запуск цикла сообщений:
	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);//Преобразует сообщения виртуальных клавиш в символьные сообщения
		DispatchMessage(&msg);//Отправляет сообщение в процедуру окна
	}
	
	return msg.wParam;
}
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static int prevWidth = -1;
	static int prevHeigth = -1;
	switch (uMsg)
	{
	case WM_CREATE:
		//MessageBox(hwnd, "Cursor check", "Info", MB_OK | MB_ICONINFORMATION);
		break;
	case WM_COMMAND:
		break;

	case WM_MOVE:
	{
		RECT rc;
		GetWindowRect(hwnd,&rc);
		CONST INT SIZE = 256;
		char titleBuff[SIZE] = "";
		sprintf(titleBuff, "%s: позиция %dx%d", g_sz_WINDOW_CLASS,rc.left,rc.top);
		SetWindowText(hwnd, titleBuff);
	}
		break;
	case WM_SIZE:
	{
		int currentWidth = LOWORD(lParam);
		int currentHeigth = HIWORD(lParam);
		if (currentHeigth != prevHeigth || currentWidth != prevWidth)
		{
			RECT rc;
			GetWindowRect(hwnd, &rc);
			CONST INT SIZE = 256;
			char titleBuff[SIZE] = "";
			sprintf(titleBuff, "%s: размер %dx%d", g_sz_WINDOW_CLASS, currentWidth,currentHeigth);
			SetWindowText(hwnd, titleBuff);
			prevHeigth = currentHeigth;
			prevWidth = currentWidth;
		}
		
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return FALSE;
}

//https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getsystemmetrics
//https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getwindowrect