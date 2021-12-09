#include "stdafx.h"
#include "Player.h"
#include "Image.h"

HRESULT Player::Init()
{
    m_dashEffectImg = IMAGE_MANAGER->FindImage(L"Image/Player/baseCharEffect.bmp");
    m_runEffectImg = IMAGE_MANAGER->FindImage(L"Image/Player/RunEffect.bmp");

    m_frameX = 0;
    m_frameY = 0;
    m_elapsedCount = 0.0f;

    m_statusAniData[static_cast<int>(PlayerStatus::Idle)] = { IMAGE_MANAGER->FindImage(L"Image/Player/baseCharIdle.bmp"), 5, 0.1f};
    m_statusAniData[static_cast<int>(PlayerStatus::Run)] = { IMAGE_MANAGER->FindImage(L"Image/Player/baseCharRun.bmp"), 8, 0.1f};
    //m_statusAniData[static_cast<int>(PlayerStatus::Jump)] = { IMAGE_MANAGER->FindImage("Image/Player/baseCharIdle.bmp"), 5, 0.1f };

    me_PlayerStatus = PlayerStatus::Idle;

    m_pos.x = WIN_SIZE_X / 2;
    m_pos.y = WIN_SIZE_Y / 2;

    m_bodyWidth = 36;
    m_bodyHeight = 38;

    m_shape.left = static_cast<long>(m_pos.x - m_bodyWidth / 2);
    m_shape.right = static_cast<long>(m_shape.left + m_bodyWidth);
    m_shape.top = static_cast<long>(m_pos.y - m_bodyHeight / 2);
    m_shape.bottom = static_cast<long>(m_pos.y + m_bodyHeight);
    
    m_hp = 100;
    m_moveSpeed = 200.0f;

    mb_isJump = false;
    mb_isDownJump = false;

    m_beforePlayerBottom = 0;
    m_jumpCount = 1;
    m_jumpStrength = 0.0f;
    m_gravity = 1300.0f;
    
    mb_isCollide = false;

    // �÷��̾ �ٶ󺸰� �ִ� ����
    mb_isLeft = false;

    // Runeffect��
    mb_isMove = false;
    m_dustEffectCount = 0.0f;
    m_dustFrameX = 0;
    m_dustMaxFrameX = 5;

    // �뽬 ����
    mb_isDash = false;
    m_angle = 0.0f;
    m_maxDashCount = 2;
    m_dashCount = m_maxDashCount;
    m_dashSpeed = 18.0f;
    m_dashRegenTime = 0.0f;

    m_dashTimer = 0;
    m_dashFrameX = 0;
    m_beforePlayerPos = {};

    // �ӽ� �׽�Ʈ ����
    makeTestRect();
    
    return S_OK;
}

void Player::Update()
{
    // ĳ���� HP ���� ����� �Ǵ��� Ȯ�ο� - �׽�Ʈ
    if (Input::GetButtonDown(VK_F3))
    {
        m_hp -= 5;
    }

    // ĳ���� ��ġ ���¿� - �׽�Ʈ
    if (Input::GetButton(VK_F2))
    {
        m_pos.x = WIN_SIZE_X / 2;
        m_pos.y = WIN_SIZE_Y / 2;
    }

    // �뽬 ����Ʈ ���� ���ؼ� ���� ����
    m_beforePlayerPos = m_pos;

    // �浹üũ �������� ���� ���� ���� ���� - �÷��̾��� �ذ� �浹�Ǵ� ���� ���κа� �浹���� �ÿ���, �浹 ó�� ������ ���ؼ�
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
    DashRegen();

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

    // �浹�� ���� �ƴϸ� �߷� ���� �Ǵ� ���������� �������� �߷� ����
	if (mb_isCollide == false && mb_isDash == false)
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

    // �Ʒ� ������ ĳ�������� �浹�˻��ϴ� ���� �ؿ����� �� �Ʒ���� �ٿ�����->false
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
        // �Ʒ����� �ƴҶ� �浹 ����
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

    
    // ���콺 ��ǥ�� ���� �÷��̾� �̹��� ����
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
    Animation(hdc, me_PlayerStatus);
    
    // RunEffect ����
    if (mb_isMove == true && mb_isJump == false)
    {
        RunEffectAnimation(hdc);
    }
    
}

void Player::Release()
{
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
    // �뽬�� �� ����� ���콺 ��ǥ�� �÷��̾� �̵�
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

void Player::DashRegen()
{
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

        m_dustFrameX = 0;
        m_dustEffectCount = 0.0f;
    }


    // ���� ����
    if (m_jumpCount > 0)
    {
        // �� ����
        if (Input::GetButtonDown(VK_SPACE) && mb_isDownJump == false)
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

void Player::Animation(HDC hdc, PlayerStatus playerStatus)
{
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

    m_statusAniData[static_cast<int>(playerStatus)].playerImage->Render(hdc, static_cast<int>(m_pos.x), static_cast<int>(m_pos.y), m_frameX, m_frameY);
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
    testRC6.top = static_cast<long>(WIN_SIZE_Y - 10);
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

