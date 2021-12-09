#include "stdafx.h"
#include "GameScene.h"
#include "Image.h"
#include "Player.h"
#include "UIManager.h"
#include "BasicShortSword.h"

HRESULT GameScene::Init()
{
    taeYeon = IMAGE_MANAGER->FindImage(L"Image/Taeyeon.bmp");

    m_cursor = IMAGE_MANAGER->FindImage(L"Image/UI/ShootingCursor2.bmp");

    m_player = new Player;
    m_player->Init();

    m_UIManager = new UIManager;
    m_UIManager->Init(m_player);

    m_basicShortSword = new BasicShortSword;
    m_basicShortSword->Init(m_player);

    return S_OK;
}

void GameScene::Update()
{
    m_player->Update();

    m_UIManager->Update();

    m_basicShortSword->Update();
}

void GameScene::Render(HDC hdc)
{
    taeYeon->Render(hdc);

    m_basicShortSword->Render(hdc);

    m_player->Render(hdc);

    m_UIManager->Render(hdc);

    m_cursor->Render(hdc, Input::GetMousePosition().x, Input::GetMousePosition().y);

}

void GameScene::Release()
{
    SAFE_RELEASE(m_player);
    SAFE_RELEASE(m_UIManager);
    SAFE_RELEASE(m_basicShortSword);
}
