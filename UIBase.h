#pragma once

#include "Config.h"
#include "CommonFunction.h"

class Player;
class UIBase
{
protected:
	Player* m_player = nullptr;

public:
	virtual ~UIBase() {}

	virtual void Init();
	virtual void Update();
	virtual void Render(HDC hdc);
	virtual void Release();

	inline void SetPlayer(Player* player) { this->m_player = player; }
};