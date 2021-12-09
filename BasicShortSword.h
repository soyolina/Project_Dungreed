#pragma once

#include "ItemBase.h"

#define BSS_POSX(playerPosX) (playerPosX + 10)
#define BSS_POSY(p) (p + 12)

class Image;
class BasicShortSword : public ItemBase
{
private:
	Image* m_img = nullptr;

	POINT m_imgRenderPos = {};
	bool mb_isReverse = false;

public:
	virtual void Init(Player* player);
	virtual void Update();
	virtual void Render(HDC hdc);
	virtual void Release();
};

