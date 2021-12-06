#pragma once

#include "Config.h"
#include "CommonFunction.h"
#include "Scene.h"

#define MAIN_LOGO_POS_X  WIN_SIZE_X / 2
#define MAIN_LOGO_POS_Y  WIN_SIZE_Y / 2 - 150

#define START_BTN_POS_X  WIN_SIZE_X / 2
#define START_BTN_POS_Y  WIN_SIZE_Y / 2 + 130
#define START_BTN_WIDTH  158
#define START_BTN_HEIGHT  47

#define QUIT_BTN_POS_X  WIN_SIZE_X / 2
#define QUIT_BTN_POS_Y  WIN_SIZE_Y / 2 + 200
#define QUIT_BTN_WIDTH  88
#define QUIT_BTN_HEIGHT  47


template <typename T>
class Button;

class Image;
class StartScene : public Scene
{
public:
	using Button = Button<StartScene>;

private:
	POINT m_startBtnPos = {};
	POINT m_quitBtnPos = {};

	Button* m_startBtn = nullptr;
	Button* m_quitBtn = nullptr;

	int m_backCloudSourX = 0;
	int m_midCloudSourX = 0;
	int m_frontCloudSourX = 0;

	Image* m_backCloud = nullptr;
	Image* m_midCloud = nullptr;
	Image* m_frontCloud = nullptr;
	Image* m_mainLogo = nullptr;
	Image* m_basicCursor = nullptr;

public:
	virtual ~StartScene() {}

	virtual HRESULT Init();
	virtual void Update();
	virtual void Render(HDC hdc);
	virtual void Release();

	void StartBtnFunc();
	void QuitBtnFunc();
};

