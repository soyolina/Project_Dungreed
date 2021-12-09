#include "stdafx.h"
#include <Windows.h>
#include "MainGame.h"

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
	// �޸� ���� ã��
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//_CrtSetBreakAlloc(457);
	//_CrtSetBreakAlloc(999);
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
	SetWindowSize(WIN_START_POS_X, WIN_START_POS_Y,
		WIN_SIZE_X, WIN_SIZE_Y);

	// ���ΰ��� �ʱ�ȭ
	g_mainGame.Init();

	// ������ ���
	ShowWindow(g_hWnd, nCmdShow);

	// �޼��� ť�� �ִ� �޽��� ó��
	MSG message;
	while(GetMessage(&message, 0, 0, 0))
	{
		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	// ���ΰ��� ����
	g_mainGame.Release();

	return message.wParam;
}

void SetWindowSize(int startX, int startY, int sizeX, int sizeY)
{
	// ���ϴ� ������ �۾����� ����
	RECT rc;
	rc.left = 0; rc.top = 0;
	rc.right = sizeX;	rc.bottom = sizeY;

	// ��Ÿ���� ���Ե� ���� ������ ũ�� ���
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, false);

	// ���� ������ �����츦 �̵���Ű�鼭 ũ�⵵ �Բ� ��ȯ
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

	case WM_PAINT:		// ������ ȭ���� �ٽ� �׷����� ��� �߻��ϴ� �޽���
		hdc = BeginPaint(g_hWnd, &ps);

		// â ũ�� �����ص� �׿����� �̹��� �׷���
		RECT rect;
		SetMapMode(hdc, MM_ANISOTROPIC);
		SetWindowExtEx(hdc, WIN_SIZE_X, WIN_SIZE_Y, NULL);
		GetClientRect(g_hWnd, &rect);
		SetViewportExtEx(hdc, rect.right, rect.bottom, NULL);

		
		g_mainGame.Render(hdc);

		EndPaint(g_hWnd, &ps);
		break;

	case WM_DESTROY:	// �ݱ� ��ư �޽���ó�� (���α׷� ����)
		PostQuitMessage(0);
		break;
	}

	return g_mainGame.MainProc(hWnd, iMessage, wParam, lParam);
}