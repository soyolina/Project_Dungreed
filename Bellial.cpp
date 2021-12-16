#include "stdafx.h"
#include "Bellial.h"
#include "Image.h"
#include "Player.h"


HRESULT Bellial::Init()
{
	// - ���� �ִϸ��̼� ��
	m_bossIdleImg = IMAGE_MANAGER->FindImage(L"Image/Boss/SkellBossIdle.bmp"); // ���� ��ü
	m_bossBackImg = IMAGE_MANAGER->FindImage(L"Image/Boss/SkellBossBack.bmp"); // ���� ��ü �� ū �߾� ����
	
	// ���� ��ü �� ���� ������
	SetParticleImgData();

	// ��ü idle , �߾ӱ��� �ִϸ��̼��� �������� 
	m_frameX = 0;
	m_frameY = 0;
	m_maxFrameX = 10;
	m_elapsedCount = 0.0f;


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

	// Attack Delay
	mb_isAttack = false;
	m_attackDelay = 0.0f;
	mb_fireLaserbeam = false;

	// - ��ӹ��� �� - GameObject����
	// ��Ʈ�ڽ� ���ÿ�
	m_pos = { BOSS_HITBOX_POSX, BOSS_HITBOX_POSY };
	m_bodyWidth = m_bossIdleImg->GetFrameWidth() - 46;
	m_bodyHeight = m_bossIdleImg->GetFrameHeight() - 66;
	
	SetHitbox();

	m_hp = 100;
	m_moveSpeed = 0.0f;
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
	// ��ü, �߾ӱ��� �ִϸ��̼�
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

	// �������� ��ƼŬ �ִϸ��̼�
	UpdateParticleAnimation();

	// ���� ������
	if (mb_isAttack == false)
	{
		m_attackDelay += TIMER_MANAGER->GetDeltaTime();
		if (m_attackDelay >= 2.0f)
		{
			int randomValue = rand() % 3;
			switch (0/*randomValue*/)
			{
			case 0:
				mb_isAttack = true;
				mb_fireLaserbeam = true;
				break;
			case 1:
				break;
			case 2:
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
	

	// HP GAGE �ٴ��� �׽�Ʈ��
	if (Input::GetButtonDown(VK_F4))
	{
		m_hp -= 5;
	}
}

void Bellial::Render(HDC hdc)
{
	// ��Ʈ�ڽ�
	Rectangle(hdc, m_shape.left, m_shape.top, m_shape.right, m_shape.bottom);

	// - ����
	// ��������
	for (int i = 0; i < 7; ++i)
	{
		if (m_particleArr[i].mb_isAnimated == true)
		{
			m_particleArr[i].m_bossParticleImg->Render(hdc, m_particleArr[i].m_particlePos.x, m_particleArr[i].m_particlePos.y, 
													m_particleArr[i].m_particleFrameX, m_frameY, 1.0f);
		}
	}

	// ū����
	m_bossBackImg->Render(hdc, static_cast<int>(BOSSBACK_POSX), static_cast<int>(BOSSBACK_POSY), m_frameX, m_frameY, 1.0f);
	
	// ���� IDLE
	m_bossIdleImg->Render(hdc, static_cast<int>(BOSS_POSX), static_cast<int>(BOSS_POSY), m_frameX, m_frameY, 1.0f);
	
	// �޼�
	RenderLeftHand(hdc, me_leftHandStatus);
	// ������
	RenderRightHand(hdc, me_rightHandStatus);

	// Boss HP UI ������
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

	// �޼� ����
	m_leftHand[static_cast<int>(status)].handImg->Render(hdc, static_cast<int>(BOSS_LEFTHAND_POSX), static_cast<int>(m_leftHandPosY), 
														m_leftHand[static_cast<int>(status)].frameX, m_leftHand[static_cast<int>(status)].frameY, 1.0f);

	// �޼� ���ݻ����� ��, �� �̹��� �����ӿ� ���� �޼� ������ �Ӹ� �� ���� �̹��� ����
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

	// ������ ���ݻ����� ��, �� �̹��� �����ӿ� ���� ������ ������ �Ӹ� �� ���� �̹��� ����
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

		Rectangle(hdc, m_laserHitbox.left, m_laserHitbox.top, m_laserHitbox.right, m_laserHitbox.bottom);
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

		Rectangle(hdc, m_laserHitbox.left, m_laserHitbox.top, m_laserHitbox.right, m_laserHitbox.bottom);
	}
}
