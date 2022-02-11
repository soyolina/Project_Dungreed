#pragma once

#include "Config.h"
#include "Collider2.h"

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
	bool mb_isHit2 = false;	// AmmoClass���� ���̴� ��
	float m_hitElapsedCount = 0.0f;
	bool mb_isDead = false;

	// ��¥ �ݶ��̴�
	Collider collider = {};

	// �浹�� ���ڿ� �浹�� ������ ��ġ�� �κ�(����)
	RECT collidedRect = {};
	RECT intersectRect = {};

public:
	virtual ~GameObject() {}

	virtual HRESULT Init();
	virtual void Update();
	virtual void Render(HDC hdc);
	virtual void Release();

	// Getter, Setter�Լ�
	int GetHp() const { return m_hp; }
	void SetHp(int num) { this->m_hp -= num; }

	int GetAttackDamage() const { return m_attackDamage; }

	bool GetIsHit() const { return mb_isHit; }
	void SetIsHit(bool isHit) { this->mb_isHit = isHit; }

	void SetIsHit2(bool isHit2) { this->mb_isHit2 = isHit2; }

	bool GetIsDead() const { return mb_isDead; }
	void SetIsDead(bool isDead) { this->mb_isDead = isDead; }

	// �ʰ� �浹�� ����
	RECT GetCollidedRect() const { return collidedRect; }
	void SetCollidedRect(RECT rc) { collidedRect = rc; }

	// �浹�Ȼ��ڿ� ��ġ�� �κ�(����)
	RECT GetIntersectRect() const { return intersectRect; }
	void SetIntersectRect(RECT rc) { intersectRect = rc; }
};

