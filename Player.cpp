#include "Player.h"
#include "Image.h"

HRESULT Player::Init()
{
    m_playerEffectImg = IMAGE_MANAGER->FindImage("Image/Player/baseCharEffect.bmp");
    m_runEffectImg = IMAGE_MANAGER->FindImage("Image/Player/RunEffect.bmp");

    m_frameX = 0;
    m_frameY = 0;
    m_elapsedCount = 0.0f;

    m_statusAniData[(int)PlayerStatus::Idle] = { IMAGE_MANAGER->FindImage("Image/Player/baseCharIdle.bmp"), 5, 0.1f};
    m_statusAniData[(int)PlayerStatus::Run] = { IMAGE_MANAGER->FindImage("Image/Player/baseCharRun.bmp"), 8, 0.1f};
    m_statusAniData[(int)PlayerStatus::Jump] = { IMAGE_MANAGER->FindImage("Image/Player/baseCharRun.bmp"), 8, 0.1f};

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

    m_jumpCount = 1;
    m_jumpStrength = 0.0f;
    m_gravity = 1300.0f;
    
    mb_isCollide = false;


    makeTestRect();
    
    return S_OK;
}

void Player::Update()
{
    // �̵� ����
    if (KEY_MANAGER->IsStayKeyDown('A'))
    {
        m_pos.x -= static_cast<int>(m_moveSpeed * TIMER_MANAGER->GetDeltaTime());
    }

    if (KEY_MANAGER->IsStayKeyDown('D'))
    {
        m_pos.x += static_cast<int>(m_moveSpeed * TIMER_MANAGER->GetDeltaTime());
    }


    // ���� ����
    if (m_jumpCount > 0)
    {
        // �� ����
		if (KEY_MANAGER->IsOnceKeyDown(VK_SPACE) && mb_isDownJump == false)
		{
			--m_jumpCount;
			m_jumpStrength = JUMP_STRENGTH;
			mb_isJump = true;
		}

        // �Ʒ� ����
        if (KEY_MANAGER->IsStayKeyDown('S') && mb_isJump == true)
        {
            mb_isDownJump = true;
            m_jumpStrength = 0.0f;
        }
    }

    // �浹���ϸ� �߷� ���� �Ǵ� ���������� �������� �߷� ����
	if (mb_isCollide == false)
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


    // �׽�Ʈ ���ڿ� �浹 üũ, �浹�� �浹�Ѹ�ŭ ĳ���� ��ġ ����
    RECT tempRc = {}; 
    bool isIntersect = false;

    for(int i = 0; i < 5; ++i)
    {
        if (IntersectRect(&tempRc, &m_shape, &rectArr[i]))
        {
            isIntersect = true;
            collidedRect = rectArr[i]; 
            intersectRect = tempRc;
        }
    }
    
    // �Ʒ� ������ ĳ�������� �浹�˻��ϴ� ���� �ؿ����� �� �Ʒ���� �ٿ�����->false
    if (mb_isDownJump == true && m_shape.top > collidedRect.bottom)
    {
        mb_isDownJump = false;
    }

    if (isIntersect)
    { 
        // �Ʒ����� �ƴҶ� �浹 ����
        if (mb_isDownJump == false && m_shape.bottom <= intersectRect.bottom)
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
        /*intersectRect = {};
        collidedRect = {};*/
    }


    SetShape(m_pos, m_bodyWidth, m_bodyHeight);

    
    // ���콺 ��ǥ�� ���� �÷��̾� �̹��� ����
    if (m_pos.x < g_ptMouse.x) { m_frameY = 0; }
    else { m_frameY = 1; }
}

void Player::Render(HDC hdc)
{
    // �׽�Ʈ ���ڿ�
    Rectangle(hdc, testRC.left, testRC.top, testRC.right, testRC.bottom);
    Rectangle(hdc, testRC2.left, testRC2.top, testRC2.right, testRC2.bottom);
    Rectangle(hdc, testRC3.left, testRC3.top, testRC3.right, testRC3.bottom);
    Rectangle(hdc, testRC4.left, testRC4.top, testRC4.right, testRC4.bottom);
    Rectangle(hdc, testRC5.left, testRC5.top, testRC5.right, testRC5.bottom);
    
    // �÷��̾�
    Rectangle(hdc, m_shape.left, m_shape.top, m_shape.right, m_shape.bottom);
    Animation(hdc, me_PlayerStatus);
}

void Player::Release()
{
}

void Player::SetShape(POINTFLOAT playerPos, int m_bodyWidth, int m_bodyHeight)
{
    m_shape.left = static_cast<long>(playerPos.x - m_bodyWidth / 2);
    m_shape.right = static_cast<long>(m_shape.left + m_bodyWidth);
    m_shape.top = static_cast<long>(playerPos.y - m_bodyHeight / 2);
    m_shape.bottom = static_cast<long>(playerPos.y + m_bodyHeight);
}

void Player::Animation(HDC hdc, PlayerStatus playerStatus)
{
    if (playerStatus < PlayerStatus::Idle || playerStatus > PlayerStatus::Jump)
        return;

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

    intersectRect = {};
    collidedRect = {};

    rectArr[0] = testRC;
    rectArr[1] = testRC2;
    rectArr[2] = testRC3;
    rectArr[3] = testRC4;
    rectArr[4] = testRC5;
}
