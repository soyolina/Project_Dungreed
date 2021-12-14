#include "stdafx.h"
#include "Bellial.h"
#include "Image.h"

HRESULT Bellial::Init()
{
	// - ��ӹ��� �� - GameObject����
	m_pos = {};
	m_bodyWidth = 0;
	m_bodyHeight = 0;
	m_shape = {};

	m_hp = 100;
	m_moveSpeed = 0.0f;
	// - �������

	// - ���� �ִϸ��̼� ��
	m_bossIdleImg = IMAGE_MANAGER->FindImage(L"Image/Boss/SkellBossIdle.bmp");
	m_bossBackImg = IMAGE_MANAGER->FindImage(L"Image/Boss/SkellBossBack.bmp");

	m_frameX = 0;
	m_frameY = 0;
	m_maxFrameX = 10;
	m_elapsedCount = 0.0f;

	// - ���� HP UI ����
	m_bossLifeBack = IMAGE_MANAGER->FindImage(L"Image/Boss/BossLifeBack.bmp");
	m_bossLifeGage = IMAGE_MANAGER->FindImage(L"Image/Boss/BossLifeRed.bmp");
	m_bossLifeBar = IMAGE_MANAGER->FindImage(L"Image/Boss/BossLifeBase.bmp");

	m_bossHpPercentage = m_bossLifeGage->GetWidth() * 0.01f; // 100���� ������ ��

    return S_OK;
}

void Bellial::Update()
{
	// �ִϸ��̼�
	m_elapsedCount += TIMER_MANAGER->GetDeltaTime();
	if (m_elapsedCount >= 0.08f)
	{
		++m_frameX;
		if (m_frameX >= m_maxFrameX)
		{
			m_frameX = 0;
		}
		m_elapsedCount = 0.0f;
	}


	// HP GAGE �ٴ��� �׽�Ʈ��
	if (Input::GetButtonDown(VK_F4))
	{
		m_hp -= 5;
	}
}

void Bellial::Render(HDC hdc)
{
	m_bossBackImg->Render(hdc, static_cast<int>(BOSSBACK_POSX), static_cast<int>(BOSSBACK_POSY), m_frameX, m_frameY, 1.0f);
	m_bossIdleImg->Render(hdc, static_cast<int>(BOSS_POSX), static_cast<int>(BOSS_POSY), m_frameX, m_frameY, 1.0f);

	// Boss HP UI ������
	m_bossLifeBack->Render(hdc, static_cast<int>(BOSS_HP_UI_POSX), BOSS_HP_UI_POSY);
	m_bossLifeGage->HpRender2(hdc, static_cast<int>(BOSS_HP_GAGE_POSX), BOSS_HP_GAGE_POSY, m_hp * m_bossHpPercentage);
	m_bossLifeBar->Render(hdc, static_cast<int>(BOSS_HP_UI_POSX), BOSS_HP_UI_POSY);
	
}

void Bellial::Release()
{
	m_bossIdleImg = nullptr;
	m_bossBackImg = nullptr;

	m_bossLifeBack = nullptr;
	m_bossLifeGage = nullptr;
	m_bossLifeBar = nullptr;
}
