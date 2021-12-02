#pragma once

#include "Config.h"
#include "CommonFunction.h"
#include "Scene.h"

class Image;
class GameScene : public Scene
{
private:
	Image* taeYeon = nullptr;


public:
	virtual HRESULT Init();
	virtual void Update();
	virtual void Render(HDC hdc);
	virtual void Release();
};

