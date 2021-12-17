#pragma once

#include "ItemBase.h"


class Image;
class BasicShortSword : public ItemBase
{
private:
	
	bool mb_isReverse = false;
	
	// - �⺻�� �̹����� 
	// ����
	Image* m_bssImg = nullptr;
	POINTFLOAT m_bssImgRenderPos = {};
	float m_angle = 0.0f;
	// �Լ�
	void SetBssImgData();

	// - �� ����Ʈ �̹��� �����Ϳ�
	// ����
	Image* m_bssEffectImg = nullptr;
	POINTFLOAT m_effectImgRenderPos = {};
	POINTFLOAT m_rotateAxis = {};
	float m_effectAngle = 0.0f;
	// �Լ�
	void SetEffectImgData();

	// - �� ����Ʈ �ִϸ��̼ǿ�
	// ����
	int m_frameX = 0; 
	int m_frameY = 0;
	int m_maxFrameX = 0;
	float m_elapsedCount = 0.0f;
	// �Լ�
	void PlayAnimation();

	// - ������ ��Ʈ�ڽ���
	// ����
	RECT m_hitboxShape = {};
	// �Լ�
	void SetHitboxShape();

	// PlgBlt�Լ���
	/*RECT m_shape = {};
	POINT m_leftTopPoint = {};
	POINT m_rightTopPoint = {};
	POINT m_leftBottomPoint = {};
	POINT m_rightBottomPoint = {};
	POINT m_rectPointarr[3] = {};*/

public:
	virtual HRESULT Init(Player* player);
	virtual void Update();
	virtual void Render(HDC hdc);
	virtual void Release();
};

