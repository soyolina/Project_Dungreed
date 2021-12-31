#include "stdafx.h"
#include "Bellial.h"
#include "Image.h"
#include "Player.h"
#include "AmmoManager.h"
#include "Collider2.h"
#include <cmath>


HRESULT Bellial::Init()
{
	// - ���� �ִϸ��̼� ��
	m_bossImg = IMAGE_MANAGER->FindImage(L"Image/Boss/SkellBossIdle.bmp"); // ���� ��ü
	m_bossHitImg = IMAGE_MANAGER->FindImage(L"Image/Boss/SkellBossIdleHit.bmp"); // ���� �ǰ�
	m_bossBackImg = IMAGE_MANAGER->FindImage(L"Image/Boss/SkellBossBack.bmp"); // ���� ��ü �� ū �߾� ����
	
	// ��ü idle , �߾ӱ��� �ִϸ��̼��� �������� 
	m_frameX = 0;
	m_frameY = 0;
	m_maxFrameX = 10;
	m_elapsedCount = 0.0f;
	
	// - ���� ������ ���� �̹���
	m_bossUpperImg = IMAGE_MANAGER->FindImage(L"Image/Boss/SkellBossDead.bmp");
	m_bossLowerImg = IMAGE_MANAGER->FindImage(L"Image/Boss/SkellBossDead0.bmp");
	mb_BossDieImgRender = false;

	for (size_t i = 0; i < 60; ++i)
	{
		m_dieEffect[i].dieEffectImg = IMAGE_MANAGER->FindImage(L"Image/Boss/DieEffect.bmp");
		POINTFLOAT randomValue = {};
		randomValue.x = static_cast<float>(rand() % (WIN_SIZE_X * 8 / 10)) + (WIN_SIZE_X * 0.1f);
		randomValue.y = static_cast<float>(rand() % (WIN_SIZE_Y));

		m_dieEffect[i].pos = { randomValue.x, randomValue.y };
		m_dieEffect[i].angle = DEGREE_TO_RADIAN(30 * i);
		m_dieEffect[i].frameX = 0;
		m_dieEffect[i].frameY = 0;
		m_dieEffect[i].maxFrameX = 11;
		m_dieEffect[i].elapsedCount = 0.0f;
		m_dieEffect[i].isImgRender = false;
	}
	m_fireworkTimer = 0.0f;
	mb_firstFirework = true;
	mb_secondFirework = false;

	// �ι�° �Ҳɳ��� ���� �������� ����
	m_fireworkFirstRange = 0;
	m_fireworkSecondRange = m_fireworkFirstRange + 12;
	m_fireworkRangeElapsedCount = 0.0f;


	// - ���� ��ü �� ���� ������
	SetParticleImgData();


	// - �� (��, ��)
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


	// �̻��� ��� ���� ��
	AmmoManager* m_ammoManager = nullptr;
	m_ammoPos = { BOSS_AMMO_POSX , BOSS_AMMO_POSY };
	m_ammoInterval = 0.0f;
	m_ammoAttackDelay = 0.0f;
	m_ammoAttackDuration = 0.0f;
	m_ammoAngle = 0.0f;
	m_ammoChangeAngle = 0.0f;
	m_totalAmmoAttackCount = 3;


	// Į ��� ���Ͽ�
	m_swordVec.resize(5);
	for (size_t i = 0; i < m_swordVec.size(); ++i)
	{
		m_swordVec[i].swordImg = IMAGE_MANAGER->FindImage(L"Image/Boss/SkellBossSword0.bmp");
		m_swordVec[i].swordPos = { BOSS_SWORD_POSX + (i * SWORD_GAP), BOSS_SWORD_POSY };
		m_swordVec[i].swordAngle = 0.0f;
		m_swordVec[i].swordStatus = SwordStatus::End;

		m_swordVec[i].swordElapsedCount = 0.0f;
		m_swordVec[i].swordAttackCount = 2;

		// ��Ʈ�ڽ� , �ݶ��̴� ����
		m_swordVec[i].swordHitboxPos = {};
		m_swordVec[i].swordHitBox = {};
		m_swordVec[i].swordCollider = ColliderManager::CreateCollider(this, m_swordVec[i].swordHitBox, ObjectType::EnemyAttack);

		// ����Ʈ ����
		m_swordVec[i].m_swordEffectImg = IMAGE_MANAGER->FindImage(L"Image/Boss/destroyEffect.bmp");;
		m_swordVec[i].m_swordEffectFrameX = 0;
		m_swordVec[i].m_swordEffectMaxFrameX = 3;
		m_swordVec[i].m_swordEffectFrameY = 0;
		m_swordVec[i].m_swordEffectElapsedCount = 0.0f;
		m_swordVec[i].m_swordEffectImgScale = 1.0f;
	}
	m_swordMoveSpeed = 1600.0f;
	m_swordEndAttackCount = 2;


	// Attack Delay
	mb_isAttack = false;
	m_attackDelay = 0.0f;
	mb_fireLaserbeam = false;
	mb_fireAmmo = false;
	mb_fireSword = false;

	// - ��ӹ��� �� - GameObject����
	// ��Ʈ�ڽ� ���ÿ�
	m_pos = { BOSS_HITBOX_POSX, BOSS_HITBOX_POSY };
	m_bodyWidth = m_bossImg->GetFrameWidth() - 46;
	m_bodyHeight = m_bossImg->GetFrameHeight() - 66;

	SetHitbox();
	// �ݶ��̴�
	m_collider = ColliderManager::CreateCollider(this, m_shape, ObjectType::Enemy);
	m_laserCollider = ColliderManager::CreateCollider(this, m_laserHitbox, ObjectType::EnemyAttack);

	m_hp = 100;
	m_moveSpeed = 0.0f;
	m_attackDamage = 0;
	mb_isHit = false;
	mb_isHit2 = false;
	m_hitElapsedCount = 0.0f;
	mb_isDead = false;
	// - �������


	// - ���� HP UI ����
	m_bossLifeBack = IMAGE_MANAGER->FindImage(L"Image/Boss/BossLifeBack.bmp");
	m_bossLifeGage = IMAGE_MANAGER->FindImage(L"Image/Boss/BossLifeRed.bmp");
	m_bossLifeBar = IMAGE_MANAGER->FindImage(L"Image/Boss/BossLifeBase.bmp");

	m_bossHpPercentage = m_bossLifeGage->GetWidth() * 0.01f; // 100���� ������ ��

	return S_OK;
}

