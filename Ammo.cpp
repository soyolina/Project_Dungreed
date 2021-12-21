#include "stdafx.h"
#include "Ammo.h"
#include "Image.h"
#include <cmath>

HRESULT Ammo::Init(LPCWSTR imgName, POINTFLOAT pos, float angle, float attackDamage, float moveSpeed, float changeAngle)
{
	// 애니메이션용
	m_ammoImg = IMAGE_MANAGER->FindImage(imgName);

	m_frameX = 0;
	m_frameY = 0;
	m_maxFrameX = m_ammoImg->GetMaxFrameX();
	m_elapsedCount = 0.0f;

    // GameObject에서 상속받은 것
	m_pos = pos;
	m_bodyWidth = m_ammoImg->GetFrameWidth();
	m_bodyHeight = m_ammoImg->GetFrameHeight();
	SetShape();

	m_hp = 0;
	m_moveSpeed = moveSpeed;
	m_attackDamage = attackDamage;

	mb_isHit = false;
	m_hitElapsedCount = 0.0f;
	mb_isDead = false;
    //

	m_angle = angle;
	m_changeAngle = changeAngle;

    return S_OK;
}

void Ammo::Update()
{
	// 아모 이미지 애니메이션용
	m_elapsedCount += TIMER_MANAGER->GetDeltaTime();
	if (m_elapsedCount >= 0.15f)
	{
		++m_frameX;
		if (m_frameX >= m_maxFrameX)
		{
			m_frameX = 0;
		}
		m_elapsedCount = 0.0f;
	}

	// 미사일 포지션 변경	
	m_pos.x += cosf(m_angle) * m_moveSpeed * TIMER_MANAGER->GetDeltaTime();
	m_pos.y += - sinf(m_angle) * m_moveSpeed * TIMER_MANAGER->GetDeltaTime();

	SetShape();
}

void Ammo::Render(HDC hdc)
{
	//Rectangle(hdc, m_shape.left, m_shape.top, m_shape.right, m_shape.bottom);
	m_ammoImg->Render(hdc, static_cast<int>(m_pos.x), static_cast<int>(m_pos.y), m_frameX, m_frameY, 1.0f);
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
