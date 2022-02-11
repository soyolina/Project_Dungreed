#pragma once
#include "Config.h"

class Image;
class MainGame
{
private:
	// Ÿ�̸�
	HANDLE hTimer = nullptr;

	// UI
	TCHAR text[128] = {};

	// �����
	Image* backBuffer = nullptr;
	

public:
	HRESULT Init();
	void Update();
	void Render(HDC hdc);
	void Release();
};

