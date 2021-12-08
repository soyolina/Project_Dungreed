#pragma once

#include "Config.h"

// ���� �θ� �� ��ü
class Scene
{
public:
	virtual ~Scene() {}

	virtual HRESULT Init();
	virtual void Update();
	virtual void Render(HDC hdc);
	virtual void Release();
};

