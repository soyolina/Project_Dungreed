#include "stdafx.h"
#include "ImageManager.h"
#include "Image.h"

void ImageManager::Init()
{
    // 태연 백그라운드
    IMAGE_MANAGER->AddImage(L"Image/Taeyeon.bmp", WIN_SIZE_X, WIN_SIZE_Y);
    
    // StartScene 관련
    IMAGE_MANAGER->AddImage(L"Image/StartScene/BackCloud.bmp", 2844, 800);
    IMAGE_MANAGER->AddImage(L"Image/StartScene/MidCloud.bmp", 2556, 800, true, RGB(255, 0, 255));
    IMAGE_MANAGER->AddImage(L"Image/StartScene/FrontCloud.bmp", 2556, 800, true, RGB(255, 0, 255));
    IMAGE_MANAGER->AddImage(L"Image/StartScene/MainLogo.bmp", 693, 333, true, RGB(255, 0, 255));
    IMAGE_MANAGER->AddImage(L"Image/StartScene/GameStart1.bmp", 158, 47, true, RGB(255, 0, 255));
    IMAGE_MANAGER->AddImage(L"Image/StartScene/GameStart2.bmp", 158, 47, true, RGB(255, 0, 255));
    IMAGE_MANAGER->AddImage(L"Image/StartScene/Quit1.bmp", 158, 47, true, RGB(255, 0, 255));
    IMAGE_MANAGER->AddImage(L"Image/StartScene/Quit2.bmp", 158, 47, true, RGB(255, 0, 255));
    IMAGE_MANAGER->AddImage(L"Image/StartScene/BasicCursor.bmp", 27, 33, true, RGB(255, 0, 255));

    // GameScene
    // Cursor
    IMAGE_MANAGER->AddImage(L"Image/UI/ShootingCursor2.bmp", 63, 63, true, RGB(255, 0, 255));
    // Player
    IMAGE_MANAGER->AddImage(L"Image/Player/baseCharIdle.bmp", 390, 150, 5, 2, true, RGB(255, 0, 255));
    IMAGE_MANAGER->AddImage(L"Image/Player/baseCharRun.bmp", 624, 150, 8, 2, true, RGB(255, 0, 255));
    IMAGE_MANAGER->AddImage(L"Image/Player/baseCharEffect.bmp", 390, 150, 5, 2, true, RGB(255, 0, 255));
    IMAGE_MANAGER->AddImage(L"Image/Player/basecharDie.bmp", 78, 75, true, RGB(255, 0, 255));
    IMAGE_MANAGER->AddImage(L"Image/Player/RunEffect.bmp", 240, 96, 5, 2, true, RGB(255, 0, 255));
    IMAGE_MANAGER->AddImage(L"Image/Player/basecharDie.bmp", 78, 75, true, RGB(255, 0, 255));
    IMAGE_MANAGER->AddImage(L"Image/Player/hit.bmp", 1440, 800, true, RGB(255, 0, 255));      // Hit 배경

    // UI
    // - DashCount
    IMAGE_MANAGER->AddImage(L"Image/UI/DashCount/DashBaseLeftEnd.bmp", 6, 24, true, RGB(255, 0, 255));
    IMAGE_MANAGER->AddImage(L"Image/UI/DashCount/DashBaseRightEnd.bmp", 6, 24, true, RGB(255, 0, 255));
    IMAGE_MANAGER->AddImage(L"Image/UI/DashCount/DashBase.bmp", 27, 24, true, RGB(255, 0, 255));
    IMAGE_MANAGER->AddImage(L"Image/UI/DashCount/DashCount.bmp", 27, 12, true, RGB(255, 0, 255));
    // - playerHP
    IMAGE_MANAGER->AddImage(L"Image/UI/HP2/PlayerLifeBase.bmp", 296, 64, true, RGB(255, 0, 255));
    IMAGE_MANAGER->AddImage(L"Image/UI/HP2/PlayerLifeBack.bmp", 296, 64, true, RGB(255, 0, 255));
    IMAGE_MANAGER->AddImage(L"Image/UI/HP2/LifeBarFull.bmp", 196, 40, true, RGB(255, 0, 255));

    // Item
    IMAGE_MANAGER->AddImage(L"Image/Item/BasicShortSword_New.bmp", 100, 100, true, RGB(255, 0, 255));
    IMAGE_MANAGER->AddImage(L"Image/Item/BasicShortSwordEffect.bmp", 300, 108, 3, 1, true, RGB(255, 0, 255));

    // Enemy
    // - Boss 
    // -- 벨리알 -- 
    
    // 본체 Idle
    IMAGE_MANAGER->AddImage(L"Image/Boss/SkellBossIdle.bmp", 2100, 285, 10, 1, true, RGB(255, 0, 255));
    // 보스가 미사일 패턴 공격시 본체 이미지
    IMAGE_MANAGER->AddImage(L"Image/Boss/SkellBossAttack.bmp", 2100, 384, 10, 1, true, RGB(255, 0, 255));   
    // 본체 피격
    IMAGE_MANAGER->AddImage(L"Image/Boss/SkellBossIdleHit.bmp", 2100, 285, 10, 1, true, RGB(255, 0, 255));
    // 보스가 미사일 패턴 공격때 피격시 본체 이미지
    IMAGE_MANAGER->AddImage(L"Image/Boss/SkellBossAttackHit.bmp", 2100, 384, 10, 1, true, RGB(255, 0, 255));
    // 본체 뒤 큰 구슬
    IMAGE_MANAGER->AddImage(L"Image/Boss/SkellBossBack.bmp", 1470, 144, 10, 1, true, RGB(255, 0, 255));
    // 본체 뒤 작은 구슬
    IMAGE_MANAGER->AddImage(L"Image/Boss/SkellBossParticle.bmp", 720, 90, 8, 1, true, RGB(255, 0, 255));
    // 보스가 죽을때 관련 이미지
    IMAGE_MANAGER->AddImage(L"Image/Boss/SkellBossDead.bmp", 210, 231, true, RGB(255, 0, 255));
    IMAGE_MANAGER->AddImage(L"Image/Boss/SkellBossDead0.bmp", 147, 93, true, RGB(255, 0, 255));
    IMAGE_MANAGER->AddImage(L"Image/Boss/DieEffect.bmp", 1320, 120, 11, 1, true, RGB(255, 0, 255));

    // Hand Idle (Left, Right)
    IMAGE_MANAGER->AddImage(L"Image/Boss/SkellBossLeftHandIdle.bmp", 1710, 189, 10, 1, true, RGB(255, 0, 255));
    IMAGE_MANAGER->AddImage(L"Image/Boss/SkellBossRightHandIdle.bmp", 1710, 189, 10, 1, true, RGB(255, 0, 255));
    // Hand Attack
    IMAGE_MANAGER->AddImage(L"Image/Boss/SkellBossLeftHandAttack.bmp", 3510, 213, 18, 1, true, RGB(255, 0, 255));
    IMAGE_MANAGER->AddImage(L"Image/Boss/SkellBossRightHandAttack.bmp", 3510, 213, 18, 1, true, RGB(255, 0, 255));
    // Hand Laser
    IMAGE_MANAGER->AddImage(L"Image/Boss/SkellBossLaserBody.bmp", 672, 294, 7, 2, true, RGB(255, 0, 255)); // Body
    IMAGE_MANAGER->AddImage(L"Image/Boss/SkellBossLaserHead.bmp", 567, 264, 7, 2, true, RGB(255, 0, 255)); // Head
    // Missile
    IMAGE_MANAGER->AddImage(L"Image/Boss/BossBullet.bmp", 156, 39, 4, 1, true, RGB(255, 0, 255));
    IMAGE_MANAGER->AddImage(L"Image/Boss/SkellBossBullet.bmp", 531, 52, 10, 1, true, RGB(255, 0, 255));
    IMAGE_MANAGER->AddImage(L"Image/Boss/BossBulletEffect.bmp", 744, 93, 8, 1, true, RGB(255, 0, 255));
    // Sword
    IMAGE_MANAGER->AddImage(L"Image/Boss/SkellBossSword0.bmp", 92, 92, true, RGB(255, 0, 255));     // Idle
    IMAGE_MANAGER->AddImage(L"Image/Boss/SkellBossSwordFire.bmp", 92, 92, true, RGB(255, 0, 255));  // Fire
    // Sword Effect
    IMAGE_MANAGER->AddImage(L"Image/Boss/destroyEffect.bmp", 558, 186, 3, 1, true, RGB(255, 0, 255));      // 칼 생성과 파괴시 이펙트
    IMAGE_MANAGER->AddImage(L"Image/Boss/BossSwordHitDown.bmp", 765, 66, 5, 1, true, RGB(255, 0, 255));    // 칼이 벽면과 충돌시 이펙트 들 
    IMAGE_MANAGER->AddImage(L"Image/Boss/BossSwordHitLeft.bmp", 330, 153, 5, 1, true, RGB(255, 0, 255));
    IMAGE_MANAGER->AddImage(L"Image/Boss/BossSwordHitRight.bmp", 330, 153, 5, 1, true, RGB(255, 0, 255));
    IMAGE_MANAGER->AddImage(L"Image/Boss/BossSwordHitUp.bmp", 765, 66, 5, 1, true, RGB(255, 0, 255));

    // HP UI
    IMAGE_MANAGER->AddImage(L"Image/Boss/BossLifeBack.bmp", 600, 77, true, RGB(255, 0, 255));
    IMAGE_MANAGER->AddImage(L"Image/Boss/BossLifeRed.bmp", 480, 77, true, RGB(255, 0, 255));
    IMAGE_MANAGER->AddImage(L"Image/Boss/BossLifeBase.bmp", 600, 77, true, RGB(255, 0, 255));

    // Tile Map Editor 용
    IMAGE_MANAGER->AddImage(L"Image/TileMapImage/TileMapImage2.bmp", 336, 192, 7, 4, true, RGB(255, 0, 255));
}

