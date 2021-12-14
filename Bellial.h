#pragma once

#include "Enemy.h"

#define BOSS_POSX  (WIN_SIZE_X * 0.5f - 23)
#define BOSS_POSY  (WIN_SIZE_Y * 0.5f - 70)

#define BOSSBACK_POSX (WIN_SIZE_X * 0.5f)
#define BOSSBACK_POSY (WIN_SIZE_Y * 0.5f)

#define BOSS_HP_UI_POSX  (WIN_SIZE_X * 0.5f)
#define BOSS_HP_UI_POSY  (WIN_SIZE_Y - 50)

#define BOSS_HP_GAGE_POSX (WIN_SIZE_X * 0.5f + 46.0f)
#define BOSS_HP_GAGE_POSY (WIN_SIZE_Y - 50)
 
class Image;
class Bellial : public Enemy
{
private:
	// 보스 애니메이션용
	Image* m_bossIdleImg = nullptr;
	Image* m_bossBackImg = nullptr;

	int m_frameX = 0;
	int m_frameY = 0;
	int m_maxFrameX = 0;
	float m_elapsedCount = 0.0f;

	// - 보스 HP UI 관련
	// 변수
	Image* m_bossLifeBack = nullptr;
	Image* m_bossLifeGage = nullptr;
	Image* m_bossLifeBar = nullptr;

	float m_bossHpPercentage = 0.0f;


public:
	virtual ~Bellial() {}

	virtual HRESULT Init();
	virtual void Update();
	virtual void Render(HDC hdc);
	virtual void Release();
};

