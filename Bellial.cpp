#include "stdafx.h"
#include "Bellial.h"
#include "Image.h"


HRESULT Bellial::Init()
{
	// - 보스 애니메이션 용
	m_bossIdleImg = IMAGE_MANAGER->FindImage(L"Image/Boss/SkellBossIdle.bmp"); // 보스 본체
	m_bossBackImg = IMAGE_MANAGER->FindImage(L"Image/Boss/SkellBossBack.bmp"); // 보스 본체 뒤 큰 중앙 구슬
	
	// 보스 본체 뒤 작은 구슬들
	SetParticleImgData();

	// 손 (좌, 우)
	m_bossLeftHandImg = IMAGE_MANAGER->FindImage(L"Image/Boss/SkellBossLeftHandIdle.bmp");
	m_bossRightHandImg = IMAGE_MANAGER->FindImage(L"Image/Boss/SkellBossRightHandIdle.bmp");
	m_leftHandPosY = BOSS_LEFTHAND_POSY;
	m_rightHandPosY = BOSS_RIGHTHAND_POSY;

	// 본체, 중앙구슬 , 좌우손 애니메이션이 쓰고있음 
	m_frameX = 0;
	m_frameY = 0;
	m_maxFrameX = 10;
	m_elapsedCount = 0.0f;

	// - 상속받은 것 - GameObject에서
	// 히트박스 셋팅용
	m_pos = { BOSS_HITBOX_POSX, BOSS_HITBOX_POSY };
	m_bodyWidth = m_bossIdleImg->GetFrameWidth() - 46;
	m_bodyHeight = m_bossIdleImg->GetFrameHeight() - 66;
	
	SetHitbox();

	m_hp = 100;
	m_moveSpeed = 0.0f;
	// - 여기까지

	// - 보스 HP UI 관련
	m_bossLifeBack = IMAGE_MANAGER->FindImage(L"Image/Boss/BossLifeBack.bmp");
	m_bossLifeGage = IMAGE_MANAGER->FindImage(L"Image/Boss/BossLifeRed.bmp");
	m_bossLifeBar = IMAGE_MANAGER->FindImage(L"Image/Boss/BossLifeBase.bmp");

	m_bossHpPercentage = m_bossLifeGage->GetWidth() * 0.01f; // 100으로 나눠준 것

    return S_OK;
}

void Bellial::Update()
{
	// 본체, 중앙구슬 , 좌우손 애니메이션
	m_elapsedCount += TIMER_MANAGER->GetDeltaTime();
	if (m_elapsedCount >= 0.07f)
	{
		++m_frameX;
		if (m_frameX >= m_maxFrameX)
		{
			m_frameX = 0;
		}
		m_elapsedCount = 0.0f;
	}

	// 작은구슬 파티클 애니메이션
	UpdateParticleAnimation();


	// HP GAGE 다는지 테스트용
	if (Input::GetButtonDown(VK_F4))
	{
		m_hp -= 5;
	}
}

void Bellial::Render(HDC hdc)
{
	// 히트박스
	//Rectangle(hdc, m_shape.left, m_shape.top, m_shape.right, m_shape.bottom);

	// - 보스
	// 작은구슬
	for (int i = 0; i < 7; ++i)
	{
		if (m_particleArr[i].mb_isAnimated == true)
		{
			m_particleArr[i].m_bossParticleImg->Render(hdc, m_particleArr[i].m_particlePos.x, m_particleArr[i].m_particlePos.y, 
													m_particleArr[i].m_particleFrameX, m_frameY, 1.0f);
		}
	}
	// 큰구슬
	m_bossBackImg->Render(hdc, static_cast<int>(BOSSBACK_POSX), static_cast<int>(BOSSBACK_POSY), m_frameX, m_frameY, 1.0f);
	// 보스 IDLE
	m_bossIdleImg->Render(hdc, static_cast<int>(BOSS_POSX), static_cast<int>(BOSS_POSY), m_frameX, m_frameY, 1.0f);
	// 손
	m_bossLeftHandImg->Render(hdc, static_cast<int>(BOSS_LEFTHAND_POSX), static_cast<int>(m_leftHandPosY), m_frameX, m_frameY, 1.0f);
	m_bossRightHandImg->Render(hdc, static_cast<int>(BOSS_RIGHTHAND_POSX), static_cast<int>(m_rightHandPosY), m_frameX, m_frameY, 1.0f);

	// Boss HP UI 렌더용
	m_bossLifeBack->Render(hdc, static_cast<int>(BOSS_HP_UI_POSX), BOSS_HP_UI_POSY);
	m_bossLifeGage->HpRender2(hdc, static_cast<int>(BOSS_HP_GAGE_POSX), BOSS_HP_GAGE_POSY, m_hp * m_bossHpPercentage);
	m_bossLifeBar->Render(hdc, static_cast<int>(BOSS_HP_UI_POSX), BOSS_HP_UI_POSY);
}

void Bellial::Release()
{
	m_bossIdleImg = nullptr;
	m_bossBackImg = nullptr;
	for (int i = 0; i < 7; ++i)
	{
		m_particleArr[i].m_bossParticleImg = nullptr;
	}

	m_bossLifeBack = nullptr;
	m_bossLifeGage = nullptr;
	m_bossLifeBar = nullptr;
}

