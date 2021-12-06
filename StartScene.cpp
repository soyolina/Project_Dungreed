#include "StartScene.h"
#include "Image.h"
#include "Button.h"

HRESULT StartScene::Init()
{
    m_backCloud = IMAGE_MANAGER->FindImage("Image/StartScene/BackCloud.bmp");
    m_midCloud = IMAGE_MANAGER->FindImage("Image/StartScene/MidCloud.bmp");
    m_frontCloud = IMAGE_MANAGER->FindImage("Image/StartScene/FrontCloud.bmp");

    m_backCloudSourX = 0;
    m_midCloudSourX = 0;
    m_frontCloudSourX = 0;

    m_mainLogo = IMAGE_MANAGER->FindImage("Image/StartScene/MainLogo.bmp");
    m_basicCursor = IMAGE_MANAGER->FindImage("Image/StartScene/BasicCursor.bmp");

    m_startBtnPos.x = START_BTN_POS_X;
    m_startBtnPos.y = START_BTN_POS_Y;

    m_startBtn = new Button(this, &StartScene::StartBtnFunc);
    m_startBtn->Init(m_startBtnPos, START_BTN_WIDTH, START_BTN_HEIGHT, "Image/StartScene/GameStart2.bmp", "Image/StartScene/GameStart1.bmp");

    m_quitBtnPos.x = QUIT_BTN_POS_X;
    m_quitBtnPos.y = QUIT_BTN_POS_Y;

    m_quitBtn = new Button(this, &StartScene::QuitBtnFunc);
    m_quitBtn->Init(m_quitBtnPos, QUIT_BTN_WIDTH, QUIT_BTN_HEIGHT, "Image/StartScene/Quit2.bmp", "Image/StartScene/Quit1.bmp");

    return S_OK;
}

void StartScene::Update()
{
    // ���������� �� �ҽ� �̹��� ���� ��ǥ ����
    m_backCloudSourX = (m_backCloudSourX + 2) % m_backCloud->GetWidth();
    m_midCloudSourX = (m_midCloudSourX + 3) % m_midCloud->GetWidth();
    m_frontCloudSourX = (m_frontCloudSourX + 4) % m_frontCloud->GetWidth();

    if(m_startBtn != nullptr)
        m_startBtn->Update();

    if(m_quitBtn != nullptr)
        m_quitBtn->Update();
}

void StartScene::Render(HDC hdc)
{
    m_backCloud->loopRender(hdc, m_backCloudSourX);
    m_midCloud->loopRender(hdc, m_midCloudSourX);
    m_frontCloud->loopRender(hdc, m_frontCloudSourX);

    m_mainLogo->Render(hdc, MAIN_LOGO_POS_X, MAIN_LOGO_POS_Y);
    
    m_startBtn->Render(hdc);
    m_quitBtn->Render(hdc);

    m_basicCursor->RenderBasic(hdc, g_ptMouse.x, g_ptMouse.y);
}

void StartScene::Release()
{
    SAFE_RELEASE(m_startBtn);
    SAFE_RELEASE(m_quitBtn);
}

void StartScene::StartBtnFunc()
{
    SCENE_MANAGER->ChangeScene("���Ӿ�");
}

void StartScene::QuitBtnFunc()
{
    PostQuitMessage(0);
}
