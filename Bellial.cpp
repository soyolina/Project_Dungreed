#include "stdafx.h"
#include "Bellial.h"
#include "Image.h"

HRESULT Bellial::Init()
{
	// 상속받은 것 - GameObject에서
	m_pos = {};
	m_bodyWidth = 0;
	m_bodyHeight = 0;
	m_shape = {};

	m_hp = 0;
	m_moveSpeed = 0.0f;

	// 보스
	BossIdleImg = IMAGE_MANAGER->FindImage(L"Image/Boss/SkellBossIdle.bmp");
	BossBackImg = IMAGE_MANAGER->FindImage(L"Image/Boss/SkellBossBack.bmp");

	m_frameX = 0;
	m_frameY = 0;
	m_maxFrameX = 10;
	m_elapsedCount = 0.0f;


    return S_OK;
}

void Bellial::Update()
{
	m_elapsedCount += TIMER_MANAGER->GetDeltaTime();
	if (m_elapsedCount >= 0.1f)
	{
		++m_frameX;
		if (m_frameX >= m_maxFrameX)
		{
			m_frameX = 0;
		}
		m_elapsedCount = 0.0f;
	}

}

void Bellial::Render(HDC hdc)
{
	BossBackImg->Render(hdc, static_cast<int>(BOSSBACK_POSX), static_cast<int>(BOSSBACK_POSY), m_frameX, m_frameY, 1.0f);
	BossIdleImg->Render(hdc, static_cast<int>(BOSS_POSX), static_cast<int>(BOSS_POSY), m_frameX, m_frameY, 1.0f);
}

void Bellial::Release()
{
	BossIdleImg = nullptr;
}
