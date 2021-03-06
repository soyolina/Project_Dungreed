#pragma once
#include "Config.h"

class Image;
class MainGame
{
private:
	// 타이머
	HANDLE hTimer = nullptr;

	// UI
	TCHAR text[128] = {};

	// 백버퍼
	Image* backBuffer = nullptr;
	

public:
	HRESULT Init();
	void Update();
	void Render(HDC hdc);
	void Release();
};

