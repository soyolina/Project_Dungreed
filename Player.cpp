#include "stdafx.h"
#include "Player.h"
#include "Image.h"
#include "ItemBase.h"
#include "Collider2.h"

HRESULT Player::Init()
{
    // 플레이어 히트시 배경
    m_hitBackGround = IMAGE_MANAGER->FindImage(L"Image/Player/hit.bmp");
    m_hitBackGroundTransparancy = 80;

    // - GameObject에서 상속받은 것
    // 캐릭터 위치
    m_pos.x = WIN_SIZE_X / 2;
    m_pos.y = WIN_SIZE_Y / 2;

    // 캐릭터 히트박스 관련
    m_bodyWidth = 36;
    m_bodyHeight = 38;

    SetShape(m_pos, m_bodyWidth, m_bodyHeight);
    // 콜라이더
    //m_collider = ColliderManager::CreateCollider(this, m_shape, ObjectType::Player);
    // 진짜 콜라이더
    intersectRect = {};
    collidedRect = {};

    collider.Init(this, &m_shape, ObjectType::Player,
        [&](Collider* other, RECT intersectedRect)
        {
            switch (other->GetType())
            {
            case ObjectType::TileMap:
            {
                RECT tempRect = other->GetHitbox();
                
                // 이전 프레임의 플레이어 bottom이(위치가) 충돌된 상자의 top보다 크다면, 플레이어가 낙하하면서 상자와 부딪힌경우이기 떄문
                // 이 경우에만 충돌했다고 체크를 하여 밑에서 충돌처리 보정을 해준다
                if (m_beforePlayerBottom <= tempRect.top)
                {
					// 아래점프 그리고 대쉬가 아닐때 충돌 보정
					if (mb_isDownJump == false && mb_isDash == false/* && m_shape.bottom <= intersectRect.bottom*/)
					{
						//mb_isCollide = true;
                        collidedRect = tempRect;
                        intersectRect = intersectedRect;

						m_pos.y -= (intersectRect.bottom - intersectRect.top);

						mb_isJump = false;
						m_jumpCount = 1;
						m_jumpStrength = 0.0f;
					}
                    else
                    {
                        //mb_isCollide = false;
                        collidedRect = {};
                        intersectRect = {};
                    }
                    
                }
                
                SetShape(m_pos, m_bodyWidth, m_bodyHeight);
            }
                break;

            case ObjectType::EnemyAttack:
                if (mb_isHit == false)
                {
                    mb_isHit = true;
                    other->GetOwner()->SetIsHit(true);
                    other->GetOwner()->SetIsHit2(true);

                    int damage = other->GetOwner()->GetAttackDamage();
                    m_hp -= damage;
                }
                break;
            }
        });

    // 충돌 처리를 각 객체 내에서 할 건데.
    // 멤버 함수를 전달
    //bind(&Player::OnCollision, this, placeholders::_1, placeholders::_2));

    // 캐릭터 정보관련
    m_hp = 100;
    m_moveSpeed = 200.0f;
    m_attackDamage = 0;
    mb_isHit = false;
    m_hitElapsedCount = 0.0f;
    mb_isDead = false;
    // - 여기까지

    // 플레이어가 바라보고 있는 방향 체크용
    mb_isLeft = false;

    // 캐릭터 상태
    me_PlayerStatus = PlayerStatus::Idle;

    // 플레이어 이미지 애니메이션 렌더용
    m_statusAniData[static_cast<int>(PlayerStatus::Idle)] = { IMAGE_MANAGER->FindImage(L"Image/Player/baseCharIdle.bmp"), 5, 0.1f};
    m_statusAniData[static_cast<int>(PlayerStatus::Run)] = { IMAGE_MANAGER->FindImage(L"Image/Player/baseCharRun.bmp"), 8, 0.1f};
    m_statusAniData[static_cast<int>(PlayerStatus::Dead)] = { IMAGE_MANAGER->FindImage(L"Image/Player/basecharDie.bmp"), 0, 0.0f};

    m_frameX = 0;
    m_frameY = 0;
    m_elapsedCount = 0.0f;
    
    // 플레이어가 이동시 발생하는 먼지 이펙트 관련
    m_runEffectImg = IMAGE_MANAGER->FindImage(L"Image/Player/RunEffect.bmp");
    mb_isMove = false;
    m_dustEffectCount = 0.0f;
    m_dustFrameX = 0;
    m_dustMaxFrameX = 5;

    // 점프관련
    mb_isJump = false;
    mb_isDownJump = false;

    m_jumpCount = 1;
    m_jumpStrength = 0.0f;
    m_gravity = 1300.0f;
    
    mb_isCollide = false;
    m_beforePlayerBottom = 0;

    // 대쉬 관련
    m_dashEffectImg = IMAGE_MANAGER->FindImage(L"Image/Player/baseCharEffect.bmp");
    mb_isDash = false;
    m_angle = 0.0f;
    m_maxDashCount = 3;
    m_dashCount = m_maxDashCount;
    m_dashSpeed = 18.0f;
    m_dashRegenTime = 0.0f;

    m_dashTimer = 0;
    m_dashFrameX = 0;
    m_beforePlayerPos = {};

    // 아이템 관련
    m_selectedWeaponIndex = 0;
    m_weapon[0] = nullptr;
    m_weapon[1] = nullptr;
    
    return S_OK;
}