void Bellial::Update()
{
	// ���� Hp�� 0�����϶�, �������� �̹��� ����Ʈ ���� 
	if (m_hp <= 0)
	{
		// �ݶ��̴� ����
		ColliderManager::DeleteCollider(ObjectType::Enemy, m_collider);
		ColliderManager::DeleteCollider(ObjectType::EnemyAttack, m_laserCollider);
		for (size_t i = 0; i < m_swordVec.size(); ++i)
		{
			ColliderManager::DeleteCollider(ObjectType::EnemyAttack, m_swordVec[i].swordCollider);
		}

		// ���� ������ ����Ʈ �ִϸ��̼� �����
		UpdateTotalDieEffectAnimation();

		// �ι�° �Ҳɳ��� ������ �̹��� ����Ʈ�� ���� �Ǿ��ٸ� ������ ���¸� dead�� �ٲ۴�.
		// ���߿� �ٲ���ҵ�.
		if (m_dieEffect[59].isImgRender == false)
		{
			mb_isDead = true;
			return;
		}
	}


	if (m_hp > 0)
	{
		// �¾��� ��
		if (mb_isHit == true)
		{
			m_hitElapsedCount += TIMER_MANAGER->GetDeltaTime();
			// ��Ʈ ������ ���� �̹��� ������ ����
			if (m_hitElapsedCount > 0.1f)
			{
				mb_isHit2 = false;
			}
			// �����ð��� 
			if (m_hitElapsedCount > 0.3f)
			{
				mb_isHit = false;
				m_hitElapsedCount = 0.0f;
			}
		}

		// ��ü, �߾ӱ��� �ִϸ��̼�
		m_elapsedCount += TIMER_MANAGER->GetDeltaTime();
		if (m_elapsedCount >= 0.07f)
		{
			++m_frameX;
			if (m_frameX >= m_maxFrameX)
			{
				m_frameX = 0;

				// ������ �̻��� �����϶� �̻��� ���ݳ��������� ������ �̹��� ���������� ��� �����صα� ����(�Թ��� ���·�)
				if (mb_fireAmmo == true && mb_readyToFire == true)
				{
					m_frameX = m_maxFrameX - 1;
				}
			}
			m_elapsedCount = 0.0f;
		}

		// �������� ��ƼŬ �ִϸ��̼�
		UpdateParticleAnimation();

		// ���� �����̿� �׿� ���� ���� �������� ����
		if (mb_isAttack == false)
		{
			m_attackDelay += TIMER_MANAGER->GetDeltaTime();
			if (m_attackDelay >= 2.0f)
			{
				int randomValue = rand() % 3;
				switch (randomValue)
				{
				case 0:
					mb_isAttack = true;
					mb_fireLaserbeam = true;
					m_attackDamage = 9;
					break;
				case 1:
					mb_isAttack = true;
					mb_fireAmmo = true;
					m_attackDamage = 6;
					break;
				case 2:
					mb_isAttack = true;
					mb_fireSword = true;
					m_attackDamage = 8;
					break;
				}

				m_attackDelay = 0.0f;
			}
		}

		// ���� ���� 1 : ������ ��
		if (mb_isAttack == true && mb_fireLaserbeam == true)
		{
			FireLaserbeam();
		}

		// �������� 2 : �̻��� 
		if (mb_isAttack == true && mb_fireAmmo == true)
		{
			FireMissile();
		}

		// �������� 3 : Į 
		if (mb_isAttack == true && mb_fireSword == true)
		{
			FireSword();
		}

		// �ݶ��̴�
		m_collider->Update(m_shape); // �ذ� ��ü
	}

}