void Bellial::SetHitbox()
{
	m_shape.left = static_cast<long>(m_pos.x - m_bodyWidth * 0.5f);
	m_shape.right = static_cast<long>(m_pos.x + m_bodyWidth * 0.5f);
	m_shape.top = static_cast<long>(m_pos.y - m_bodyHeight * 0.5f);
	m_shape.bottom = static_cast<long>(m_pos.y + m_bodyHeight * 0.5f);
}

void Bellial::SetParticleInitialPos(int num)
{
	int supplementAxisValue = 60;
	int supplementDiagonalValue = 30;

	switch (num + 1)
	{
	case 1:
		m_particleArr[num].m_particleIdlePos = { BOSSBACK_POSX - supplementDiagonalValue, BOSSBACK_POSY - supplementDiagonalValue };
		break;

	case 2:
		m_particleArr[num].m_particleIdlePos = { BOSSBACK_POSX - supplementAxisValue, BOSSBACK_POSY };
		break;

	case 3:
		m_particleArr[num].m_particleIdlePos = { BOSSBACK_POSX - supplementDiagonalValue, BOSSBACK_POSY + supplementDiagonalValue };
		break;

	case 4:
		m_particleArr[num].m_particleIdlePos = { BOSSBACK_POSX , BOSSBACK_POSY + supplementAxisValue };
		break;

	case 5:
		m_particleArr[num].m_particleIdlePos = { BOSSBACK_POSX + supplementDiagonalValue, BOSSBACK_POSY + supplementDiagonalValue };
		break;

	case 6:
		m_particleArr[num].m_particleIdlePos = { BOSSBACK_POSX + supplementAxisValue, BOSSBACK_POSY };
		break;

	case 7:
		m_particleArr[num].m_particleIdlePos = { BOSSBACK_POSX + supplementDiagonalValue, BOSSBACK_POSY - supplementDiagonalValue };
		break;
	}
}

void Bellial::SetParticleImgData()
{
	for (int i = 0; i < 7; ++i)
	{
		SetParticleInitialPos(i);

		m_particleArr[i].m_bossParticleImg = IMAGE_MANAGER->FindImage(L"Image/Boss/SkellBossParticle.bmp");
		m_particleArr[i].m_particlePos = m_particleArr[i].m_particleIdlePos;
		m_particleArr[i].m_particleMoveSpeed = 7.0f;
		m_particleArr[i].m_particleFrameX = 0;
		m_particleArr[i].m_particleMaxFrameX = 8;
		m_particleArr[i].m_particleElapsedCount = 0.0f;
		m_particleArr[i].mb_isAnimated = false;
	}
}

void Bellial::SetParticleImgPos(int num)
{
	switch (num + 1)
	{
	case 1:
		m_particleArr[num].m_particlePos.x -= m_particleArr[num].m_particleMoveSpeed;
		m_particleArr[num].m_particlePos.y -= m_particleArr[num].m_particleMoveSpeed;
		break;

	case 2:
		m_particleArr[num].m_particlePos.x -= m_particleArr[num].m_particleMoveSpeed;
		break;

	case 3:
		m_particleArr[num].m_particlePos.x -= m_particleArr[num].m_particleMoveSpeed;
		m_particleArr[num].m_particlePos.y += m_particleArr[num].m_particleMoveSpeed;
		break;

	case 4:
		m_particleArr[num].m_particlePos.y += m_particleArr[num].m_particleMoveSpeed;
		break;

	case 5:
		m_particleArr[num].m_particlePos.x += m_particleArr[num].m_particleMoveSpeed;
		m_particleArr[num].m_particlePos.y += m_particleArr[num].m_particleMoveSpeed;
		break;

	case 6:
		m_particleArr[num].m_particlePos.x += m_particleArr[num].m_particleMoveSpeed;
		break;

	case 7:
		m_particleArr[num].m_particlePos.x += m_particleArr[num].m_particleMoveSpeed;
		m_particleArr[num].m_particlePos.y -= m_particleArr[num].m_particleMoveSpeed;
		break;
	}
}

void Bellial::UpdateParticleAnimation()
{
	for (int i = 0; i < 7; ++i)
	{
		if (m_particleArr[i].mb_isAnimated == false)
		{
			int randomValue = rand() % 120;
			if (randomValue == 1) { m_particleArr[i].mb_isAnimated = true; }
		}

		if (m_particleArr[i].mb_isAnimated == true)
		{
			m_particleArr[i].m_particleElapsedCount += TIMER_MANAGER->GetDeltaTime();
			if (m_particleArr[i].m_particleElapsedCount >= 0.08f)
			{
				// pos 위치 변경 - 이미지 렌더 위치 변경 (이동하는 것처럼)
				SetParticleImgPos(i);

				++m_particleArr[i].m_particleFrameX;
				if (m_particleArr[i].m_particleFrameX >= m_particleArr[i].m_particleMaxFrameX)
				{
					m_particleArr[i].m_particleFrameX = 0;
					m_particleArr[i].m_particlePos = m_particleArr[i].m_particleIdlePos;
					m_particleArr[i].mb_isAnimated = false;
				}
				m_particleArr[i].m_particleElapsedCount = 0.0f;
			}
		}
	}
}
