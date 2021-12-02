#pragma once

#include "Config.h"
#include "CommonFunction.h"
#include "Scene.h"

#define MAIN_LOGO_POS_X  WIN_SIZE_X / 2
#define MAIN_LOGO_POS_Y  WIN_SIZE_Y / 2 - 150

#define START_BTN_POS_X  WIN_SIZE_X / 2
#define START_BTN_POS_Y  WIN_SIZE_Y / 2 + 130

#define QUIT_BTN_POS_X  WIN_SIZE_X / 2
#define QUIT_BTN_POS_Y  WIN_SIZE_Y / 2 + 200

template <typename T>
class Button;

class Image;
class StartScene : public Scene
{
public:
	using Button = Button<StartScene>;

private:
	POINT startBtnPos = {};
	POINT quitBtnPos = {};

	Button* startBtn2 = nullptr;
	Button* quitBtn2 = nullptr;



	RECT startRect = {};
	RECT quitRect = {};

	int backCloudSourX = 0;
	int midCloudSourX = 0;
	int frontCloudSourX = 0;

	Image* backCloud = nullptr;
	Image* midCloud = nullptr;
	Image* frontCloud = nullptr;
	Image* mainLogo = nullptr;
	Image* startBtn = nullptr;
	Image* quitBtn = nullptr;
	Image* basicCursor = nullptr;

public:
	virtual ~StartScene() {}

	virtual HRESULT Init();
	virtual void Update();
	virtual void Render(HDC hdc);
	virtual void Release();

	void StartBtnFunc();
	void QuitBtnFunc();
};

