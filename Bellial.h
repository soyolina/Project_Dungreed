#pragma once

#include "Enemy.h"

#define BOSS_POSX  (WIN_SIZE_X * 0.5f - 30)
#define BOSS_POSY  (WIN_SIZE_Y * 0.5f - 30)

#define BOSSBACK_POSX (WIN_SIZE_X * 0.5f)
#define BOSSBACK_POSY (WIN_SIZE_Y * 0.5f)

class Image;
class Bellial : public Enemy
{
private:
	Image* BossIdleImg = nullptr;
	Image* BossBackImg = nullptr;

	int m_frameX = 0;
	int m_frameY = 0;
	int m_maxFrameX = 0;
	float m_elapsedCount = 0.0f;

	// 보스 HP UI 관련


public:
	virtual ~Bellial() {}

	virtual HRESULT Init();
	virtual void Update();
	virtual void Render(HDC hdc);
	virtual void Release();
};

