#pragma once

#include "ItemBase.h"


class Image;
class BasicShortSword : public ItemBase
{
private:
	Image* m_bssImg = nullptr;

	POINTFLOAT m_bssImgRenderPos = {};
	RECT m_shape = {};

	float m_angle = 0.0f;

	bool mb_isReverse = false;

	/*POINT m_rectPointarr[3] = {};
	POINT m_leftTopPoint = {};
	POINT m_rightTopPoint = {};
	POINT m_leftBottomPoint = {};

	POINT m_rightBottomPoint = {};*/

public:
	virtual void Init(Player* player);
	virtual void Update();
	virtual void Render(HDC hdc);
	virtual void Release();

	void SetShape();
};

