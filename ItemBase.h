#pragma once

#include "Config.h"
#include "CommonFunction.h"

class Player;
class ItemBase
{
protected:
	Player* m_player = nullptr;
	POINTFLOAT m_playerPos = {};

public:
	virtual void Init();
	virtual void Update();
	virtual void Render(HDC hdc);
	virtual void Release();
};

