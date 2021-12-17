#pragma once

#include "Config.h"
#include "CommonFunction.h"

class GameObject
{
protected:
	POINTFLOAT m_pos = {};
	int m_bodyWidth = 0;
	int m_bodyHeight = 0;
	RECT m_shape = {};

	int m_hp = 0;
	float m_moveSpeed = 0.0f;
	int m_attackDamage = 0;
	bool mb_isHit = false;
	float m_hitElapsedCount = 0.0f;
	bool mb_isDead = false;

public:
	virtual ~GameObject() {}

	virtual HRESULT Init();
	virtual void Update();
	virtual void Render(HDC hdc);
	virtual void Release();

	// Getter, SetterÇÔ¼ö
	int GetHp() const { return m_hp; }
	void SetHp(int num) { this->m_hp -= num; }

	int GetAttackDamage() const { return m_attackDamage; }

	bool GetIsHit() const { return mb_isHit; }
	void SetIsHit(bool isHit) { this->mb_isHit = isHit; }

	bool GetIsDead() const { return mb_isDead; }
	void SetIsDead(bool isDead) { this->mb_isDead = isDead; }
};

