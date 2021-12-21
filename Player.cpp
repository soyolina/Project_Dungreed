#include "stdafx.h"
#include "Player.h"
#include "Image.h"
#include "ItemBase.h"
#include "Collider2.h"

HRESULT Player::Init()
{
    // �÷��̾� ��Ʈ�� ���
    m_hitBackGround = IMAGE_MANAGER->FindImage(L"Image/Player/hit.bmp");
    m_hitBackGroundTransparancy = 80;

    // - GameObject���� ��ӹ��� ��
    // ĳ���� ��ġ
    m_pos.x = WIN_SIZE_X / 2;
    m_pos.y = WIN_SIZE_Y / 2;

    // ĳ���� ��Ʈ�ڽ� ����
    m_bodyWidth = 36;
    m_bodyHeight = 38;

    SetShape(m_pos, m_bodyWidth, m_bodyHeight);
    // �ݶ��̴�
    m_collider = ColliderManager::CreateCollider(this, m_shape, ObjectType::Player);

    // ĳ���� ��������
    m_hp = 100;
    m_moveSpeed = 200.0f;
    m_attackDamage = 0;
    mb_isHit = false;
    m_hitElapsedCount = 0.0f;
    mb_isDead = false;
    // - �������

    // �÷��̾ �ٶ󺸰� �ִ� ���� üũ��
    mb_isLeft = false;

    // ĳ���� ����
    me_PlayerStatus = PlayerStatus::Idle;

    // �÷��̾� �̹��� �ִϸ��̼� ������
    m_statusAniData[static_cast<int>(PlayerStatus::Idle)] = { IMAGE_MANAGER->FindImage(L"Image/Player/baseCharIdle.bmp"), 5, 0.1f};
    m_statusAniData[static_cast<int>(PlayerStatus::Run)] = { IMAGE_MANAGER->FindImage(L"Image/Player/baseCharRun.bmp"), 8, 0.1f};
    m_statusAniData[static_cast<int>(PlayerStatus::Dead)] = { IMAGE_MANAGER->FindImage(L"Image/Player/basecharDie.bmp"), 0, 0.0f};

    m_frameX = 0;
    m_frameY = 0;
    m_elapsedCount = 0.0f;
    
    // �÷��̾ �̵��� �߻��ϴ� ���� ����Ʈ ����
    m_runEffectImg = IMAGE_MANAGER->FindImage(L"Image/Player/RunEffect.bmp");
    mb_isMove = false;
    m_dustEffectCount = 0.0f;
    m_dustFrameX = 0;
    m_dustMaxFrameX = 5;

    // ��������
    mb_isJump = false;
    mb_isDownJump = false;

    m_jumpCount = 1;
    m_jumpStrength = 0.0f;
    m_gravity = 1300.0f;
    
    mb_isCollide = false;
    m_beforePlayerBottom = 0;

    // �뽬 ����
    m_dashEffectImg = IMAGE_MANAGER->FindImage(L"Image/Player/baseCharEffect.bmp");
    mb_isDash = false;
    m_angle = 0.0f;
    m_maxDashCount = 2;
    m_dashCount = m_maxDashCount;
    m_dashSpeed = 18.0f;
    m_dashRegenTime = 0.0f;

    m_dashTimer = 0;
    m_dashFrameX = 0;
    m_beforePlayerPos = {};

    // ������ ����
    m_selectedWeaponIndex = 0;
    m_weapon[0] = nullptr;
    m_weapon[1] = nullptr;

    

    // �ӽ� �׽�Ʈ ����
    makeTestRect();
    
    return S_OK;
}

