#pragma once

#include "ItemBase.h"


class Image;
class BasicShortSword : public ItemBase
{
private:
	Image* m_bssImg = nullptr;
	Image* m_bssEffectImg = nullptr;

	POINTFLOAT m_bssImgRenderPos = {};
	float m_angle = 0.0f;

	bool mb_isReverse = false;

	// 검 이펙트 이미지용
	POINTFLOAT m_effectImgRenderPos = {};
	POINTFLOAT m_rotateAxis = {};
	float m_effectAngle = 0.0f;

	int m_frameX = 0; 
	int m_frameY = 0;
	int m_maxFrameX = 0;
	float m_elapsedCount = 0.0f;

	// 아이템 히트박스용
	RECT m_hitboxShape = {};

	// PlgBlt함수용
	/*RECT m_shape = {};
	POINT m_leftTopPoint = {};
	POINT m_rightTopPoint = {};
	POINT m_leftBottomPoint = {};
	POINT m_rightBottomPoint = {};
	POINT m_rectPointarr[3] = {};*/

public:
	virtual void Init(Player* player);
	virtual void Update();
	virtual void Render(HDC hdc);
	virtual void Release();

	void Animation();
	void SetShape();
};

