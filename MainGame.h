#pragma once
#include "Config.h"

class Image;
class MainGame
{
private:
	// Ÿ�̸�
	HANDLE hTimer = nullptr;

	// UI
	char text[128] = {};

	// �����
	Image* backBuffer = nullptr;
	

public:
	HRESULT Init();
	void Update();
	void Render(HDC hdc);
	void Release();

	LRESULT MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
};