void Player::Update()
{
    // ������, �׾����� ������Ʈ���Ҳ��� - �׷��� ������
    if (m_hp <= 0)
    {
        mb_isDead = true;
        me_PlayerStatus = PlayerStatus::Dead;
        return;
    }

    // �¾�����
    if (mb_isHit == true)
    {
        Hit();
    }

    // ĳ���� ��ġ ���¿� - �׽�Ʈ
    if (Input::GetButton(VK_F2))
    {
        m_pos.x = WIN_SIZE_X / 2;
        m_pos.y = WIN_SIZE_Y / 2;
    }

    // ������ ���õ� �� - ���콺 ��Ŭ���� ���� => Attack()
    if (Input::GetButtonDown(VK_LBUTTON))
    {
        Attack();
    }

    // �뽬 ����Ʈ ���� ���ؼ� ���� ����
    m_beforePlayerPos = m_pos;

    // �浹üũ �� �������� ���� ���� ���� ���� - �÷��̾��� �ذ� �浹�Ǵ� ���� ���κа� �浹���� �ÿ���, �浹 ó�� ������ ���ؼ�
    m_beforePlayerBottom = m_shape.bottom;

    // ��Ŭ���� �뽬 
    if (Input::GetButtonDown(VK_RBUTTON) && m_dashCount > 0)
    {
        mb_isDash = true;
        m_angle = GetAngle(m_pos, Input::GetMousePosition());
        m_jumpStrength = 0.0f;
        --m_dashCount;
    }

    // �뽬 Ƚ�� ȸ��
    RegenDash();

    // ��� ����
    if (mb_isDash == true)
    {
		Dash();
    }
    // �뽬���°� �ƴ� ��, Ű�Է¿� ���� �̵�, ���� ó��
    else
    {
		Move();
    }

    // �߷����� ����
    // �浹�� ���� �ƴϸ� or �뽬 ���� �ƴϸ� �߷� ���� �Ǵ� ���������� �������� �߷� ����
	if (mb_isCollide == false && mb_isDash == false)
    {
        ApplyGravity();
    }

    // �Ʒ� ������ ĳ���� ���� �浹�˻��ϴ� ���� �ؿ����� �� �Ʒ���� �ٿ�����->false
    if (mb_isDownJump == true && m_shape.bottom > collidedRect.bottom)
    {
        mb_isDownJump = false;
        intersectRect = {};
        collidedRect = {};
    }

    // �׽�Ʈ ���ڿ� �浹 üũ, �浹�� �浹�Ѹ�ŭ ĳ���� ��ġ ����
    RECT tempRc = {}; 
    bool isIntersect = false;

    for(int i = 0; i < 6; ++i)
    {
        if (IntersectRect(&tempRc, &m_shape, &rectArr[i]))
        {
            // ���� �������� �÷��̾� bottom��(��ġ��) �浹�� ������ top���� ũ�ٸ�, �÷��̾ �����ϸ鼭 ���ڿ� �ε�������̱� ����
            // �� ��쿡�� �浹�ߴٰ� üũ�� �Ͽ� �ؿ��� �浹ó�� ������ ���ش�
            if (m_beforePlayerBottom <= rectArr[i].top)
            {
				isIntersect = true;
				collidedRect = rectArr[i];
				intersectRect = tempRc;
            }
        }
    }

    if (isIntersect)
    { 
        // �Ʒ����� �׸��� �뽬�� �ƴҶ� �浹 ����
        if (mb_isDownJump == false && mb_isDash == false/* && m_shape.bottom <= intersectRect.bottom*/)
        {
            mb_isCollide = true;

			m_pos.y -= (intersectRect.bottom - intersectRect.top);

            mb_isJump = false;
            m_jumpCount = 1;
            m_jumpStrength = 0.0f;
        }
    }
    else
    { 
        mb_isCollide = false; 
    }


    SetShape(m_pos, m_bodyWidth, m_bodyHeight);

    // �ݶ��̴� ������Ʈ
    m_collider->Update(m_shape);
}

