#pragma once

#include "ItemBase.h"


class Image;
class BasicShortSword : public ItemBase
{
private:
	bool mb_isReverse = false;
	
	// - 기본검 이미지용 
	// 변수
	Image* m_bssImg = nullptr;
	POINTFLOAT m_bssImgRenderPos = {};
	float m_angle = 0.0f;
	// 함수
	void SetBssImgData();

	// - 검 이펙트 이미지 데이터용
	// 변수
	Image* m_bssEffectImg = nullptr;
	POINTFLOAT m_effectImgRenderPos = {};
	POINTFLOAT m_rotateAxis = {};
	float m_effectAngle = 0.0f;
	// 함수
	void SetEffectImgData();

	// - 검 이펙트 애니메이션용
	// 변수
	int m_frameX = 0; 
	int m_frameY = 0;
	int m_maxFrameX = 0;
	float m_elapsedCount = 0.0f;
	// 함수
	void PlayAnimation();

	// - 아이템 히트박스용
	// 변수
	RECT m_shape = {};
	// 함수
	void SetHitboxShape();

public:
	virtual HRESULT Init(Player* player);
	virtual void Update();
	virtual void Render(HDC hdc);
	virtual void Release();
};

