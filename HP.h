#pragma once

#include "UIBase.h"

#define LIFEBASE_POSX (30)
#define LIFEBASE_POSY (20)

#define LIFEGAGE_POSX (LIFEBASE_POSX + 88.5f)
#define LIFEGAGE_POSY (LIFEBASE_POSY + 12)

class Image;
class HP : public UIBase
{
private:
	Image* m_playerLifeBase = nullptr;
	Image* m_playerLifeGray = nullptr;
	Image* m_playerLifeGage = nullptr;

	int m_playerHP = 0;
	float m_playerHpPercentage = 0.0f;

public:
	virtual ~HP() {}

	virtual void Init();
	virtual void Update();
	virtual void Render(HDC hdc);
	virtual void Release();

};

