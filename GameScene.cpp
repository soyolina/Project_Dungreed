#include "GameScene.h"
#include "Image.h"

HRESULT GameScene::Init()
{
    taeYeon = IMAGE_MANAGER->FindImage("Image/Taeyeon.bmp");

    return S_OK;
}

void GameScene::Update()
{
}

void GameScene::Render(HDC hdc)
{
    taeYeon->Render(hdc);
}

void GameScene::Release()
{
}
