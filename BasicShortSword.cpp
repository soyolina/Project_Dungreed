#include "stdafx.h"
#include "BasicShortSword.h"
#include "Image.h"
#include "Player.h"

#include <cmath>

void BasicShortSword::Init(Player* player)
{
	m_bssImg = IMAGE_MANAGER->FindImage(L"Image/Item/BasicShortSword_New.bmp");

	m_player = player;
	m_playerPos = m_player->GetPlayerPos();
	m_bssImgRenderPos = {};
	
	m_shape = {};

	m_angle = 0.0f;

	for (int i = 0; i < 3; ++i)
	{
		m_rectPointarr[i] = {};
	}

	m_leftTopPoint = {};
	m_rightTopPoint = {};
	m_leftBottomPoint = {};

	mb_isReverse = false;
}

void BasicShortSword::Update()
{
	m_playerPos = m_player->GetPlayerPos();

	// 이미지 반전 위해서
	mb_isReverse = m_player->GetPlayerReverse();
	
	if (mb_isReverse == false)
	{
		m_bssImgRenderPos.x = m_playerPos.x + 10.0f;
		m_bssImgRenderPos.y = m_playerPos.y + 12.0f;
	}
	else
	{
		m_bssImgRenderPos.x = m_playerPos.x - 10.0f;
		m_bssImgRenderPos.y = m_playerPos.y + 12.0f;
	}

	SetShape();
	
	m_angle = GetAngle(m_bssImgRenderPos, Input::GetMousePosition());

	m_angle = -m_angle;

	// PlgBlt 이미지 렌더함수 쓰기 위한 용도였음.
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
	/*MoveToEx(hdc, m_leftTopPoint.x, m_leftTopPoint.y, NULL);
	LineTo(hdc, m_leftBottomPoint.x, m_leftBottomPoint.y);
	LineTo(hdc, m_rightBottomPoint.x, m_rightBottomPoint.y);
	LineTo(hdc, m_rightTopPoint.x, m_rightTopPoint.y);
	LineTo(hdc, m_leftTopPoint.x, m_leftTopPoint.y);*/

	// PlgBlt 이미지 렌더함수 쓰기 위한 용도였음.
	/*m_bssImg->RotateRender2(hdc, m_rectPointarr);*/
	 
	Rectangle(hdc, m_shape.left, m_shape.top, m_shape.right, m_shape.bottom);


	m_bssImg->ImgRotateRender(hdc, m_bssImgRenderPos.x, m_bssImgRenderPos.y, m_angle);

	//m_bssImg->Render(hdc, static_cast<int>(m_bssImgRenderPos.x), static_cast<int>(m_bssImgRenderPos.y), 1.0f, mb_isReverse);
}

void BasicShortSword::Release()
{
	m_bssImg = nullptr;
}

void BasicShortSword::SetShape()
{
	m_shape.left = m_bssImgRenderPos.x - m_bssImg->GetWidth() * 0.5;
	m_shape.top = m_bssImgRenderPos.y - m_bssImg->GetHeight() * 0.5;
	m_shape.right = m_bssImgRenderPos.x + m_bssImg->GetWidth() * 0.5;
	m_shape.bottom = m_bssImgRenderPos.y + m_bssImg->GetHeight() * 0.5;
}
