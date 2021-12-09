#include "stdafx.h"
#include <Windows.h>
#include "MainGame.h"

// 메모리 누수 찾기
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

// 콘솔 창 띄우기
#ifdef UNICODE
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#else
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif


// 전역 변수
//POINT g_ptMouse;
POINT g_ptClickedMouse;

HINSTANCE g_hInstance;
HWND g_hWnd;
LPCWSTR g_lpszClass = (LPCWSTR)TEXT("Dungreed");
MainGame	g_mainGame;


void SetWindowSize(int startX, int startY, int sizeX, int sizeY);

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage,
	WPARAM wParam, LPARAM lParam);

int APIENTRY wWinMain(
	_In_ HINSTANCE _hInstance, 
	_In_opt_ HINSTANCE _hPrevInstance,
	_In_ LPWSTR _lpszCmdParam, _In_ int nCmdShow)
{
	// 메모리 누수 찾기
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//_CrtSetBreakAlloc(457);
	//_CrtSetBreakAlloc(999);
#endif


	// 윈도우를 생성하기 위한 기본 셋팅
	g_hInstance = _hInstance;
	WNDCLASS wndClass;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hInstance = _hInstance;
	wndClass.lpfnWndProc = WndProc;
	wndClass.lpszClassName = g_lpszClass;
	wndClass.lpszMenuName = g_lpszClass;
	wndClass.style = CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS;

	// 윈도우 클래스 등록
	RegisterClass(&wndClass);

	// 윈도우 생성
	g_hWnd = CreateWindow(g_lpszClass, g_lpszClass, WS_OVERLAPPEDWINDOW,
		WIN_START_POS_X, WIN_START_POS_Y, WIN_SIZE_X, WIN_SIZE_Y,
		NULL, NULL, _hInstance, NULL);

	// 윈도우 창 셋
	SetWindowSize(WIN_START_POS_X, WIN_START_POS_Y,
		WIN_SIZE_X, WIN_SIZE_Y);

	// 메인게임 초기화
	g_mainGame.Init();

	// 윈도우 출력
	ShowWindow(g_hWnd, nCmdShow);

	// 메세지 큐에 있는 메시지 처리
	MSG message;
	while(GetMessage(&message, 0, 0, 0))
	{
		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	// 메인게임 해제
	g_mainGame.Release();

	return message.wParam;
}

void SetWindowSize(int startX, int startY, int sizeX, int sizeY)
{
	// 원하는 윈도우 작업영역 설정
	RECT rc;
	rc.left = 0; rc.top = 0;
	rc.right = sizeX;	rc.bottom = sizeY;

	// 스타일이 포함된 실제 윈도우 크기 계산
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, false);

	// 계산된 값으로 윈도우를 이동시키면서 크기도 함께 변환
	MoveWindow(g_hWnd, startX, startY,
		rc.right - rc.left, rc.bottom - rc.top, true);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;

	static bool isUpdate = true;

	switch (iMessage)
	{
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_RETURN:
			isUpdate = !isUpdate;
			break;
		}
		break;

	case WM_TIMER:
		if (isUpdate)
		{
			g_mainGame.Update();
		}

		break;

	case WM_PAINT:		// 윈도우 화면이 다시 그려지는 경우 발생하는 메시지
		hdc = BeginPaint(g_hWnd, &ps);

		// 창 크기 조절해도 그에따라 이미지 그려줌
		RECT rect;
		SetMapMode(hdc, MM_ANISOTROPIC);
		SetWindowExtEx(hdc, WIN_SIZE_X, WIN_SIZE_Y, NULL);
		GetClientRect(g_hWnd, &rect);
		SetViewportExtEx(hdc, rect.right, rect.bottom, NULL);

		
		g_mainGame.Render(hdc);

		EndPaint(g_hWnd, &ps);
		break;

	case WM_DESTROY:	// 닫기 버튼 메시지처리 (프로그램 종료)
		PostQuitMessage(0);
		break;
	}

	return g_mainGame.MainProc(hWnd, iMessage, wParam, lParam);
}