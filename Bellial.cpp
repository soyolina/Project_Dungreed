#include "stdafx.h"
#include "Bellial.h"
#include "Image.h"
#include "Player.h"
#include "AmmoManager.h"
#include "Collider2.h"


HRESULT Bellial::Init()
{
	// - 보스 애니메이션 용
	m_bossImg = IMAGE_MANAGER->FindImage(L"Image/Boss/SkellBossIdle.bmp"); // 보스 본체
	m_bossHitImg = IMAGE_MANAGER->FindImage(L"Image/Boss/SkellBossIdleHit.bmp"); // 보스 피격
	m_bossBackImg = IMAGE_MANAGER->FindImage(L"Image/Boss/SkellBossBack.bmp"); // 보스 본체 뒤 큰 중앙 구슬
	
	// 보스 본체 뒤 작은 구슬들
	SetParticleImgData();

	// 본체 idle , 중앙구슬 애니메이션이 쓰고있음 
	m_frameX = 0;
	m_frameY = 0;
	m_maxFrameX = 10;
	m_elapsedCount = 0.0f;


	// - 손 (좌, 우)
	m_lefthandElapsedCount = 0.0f;
	m_righthandElapsedCount = 0.0f;

	// LeftHand Idle
	m_leftHand[static_cast<int>(LeftHandStatus::Idle)].handImg = IMAGE_MANAGER->FindImage(L"Image/Boss/SkellBossLeftHandIdle.bmp");
	m_leftHand[static_cast<int>(LeftHandStatus::Idle)].frameX = 0;
	m_leftHand[static_cast<int>(LeftHandStatus::Idle)].frameY = 0;
	m_leftHand[static_cast<int>(LeftHandStatus::Idle)].maxFrameX = 10;
	m_leftHand[static_cast<int>(LeftHandStatus::Idle)].totalTime = 0.07f;

	// RightHand Idle
	m_rightHand[static_cast<int>(RightHandStatus::Idle)].handImg = IMAGE_MANAGER->FindImage(L"Image/Boss/SkellBossRightHandIdle.bmp");
	m_rightHand[static_cast<int>(RightHandStatus::Idle)].frameX = 0;
	m_rightHand[static_cast<int>(RightHandStatus::Idle)].frameY = 0;
	m_rightHand[static_cast<int>(RightHandStatus::Idle)].maxFrameX = 10;
	m_rightHand[static_cast<int>(RightHandStatus::Idle)].totalTime = 0.07f;

	// LeftHand Attack
	m_leftHand[static_cast<int>(LeftHandStatus::Attack)].handImg = IMAGE_MANAGER->FindImage(L"Image/Boss/SkellBossLeftHandAttack.bmp");
	m_leftHand[static_cast<int>(LeftHandStatus::Attack)].frameX = 0;
	m_leftHand[static_cast<int>(LeftHandStatus::Attack)].frameY = 0;
	m_leftHand[static_cast<int>(LeftHandStatus::Attack)].maxFrameX = 18;
	m_leftHand[static_cast<int>(LeftHandStatus::Attack)].totalTime = 0.07f;

	// RightHand Attack
	m_rightHand[static_cast<int>(RightHandStatus::Attack)].handImg = IMAGE_MANAGER->FindImage(L"Image/Boss/SkellBossRightHandAttack.bmp");
	m_rightHand[static_cast<int>(RightHandStatus::Attack)].frameX = 0;
	m_rightHand[static_cast<int>(RightHandStatus::Attack)].frameY = 0;
	m_rightHand[static_cast<int>(RightHandStatus::Attack)].maxFrameX = 18;
	m_rightHand[static_cast<int>(RightHandStatus::Attack)].totalTime = 0.07f;

	me_leftHandStatus = LeftHandStatus::Idle;
	me_rightHandStatus = RightHandStatus::Idle;

	m_leftHandPosY = BOSS_LEFTHAND_POSY;
	m_rightHandPosY = BOSS_RIGHTHAND_POSY;
	m_handMoveSpeed = 5.0f;

	mb_isLefthandAttack = false;
	m_handAttackDelay = 0.0f;
	m_totalHandAttackCount = 3;

	mb_readyToFire = false;

	// Laser
	m_laserBody = IMAGE_MANAGER->FindImage(L"Image/Boss/SkellBossLaserBody.bmp");
	m_laserHead = IMAGE_MANAGER->FindImage(L"Image/Boss/SkellBossLaserHead.bmp");

	m_laserHitbox = {};
	m_leftLaserFrameX = 0;
	m_rightLaserFrameX = 0;
	m_LaserMaxFrameX = 7;
	m_leftLaserElapsedCount = 0.0f;
	m_rightLaserElapsedCount = 0.0f;
	mb_leftLaserAttack = false;
	mb_rightLaserAttack = false;

	m_rightLaserBodyFrameX = m_LaserMaxFrameX - 1;

	
	// 미사일 쏘는 패턴 용
	AmmoManager* m_ammoManager = nullptr;
	m_ammoPos = { BOSS_AMMO_POSX , BOSS_AMMO_POSY };
	m_ammoInterval = 0.0f;
	m_ammoAttackDelay = 0.0f;
	m_ammoAttackDuration = 0.0f;
	m_ammoAngle = 0.0f;
	m_ammoChangeAngle = 0.0f;
	m_totalAmmoAttackCount = 3;


	// Attack Delay
	mb_isAttack = false;
	m_attackDelay = 0.0f;
	mb_fireLaserbeam = false;
	mb_fireAmmo = false;

	// - 상속받은 것 - GameObject에서
	// 히트박스 셋팅용
	m_pos = { BOSS_HITBOX_POSX, BOSS_HITBOX_POSY };
	m_bodyWidth = m_bossImg->GetFrameWidth() - 46;
	m_bodyHeight = m_bossImg->GetFrameHeight() - 66;
	
	SetHitbox();
	// 콜라이더
	m_collider = ColliderManager::CreateCollider(this, m_shape, ObjectType::Enemy);
	m_laserCollider = nullptr; 

	m_hp = 100;
	m_moveSpeed = 0.0f;
	m_attackDamage = 0;
	mb_isHit = false;
	m_hitElapsedCount = 0.0f;
	mb_isDead = false;
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
	// 죽는 거 셋팅
	if (m_hp <= 0)
	{
		mb_isDead = true;
		return;
	}

	// 맞았을 때
	if (mb_isHit == true)
	{
		m_hitElapsedCount += TIMER_MANAGER->GetDeltaTime();
		if (m_hitElapsedCount > 0.2f)
		{
			mb_isHit = false;
			m_hitElapsedCount = 0.0f;
		}
	}

	// 본체, 중앙구슬 애니메이션
	m_elapsedCount += TIMER_MANAGER->GetDeltaTime();
	if (m_elapsedCount >= 0.07f)
	{
		++m_frameX;
		if (m_frameX >= m_maxFrameX)
		{
			m_frameX = 0;

			// 보스가 미사일 패턴일땐 미사일 공격끝날때까지 마지막 이미지 프레임으로 계속 고정해두기 위해(입벌린 상태로)
			if (mb_fireAmmo == true && mb_readyToFire == true)
			{
				m_frameX = m_maxFrameX - 1;
			}
		}
		m_elapsedCount = 0.0f;
	}

	// 작은구슬 파티클 애니메이션
	UpdateParticleAnimation();

	// 공격 딜레이와 그에 따른 랜덤 공격패턴 설정
	if (mb_isAttack == false)
	{
		m_attackDelay += TIMER_MANAGER->GetDeltaTime();
		if (m_attackDelay >= 2.0f)
		{
			int randomValue = rand() % 3;
			switch (1/*randomValue*/)
			{
			case 0:
				mb_isAttack = true;
				mb_fireLaserbeam = true;
				m_attackDamage = 10;
				break;
			case 1:
				mb_isAttack = true;
				mb_fireAmmo = true;
				m_attackDamage = 8;
				break;
			case 2:
				break;
			}

			m_attackDelay = 0.0f;
		}
		
	}

	// 공격 패턴 1 : 레이저 빔
	if (mb_isAttack == true && mb_fireLaserbeam == true)
	{
		FireLaserbeam();
	}

	// 공격패턴 2 : 미사일 
	if (mb_isAttack == true && mb_fireAmmo == true)
	{
		FireMissile();
	}
	
	// 콜라이더
	m_collider->Update(m_shape); // 해골 본체
}