void Player::Render(HDC hdc)
{
    // �׽�Ʈ ���ڿ�
    Rectangle(hdc, testRC.left, testRC.top, testRC.right, testRC.bottom);
    Rectangle(hdc, testRC2.left, testRC2.top, testRC2.right, testRC2.bottom);
    Rectangle(hdc, testRC3.left, testRC3.top, testRC3.right, testRC3.bottom);
    Rectangle(hdc, testRC4.left, testRC4.top, testRC4.right, testRC4.bottom);
    Rectangle(hdc, testRC5.left, testRC5.top, testRC5.right, testRC5.bottom);
    Rectangle(hdc, testRC6.left, testRC6.top, testRC6.right, testRC6.bottom);
    
    // DashEffect ����
    if (mb_isDash == true)
    {
        m_dashEffectImg->Render(hdc, static_cast<int>(m_beforePlayerPos.x), static_cast<int>(m_beforePlayerPos.y), m_dashFrameX, m_frameY);
    }

    // �÷��̾�
    //Rectangle(hdc, m_shape.left, m_shape.top, m_shape.right, m_shape.bottom);
    PlayAnimation(hdc, me_PlayerStatus);
    
    // RunEffect ����
    if (mb_isMove == true && mb_isJump == false)
    {
        RunEffectAnimation(hdc);
    }
}

void Player::Release()
{
    m_hitBackGround = nullptr;
    m_statusAniData[static_cast<int>(PlayerStatus::Idle)].playerImage = nullptr;
    m_statusAniData[static_cast<int>(PlayerStatus::Run)].playerImage = nullptr;
    m_statusAniData[static_cast<int>(PlayerStatus::Dead)].playerImage = nullptr;
    m_runEffectImg = nullptr;
    m_dashEffectImg = nullptr;

    // �ݶ��̴�
    m_collider = nullptr;
}

void Player::SetShape(const POINTFLOAT& playerPos, const int& m_bodyWidth, const int& m_bodyHeight)
{
    m_shape.left = static_cast<long>(playerPos.x - m_bodyWidth / 2);
    m_shape.right = static_cast<long>(m_shape.left + m_bodyWidth);
    m_shape.top = static_cast<long>(playerPos.y - m_bodyHeight / 2);
    m_shape.bottom = static_cast<long>(playerPos.y + m_bodyHeight);
}

void Player::Dash()
{
    // �뽬 �� ����� ���콺 ��ǥ�� �÷��̾� �̵�
	m_pos.x += cosf(m_angle) * m_dashSpeed /** TIMER_MANAGER->GetDeltaTime() / 0.1f*/;
	m_pos.y -= sinf(m_angle) * m_dashSpeed /** TIMER_MANAGER->GetDeltaTime() / 0.1f*/;
	SetShape(m_pos, m_bodyWidth, m_bodyHeight);
		
    // �뽬 ����Ʈ frameX�� �뽬�� ������ ���� ����
    if (mb_isDash == true)
    {
        ++m_dashTimer;
        if (m_dashTimer == 1)
            m_dashFrameX = 0;

        if (m_dashTimer == 3)
            m_dashFrameX = 1;

        if (m_dashTimer == 5)
            m_dashFrameX = 2;

        if (m_dashTimer == 7)
            m_dashFrameX = 3;

        if (m_dashTimer == 9)
            m_dashFrameX = 4;

        if (m_dashTimer >= 10)
        {
            m_dashTimer = 0;
            m_dashFrameX = 0;
            mb_isDash = false;
        }
    }
}

void Player::RegenDash()
{
    if (m_dashCount == m_maxDashCount) return;

    m_dashRegenTime += TIMER_MANAGER->GetDeltaTime();
    if (m_dashRegenTime > 1.5f)
    {
        if (m_dashCount < m_maxDashCount)
        {
            ++m_dashCount;
        }
        m_dashRegenTime = 0.0f;
    }
}

