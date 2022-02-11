#pragma once

#include "Config.h"
#include "CommonFunction.h"
#include "GameObject.h"

class Player;
class ItemBase : public GameObject
{
protected:
	Player* m_player = nullptr;
	POINTFLOAT m_playerPos = {};

	// 기본검 이미지 위치용이랑 플레이어와 아이템 렌더 순서용
	bool mb_imgToggle = false;

	// 공격인지 아닌지 셋팅
	bool mb_isAttack = false;
	bool mb_isAttack2 = false;

	// 아이템 공격 스피드
	float m_attackSpeed = 0.0f;

public:
	virtual HRESULT Init();
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
};

