#include "stdafx.h"
#include <Windows.h>
#include "MainGame.h"
#include "CommonFunction.h"

// �޸� ���� ã��
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

// �ܼ� â ����
#ifdef UNICODE
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#else
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif


// ���� ����
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
	// �޸� ���� ã��
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//_CrtSetBreakAlloc(2);
#endif

	// �����츦 �����ϱ� ���� �⺻ ����
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

	// ������ Ŭ���� ���
	RegisterClass(&wndClass);

	// ������ ����
	g_hWnd = CreateWindow(g_lpszClass, g_lpszClass, WS_OVERLAPPEDWINDOW,
		WIN_START_POS_X, WIN_START_POS_Y, WIN_SIZE_X, WIN_SIZE_Y,
		NULL, NULL, _hInstance, NULL);

	// ������ â ��
	SetWindowSize(WIN_START_POS_X, WIN_START_POS_Y, WIN_SIZE_X, WIN_SIZE_Y);

	// ���ΰ��� �ʱ�ȭ
	g_mainGame.Init();

	// ������ ���
	ShowWindow(g_hWnd, nCmdShow);

	// ������ ȭ�鿡 ���� HDC�� ��Ʈ�� ����
	HDC _hDC = GetDC(g_hWnd);
	HBITMAP _Bitmap = CreateCompatibleBitmap(_hDC, WIN_SIZE_X, WIN_SIZE_Y);
	SelectObject(_hDC, _Bitmap);

	// �޼��� ť�� �ִ� �޽��� ó��
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

	// ���ΰ��� ����
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

// â ũ�� �����ص� �׿����� �̹��� �׷���
//RECT rect;
//SetMapMode(hdc, MM_ANISOTROPIC);
//SetWindowExtEx(hdc, WIN_SIZE_X, WIN_SIZE_Y, NULL);
//GetClientRect(g_hWnd, &rect);
//SetViewportExtEx(hdc, rect.right, rect.bottom, NULL);