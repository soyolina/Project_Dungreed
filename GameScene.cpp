#include "stdafx.h"
#include "GameScene.h"
#include "Image.h"
#include "Player.h"
#include "UIManager.h"
#include "BasicShortSword.h"
#include "Bellial.h"
#include "AmmoManager.h"
#include "DrawTilemap.h"

HRESULT GameScene::Init()
{
    taeYeon = IMAGE_MANAGER->FindImage(L"Image/Taeyeon.bmp");

    m_cursor = IMAGE_MANAGER->FindImage(L"Image/UI/ShootingCursor2.bmp");

    // Ÿ�ϸ� 
    m_bellialStageBackground = new DrawTilemap;
    m_bellialStageBackground->Init(static_cast<int>(mapName::BellialStageBackgournd));
    m_bellialStageForeground = new DrawTilemap;
    m_bellialStageForeground->Init(static_cast<int>(mapName::BellialStageForeground));


    m_ammoManager = new AmmoManager;


    m_player = new Player;
    m_player->Init();

    // UI�����ϴ� �Ŵ���
    m_UIManager = new UIManager;
    m_UIManager->Init(m_player);

    // ������ �⺻��
    m_basicShortSword = new BasicShortSword;
    m_basicShortSword->Init(m_player);

    // ������ ����
    m_player->SetItem(0, m_basicShortSword);

    // ����
    m_bellial = new Bellial;
    m_bellial->Init();
    m_bellial->SetPlayer(m_player);
    m_bellial->SetAmmoManager(m_ammoManager);

    return S_OK;
}

void GameScene::Update()
{
    m_player->Update();

    m_UIManager->Update();

    m_basicShortSword->Update();

    m_bellial->Update();

    m_ammoManager->Update();

    // collider
    ColliderManager::CheckCollision();
}

void GameScene::Render(HDC hdc)
{
    taeYeon->Render(hdc);

    // Ÿ�ϸ�
    m_bellialStageBackground->Render(hdc);
    m_bellialStageForeground->Render(hdc);

    // collider
    if (Input::GetButton(VK_F5))
    {
        ColliderManager::Render(hdc);
    }

    m_bellial->Render(hdc);

    // ��� ���¿� ���� �̹��� ���� ���� �޶����� �ҷ���
    if (m_basicShortSword->GetImgToggle() == false)
    {
		m_basicShortSword->Render(hdc);
        m_player->Render(hdc);
    }

    if (m_basicShortSword->GetImgToggle() == true)
    {
        m_player->Render(hdc);
        m_basicShortSword->Render(hdc);
    }

    m_ammoManager->Render(hdc);

    m_UIManager->Render(hdc);

    m_cursor->Render(hdc, Input::GetMousePosition().x, Input::GetMousePosition().y);

}

void GameScene::Release()
{
    SAFE_RELEASE(m_player);
    SAFE_RELEASE(m_UIManager);
    SAFE_RELEASE(m_basicShortSword);
    SAFE_RELEASE(m_bellial);
    SAFE_RELEASE(m_ammoManager);
    SAFE_RELEASE(m_bellialStageBackground);
    SAFE_RELEASE(m_bellialStageForeground);
}
