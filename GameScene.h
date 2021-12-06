#pragma once

#include "Config.h"
#include "CommonFunction.h"
#include "Scene.h"

class Player;
class Image;
class GameScene : public Scene
{
private:
	Image* taeYeon = nullptr;

	Image* m_cursor = nullptr;

	Player* m_player = nullptr;


public:
	virtual ~GameScene() {}

	virtual HRESULT Init();
	virtual void Update();
	virtual void Render(HDC hdc);
	virtual void Release();
};

