#include "StartScene.h"
#include "Image.h"
#include "Button.h"

HRESULT StartScene::Init()
{
    backCloud = IMAGE_MANAGER->FindImage("Image/StartScene/BackCloud.bmp");
    midCloud = IMAGE_MANAGER->FindImage("Image/StartScene/MidCloud.bmp");
    frontCloud = IMAGE_MANAGER->FindImage("Image/StartScene/FrontCloud.bmp");

    backCloudSourX = 0;
    midCloudSourX = 0;
    frontCloudSourX = 0;

    mainLogo = IMAGE_MANAGER->FindImage("Image/StartScene/MainLogo.bmp");
    basicCursor = IMAGE_MANAGER->FindImage("Image/StartScene/BasicCursor.bmp");

    startBtnPos.x = START_BTN_POS_X;
    startBtnPos.y = START_BTN_POS_Y;

    startBtn = new Button(this, &StartScene::StartBtnFunc);
    startBtn->Init(startBtnPos, START_BTN_WIDTH, START_BTN_HEIGHT, "Image/StartScene/GameStart2.bmp", "Image/StartScene/GameStart1.bmp");

    quitBtnPos.x = QUIT_BTN_POS_X;
    quitBtnPos.y = QUIT_BTN_POS_Y;

    quitBtn = new Button(this, &StartScene::QuitBtnFunc);
    quitBtn->Init(quitBtnPos, QUIT_BTN_WIDTH, QUIT_BTN_HEIGHT, "Image/StartScene/Quit2.bmp", "Image/StartScene/Quit1.bmp");

    return S_OK;
}

void StartScene::Update()
{
    // 루프렌더의 각 소스 이미지 시작 좌표 갱신
    backCloudSourX += 1 % backCloud->GetWidth();
    midCloudSourX += 2 % midCloud->GetWidth();
    frontCloudSourX += 3 % frontCloud->GetWidth();

    if(startBtn != nullptr)
        startBtn->Update();

    if(quitBtn != nullptr)
        quitBtn->Update();
}

void StartScene::Render(HDC hdc)
{
    backCloud->loopRender(hdc, backCloudSourX);
    midCloud->loopRender(hdc, midCloudSourX);
    frontCloud->loopRender(hdc, frontCloudSourX);

    mainLogo->Render(hdc, MAIN_LOGO_POS_X, MAIN_LOGO_POS_Y);
    
    startBtn->Render(hdc);
    quitBtn->Render(hdc);

    basicCursor->RenderBasic(hdc, g_ptMouse.x, g_ptMouse.y);
}

void StartScene::Release()
{
    SAFE_RELEASE(startBtn);
    SAFE_RELEASE(quitBtn);
}

void StartScene::StartBtnFunc()
{
    SCENE_MANAGER->ChangeScene("게임씬");
}

void StartScene::QuitBtnFunc()
{
    PostQuitMessage(0);
}
