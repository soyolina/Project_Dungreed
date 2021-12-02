#pragma once
#include "Config.h"

class Image;
class MainGame
{
private:
	// 타이머
	HANDLE hTimer = nullptr;

	// UI
	char text[128] = {};

	// 백버퍼
	Image* backBuffer = nullptr;
	

public:
	HRESULT Init();
	void Update();
	void Render(HDC hdc);
	void Release();

	LRESULT MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
};

