#include "stdafx.h"
#include <Windows.h>
#include "MainGame.h"
#include "CommonFunction.h"

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
HINSTANCE g_hInstance;
HWND g_hWnd;
LPCWSTR g_lpszClass = (LPCWSTR)TEXT("Dungreed");
MainGame	g_mainGame;


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

	//_CrtSetBreakAlloc(2);
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
	SetWindowSize(WIN_START_POS_X, WIN_START_POS_Y, WIN_SIZE_X, WIN_SIZE_Y);

	// 메인게임 초기화
	g_mainGame.Init();

	// 윈도우 출력
	ShowWindow(g_hWnd, nCmdShow);

	// 윈도우 화면에 보일 HDC랑 비트맵 설정
	HDC _hDC = GetDC(g_hWnd);
	HBITMAP _Bitmap = CreateCompatibleBitmap(_hDC, WIN_SIZE_X, WIN_SIZE_Y);
	SelectObject(_hDC, _Bitmap);

	// 메세지 큐에 있는 메시지 처리
	MSG msg;

	while (TRUE)
	{
		if (PeekMessage(&msg, nullptr, NULL, NULL, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			g_mainGame.Update();
			g_mainGame.Render(_hDC);
		}
	}

	// 메인게임 해제
	g_mainGame.Release();

	return static_cast<INT32>(msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, iMessage, wParam, lParam);
}

// 창 크기 조절해도 그에따라 이미지 그려줌
//RECT rect;
//SetMapMode(hdc, MM_ANISOTROPIC);
//SetWindowExtEx(hdc, WIN_SIZE_X, WIN_SIZE_Y, NULL);
//GetClientRect(g_hWnd, &rect);
//SetViewportExtEx(hdc, rect.right, rect.bottom, NULL);