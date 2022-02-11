#include "stdafx.h"
#include "Ammo.h"
#include "Image.h"
#include "Collider2.h"
#include <cmath>

HRESULT Ammo::Init(LPCWSTR imgName, POINTFLOAT pos, float angle, int attackDamage, float moveSpeed, ObjectType objType, float changeAngle)
{
	// 애니메이션용
	m_ammoImg = IMAGE_MANAGER->FindImage(imgName);

	m_frameX = 0;
	m_frameY = 0;
	m_maxFrameX = m_ammoImg->GetMaxFrameX();
	m_elapsedCount = 0.0f;
	m_frameChangeTime = 0.15f;

    // GameObject에서 상속받은 것
	m_pos = pos;
	m_bodyWidth = m_ammoImg->GetFrameWidth();
	m_bodyHeight = m_ammoImg->GetFrameHeight();
	SetShape();

	// 진짜 콜라이더	
	collider.Init(this, &m_shape, objType, [](auto, auto) { });

	m_hp = 0;
	m_moveSpeed = moveSpeed;
	m_attackDamage = attackDamage;

	mb_isHit = false;
	mb_isHit2 = false;
	m_hitElapsedCount = 0.0f;
	mb_isDead = false;
    //

	m_angle = angle;
	m_changeAngle = changeAngle;

    return S_OK;
}

void Ammo::Update()
{
	// 화면 밖으로 벗어날시 미사일 상태 -> Dead 로 바꿈
	if (m_shape.right < 0 || m_shape.left > WIN_SIZE_X || m_shape.bottom < 0 || m_shape.top > WIN_SIZE_Y)
	{
		mb_isDead = true;
		return;
	}

	// 히트 당했을때 히트당한 이미지 데이터 셋팅
	if (mb_isHit == true)
	{
		// 나중에 추가할 조건 : 아모가 보스의 미사일 패턴일 때는 저 이미지 쓸꺼다. 
		if (mb_isHit2 == true)
		{
			m_ammoImg = IMAGE_MANAGER->FindImage(L"Image/Boss/BossBulletEffect.bmp");
			m_frameX = 0;
			m_maxFrameX = m_ammoImg->GetMaxFrameX();
			m_elapsedCount = 0.0f;
			m_frameChangeTime = 0.07f;

			mb_isHit2 = false;
		}
	}

	// 아모 이미지 애니메이션용
	m_elapsedCount += TIMER_MANAGER->GetDeltaTime();
	if (m_elapsedCount >= m_frameChangeTime)
	{
		++m_frameX;
		if (m_frameX >= m_maxFrameX)
		{
			m_frameX = 0;

			// 미사일 히트상태고 히트당했을때의 이미지 애니메이션 다돌았으면 Dead상태로 셋팅해줌.
			if (mb_isHit == true)
			{
				mb_isDead = true;
			}
		}
		m_elapsedCount = 0.0f;
	}

	// 미사일 포지션 변경	
	if (mb_isHit == false)	// 히트 상태가 아닐때만
	{
		m_pos.x += cosf(m_angle) * m_moveSpeed * TIMER_MANAGER->GetDeltaTime();
		m_pos.y += -sinf(m_angle) * m_moveSpeed * TIMER_MANAGER->GetDeltaTime();
	}
	
	SetShape();
}

void Ammo::Render(HDC hdc)
{
	if (mb_isDead == false)
	{
		m_ammoImg->Render(hdc, static_cast<int>(m_pos.x), static_cast<int>(m_pos.y), m_frameX, m_frameY, 1.0f);
	}
}

void Ammo::Release()
{
	m_ammoImg = nullptr;
}

void Ammo::SetShape()
{
	m_shape.left = static_cast<long>(m_pos.x - m_bodyWidth * 0.5f);
	m_shape.right = static_cast<long>(m_pos.x + m_bodyWidth * 0.5f);
	m_shape.top = static_cast<long>(m_pos.y - m_bodyHeight * 0.5f);
	m_shape.bottom = static_cast<long>(m_pos.y + m_bodyHeight * 0.5f);
}
