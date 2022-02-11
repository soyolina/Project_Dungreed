#include "stdafx.h"
#include "MainGame.h"
#include "Image.h"


HRESULT MainGame::Init()
{
	//KEY_MANAGER->Init();
	// 
	// Input Ŭ���� �߰�
	Input::Init(g_hWnd);
	IMAGE_MANAGER->Init();
	SCENE_MANAGER->Init();
	TIMER_MANAGER->Init();

	SCENE_MANAGER->ChangeScene(L"��ŸƮ��");
	//SCENE_MANAGER->ChangeScene(L"Ÿ�ϸ�����");

	srand((unsigned int) time(nullptr));

	// Ÿ�̸� ����
	hTimer = (HANDLE)SetTimer(g_hWnd, 0, 10, NULL);

	// �����
	backBuffer = new Image;
	backBuffer->Init(TILE_MAP_TOOL_SIZE_X, TILE_MAP_TOOL_SIZE_Y);

	return S_OK;
}

void MainGame::Update()
{
	Input::Update();

	TIMER_MANAGER->Update();

	SCENE_MANAGER->Update();

	InvalidateRect(g_hWnd, NULL, false);
}

void MainGame::Render(HDC hdc)
{
	HDC hBackBufferDC = backBuffer->GetMemDC();

	PatBlt(hBackBufferDC, 0, 0, TILE_MAP_TOOL_SIZE_X, TILE_MAP_TOOL_SIZE_Y, WHITENESS);

	SCENE_MANAGER->Render(hBackBufferDC);

	TIMER_MANAGER->Render(hBackBufferDC);
	
	wsprintf(text, L"MousePosX : %d", Input::GetMousePosition().x);
	TextOut(hBackBufferDC, 200, 10, text, wcslen(text));

	wsprintf(text, L"MousePosY : %d", Input::GetMousePosition().y);
	TextOut(hBackBufferDC, 200, 40, text, wcslen(text));

	backBuffer->Render(hdc);
}

void MainGame::Release()
{
	SAFE_RELEASE(backBuffer);

	IMAGE_MANAGER->Release();
	SCENE_MANAGER->Release();
	TIMER_MANAGER->Release();

	// Ÿ�̸� ��ü ����
	KillTimer(g_hWnd, 0);
}
