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
    startBtn = IMAGE_MANAGER->FindImage("Image/StartScene/GameStart2.bmp");
    quitBtn = IMAGE_MANAGER->FindImage("Image/StartScene/Quit2.bmp");
    basicCursor = IMAGE_MANAGER->FindImage("Image/StartScene/BasicCursor.bmp");

    startRect = RectCenter(START_BTN_POS_X, START_BTN_POS_Y, startBtn->GetWidth(), startBtn->GetHeight());
    quitRect = RectCenter(QUIT_BTN_POS_X, QUIT_BTN_POS_Y, quitBtn->GetWidth() - 70, quitBtn->GetHeight());

    startBtnPos.x = START_BTN_POS_X;
    startBtnPos.y = START_BTN_POS_Y;

    quitBtnPos.x = QUIT_BTN_POS_X;
    quitBtnPos.y = QUIT_BTN_POS_Y;

    return S_OK;
}

void StartScene::Update()
{
    // ���������� �� �ҽ� �̹��� ���� ��ǥ ����
    backCloudSourX += 1 % backCloud->GetWidth();
    midCloudSourX += 2 % midCloud->GetWidth();
    frontCloudSourX += 3 % frontCloud->GetWidth();

    // ���� ���� ��ư
    if (PtInRect(&startRect, g_ptMouse))
    {
        startBtn = IMAGE_MANAGER->FindImage("Image/StartScene/GameStart1.bmp");

        if (KEY_MANAGER->IsOnceKeyDown(VK_LBUTTON))
        {
            SCENE_MANAGER->ChangeScene("���Ӿ�");
        }
    }
    else
    {
        startBtn = IMAGE_MANAGER->FindImage("Image/StartScene/GameStart2.bmp");
    }

    // ���� ��ư
    if (PtInRect(&quitRect, g_ptMouse))
    {
        quitBtn = IMAGE_MANAGER->FindImage("Image/StartScene/Quit1.bmp");

        if (KEY_MANAGER->IsOnceKeyDown(VK_LBUTTON))
        {
            PostQuitMessage(0);
        }
    }
    else
    {
        quitBtn = IMAGE_MANAGER->FindImage("Image/StartScene/Quit2.bmp");
    }
}

void StartScene::Render(HDC hdc)
{
    backCloud->loopRender(hdc, backCloudSourX);
    midCloud->loopRender(hdc, midCloudSourX);
    frontCloud->loopRender(hdc, frontCloudSourX);

    mainLogo->Render(hdc, MAIN_LOGO_POS_X, MAIN_LOGO_POS_Y);
    
    //Rectangle(hdc, startRect.left, startRect.top, startRect.right, startRect.bottom);
    startBtn->Render(hdc, START_BTN_POS_X, START_BTN_POS_Y);

    //Rectangle(hdc, quitRect.left, quitRect.top, quitRect.right, quitRect.bottom);
    quitBtn->Render(hdc, QUIT_BTN_POS_X, QUIT_BTN_POS_Y);

    basicCursor->RenderBasic(hdc, g_ptMouse.x, g_ptMouse.y);
}

void StartScene::Release()
{
}

void StartScene::StartBtnFunc()
{
}

void StartScene::QuitBtnFunc()
{
}
