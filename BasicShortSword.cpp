#include "stdafx.h"
#include "BasicShortSword.h"
#include "Image.h"
#include "Player.h"

#include <cmath>

void BasicShortSword::Init(Player* player)
{
	m_bssImg = IMAGE_MANAGER->FindImage(L"Image/Item/BasicShortSword_New.bmp");
	m_bssEffectImg = IMAGE_MANAGER->FindImage(L"Image/Item/BasicShortSwordEffect.bmp");

	m_player = player;
	m_playerPos = m_player->GetPlayerPos();
	m_bssImgRenderPos = {};
	

	m_angle = 0.0f;

	mb_isReverse = false;
	mb_isAttack = false;

	// 검 이펙트 이미지 용
	m_effectImgRenderPos = {};
	m_rotateAxis = {};
	m_effectAngle = 0.0f;
	
	m_frameX = 0;
	m_frameY = 0;
	m_maxFrameX = 3;
	m_elapsedCount = 0.0f;

	// 아이템 히트박스용
	m_hitboxShape = {};
	
	// PlgBlt 함수 쓰기 위해 만들어 놓은 것
	/*m_shape = {};
	m_leftTopPoint = {};
	m_rightTopPoint = {};
	m_leftBottomPoint = {};
	m_rightBottomPoint = {};

	for (int i = 0; i < 3; ++i)
	{
		m_rectPointarr[i] = {};
	}*/
}

void BasicShortSword::Update()
{
	m_playerPos = m_player->GetPlayerPos();

	// 이미지 반전 위해서
	mb_isReverse = m_player->GetPlayerReverse();
	
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
	else if(mb_imgToggle == true && mb_isReverse == true)
	{
		m_angle += DEGREE_TO_RADIAN(220);
	}
	m_angle = -m_angle;

	// 검 이펙트 이미지와 히트박스 용 - 플레이서 클래스에서 좌클릭을 누르면 mb_isAttack이 true가 됨.
	if (mb_isAttack == true)
	{
		// 이펙트 애니메이션 
		Animation();

		if (mb_isAttack2 == true)
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

			// 히트박스 shape 설정
			SetShape();

			mb_isAttack2 = false;
		}
	}
	
	// PlgBlt 이미지 렌더함수 쓰기 위한 용도였음. 마젠타 색이 안빠져서 실패.
	/*m_leftTopPoint = { (LONG)(m_bssImgRenderPos.x + (float)(m_shape.left - m_bssImgRenderPos.x) * cosf(m_angle) - (float)(m_shape.top - m_bssImgRenderPos.y) * sinf(m_angle)),
					(LONG)(m_bssImgRenderPos.y + (float)(m_shape.left - m_bssImgRenderPos.x) * sinf(m_angle) + (float)(m_shape.top - m_bssImgRenderPos.y) * cosf(m_angle)) };

	m_rightTopPoint = { (LONG)(m_bssImgRenderPos.x + (float)(m_shape.right - m_bssImgRenderPos.x) * cosf(m_angle) - (float)(m_shape.top - m_bssImgRenderPos.y) * sinf(m_angle)),
					(LONG)(m_bssImgRenderPos.y + (float)(m_shape.right - m_bssImgRenderPos.x) * sinf(m_angle) + (float)(m_shape.top - m_bssImgRenderPos.y) * cosf(m_angle)) };

	m_leftBottomPoint = { (LONG)(m_bssImgRenderPos.x + (float)(m_shape.left - m_bssImgRenderPos.x) * cosf(m_angle) - (float)(m_shape.bottom - m_bssImgRenderPos.y) * sinf(m_angle)),
					(LONG)(m_bssImgRenderPos.y + (float)(m_shape.left - m_bssImgRenderPos.x) * sinf(m_angle) + (float)(m_shape.bottom - m_bssImgRenderPos.y) * cosf(m_angle)) };


	m_rectPointarr[0] = m_leftTopPoint;
	m_rectPointarr[1] = m_rightTopPoint;
	m_rectPointarr[2] = m_leftBottomPoint;

	m_rightBottomPoint.x = m_rightTopPoint.x + m_leftBottomPoint.x - m_leftTopPoint.x;
	m_rightBottomPoint.y = m_rightTopPoint.y + m_leftBottomPoint.y - m_leftTopPoint.y;*/
}

void BasicShortSword::Render(HDC hdc)
{
	// 검 이미지 렌더
	m_bssImg->ImgRotateRender(hdc, static_cast<int>(m_bssImgRenderPos.x), static_cast<int>(m_bssImgRenderPos.y), m_angle);

	// 검 이펙트 이미지 렌더용
	if (mb_isAttack == true)
	{
		// 검 히트박스 
		//Rectangle(hdc, m_hitboxShape.left, m_hitboxShape.top, m_hitboxShape.right, m_hitboxShape.bottom);

		m_bssEffectImg->ImgRotateFrameRender(hdc, static_cast<int>(m_effectImgRenderPos.x), static_cast<int>(m_effectImgRenderPos.y), m_frameX, m_frameY, m_effectAngle);
		
		MoveToEx(hdc, m_hitboxShape.left, m_hitboxShape.top, NULL);
		LineTo(hdc, m_hitboxShape.left, m_hitboxShape.bottom);
		LineTo(hdc, m_hitboxShape.right, m_hitboxShape.bottom);
		LineTo(hdc, m_hitboxShape.right, m_hitboxShape.top);
		LineTo(hdc, m_hitboxShape.left, m_hitboxShape.top);
	}

	// PlgBlt 이미지 렌더함수 쓰기 위한 용도였음.
	/*m_bssImg->RotateRender2(hdc, m_rectPointarr);
	MoveToEx(hdc, m_leftTopPoint.x, m_leftTopPoint.y, NULL);
	LineTo(hdc, m_leftBottomPoint.x, m_leftBottomPoint.y);
	LineTo(hdc, m_rightBottomPoint.x, m_rightBottomPoint.y);
	LineTo(hdc, m_rightTopPoint.x, m_rightTopPoint.y);
	LineTo(hdc, m_leftTopPoint.x, m_leftTopPoint.y);*/
}

void BasicShortSword::Release()
{
	m_bssImg = nullptr;
	m_bssEffectImg = nullptr;
}

void BasicShortSword::Animation()
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

void BasicShortSword::SetShape()
{
	m_hitboxShape.left = m_effectImgRenderPos.x - m_bssEffectImg->GetFrameWidth() * 0.5f;
	m_hitboxShape.right = m_effectImgRenderPos.x + m_bssEffectImg->GetFrameWidth() * 0.5f;
	m_hitboxShape.top = m_effectImgRenderPos.y - m_bssEffectImg->GetFrameHeight() * 0.5f;
	m_hitboxShape.bottom = m_effectImgRenderPos.y + m_bssEffectImg->GetFrameHeight() * 0.5f;
}