void Bellial::Render(HDC hdc)
{
	if (mb_BossDieImgRender == true)
	{
		// ���� �����ǿ� ���õ� �� ���� �ʿ�
		m_bossLowerImg->Render(hdc, static_cast<int>(BOSS_POSX) + 20, static_cast<int>(BOSS_POSY) + 110);
		m_bossUpperImg->Render(hdc, static_cast<int>(BOSS_POSX), static_cast<int>(BOSS_POSY));
	}
	else
	{
		// Į ���� ����
		if (mb_isAttack == true && mb_fireSword == true && m_hp > 0)
		{
			RenderSword(hdc);
		}

		// - ����
		// ��������
		if (m_hp > 0)
		{
			for (int i = 0; i < 7; ++i)
			{
				if (m_particleArr[i].mb_isAnimated == true)
				{
					m_particleArr[i].m_bossParticleImg->Render(hdc, static_cast<int>(m_particleArr[i].m_particlePos.x),
						static_cast<int>(m_particleArr[i].m_particlePos.y), m_particleArr[i].m_particleFrameX, m_frameY, 1.0f);
				}
			}
		}

		// ū����
		m_bossBackImg->Render(hdc, static_cast<int>(BOSSBACK_POSX), static_cast<int>(BOSSBACK_POSY), m_frameX, m_frameY, 1.0f);

		// ���� IDLE
		if (mb_isHit2 == true && m_hp > 0)
		{
			m_bossHitImg->Render(hdc, static_cast<int>(BOSS_POSX), static_cast<int>(BOSS_POSY), m_frameX, m_frameY, 1.0f);
		}
		else
		{
			m_bossImg->Render(hdc, static_cast<int>(BOSS_POSX), static_cast<int>(BOSS_POSY), m_frameX, m_frameY, 1.0f);
		}

		// �޼�
		RenderLeftHand(hdc, me_leftHandStatus);
		// ������
		RenderRightHand(hdc, me_rightHandStatus);
	}

	// ���� hp�� 0�����϶� �״� ����Ʈ �̹��� ������
	if (m_hp <= 0)
	{
		RenderDieEffectAnimation(hdc);
	}

	// Boss HP UI ������
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



void Bellial::UpdateFirstDieEffectAnimation()
{
	if (mb_firstFirework == true)
	{
		for (size_t i = 0; i < 60; ++i)
		{
			// �������� ���� �ִϸ��̼� ������ؼ�
			if (m_dieEffect[i].isImgRender == false)
			{
				int randomValue = rand() % 120;
				if (randomValue == 1)
				{
					m_dieEffect[i].isImgRender = true;
				}
			}

			// �ִϸ��̼� ������ �����
			if (m_dieEffect[i].isImgRender == true)
			{
				m_dieEffect[i].elapsedCount += TIMER_MANAGER->GetDeltaTime();
				if (m_dieEffect[i].elapsedCount > 0.07f)
				{
					++m_dieEffect[i].frameX;
					if (m_dieEffect[i].frameX >= m_dieEffect[i].maxFrameX)
					{
						m_dieEffect[i].frameX = 0;
					}
					m_dieEffect[i].elapsedCount = 0.0f;
				}
			}
		}
	}
}

void Bellial::SetFirstDieEffectDuration()
{
	if (mb_secondFirework == false)
	{
		m_fireworkTimer += TIMER_MANAGER->GetDeltaTime();
		if (m_fireworkTimer > 4.5f)
		{
			mb_firstFirework = false;
			mb_secondFirework = true;

			for (size_t i = 0; i < 60; ++i)
			{
				m_dieEffect[i].pos = { BOSSBACK_POSX , BOSSBACK_POSY };
				m_dieEffect[i].pos.x += cosf(m_dieEffect[i].angle) * ((i / 12 + 1) * 70);
				m_dieEffect[i].pos.y += sinf(m_dieEffect[i].angle) * ((i / 12 + 1) * 70);

				m_dieEffect[i].frameX = 0;
				m_dieEffect[i].elapsedCount = 0.0f;
				m_dieEffect[i].isImgRender = false;
			}
			m_fireworkTimer = 0.0f;
		}
	}
}

void Bellial::UpdateSecondDieEffectAnimation()
{
	if (mb_secondFirework == true)
	{
		// �ι�° �Ҳɳ��̰� �߽����κ��� �Ѵܰ�� ���������� ���� dieEffect �迭 �̹��� ���� ���� �������ؼ�
		if (m_fireworkSecondRange < 60)
		{
			m_fireworkRangeElapsedCount += TIMER_MANAGER->GetDeltaTime();
			if (m_fireworkRangeElapsedCount > 0.3f)
			{
				for (size_t i = m_fireworkFirstRange; i < m_fireworkSecondRange; ++i)
				{
					m_dieEffect[i].isImgRender = true;
				}
				m_fireworkFirstRange += 12;
				m_fireworkSecondRange = m_fireworkFirstRange + 12;

				// �̶� ������ �ذ� ����� �ٲ��ֱ� ���ؼ� ������ ��, �Ʒ��� ���� �ذ�������
				mb_BossDieImgRender = true;

				m_fireworkRangeElapsedCount = 0.0f;
			}
		}

		// �ִϸ��̼� ������ �����
		for (size_t i = 0; i < 60; ++i)
		{
			if (m_dieEffect[i].isImgRender == true)
			{
				m_dieEffect[i].elapsedCount += TIMER_MANAGER->GetDeltaTime();
				if (m_dieEffect[i].elapsedCount > 0.07f)
				{
					++m_dieEffect[i].frameX;
					if (m_dieEffect[i].frameX >= m_dieEffect[i].maxFrameX)
					{
						m_dieEffect[i].frameX = 0;
						m_dieEffect[i].isImgRender = false;
					}
					m_dieEffect[i].elapsedCount = 0.0f;
				}
			}
		}
	}
}

void Bellial::UpdateTotalDieEffectAnimation()
{
	// ���� ������ ù���� �Ҳɳ���
	UpdateFirstDieEffectAnimation();

	// ù��° ���� �ִϸ��̼� ����ð��� �� �ð��� ������ ù��° ���� ������, �ι�° ������ ���� ������ ����
	SetFirstDieEffectDuration();

	// �ι�° �Ҳɳ���
	UpdateSecondDieEffectAnimation();
}

void Bellial::RenderDieEffectAnimation(HDC hdc)
{
	for (size_t i = 0; i < 60; ++i)
	{
		// ���� �� ù��° �̹��� ����Ʈ ����(������ ��ġ �Ҳɳ���)
		if (m_dieEffect[i].isImgRender == true && mb_firstFirework == true)
		{
			m_dieEffect[i].dieEffectImg->Render(hdc, static_cast<int>(m_dieEffect[i].pos.x),
				static_cast<int>(m_dieEffect[i].pos.y), m_dieEffect[i].frameX, m_dieEffect[i].frameY, 1.0f);
		}
		// ���� �� �ι�° �̹��� ����Ʈ ����(���� �߽����� ����Ʈ�� ���������� ��)
		if (m_dieEffect[i].isImgRender == true && mb_secondFirework == true)
		{
			m_dieEffect[i].dieEffectImg->Render(hdc, static_cast<int>(m_dieEffect[i].pos.x),
				static_cast<int>(m_dieEffect[i].pos.y), m_dieEffect[i].frameX, m_dieEffect[i].frameY, 1.0f);
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
				// pos ��ġ ���� - �̹��� ���� ��ġ ���� (�̵��ϴ� ��ó��)
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
	// ���ݴ������� ��! ����
	if (m_totalHandAttackCount <= 0)
	{
		mb_isAttack = false;
		mb_fireLaserbeam = false;
		mb_isLefthandAttack = false;
		m_totalHandAttackCount = 3;
		m_handAttackDelay = 0.0f;
		return;
	}

	// ������ �÷��̾� ������ ������ �򲨴�
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
		// ������ �÷��̾� ���ٸ� �÷��̾� ��ġ�� �� �̵�
		else
		{
			if (mb_isLefthandAttack == false)	// �޼� ��ġ �÷��̾� ����
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
			else // ������ ��ġ �÷��̾� ����
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

	// �޼� �Ǵ� ������ ������ �߻� ����
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

	if (m_hp > 0)
	{
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
	}


	// �޼� ����
	m_leftHand[static_cast<int>(status)].handImg->Render(hdc, static_cast<int>(BOSS_LEFTHAND_POSX), static_cast<int>(m_leftHandPosY),
		m_leftHand[static_cast<int>(status)].frameX, m_leftHand[static_cast<int>(status)].frameY, 1.0f);

	// �޼� ���ݻ����� ��, �� �̹��� �����ӿ� ���� �޼� ������ �Ӹ� �� ���� �̹��� ����
	if (m_hp > 0)
	{
		RenderLeftHandLaser(hdc);
	}
}

void Bellial::RenderRightHand(HDC hdc, RightHandStatus status)
{
	if (status < RightHandStatus::Idle || status > RightHandStatus::End) return;

	if (m_hp > 0)
	{
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
	}


	m_rightHand[static_cast<int>(status)].handImg->Render(hdc, static_cast<int>(BOSS_RIGHTHAND_POSX), static_cast<int>(m_rightHandPosY),
		m_rightHand[static_cast<int>(status)].frameX, m_rightHand[static_cast<int>(status)].frameY, 1.0f);

	// ������ ���ݻ����� ��, �� �̹��� �����ӿ� ���� ������ ������ �Ӹ� �� ���� �̹��� ����
	if (m_hp > 0)
	{
		RenderRightHandLaser(hdc);
	}
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
		// �޼� ������ ��Ʈ�ڽ� ���úκ�
		SetLeftLaserHitbox(hdc);

		m_laserHead->Render(hdc, static_cast<int>(BOSS_LEFT_LASER_POSX), static_cast<int>(BOSS_LEFT_LASER_POSY), m_leftLaserFrameX, 0, 1.0f);

		// ������ ���� ���� ���� �κ� - ���� �̹����� ȭ�鿡 ���������� �׷����ϹǷ�
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
		// ������ ������ ��Ʈ�ڽ� ���úκ�
		SetRightLaserHitbox(hdc);

		m_laserHead->Render(hdc, static_cast<int>(BOSS_RIGHT_LASER_POSX), static_cast<int>(BOSS_RIGHT_LASER_POSY), m_rightLaserFrameX, 1, 1.0f);

		// ������ ���� ���� ���� �κ� - ���� �̹����� ȭ�鿡 ���������� �׷����ϹǷ�
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

		// ������ �ݶ��̴�
		m_laserCollider->Update(m_laserHitbox);
	}
	else
	{
		m_laserHitbox.left = 0;
		m_laserHitbox.top = 0;
		m_laserHitbox.right = 0;
		m_laserHitbox.bottom = 0;

		// ������ �ݶ��̴�
		m_laserCollider->Update(m_laserHitbox);
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
		// ������ �ݶ��̴�
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

		// ������ �ݶ��̴�
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


void Bellial::FireMissile()
{
	// ����Ƚ�� �� ������ ����
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

		// ���� ��ü �̹��� ���ÿ�, ������ �������� �ٽ� Idle�� ��� ��ȯ
		m_bossImg = IMAGE_MANAGER->FindImage(L"Image/Boss/SkellBossIdle.bmp");
		m_bossHitImg = IMAGE_MANAGER->FindImage(L"Image/Boss/SkellBossIdleHit.bmp");
		m_frameX = 0;
		m_elapsedCount = 0.0f;
		SetHitbox();

		return;
	}

	// �̻��� ����Ƚ������ �߻� ����
	if (mb_readyToFire == false)
	{
		m_ammoInterval += TIMER_MANAGER->GetDeltaTime();
		if (m_ammoInterval >= 1.5f && m_totalAmmoAttackCount > 0)
		{
			int randomValue = rand() % 2;
			randomValue ? m_ammoChangeAngle = 10.0f : m_ammoChangeAngle = -10.0f;

			mb_readyToFire = true;

			m_ammoInterval = 0.0f;

			// ���� ��ü �̹��� ���ÿ�, ���ݽÿ��� �׿� �´� ���� ��ü�̹����� ��Ʈ �̹����� �ٲٰ�, ��Ʈ�ڽ��� �׿� ���� ����
			m_bossImg = IMAGE_MANAGER->FindImage(L"Image/Boss/SkellBossAttack.bmp");
			m_bossHitImg = IMAGE_MANAGER->FindImage(L"Image/Boss/SkellBossAttackHit.bmp");
			m_frameX = 0;
			m_elapsedCount = 0.0f;
			SetHitbox();
		}
	}

	if (mb_readyToFire == true)
	{
		// �ѹ� ���ݶ� �̻��� ��°� ���ӽð�
		m_ammoAttackDuration += TIMER_MANAGER->GetDeltaTime();
		if (m_ammoAttackDuration >= 1.5f)
		{
			mb_readyToFire = false;
			m_ammoAttackDelay = 0.0f;
			m_ammoAngle = 0.0f;
			--m_totalAmmoAttackCount;

			m_ammoAttackDuration = 0.0f;

			// ���� ��ü �̹��� ���ÿ�, ������ �ƴҽÿ��� �ٽ� Idle�� ��� ��ȯ
			m_bossImg = IMAGE_MANAGER->FindImage(L"Image/Boss/SkellBossIdle.bmp");
			m_bossHitImg = IMAGE_MANAGER->FindImage(L"Image/Boss/SkellBossIdleHit.bmp");
			m_frameX = 0;
			m_elapsedCount = 0.0f;
			SetHitbox();
		}

		// ������ �̻��ϵ鰣�� �߻� ����
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


void Bellial::SetSwordHitbox(int index, float angle)
{
	if (m_swordVec[index].swordStatus == SwordStatus::Fire)
	{
		m_swordVec[index].swordHitboxPos.x = m_swordVec[index].swordPos.x - 120;
		m_swordVec[index].swordHitboxPos.y = m_swordVec[index].swordPos.y;


		m_swordVec[index].swordHitboxPos = { (m_swordVec[index].swordPos.x +
			(m_swordVec[index].swordHitboxPos.x - m_swordVec[index].swordPos.x) * cosf(angle)
			- (m_swordVec[index].swordHitboxPos.y - m_swordVec[index].swordPos.y) * sinf(angle)),

		(m_swordVec[index].swordPos.y +
			(m_swordVec[index].swordHitboxPos.x - m_swordVec[index].swordPos.x) * sinf(angle)
			+ (m_swordVec[index].swordHitboxPos.y - m_swordVec[index].swordPos.y) * cosf(angle))
		};

		m_swordVec[index].swordHitBox.left = static_cast<long>(m_swordVec[index].swordHitboxPos.x - 18);
		m_swordVec[index].swordHitBox.right = static_cast<long>(m_swordVec[index].swordHitboxPos.x + 18);
		m_swordVec[index].swordHitBox.top = static_cast<long>(m_swordVec[index].swordHitboxPos.y - 18);
		m_swordVec[index].swordHitBox.bottom = static_cast<long>(m_swordVec[index].swordHitboxPos.y + 18);

		//�ݶ��̴�
		m_swordVec[index].swordCollider->Update(m_swordVec[index].swordHitBox);
	}
	else
	{
		m_swordVec[index].swordHitBox.left = 0;
		m_swordVec[index].swordHitBox.right = 0;
		m_swordVec[index].swordHitBox.top = 0;
		m_swordVec[index].swordHitBox.bottom = 0;

		// �ݶ��̴�
		m_swordVec[index].swordCollider->Update(m_swordVec[index].swordHitBox);
	}
	/*m_leftTopPoint = { (LONG)(m_swordVec[index].swordPos.x +
		(float)(m_swordVec[index].swordHitBox.left - m_swordVec[index].swordPos.x) * cosf(angle)
		- (float)(m_swordVec[index].swordHitBox.top - m_swordVec[index].swordPos.y) * sinf(angle)),

		(LONG)(m_swordVec[index].swordPos.y +
		(float)(m_swordVec[index].swordHitBox.left - m_swordVec[index].swordPos.x) * sinf(angle)
		+ (float)(m_swordVec[index].swordHitBox.top - m_swordVec[index].swordPos.y) * cosf(angle)) };


	m_rightTopPoint = { (LONG)(m_swordVec[index].swordPos.x +
		(float)(m_swordVec[index].swordHitBox.right - m_swordVec[index].swordPos.x) * cosf(angle)
		- (float)(m_swordVec[index].swordHitBox.top - m_swordVec[index].swordPos.y) * sinf(angle)),

		(LONG)(m_swordVec[index].swordPos.y +
		(float)(m_swordVec[index].swordHitBox.right - m_swordVec[index].swordPos.x) * sinf(angle)
		+ (float)(m_swordVec[index].swordHitBox.top - m_swordVec[index].swordPos.y) * cosf(angle)) };


	m_leftBottomPoint = { (LONG)(m_swordVec[index].swordPos.x +
		(float)(m_swordVec[index].swordHitBox.left - m_swordVec[index].swordPos.x) * cosf(angle)
		- (float)(m_swordVec[index].swordHitBox.bottom - m_swordVec[index].swordPos.y) * sinf(angle)),

		(LONG)(m_swordVec[index].swordPos.y +
		(float)(m_swordVec[index].swordHitBox.left - m_swordVec[index].swordPos.x) * sinf(angle)
		+ (float)(m_swordVec[index].swordHitBox.bottom - m_swordVec[index].swordPos.y) * cosf(angle)) };*/
}

void Bellial::CheckSwordPatternEnd()
{
	// Į���� ����Ƚ���� �������� üũ�ϰ� Count�� ������
	for (size_t i = 0; i < m_swordVec.size(); ++i)
	{
		if (m_swordVec[i].swordAttackCount == 0)
		{
			++m_swordEndAttackCount;
		}
	}

	// ��� Į�� ����Ƚ���� �������� ������ Sword ���� ������ ��������. ���� return
	if (m_swordEndAttackCount == 5)
	{
		mb_isAttack = false;
		mb_fireSword = false;
		m_swordEndAttackCount = 0;

		for (size_t i = 0; i < m_swordVec.size(); ++i)
		{
			m_swordVec[i].swordAttackCount = 2;
		}

		return;
	}
	else
	{
		m_swordEndAttackCount = 0;
	}
}

void Bellial::CreateSword()
{
	for (size_t i = 0; i < m_swordVec.size(); ++i)
	{
		if (m_swordVec[i].swordStatus == SwordStatus::End && m_swordVec[i].swordAttackCount > 0)
		{
			m_swordVec[i].swordElapsedCount += TIMER_MANAGER->GetDeltaTime();
			if (m_swordVec[i].swordElapsedCount >= (1.0f + i * 0.2f))
			{
				m_swordVec[i].swordPos = { BOSS_SWORD_POSX + (i * SWORD_GAP), BOSS_SWORD_POSY };
				m_swordVec[i].swordStatus = SwordStatus::Create;
				m_swordVec[i].swordElapsedCount = 0.0f;
			}
		}
	}
}

void Bellial::ActUponSwordStatus()
{
	for (size_t i = 0; i < m_swordVec.size(); ++i)
	{
		switch (m_swordVec[i].swordStatus)
		{
		case Bellial::SwordStatus::Create:
			m_swordVec[i].m_swordEffectImg = IMAGE_MANAGER->FindImage(L"Image/Boss/destroyEffect.bmp");

			// �÷��̾�� �� ���� ���
			m_swordVec[i].swordAngle = -GetAngle2(m_player->GetPlayerPos(), m_swordVec[i].swordPos);

			// ����Ʈ �̹��� �����ӿ�
			m_swordVec[i].m_swordEffectElapsedCount += TIMER_MANAGER->GetDeltaTime();
			if (m_swordVec[i].m_swordEffectElapsedCount > 0.07f)
			{
				++m_swordVec[i].m_swordEffectFrameX;
				if (m_swordVec[i].m_swordEffectFrameX >= m_swordVec[i].m_swordEffectMaxFrameX)
				{
					m_swordVec[i].m_swordEffectImg = IMAGE_MANAGER->FindImage(L"Image/Boss/BossSwordHitDown.bmp");
					m_swordVec[i].m_swordEffectFrameX = 0;
					m_swordVec[i].m_swordEffectMaxFrameX = 5;
					m_swordVec[i].m_swordEffectElapsedCount = 0.0f;
					m_swordVec[i].m_swordEffectImgScale = 2.0f;

					// ����Ʈ �̹��� �ִϸ��̼� ������ Į ���¸� Idle�� ��ȯ
					m_swordVec[i].swordStatus = SwordStatus::Idle;
				}
				m_swordVec[i].m_swordEffectElapsedCount = 0.0f;
			}
			break;

		case Bellial::SwordStatus::Idle:
			m_swordVec[i].swordAngle = -GetAngle2(m_player->GetPlayerPos(), m_swordVec[i].swordPos);

			// ��Ʈ�ڽ��� �ݶ��̴� ���� �� ������Ʈ 
			SetSwordHitbox(i, m_swordVec[i].swordAngle);

			// ���� �ð� ���� �� Į ���¸� Fire�� ��ȯ
			m_swordVec[i].swordElapsedCount += TIMER_MANAGER->GetDeltaTime();
			if (m_swordVec[i].swordElapsedCount > 1.5f)
			{
				m_swordVec[i].swordElapsedCount = 0.0f;
				m_swordVec[i].swordStatus = SwordStatus::Fire;
				m_swordVec[i].swordImg = IMAGE_MANAGER->FindImage(L"Image/Boss/SkellBossSwordFire.bmp");
			}
			break;

		case Bellial::SwordStatus::Fire:
			// �� ������ �� ��ġ �̵�(�߻�)
			m_swordVec[i].swordPos.x += -cosf(m_swordVec[i].swordAngle) * m_swordMoveSpeed * TIMER_MANAGER->GetDeltaTime();
			m_swordVec[i].swordPos.y += -sinf(m_swordVec[i].swordAngle) * m_swordMoveSpeed * TIMER_MANAGER->GetDeltaTime();

			// ��Ʈ�ڽ��� �ݶ��̴� ���� �� ������Ʈ 
			SetSwordHitbox(i, m_swordVec[i].swordAngle);

			// ���� pos�� ȭ�� ������ ������ Hit���·� ��ȯ
			if (m_swordVec[i].swordPos.x < 0 || m_swordVec[i].swordPos.x > WIN_SIZE_X ||
				m_swordVec[i].swordPos.y < 0 || m_swordVec[i].swordPos.y > WIN_SIZE_Y)
			{
				m_swordVec[i].swordStatus = SwordStatus::Hit;
				m_swordVec[i].swordImg = IMAGE_MANAGER->FindImage(L"Image/Boss/SkellBossSword0.bmp");
			}
			break;

		case Bellial::SwordStatus::Hit:

			// ��Ʈ�ڽ��� �ݶ��̴� ���� �� ������Ʈ 
			SetSwordHitbox(i, m_swordVec[i].swordAngle);

			// ���� ���� ���������� ���� ����Ʈ �̹��� ����
			if (m_swordVec[i].swordPos.x <= 0)
			{
				m_swordVec[i].m_swordEffectImg = IMAGE_MANAGER->FindImage(L"Image/Boss/BossSwordHitLeft.bmp");
			}
			else if (m_swordVec[i].swordPos.x >= WIN_SIZE_X)
			{
				m_swordVec[i].m_swordEffectImg = IMAGE_MANAGER->FindImage(L"Image/Boss/BossSwordHitRight.bmp");
			}
			else if (m_swordVec[i].swordPos.y <= 0)
			{
				m_swordVec[i].m_swordEffectImg = IMAGE_MANAGER->FindImage(L"Image/Boss/BossSwordHitUp.bmp");
			}
			else
			{
				m_swordVec[i].m_swordEffectImg = IMAGE_MANAGER->FindImage(L"Image/Boss/BossSwordHitDown.bmp");
			}

			// ����Ʈ �̹��� �����ӿ�
			m_swordVec[i].m_swordEffectElapsedCount += TIMER_MANAGER->GetDeltaTime();
			if (m_swordVec[i].m_swordEffectElapsedCount > 0.1f)
			{
				++m_swordVec[i].m_swordEffectFrameX;
				if (m_swordVec[i].m_swordEffectFrameX >= m_swordVec[i].m_swordEffectMaxFrameX)
				{
					m_swordVec[i].m_swordEffectImg = IMAGE_MANAGER->FindImage(L"Image/Boss/destroyEffect.bmp");
					m_swordVec[i].m_swordEffectFrameX = 0;
					m_swordVec[i].m_swordEffectMaxFrameX = 3;
					m_swordVec[i].m_swordEffectElapsedCount = 0.0f;
					m_swordVec[i].m_swordEffectImgScale = 1.0f;

					// ����Ʈ �̹��� �ִϸ��̼� ������ Į ���¸� Destroy�� ��ȯ
					m_swordVec[i].swordStatus = SwordStatus::Destroy;
				}
				m_swordVec[i].m_swordEffectElapsedCount = 0.0f;
			}

			break;

		case Bellial::SwordStatus::Destroy:

			// ����Ʈ �ִϸ��̼� ��
			m_swordVec[i].m_swordEffectElapsedCount += TIMER_MANAGER->GetDeltaTime();
			if (m_swordVec[i].m_swordEffectElapsedCount > 0.07f)
			{
				++m_swordVec[i].m_swordEffectFrameX;
				if (m_swordVec[i].m_swordEffectFrameX >= m_swordVec[i].m_swordEffectMaxFrameX)
				{
					m_swordVec[i].m_swordEffectFrameX = 0;
					m_swordVec[i].m_swordEffectElapsedCount = 0.0f;

					// ����Ʈ �̹��� �ִϸ��̼� ������ Į ���¸� End�� ��ȯ
					m_swordVec[i].swordStatus = SwordStatus::End;
					--m_swordVec[i].swordAttackCount;
				}
				m_swordVec[i].m_swordEffectElapsedCount = 0.0f;
			}
			break;

		case Bellial::SwordStatus::End:

			break;
		}
	}
}

void Bellial::FireSword()
{
	// Į ���� ������ �������� �˻�, ���̸� Ż��
	CheckSwordPatternEnd();

	// Į ���ʴ�� �����ϴ� �κ�
	CreateSword();

	// Į�� ���¿� ���� Į���� �ൿ��
	ActUponSwordStatus();
}

void Bellial::RenderSword(HDC hdc)
{
	for (size_t i = 0; i < m_swordVec.size(); ++i)
	{
		// �� ����
		if (m_swordVec[i].swordStatus != SwordStatus::End)
		{
			m_swordVec[i].swordImg->ImgRotateRender(hdc, static_cast<int>(m_swordVec[i].swordPos.x), static_cast<int>(m_swordVec[i].swordPos.y),
				m_swordVec[i].swordAngle, 3.0f);
			/*Rectangle(hdc, m_swordVec[i].swordHitBox.left, m_swordVec[i].swordHitBox.top,
							m_swordVec[i].swordHitBox.right, m_swordVec[i].swordHitBox.bottom);*/

							// ��Ʈ�ڽ� ������ ����ҷ��� ������ �� - IntersectRect�Լ��� �浹üũ�� �� �� ���� ����
							/*MoveToEx(hdc, m_leftTopPoint.x, m_leftTopPoint.y, NULL);
										   LineTo(hdc, m_rightTopPoint.x, m_rightTopPoint.y);
										   LineTo(hdc, m_rightBottomPoint.x, m_rightBottomPoint.y);
										   LineTo(hdc, m_leftBottomPoint.x, m_leftBottomPoint.y);
										   LineTo(hdc, m_leftTopPoint.x, m_leftTopPoint.y);*/
		}

		// �� ����Ʈ ����
		if (m_swordVec[i].swordStatus == SwordStatus::Create || m_swordVec[i].swordStatus == SwordStatus::Hit
			|| m_swordVec[i].swordStatus == SwordStatus::Destroy)
		{
			// Create�� Destroy ���¿��� �̹��� ����Ʈ ������ ������ �ʿ�������, Hit�����ϋ��� ������ 0�̿��ؼ� 
			float angle = m_swordVec[i].swordAngle;
			if (m_swordVec[i].swordStatus == SwordStatus::Hit) { angle = 0.0f; }

			m_swordVec[i].m_swordEffectImg->ImgRotateFrameRender(hdc,
				static_cast<int>(m_swordVec[i].swordPos.x), static_cast<int>(m_swordVec[i].swordPos.y),
				m_swordVec[i].m_swordEffectFrameX, m_swordVec[i].m_swordEffectFrameY, angle,
				m_swordVec[i].m_swordEffectImgScale);
		}
	}
}