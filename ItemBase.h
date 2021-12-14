#pragma once

#include "Config.h"
#include "CommonFunction.h"

class Player;
class ItemBase
{
protected:
	Player* m_player = nullptr;
	POINTFLOAT m_playerPos = {};

	// 기본검 이미지 위치용이랑 플레이어와 아이템 렌더 순서용
	bool mb_imgToggle = false;

	// 공격인지 아닌지 셋팅
	bool mb_isAttack = false;
	bool mb_isAttack2 = false;

public:
	virtual void Init();
	virtual void Update();
	virtual void Render(HDC hdc);
	virtual void Release();

	// 아이템 이미지 렌더용 토글키
	void SetImgToggle() 
	{ 
		if (GetImgToggle() == false)
		{
			this->mb_imgToggle = true;
		}
		else
		{
			this->mb_imgToggle = false;
		}
	}
	bool GetImgToggle() const { return mb_imgToggle; }

	// 공격중인지 아닌지 셋팅
	void SetIsAttack(bool isAttack) { this->mb_isAttack = isAttack; }
	void SetIsAttack2(bool isAttack) { this->mb_isAttack2 = isAttack; }

	// 아이템 이펙트 이미지 렌더 위치 설정하기
	//void SetEffectImgData(POINTFLOAT playerPos, int posNumX, int posNumY, int axisNumX, int axisNumY, float angle);
};

