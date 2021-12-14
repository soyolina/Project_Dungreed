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

public:
	virtual ~GameObject() {}

	virtual HRESULT Init();
	virtual void Update();
	virtual void Render(HDC hdc);
	virtual void Release();
};