void Player::Move()
{
    // �̵� ����
    if (Input::GetButton('A'))
    {
        m_pos.x -= m_moveSpeed * TIMER_MANAGER->GetDeltaTime();
        me_PlayerStatus = PlayerStatus::Run;
        mb_isMove = true;
    }
    if (Input::GetButtonUp('A'))
    {
        me_PlayerStatus = PlayerStatus::Idle;
        m_frameX = 0;
        m_elapsedCount = 0.0f;

        m_dustFrameX = 0;
        m_dustEffectCount = 0.0f;
    }

    if (Input::GetButton('D'))
    {
        m_pos.x += m_moveSpeed * TIMER_MANAGER->GetDeltaTime();
        me_PlayerStatus = PlayerStatus::Run;
        mb_isMove = true;
    }
    if (Input::GetButtonUp('D'))
    {
        me_PlayerStatus = PlayerStatus::Idle;
        m_frameX = 0;
        m_elapsedCount = 0.0f;

        m_dustFrameX = 0;
        m_dustEffectCount = 0.0f;
    }


    // ���� ����
    if (m_jumpCount > 0)
    {
        // �� ����
        if (Input::GetButtonDown(VK_SPACE) /*&& mb_isDownJump == false*/)
        {
            --m_jumpCount;
            m_jumpStrength = JUMP_STRENGTH;
            mb_isJump = true;
        }

        // �Ʒ� ����
        if (Input::GetButton('S') && Input::GetButtonDown(VK_SPACE))
        {
            mb_isDownJump = true;
            m_jumpStrength = 0.0f;
        }
    }
}

void Player::ApplyGravity()
{
    m_pos.y -= m_jumpStrength * TIMER_MANAGER->GetDeltaTime();
    SetShape(m_pos, m_bodyWidth, m_bodyHeight);

    m_jumpStrength -= m_gravity * TIMER_MANAGER->GetDeltaTime();

    // jumpStrength �߷� ��������� ���Ѽ� ����
    if (m_jumpStrength < -JUMP_STRENGTH)
    {
        m_jumpStrength = -JUMP_STRENGTH;
    }
}

void Player::PlayAnimation(HDC hdc, PlayerStatus playerStatus)
{
    // ���콺 ��ǥ�� ���� �÷��̾� �̹��� ���� - ���׾�������
    if (playerStatus != PlayerStatus::Dead)
    {
		if (m_pos.x < Input::GetMousePosition().x)
		{
			m_frameY = 0;
			mb_isLeft = false;
		}
		else
		{
			m_frameY = 1;
			mb_isLeft = true;
		}
    }

    if (playerStatus < PlayerStatus::Idle || playerStatus > PlayerStatus::End)
        return;
    
    // �÷��̾� �̹��� ���� ����
    m_elapsedCount += TIMER_MANAGER->GetDeltaTime();
    if (m_elapsedCount > m_statusAniData[static_cast<int>(playerStatus)].totalTime)
    {
        ++m_frameX;
        if (m_frameX >= m_statusAniData[static_cast<int>(playerStatus)].maxFrameX)
        {
            m_frameX = 0;
        }
        m_elapsedCount = 0.0f;
    }

    if (playerStatus == PlayerStatus::Dead)
    {
        m_statusAniData[static_cast<int>(playerStatus)].playerImage->Render(hdc, static_cast<int>(m_pos.x), static_cast<int>(m_pos.y), 1.0f, mb_isLeft);
    }
    else if (mb_isHit == true)
    {
        m_statusAniData[static_cast<int>(playerStatus)].playerImage->ImgAlphaBlendFrameRender(hdc, static_cast<int>(m_pos.x), static_cast<int>(m_pos.y), 
                                                                                        m_frameX, m_frameY, 120);
        m_hitBackGround->ImgAlphaBlendRender(hdc, static_cast<int>(WIN_SIZE_X / 2), static_cast<int>(WIN_SIZE_Y / 2), m_hitBackGroundTransparancy);
    }
    else
    {
		m_statusAniData[static_cast<int>(playerStatus)].playerImage->Render(hdc, static_cast<int>(m_pos.x), static_cast<int>(m_pos.y), m_frameX, m_frameY);
    }
}

