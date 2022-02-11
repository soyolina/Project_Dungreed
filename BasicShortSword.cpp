#include "stdafx.h"
#include "BasicShortSword.h"
#include "Image.h"
#include "Player.h"
#include "Collider2.h"

#include <cmath>

HRESULT BasicShortSword::Init(Player* player)
{
	m_player = player;
	m_playerPos = m_player->GetPlayerPos();
	mb_isReverse = false;

	// 아이템 속성
	m_attackSpeed = 0.0f;

	// 기본검 이미지용
	m_bssImg = IMAGE_MANAGER->FindImage(L"Image/Item/BasicShortSword_New.bmp");
	m_bssImgRenderPos = {};
	m_angle = 0.0f;

	// 공격인지 아닌지 
	mb_isAttack = false;
	mb_isAttack2 = false;

	// 검 이펙트 이미지 용
	m_bssEffectImg = IMAGE_MANAGER->FindImage(L"Image/Item/BasicShortSwordEffect.bmp");
	m_effectImgRenderPos = {};
	m_rotateAxis = {};
	m_effectAngle = 0.0f;
	
	// 검 이펙트 애니메이션 용
	m_frameX = 0;
	m_frameY = 0;
	m_maxFrameX = 3;
	m_elapsedCount = 0.0f;

	// 공격력
	m_attackDamage = 20;

	// 아이템 히트박스용
	m_shape = {};

	// 진짜 콜라이더
	collider.Init(this, &m_shape, ObjectType::Item, 
		[&](Collider* other, RECT intersectedRect)
		{
			switch (other->GetType())
			{
			case ObjectType::Enemy:
			{
				if (other->GetOwner()->GetIsHit() == false)
				{
					other->GetOwner()->SetIsHit(true);
					other->GetOwner()->SetIsHit2(true);

					other->GetOwner()->SetHp(m_attackDamage);
				}
			}
			break;
			}
		}
	);

	return S_OK;
}

void BasicShortSword::Update()
{
	// 플레이어 사망시는 업뎃안할꺼임
	if (m_player->GetIsDead()) { return; }

	m_playerPos = m_player->GetPlayerPos();

	// 이미지 반전 위해서
	mb_isReverse = m_player->GetPlayerReverse();
	
	// 검 이미지 렌더용 pos와 각도 설정
	SetBssImgData();

	// 검 이펙트 이미지와 히트박스 용 - 플레이서 클래스에서 좌클릭을 누르면 mb_isAttack이 true가 됨.
	if (mb_isAttack == true)
	{
		// 이펙트 이미지 애니메이션 
		PlayAnimation();

		if (mb_isAttack2 == true)
		{
			// 검 이펙트 이미지 데이터 셋팅(pos와 angle등)
			SetEffectImgData();

			// 히트박스 shape 설정
			SetHitboxShape();

			mb_isAttack2 = false;
		}
	}
	else
	{
		SetHitboxShape();
	}
}

void BasicShortSword::Render(HDC hdc)
{
	// 검 이미지 렌더
	m_bssImg->ImgRotateRender(hdc, static_cast<int>(m_bssImgRenderPos.x), static_cast<int>(m_bssImgRenderPos.y), m_angle);

	// 공격일시
	if (mb_isAttack == true)
	{
		// 검 이펙트 이미지 렌더용
		m_bssEffectImg->ImgRotateFrameRender(hdc, static_cast<int>(m_effectImgRenderPos.x), static_cast<int>(m_effectImgRenderPos.y), m_frameX, m_frameY, m_effectAngle);
	}
}

void BasicShortSword::Release()
{
	m_bssImg = nullptr;
	m_bssEffectImg = nullptr;
}

void BasicShortSword::PlayAnimation()
{
	m_elapsedCount += TIMER_MANAGER->GetDeltaTime();
	if (m_elapsedCount >= 0.06f)
	{
		++m_frameX;
		if (m_frameX >= m_maxFrameX)
		{
			m_frameX = 0;
			mb_isAttack = false;
		}
		m_elapsedCount = 0.0f;
	}
}

void BasicShortSword::SetBssImgData()
{
	if (mb_isReverse == false)
	{
		m_bssImgRenderPos.x = m_playerPos.x + 20.0f;
		m_bssImgRenderPos.y = m_playerPos.y + 12.0f;
		// pos와 마우스간의 각도에서 무기를 대각선으로 들게해주기 위해 각도 더해줌
		m_angle = GetAngle(m_bssImgRenderPos, Input::GetMousePosition()) + DEGREE_TO_RADIAN(110);
	}
	else
	{
		m_bssImgRenderPos.x = m_playerPos.x - 20.0f;
		m_bssImgRenderPos.y = m_playerPos.y + 12.0f;
		m_angle = GetAngle(m_bssImgRenderPos, Input::GetMousePosition()) - DEGREE_TO_RADIAN(110);
	}

	// 플레이어 클래스에서 좌클릭(공격) 할 때마다 mb_imgToggle이 껐다 켜졌다 하면서, 그에 따라 각도 조정
	if (mb_imgToggle == true && mb_isReverse == false)
	{
		m_angle -= DEGREE_TO_RADIAN(220);
	}
	else if (mb_imgToggle == true && mb_isReverse == true)
	{
		m_angle += DEGREE_TO_RADIAN(220);
	}
	m_angle = -m_angle;
}

void BasicShortSword::SetEffectImgData()
{
	if (mb_isReverse == false)
	{
		m_effectImgRenderPos.x = m_playerPos.x + 55.0f;
	}
	else
	{
		m_effectImgRenderPos.x = m_playerPos.x - 55.0f;
	}
	m_effectImgRenderPos.y = m_playerPos.y + 12.0f;

	m_rotateAxis.x = m_playerPos.x;
	m_rotateAxis.y = m_playerPos.y + 12;

	m_effectAngle = GetAngle(m_rotateAxis, Input::GetMousePosition());
	m_effectAngle = -m_effectAngle;

	// 회전 변환 행렬 공식 적용한 것
	m_effectImgRenderPos = { m_rotateAxis.x + fabs(m_rotateAxis.x - m_effectImgRenderPos.x) * cosf(m_effectAngle)
									- fabs(m_rotateAxis.y - m_effectImgRenderPos.y) * sinf(m_effectAngle),

							m_rotateAxis.y + fabs(m_rotateAxis.x - m_effectImgRenderPos.x) * sinf(m_effectAngle)
									 + fabs(m_rotateAxis.y - m_effectImgRenderPos.y) * cosf(m_effectAngle) };
}

void BasicShortSword::SetHitboxShape()
{
	if (mb_isAttack == true)
	{
		m_shape.left = static_cast<long>(m_effectImgRenderPos.x - m_bssEffectImg->GetFrameWidth() * 0.5f);
		m_shape.right = static_cast<long>(m_effectImgRenderPos.x + m_bssEffectImg->GetFrameWidth() * 0.5f);
		m_shape.top = static_cast<long>(m_effectImgRenderPos.y - m_bssEffectImg->GetFrameHeight() * 0.5f);
		m_shape.bottom = static_cast<long>(m_effectImgRenderPos.y + m_bssEffectImg->GetFrameHeight() * 0.5f);
	}
	else
	{
		m_shape.left = 0;
		m_shape.right = 0;
		m_shape.top = 0;
		m_shape.bottom = 0;
	}
}