void Player::Update()
{
    // 죽을때, 죽었으면 업데이트안할꺼다 - 그래서 리턴함
    if (m_hp <= 0)
    {
        mb_isDead = true;
        me_PlayerStatus = PlayerStatus::Dead;
        return;
    }

    // 맞았을때
    if (mb_isHit == true)
    {
        Hit();
    }

    // 캐릭터 위치 리셋용 - 테스트
    if (Input::GetButton(VK_F2))
    {
        m_pos.x = WIN_SIZE_X / 2;
        m_pos.y = WIN_SIZE_Y / 2;
    }

    // 아이템 관련된 것 - 마우스 좌클릭시 공격 => Attack()
    if (Input::GetButtonDown(VK_LBUTTON))
    {
        Attack();
    }

    // 대쉬 이펙트 렌더 위해서 쓰는 변수
    m_beforePlayerPos = m_pos;

    // 충돌체크 후 보정할지 말지 위해 쓰는 변수 - 플레이어의 밑과 충돌되는 상자 윗부분과 충돌했을 시에만, 충돌 처리 보정을 위해서
    m_beforePlayerBottom = m_shape.bottom;

    // 우클릭시 대쉬 
    if (Input::GetButtonDown(VK_RBUTTON) && m_dashCount > 0)
    {
        mb_isDash = true;
        m_angle = GetAngle(m_pos, Input::GetMousePosition());
        m_jumpStrength = 0.0f;
        --m_dashCount;
    }

    // 대쉬 횟수 회복
    RegenDash();

    // 대시 관련
    if (mb_isDash == true)
    {
		Dash();
    }
    // 대쉬상태가 아닐 때, 키입력에 따른 이동, 점프 처리
    else
    {
		Move();
    }

    // 중력적용 관련
    // 충돌일 때가 아니면 or 대쉬 상태 아니면 중력 적용 또는 점프했을시 점프힘에 중력 적용
	if (mb_isDash == false /* && mb_isCollide == false */ )
    {
        ApplyGravity();
    }

    // 아래 점프시 캐릭터 밑이 충돌검사하는 상자 밑에보다 더 아래라면 다운점프->false
    if (mb_isDownJump == true && m_shape.bottom > collidedRect.bottom)
    {
        mb_isDownJump = false;
        intersectRect = {};
        collidedRect = {};
    }

    SetShape(m_pos, m_bodyWidth, m_bodyHeight);
}

void Player::Render(HDC hdc)
{
    // 테스트 상자용
    //Rectangle(hdc, testRC.left, testRC.top, testRC.right, testRC.bottom);
    //Rectangle(hdc, testRC2.left, testRC2.top, testRC2.right, testRC2.bottom);
    //Rectangle(hdc, testRC3.left, testRC3.top, testRC3.right, testRC3.bottom);
    //Rectangle(hdc, testRC4.left, testRC4.top, testRC4.right, testRC4.bottom);
    //Rectangle(hdc, testRC5.left, testRC5.top, testRC5.right, testRC5.bottom);
    //Rectangle(hdc, testRC6.left, testRC6.top, testRC6.right, testRC6.bottom);
    
    // DashEffect 관련
    if (mb_isDash == true)
    {
        m_dashEffectImg->Render(hdc, static_cast<int>(m_beforePlayerPos.x), static_cast<int>(m_beforePlayerPos.y), m_dashFrameX, m_frameY);
    }

    // 플레이어
    //Rectangle(hdc, m_shape.left, m_shape.top, m_shape.right, m_shape.bottom);
    PlayAnimation(hdc, me_PlayerStatus);
    
    // RunEffect 관련
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
    // 대쉬 그 당시의 마우스 좌표로 플레이어 이동
	m_pos.x += cosf(m_angle) * m_dashSpeed /** TIMER_MANAGER->GetDeltaTime() / 0.1f*/;
	m_pos.y -= sinf(m_angle) * m_dashSpeed /** TIMER_MANAGER->GetDeltaTime() / 0.1f*/;
	SetShape(m_pos, m_bodyWidth, m_bodyHeight);
		
    // 대쉬 이펙트 frameX와 대쉬가 끝나는 지점 설정
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
    // 이동 관련
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


    // 점프 관련
    if (m_jumpCount > 0)
    {
        // 위 점프
        if (Input::GetButtonDown(VK_SPACE) /*&& mb_isDownJump == false*/)
        {
            --m_jumpCount;
            m_jumpStrength = JUMP_STRENGTH;
            mb_isJump = true;
        }

        // 아래 점프
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

    // jumpStrength 중력 영향받을때 하한선 적용
    if (m_jumpStrength < -JUMP_STRENGTH)
    {
        m_jumpStrength = -JUMP_STRENGTH;
    }
}

void Player::PlayAnimation(HDC hdc, PlayerStatus playerStatus)
{
    // 마우스 좌표에 따른 플레이어 이미지 반전 - 안죽었을때만
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
    
    // 플레이어 이미지 렌더 관련
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

// RunEffect 렌더
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

        m_weapon[m_selectedWeaponIndex]->SetImgToggle(); // !GetImgToggle()써서 해라. 교수님말씀.
    }
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

    // 배경관련 투명도 결정
    --m_hitBackGroundTransparancy;
    if (m_hitBackGroundTransparancy == 0 || m_hitBackGroundTransparancy > 80)
    {
        m_hitBackGroundTransparancy = 0;
    }
}