void Bellial::Render(HDC hdc)
{
	// - 보스
	// 작은구슬
	for (int i = 0; i < 7; ++i)
	{
		if (m_particleArr[i].mb_isAnimated == true)
		{
			m_particleArr[i].m_bossParticleImg->Render(hdc, static_cast<int>(m_particleArr[i].m_particlePos.x), 
											static_cast<int>(m_particleArr[i].m_particlePos.y), m_particleArr[i].m_particleFrameX, m_frameY, 1.0f);
		}
	}

	// 큰구슬
	m_bossBackImg->Render(hdc, static_cast<int>(BOSSBACK_POSX), static_cast<int>(BOSSBACK_POSY), m_frameX, m_frameY, 1.0f);
	
	// 보스 IDLE
	if (mb_isHit == true)
	{
		m_bossHitImg->Render(hdc, static_cast<int>(BOSS_POSX), static_cast<int>(BOSS_POSY), m_frameX, m_frameY, 1.0f);
	}
	else
	{
		m_bossImg->Render(hdc, static_cast<int>(BOSS_POSX), static_cast<int>(BOSS_POSY), m_frameX, m_frameY, 1.0f);
	}
	
	// 왼손
	RenderLeftHand(hdc, me_leftHandStatus);
	// 오른손
	RenderRightHand(hdc, me_rightHandStatus);

	// Boss HP UI 렌더용
	m_bossLifeBack->Render(hdc, static_cast<int>(BOSS_HP_UI_POSX), BOSS_HP_UI_POSY);
	m_bossLifeGage->HpRender2(hdc, static_cast<int>(BOSS_HP_GAGE_POSX), BOSS_HP_GAGE_POSY, m_hp * m_bossHpPercentage);
	m_bossLifeBar->Render(hdc, static_cast<int>(BOSS_HP_UI_POSX), BOSS_HP_UI_POSY);
}

