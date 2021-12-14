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

	// �� ����Ʈ �̹��� ��
	m_effectImgRenderPos = {};
	m_rotateAxis = {};
	m_effectAngle = 0.0f;
	
	m_frameX = 0;
	m_frameY = 0;
	m_maxFrameX = 3;
	m_elapsedCount = 0.0f;

	// ������ ��Ʈ�ڽ���
	m_hitboxShape = {};
	
	// PlgBlt �Լ� ���� ���� ����� ���� ��
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

	// �̹��� ���� ���ؼ�
	mb_isReverse = m_player->GetPlayerReverse();
	
	if (mb_isReverse == false)
	{
		m_bssImgRenderPos.x = m_playerPos.x + 20.0f;
		m_bssImgRenderPos.y = m_playerPos.y + 12.0f;
		// pos�� ���콺���� �������� ���⸦ �밢������ ������ֱ� ���� ���� ������
		m_angle = GetAngle(m_bssImgRenderPos, Input::GetMousePosition()) + DEGREE_TO_RADIAN(110);	
	}
	else
	{
		m_bssImgRenderPos.x = m_playerPos.x - 20.0f;
		m_bssImgRenderPos.y = m_playerPos.y + 12.0f;
		m_angle = GetAngle(m_bssImgRenderPos, Input::GetMousePosition()) - DEGREE_TO_RADIAN(110);
	}	

	// �÷��̾� Ŭ�������� ��Ŭ��(����) �� ������ mb_imgToggle�� ���� ������ �ϸ鼭, �׿� ���� ���� ����
	if (mb_imgToggle == true && mb_isReverse == false)
	{
		m_angle -= DEGREE_TO_RADIAN(220);
	}
	else if(mb_imgToggle == true && mb_isReverse == true)
	{
		m_angle += DEGREE_TO_RADIAN(220);
	}
	m_angle = -m_angle;

	// �� ����Ʈ �̹����� ��Ʈ�ڽ� �� - �÷��̼� Ŭ�������� ��Ŭ���� ������ mb_isAttack�� true�� ��.
	if (mb_isAttack == true)
	{
		// ����Ʈ �ִϸ��̼� 
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

			// ȸ�� ��ȯ ��� ���� ������ ��
			m_effectImgRenderPos = { m_rotateAxis.x + fabs(m_rotateAxis.x - m_effectImgRenderPos.x) * cosf(m_effectAngle)
											- fabs(m_rotateAxis.y - m_effectImgRenderPos.y) * sinf(m_effectAngle),

									m_rotateAxis.y + fabs(m_rotateAxis.x - m_effectImgRenderPos.x) * sinf(m_effectAngle)
											 + fabs(m_rotateAxis.y - m_effectImgRenderPos.y) * cosf(m_effectAngle) };

			// ��Ʈ�ڽ� shape ����
			SetShape();

			mb_isAttack2 = false;
		}
	}
	
	// PlgBlt �̹��� �����Լ� ���� ���� �뵵����. ����Ÿ ���� �Ⱥ����� ����.
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
	// �� �̹��� ����
	m_bssImg->ImgRotateRender(hdc, static_cast<int>(m_bssImgRenderPos.x), static_cast<int>(m_bssImgRenderPos.y), m_angle);

	// �� ����Ʈ �̹��� ������
	if (mb_isAttack == true)
	{
		// �� ��Ʈ�ڽ� 
		//Rectangle(hdc, m_hitboxShape.left, m_hitboxShape.top, m_hitboxShape.right, m_hitboxShape.bottom);

		m_bssEffectImg->ImgRotateFrameRender(hdc, static_cast<int>(m_effectImgRenderPos.x), static_cast<int>(m_effectImgRenderPos.y), m_frameX, m_frameY, m_effectAngle);
		
		MoveToEx(hdc, m_hitboxShape.left, m_hitboxShape.top, NULL);
		LineTo(hdc, m_hitboxShape.left, m_hitboxShape.bottom);
		LineTo(hdc, m_hitboxShape.right, m_hitboxShape.bottom);
		LineTo(hdc, m_hitboxShape.right, m_hitboxShape.top);
		LineTo(hdc, m_hitboxShape.left, m_hitboxShape.top);
	}

	// PlgBlt �̹��� �����Լ� ���� ���� �뵵����.
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
