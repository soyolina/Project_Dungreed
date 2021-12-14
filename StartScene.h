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
	// - 버튼 관련
	// 변수
	Button* m_startBtn = nullptr;
	Button* m_quitBtn = nullptr;
	POINT m_startBtnPos = {};
	POINT m_quitBtnPos = {};
	// 함수
	void StartBtnFunc();
	void QuitBtnFunc();

	// - 이미지 루프랜더에서
	// 쓸 이미지 
	Image* m_backCloud = nullptr;
	Image* m_midCloud = nullptr;
	Image* m_frontCloud = nullptr;
	// 쓸 이미지 원본(소스) 렌더 위치 posX 관련
	int m_backCloudSourX = 0;
	int m_midCloudSourX = 0;
	int m_frontCloudSourX = 0;

	// 메인로고 이미지
	Image* m_mainLogo = nullptr;
	
	// 커서 이미지
	Image* m_basicCursor = nullptr;

public:
	virtual ~StartScene() {}

	virtual HRESULT Init();
	virtual void Update();
	virtual void Render(HDC hdc);
	virtual void Release();
};