void Bellial::Release()
{
	m_bossImg = nullptr;
	m_bossBackImg = nullptr;
	for (int i = 0; i < 7; ++i)
	{
		m_particleArr[i].m_bossParticleImg = nullptr;
	}

	for (int i = static_cast<int>(LeftHandStatus::Idle); i < static_cast<int>(LeftHandStatus::End); ++i)
	{
		m_leftHand[i].handImg = nullptr;
	}
	for (int i = static_cast<int>(RightHandStatus::Idle); i < static_cast<int>(RightHandStatus::End); ++i)
	{
		m_rightHand[i].handImg = nullptr;
	}
	m_laserBody = nullptr;
	m_laserHead = nullptr;

	m_bossLifeBack = nullptr;
	m_bossLifeGage = nullptr;
	m_bossLifeBar = nullptr;
}

void Bellial::FireMissile()
{
	// 공격횟수 다 썼으면 리턴
	if (m_totalAmmoAttackCount == 0)
	{
		mb_isAttack = false;
		mb_fireAmmo = false;

		m_ammoAngle = 0.0f;
		m_ammoChangeAngle = 0.0f;
		m_ammoInterval = 0.0f;
		m_ammoAttackDelay = 0.0f;
		m_ammoAttackDuration = 0.0f;
		m_totalAmmoAttackCount = 3;

		mb_readyToFire = false;

		// 보스 본체 이미지 셋팅용, 공격이 끝났으니 다시 Idle로 모두 전환
		m_bossImg = IMAGE_MANAGER->FindImage(L"Image/Boss/SkellBossIdle.bmp");
		m_bossHitImg = IMAGE_MANAGER->FindImage(L"Image/Boss/SkellBossIdleHit.bmp");
		m_frameX = 0;
		m_elapsedCount = 0.0f;
		SetHitbox();

		return;
	}

	// 미사일 공격횟수간의 발사 간격
	if (mb_readyToFire == false)
	{
		m_ammoInterval += TIMER_MANAGER->GetDeltaTime();
		if (m_ammoInterval >= 1.5f && m_totalAmmoAttackCount > 0)
		{
			int randomValue = rand() % 2;
			randomValue ? m_ammoChangeAngle = 10.0f : m_ammoChangeAngle = -10.0f;

			mb_readyToFire = true;

			m_ammoInterval = 0.0f;

			// 보스 본체 이미지 셋팅용, 공격시에는 그에 맞는 보스 본체이미지랑 히트 이미지로 바꾸고, 히트박스도 그에 따라 생성
			m_bossImg = IMAGE_MANAGER->FindImage(L"Image/Boss/SkellBossAttack.bmp");
			m_bossHitImg = IMAGE_MANAGER->FindImage(L"Image/Boss/SkellBossAttackHit.bmp");
			m_frameX = 0;
			m_elapsedCount = 0.0f;
			SetHitbox();
		}
	}

	if (mb_readyToFire == true)
	{
		// 한번 공격때 미사일 쏘는거 지속시간
		m_ammoAttackDuration += TIMER_MANAGER->GetDeltaTime();
		if (m_ammoAttackDuration >= 1.5f)
		{
			mb_readyToFire = false;
			m_ammoAttackDelay = 0.0f;
			m_ammoAngle = 0.0f;
			--m_totalAmmoAttackCount;

			m_ammoAttackDuration = 0.0f;

			// 보스 본체 이미지 셋팅용, 공격이 아닐시에는 다시 Idle로 모두 전환
			m_bossImg = IMAGE_MANAGER->FindImage(L"Image/Boss/SkellBossIdle.bmp");
			m_bossHitImg = IMAGE_MANAGER->FindImage(L"Image/Boss/SkellBossIdleHit.bmp");
			m_frameX = 0;
			m_elapsedCount = 0.0f;
			SetHitbox();
		}

		// 각각의 미사일들간의 발사 간격
		m_ammoAttackDelay += TIMER_MANAGER->GetDeltaTime();
		if (m_ammoAttackDelay >= 0.1f)
		{
			for (size_t i = 0; i < 4; ++i)
			{
				m_ammoManager->MakeAmmo(L"Image/Boss/BossBullet.bmp", m_ammoPos, m_ammoAngle, m_attackDamage, 400, ObjectType::EnemyAttack);
				m_ammoAngle += DEGREE_TO_RADIAN(90);
			}
			m_ammoAngle += DEGREE_TO_RADIAN(m_ammoChangeAngle);

			m_ammoAttackDelay = 0.0f;
		}
	}
}

