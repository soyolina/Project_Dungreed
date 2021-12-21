#pragma once

#include "GameObject.h"

class Image;
class Ammo : public GameObject
{
private:
	Image* m_ammoImg = nullptr;

	int m_frameX = 0;
	int m_maxFrameX = 0;
	int m_frameY = 0;
	float m_elapsedCount = 0.0f;
	float m_frameChangeTime = 0.0f;

	float m_angle = 0.0f;
	float m_changeAngle = 0.0f;

public:
	virtual ~Ammo() {}

	virtual HRESULT Init(LPCWSTR imgName, POINTFLOAT pos, float angle, int attackDamage, float moveSpeed, ObjectType objType, float changeAngle = 0);
	virtual void Update();
	virtual void Render(HDC hdc);
	virtual void Release();

	void SetShape();
};