void ImageManager::Release()
{
    for (auto& elem : mapImages)
    {
        SAFE_RELEASE(elem.second);
    }
    mapImages.clear();
}

// 프레임이 없는 것
Image* ImageManager::AddImage(LPCWSTR fileName, int width, int height, bool isTrans, COLORREF transColor)
{
    // 이미지를 찾아보고 만약 있으면 추가 안함.
    if (FindImage(fileName))
    {
        return nullptr;
    }

    Image* img = new Image;
    if(FAILED(img->Init(fileName, width, height, isTrans, transColor)))
    {
        SAFE_RELEASE(img);
        return nullptr;
    }

    mapImages.insert(make_pair(fileName, img));

    return img;
}

// 프레임이 있는 것
Image* ImageManager::AddImage(LPCWSTR fileName, int width, int height, int maxFrameX, int maxFrameY, bool isTrans, COLORREF transColor)
{
    if (FindImage(fileName))
    {
        return nullptr;
    }

    Image* img = new Image;
    if (FAILED(img->Init(fileName, width, height, maxFrameX, maxFrameY, isTrans, transColor)))
    {
        SAFE_RELEASE(img);
        return nullptr;
    }

    mapImages.insert(make_pair(fileName, img));

    return img;
}

Image* ImageManager::FindImage(LPCWSTR fileName)
{
    map<wstring, Image*>::iterator it = mapImages.find(fileName);
    if (it == mapImages.end())
    {
        return nullptr;
    }

    return it->second;
}

void ImageManager::DeleteImage(LPCWSTR fileName)
{
    map<wstring, Image*>::iterator it = mapImages.find(fileName);
    if (it == mapImages.end())
    {
        return;
    }

    SAFE_RELEASE(it->second);		// Image* // Image 동적할당 해제
    mapImages.erase(it);
}