void Bellial::SetHitbox()
{
	if (mb_fireAmmo == true && mb_readyToFire == true)
	{
		m_shape.left = static_cast<long>(m_pos.x - m_bossImg->GetFrameWidth() * 0.5f + 23);
		m_shape.right = static_cast<long>(m_pos.x + m_bossImg->GetFrameWidth() * 0.5f - 23);
		m_shape.top = static_cast<long>(m_pos.y - m_bossImg->GetFrameHeight() * 0.5f + 27);
		m_shape.bottom = static_cast<long>(m_pos.y + m_bossImg->GetFrameHeight() * 0.5f - 40);
	}
	else
	{
		m_shape.left = static_cast<long>(m_pos.x - m_bodyWidth * 0.5f);
		m_shape.right = static_cast<long>(m_pos.x + m_bodyWidth * 0.5f);
		m_shape.top = static_cast<long>(m_pos.y - m_bodyHeight * 0.5f);
		m_shape.bottom = static_cast<long>(m_pos.y + m_bodyHeight * 0.5f);
	}
	
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

void Bellial::FireLaserbeam()
{
	// 공격다했으면 끝! 리턴
	if (m_totalHandAttackCount <= 0)
	{
		mb_isAttack = false;
		mb_fireLaserbeam = false;
		mb_isLefthandAttack = false;
		m_totalHandAttackCount = 3;
		m_handAttackDelay = 0.0f;
		return;
	}

	// 범위에 플레이어 있으면 레이저 쏠꺼다
	m_handAttackDelay += TIMER_MANAGER->GetDeltaTime();
	if (m_handAttackDelay > 1.0f)
	{
		if ((mb_isLefthandAttack == false &&
			(int)m_leftHandPosY - 20 < (int)m_player->GetPlayerPos().y && (int)m_leftHandPosY + 20 > (int)m_player->GetPlayerPos().y)
			||
			(mb_isLefthandAttack == true &&
				(int)m_rightHandPosY - 20 < (int)m_player->GetPlayerPos().y && (int)m_rightHandPosY + 20 > (int)m_player->GetPlayerPos().y))

		{
			mb_readyToFire = true;
		}
		// 범위에 플레이어 없다면 플레이어 위치로 손 이동
		else
		{
			if (mb_isLefthandAttack == false)	// 왼손 위치 플레이어 추적
			{
				if (m_leftHandPosY > m_player->GetPlayerPos().y)
				{
					m_leftHandPosY -= m_handMoveSpeed;
				}
				else
				{
					m_leftHandPosY += m_handMoveSpeed;
				}
			}
			else // 오른손 위치 플레이어 추적
			{
				if (m_rightHandPosY > m_player->GetPlayerPos().y)
				{
					m_rightHandPosY -= m_handMoveSpeed;
				}
				else
				{
					m_rightHandPosY += m_handMoveSpeed;
				}
			}
		}
	}

	// 왼손 또는 오른손 레이저 발사 셋팅
	if (mb_readyToFire == true)
	{
		if (mb_isLefthandAttack == false)
		{
			me_leftHandStatus = LeftHandStatus::Attack;
			mb_isLefthandAttack = true;
			--m_totalHandAttackCount;
		}
		else
		{
			me_rightHandStatus = RightHandStatus::Attack;
			mb_isLefthandAttack = false;
			--m_totalHandAttackCount;
		}

		m_handAttackDelay = 0.0f;
		mb_readyToFire = false;
	}
}

void Bellial::RenderLeftHand(HDC hdc, LeftHandStatus status)
{
	if (status < LeftHandStatus::Idle || status > LeftHandStatus::End) return;

	m_lefthandElapsedCount += TIMER_MANAGER->GetDeltaTime();
	if (m_lefthandElapsedCount >= m_leftHand[static_cast<int>(status)].totalTime)
	{
		++m_leftHand[static_cast<int>(status)].frameX;
		if (m_leftHand[static_cast<int>(status)].frameX >= m_leftHand[static_cast<int>(status)].maxFrameX)
		{
			m_leftHand[static_cast<int>(status)].frameX = 0;
			
			if (status == LeftHandStatus::Attack)
			{
				me_leftHandStatus = LeftHandStatus::Idle;
			}
		}
		m_lefthandElapsedCount = 0.0f;
	}

	// 왼손 렌더
	m_leftHand[static_cast<int>(status)].handImg->Render(hdc, static_cast<int>(BOSS_LEFTHAND_POSX), static_cast<int>(m_leftHandPosY), 
														m_leftHand[static_cast<int>(status)].frameX, m_leftHand[static_cast<int>(status)].frameY, 1.0f);

	// 왼손 공격상태일 때, 그 이미지 프레임에 맞춰 왼손 레이저 머리 및 몸통 이미지 렌더
	RenderLeftHandLaser(hdc);
}

void Bellial::RenderRightHand(HDC hdc, RightHandStatus status)
{
	if (status < RightHandStatus::Idle || status > RightHandStatus::End) return;

	m_righthandElapsedCount += TIMER_MANAGER->GetDeltaTime();
	if (m_righthandElapsedCount >= m_rightHand[static_cast<int>(status)].totalTime)
	{
		++m_rightHand[static_cast<int>(status)].frameX;
		if (m_rightHand[static_cast<int>(status)].frameX >= m_rightHand[static_cast<int>(status)].maxFrameX)
		{
			m_rightHand[static_cast<int>(status)].frameX = 0;

			if (status == RightHandStatus::Attack)
			{
				me_rightHandStatus = RightHandStatus::Idle;
			}
		}
		m_righthandElapsedCount = 0.0f;
	}

	m_rightHand[static_cast<int>(status)].handImg->Render(hdc, static_cast<int>(BOSS_RIGHTHAND_POSX), static_cast<int>(m_rightHandPosY),
														m_rightHand[static_cast<int>(status)].frameX, m_rightHand[static_cast<int>(status)].frameY, 1.0f);

	// 오른손 공격상태일 때, 그 이미지 프레임에 맞춰 오른손 레이저 머리 및 몸통 이미지 렌더
	RenderRightHandLaser(hdc);
}

void Bellial::RenderLeftHandLaser(HDC hdc)
{
	if (m_leftHand[static_cast<int>(LeftHandStatus::Attack)].frameX == 9)
	{
		mb_leftLaserAttack = true;
	}

	if (mb_leftLaserAttack == true)
	{
		m_leftLaserElapsedCount += TIMER_MANAGER->GetDeltaTime();
		if (m_leftLaserElapsedCount > 0.07f)
		{
			++m_leftLaserFrameX;
			if (m_leftLaserFrameX >= m_LaserMaxFrameX)
			{
				m_leftLaserFrameX = 0;
				mb_leftLaserAttack = false;
			}
			m_leftLaserElapsedCount = 0.0f;
		}
	}

	if (mb_leftLaserAttack == true)
	{
		// 왼손 레이저 히트박스 관련부분
		SetLeftLaserHitbox(hdc);

		m_laserHead->Render(hdc, static_cast<int>(BOSS_LEFT_LASER_POSX), static_cast<int>(BOSS_LEFT_LASER_POSY), m_leftLaserFrameX, 0, 1.0f);

		// 레이저 몸통 길이 관련 부분 - 몸통 이미지를 화면에 닿을때까지 그려야하므로
		float LeftLaserBodyPosX = BOSS_LEFT_LASER_POSX + m_laserHead->GetFrameWidth() * 0.5f;
		float LeftLaserBodyPosY = BOSS_LEFT_LASER_POSY - m_laserHead->GetFrameHeight() * 0.5f - 11;
		while (LeftLaserBodyPosX <= WIN_SIZE_X)
		{
			m_laserBody->RenderBasic(hdc, static_cast<int>(LeftLaserBodyPosX), static_cast<int>(LeftLaserBodyPosY), m_leftLaserFrameX, 0);
			LeftLaserBodyPosX += m_laserBody->GetFrameWidth();
		}
	}
}

void Bellial::RenderRightHandLaser(HDC hdc)
{
	if (m_rightHand[static_cast<int>(LeftHandStatus::Attack)].frameX == 9)
	{
		mb_rightLaserAttack = true;
	}

	if (mb_rightLaserAttack == true)
	{
		m_rightLaserElapsedCount += TIMER_MANAGER->GetDeltaTime();
		if (m_rightLaserElapsedCount > 0.07f)
		{
			++m_rightLaserFrameX;
			--m_rightLaserBodyFrameX;

			if (m_rightLaserFrameX >= m_LaserMaxFrameX)
			{
				m_rightLaserFrameX = 0;
				mb_rightLaserAttack = false;

				m_rightLaserBodyFrameX = m_LaserMaxFrameX - 1;
			}
			m_rightLaserElapsedCount = 0.0f;
		}
	}

	if (mb_rightLaserAttack == true)
	{
		// 오른손 레이저 히트박스 관련부분
		SetRightLaserHitbox(hdc);

		m_laserHead->Render(hdc, static_cast<int>(BOSS_RIGHT_LASER_POSX), static_cast<int>(BOSS_RIGHT_LASER_POSY), m_rightLaserFrameX, 1, 1.0f);

		// 레이저 몸통 길이 관련 부분 - 몸통 이미지를 화면에 닿을때까지 그려야하므로
		float rightLaserBodyPosX = BOSS_RIGHT_LASER_POSX - m_laserHead->GetFrameWidth() - 55;
		float rightLaserBodyPosY = BOSS_RIGHT_LASER_POSY - m_laserHead->GetFrameHeight() * 0.5f - 13;

		while (rightLaserBodyPosX + m_laserBody->GetFrameWidth() >= 0)
		{
			m_laserBody->RenderBasic(hdc, static_cast<int>(rightLaserBodyPosX), static_cast<int>(rightLaserBodyPosY), m_rightLaserBodyFrameX, 1);
			rightLaserBodyPosX -= m_laserBody->GetFrameWidth();
		}
	}
}

void Bellial::SetLeftLaserHitbox(HDC hdc)
{
	if (m_leftLaserFrameX >= 0 && m_leftLaserFrameX < 3)
	{
		m_laserHitbox.left = static_cast<long>(BOSS_LEFT_LASER_POSX - m_laserHead->GetFrameWidth() * 0.5f + 9);
		m_laserHitbox.top = static_cast<long>(BOSS_LEFT_LASER_POSY - m_laserHead->GetFrameHeight() * 0.5f + 5);
		m_laserHitbox.right = static_cast<long>(WIN_SIZE_X);
		m_laserHitbox.bottom = static_cast<long>(BOSS_LEFT_LASER_POSY + m_laserHead->GetFrameHeight() * 0.5f - 5);

		//Rectangle(hdc, m_laserHitbox.left, m_laserHitbox.top, m_laserHitbox.right, m_laserHitbox.bottom);

		// 레이저 콜라이더
		if (m_laserCollider == nullptr)
		{
			m_laserCollider = ColliderManager::CreateCollider(this, m_laserHitbox, ObjectType::EnemyAttack);
		}
		else
		{
			m_laserCollider->Update(m_laserHitbox);
		}
	}
	else
	{
		m_laserHitbox.left = 0;
		m_laserHitbox.top = 0;
		m_laserHitbox.right = 0;
		m_laserHitbox.bottom = 0;

		// 레이저 콜라이더
		if (m_laserCollider == nullptr)
		{
			m_laserCollider = ColliderManager::CreateCollider(this, m_laserHitbox, ObjectType::EnemyAttack);
		}
		else
		{
			m_laserCollider->Update(m_laserHitbox);
		}
	}
}

void Bellial::SetRightLaserHitbox(HDC hdc)
{
	if (m_rightLaserFrameX >= 0 && m_rightLaserFrameX < 3)
	{
		m_laserHitbox.left = 0;
		m_laserHitbox.top = static_cast<long>(BOSS_RIGHT_LASER_POSY - m_laserHead->GetFrameHeight() * 0.5f + 5);
		m_laserHitbox.right = static_cast<long>(BOSS_RIGHT_LASER_POSX + m_laserHead->GetFrameWidth() * 0.5f - 9);
		m_laserHitbox.bottom = static_cast<long>(BOSS_RIGHT_LASER_POSY + m_laserHead->GetFrameHeight() * 0.5f - 5);

		//Rectangle(hdc, m_laserHitbox.left, m_laserHitbox.top, m_laserHitbox.right, m_laserHitbox.bottom);
		// 레이저 콜라이더
		if (m_laserCollider == nullptr)
		{
			m_laserCollider = ColliderManager::CreateCollider(this, m_laserHitbox, ObjectType::EnemyAttack);
		}
		else
		{
			m_laserCollider->Update(m_laserHitbox);
		}
	}
	else
	{
		m_laserHitbox.left = 0;
		m_laserHitbox.top = 0;
		m_laserHitbox.right = 0;
		m_laserHitbox.bottom = 0;

		// 레이저 콜라이더
		if (m_laserCollider == nullptr)
		{
			m_laserCollider = ColliderManager::CreateCollider(this, m_laserHitbox, ObjectType::EnemyAttack);
		}
		else
		{
			m_laserCollider->Update(m_laserHitbox);
		}
	}
}
