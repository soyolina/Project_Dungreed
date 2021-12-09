#pragma once

#include "ItemBase.h"

#define BSS_POSX(playerPosX, direction) (playerPosX + 10 * direction)
#define BSS_POSY(p) (p + 12)

class Image;
class BasicShortSword : public ItemBase
{
private:
	Image* m_img = nullptr;

	int m_direction = 0;

public:
	virtual void Init(Player* player);
	virtual void Update();
	virtual void Render(HDC hdc);
	virtual void Release();
};

