#pragma once

#include "Config.h"
#include "CommonFunction.h"

class Player;
class UIBase;
class UIManager
{
private:
	vector<UIBase*> m_allUI = {};

	Player* m_player = nullptr;

public:
	void Init(Player* player);
	void Update();
	void Render(HDC hdc);
	void Release();

	inline void SetPlayer(Player* player) { this->m_player = player; }
};

