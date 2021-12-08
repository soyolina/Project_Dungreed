#pragma once

#include "Config.h"

// ¾ÀÀÇ ºÎ¸ð°¡ µÉ °´Ã¼
class Scene
{
public:
	virtual ~Scene() {}

	virtual HRESULT Init();
	virtual void Update();
	virtual void Render(HDC hdc);
	virtual void Release();
};

