#pragma once

#include "GameObject.h"

class Enemy : public GameObject
{
protected:

public:
	virtual ~Enemy() {}

	virtual HRESULT Init();
	virtual void Update();
	virtual void Render(HDC hdc);
	virtual void Release();
};

