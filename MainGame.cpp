#include "MainGame.h"
#include "Image.h"
#include "StartScene.h"
#include "GameScene.h"

HRESULT MainGame::Init()
{
	IMAGE_MANAGER->Init();
	KEY_MANAGER->Init();
	SCENE_MANAGER;
	TIMER_MANAGER->Init();

	SCENE_MANAGER->AddScene("스타트씬", new StartScene());
	SCENE_MANAGER->AddScene("게임씬", new GameScene());

	SCENE_MANAGER->ChangeScene("스타트씬");


	srand((unsigned int) time(nullptr));

	// 타이머 셋팅
	hTimer = (HANDLE)SetTimer(g_hWnd, 0, 10, NULL);

	// 백버퍼
	backBuffer = new Image;
	backBuffer->Init(WIN_SIZE_X, WIN_SIZE_Y);

	return S_OK;
}

void MainGame::Update()
{
	TIMER_MANAGER->Update();

	SCENE_MANAGER->Update();

	InvalidateRect(g_hWnd, NULL, false);
}

void MainGame::Render(HDC hdc)
{
	HDC hBackBufferDC = backBuffer->GetMemDC();

	PatBlt(hBackBufferDC, 0, 0, WIN_SIZE_X, WIN_SIZE_Y, WHITENESS);

	SCENE_MANAGER->Render(hBackBufferDC);

	TIMER_MANAGER->Render(hBackBufferDC);


	wsprintf(text, "MousePosX : %d", g_ptMouse.x);
	TextOut(hBackBufferDC, 200, 10, text, strlen(text));

	wsprintf(text, "MousePosY : %d", g_ptMouse.y);
	TextOut(hBackBufferDC, 200, 40, text, strlen(text));

	backBuffer->Render(hdc);
}

void MainGame::Release()
{
	SAFE_RELEASE(backBuffer);

	IMAGE_MANAGER->Release();
	SCENE_MANAGER->Release();
	TIMER_MANAGER->Release();

	// 타이머 객체 삭제
	KillTimer(g_hWnd, 0);
}

LRESULT MainGame::MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_SPACE:
			break;
		case VK_UP:
			break;
		case VK_DOWN:
			break;
		case VK_LEFT:
			break;
		case VK_RIGHT:
			break;
		}
		break;

	case WM_LBUTTONDOWN:
		g_ptClickedMouse.x = LOWORD(lParam);
		g_ptClickedMouse.y = HIWORD(lParam);
		break;
	case WM_LBUTTONUP:
		break;
	case WM_RBUTTONDOWN:
		break;
	case WM_MOUSEMOVE:
		g_ptMouse.x = GET_X_LPARAM(lParam);
		g_ptMouse.y = GET_Y_LPARAM(lParam);
		break;
	}
	return DefWindowProc(hWnd, iMessage, wParam, lParam);
}
