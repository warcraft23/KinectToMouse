#include <windows.h>
#include <stdio.h>
#include <tchar.h>

int CDECL MessageBoxPrintf(TCHAR * szCaption,TCHAR  * szFormat,...){
	TCHAR szBuffer[1028];
	va_list pArgList;
	va_start(pArgList,szFormat);
	_vsntprintf(szBuffer,sizeof(szBuffer)/sizeof(TCHAR),szFormat,pArgList);
	va_end(pArgList);
	return MessageBox(NULL,szBuffer,szCaption,0);
}


int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR szCmdLine, int iCmdShow)
{
	int fMouse;
	int cButtons;
	double cX,cY;
	
	static TCHAR appName[]=TEXT("Get&Set Cursor Position");
	HWND hwnd;
	POINT cursorPoint;

	hwnd=CreateWindow(appName,TEXT("Get&Set Cursor Position"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,
		NULL,NULL,hInstance,NULL);

	GetCursorPos(&cursorPoint);
	
	MessageBoxPrintf(TEXT("BufferNum"),TEXT("Mouse Position is (%.2f,%.2f)"),cursorPoint.x,cursorPoint.y);
	
	ScreenToClient(hwnd,&cursorPoint);

	ClientToScreen(hwnd,&cursorPoint);
	cursorPoint.x=0;
	cursorPoint.y=0;
	SetCursorPos(cursorPoint.x,cursorPoint.y);
	
	
	return 0 ;
}