// RunEffect ����
void Player::RunEffectAnimation(HDC hdc)
{
	m_dustEffectCount += TIMER_MANAGER->GetDeltaTime();
	if (m_dustEffectCount > 0.1f)
	{
		++m_dustFrameX;
		if (m_dustFrameX >= m_dustMaxFrameX)
		{
			m_dustFrameX = 0;
			mb_isMove = false;
		}
		m_dustEffectCount = 0.0f;
	}

	if (mb_isLeft == false && mb_isMove == true)
	{
		m_runEffectImg->Render(hdc, static_cast<int>(m_pos.x - 30), static_cast<int>(m_pos.y + 20), m_dustFrameX, m_frameY);
	}
	else if (mb_isLeft == true && mb_isMove == true)
	{
		m_runEffectImg->Render(hdc, static_cast<int>(m_pos.x + 30), static_cast<int>(m_pos.y + 20), m_dustFrameX, m_frameY);
	}
}

void Player::Attack()
{
    if (m_weapon[m_selectedWeaponIndex] != nullptr)
    {
        m_weapon[m_selectedWeaponIndex]->SetIsAttack(true);
        m_weapon[m_selectedWeaponIndex]->SetIsAttack2(true);

        m_weapon[m_selectedWeaponIndex]->SetImgToggle(); // !GetImgToggle()�Ἥ �ض�. �����Ը���.
    }
}

void Player::makeTestRect()
{
    // �׽�Ʈ ����
    testRC.left = static_cast<long>(m_pos.x - m_bodyWidth);
    testRC.right = static_cast<long>(m_pos.x + m_bodyWidth);
    testRC.top = static_cast<long>(m_pos.y - m_bodyHeight / 2 + 200);
    testRC.bottom = static_cast<long>(m_pos.y + m_bodyHeight / 2 + 200);

    testRC2.left = static_cast<long>(m_pos.x - m_bodyWidth);
    testRC2.right = static_cast<long>(m_pos.x + m_bodyWidth);
    testRC2.top = static_cast<long>(m_pos.y - m_bodyHeight / 2 + 400);
    testRC2.bottom = static_cast<long>(m_pos.y + m_bodyHeight / 2 + 400);

    testRC3.left = static_cast<long>(m_pos.x - m_bodyWidth - 100);
    testRC3.right = static_cast<long>(m_pos.x + m_bodyWidth - 100);
    testRC3.top = static_cast<long>(m_pos.y - m_bodyHeight / 2 + 300);
    testRC3.bottom = static_cast<long>(m_pos.y + m_bodyHeight / 2 + 300);

    testRC4.left = static_cast<long>(m_pos.x - m_bodyWidth + 100);
    testRC4.right = static_cast<long>(m_pos.x + m_bodyWidth + 100);
    testRC4.top = static_cast<long>(m_pos.y - m_bodyHeight / 2 + 300);
    testRC4.bottom = static_cast<long>(m_pos.y + m_bodyHeight / 2 + 300);

    testRC5.left = static_cast<long>(m_pos.x - m_bodyWidth);
    testRC5.right = static_cast<long>(m_pos.x + m_bodyWidth);
    testRC5.top = static_cast<long>(m_pos.y - m_bodyHeight / 2 + 100);
    testRC5.bottom = static_cast<long>(m_pos.y + m_bodyHeight / 2 + 100);

    testRC6.left = static_cast<long>(0);
    testRC6.right = static_cast<long>(WIN_SIZE_X);
    testRC6.top = static_cast<long>(WIN_SIZE_Y);
    testRC6.bottom = static_cast<long>(WIN_SIZE_Y + 90);

    intersectRect = {};
    collidedRect = {};

    rectArr[0] = testRC;
    rectArr[1] = testRC2;
    rectArr[2] = testRC3;
    rectArr[3] = testRC4;
    rectArr[4] = testRC5;
    rectArr[5] = testRC6;
}

void Player::Hit()
{
    m_hitElapsedCount += TIMER_MANAGER->GetDeltaTime();
    if (m_hitElapsedCount > 1.0f)
    {
        mb_isHit = false;
        m_hitElapsedCount = 0.0f;
        m_hitBackGroundTransparancy = 80;
    }

    // ������ ���� ����
    --m_hitBackGroundTransparancy;
    if (m_hitBackGroundTransparancy == 0 || m_hitBackGroundTransparancy > 80)
    {
        m_hitBackGroundTransparancy = 0;
    }
}



