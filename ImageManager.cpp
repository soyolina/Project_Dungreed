#include "ImageManager.h"
#include "Image.h"

void ImageManager::Init()
{
    // �¿� ��׶���
    IMAGE_MANAGER->AddImage("Image/Taeyeon.bmp", WIN_SIZE_X, WIN_SIZE_Y);
    
    // StartScene ����
    IMAGE_MANAGER->AddImage("Image/StartScene/BackCloud.bmp", 2844, 800);
    IMAGE_MANAGER->AddImage("Image/StartScene/MidCloud.bmp", 2556, 800, true, RGB(255, 0, 255));
    IMAGE_MANAGER->AddImage("Image/StartScene/FrontCloud.bmp", 2556, 800, true, RGB(255, 0, 255));
    IMAGE_MANAGER->AddImage("Image/StartScene/MainLogo.bmp", 693, 333, true, RGB(255, 0, 255));
    IMAGE_MANAGER->AddImage("Image/StartScene/GameStart1.bmp", 158, 47, true, RGB(255, 0, 255));
    IMAGE_MANAGER->AddImage("Image/StartScene/GameStart2.bmp", 158, 47, true, RGB(255, 0, 255));
    IMAGE_MANAGER->AddImage("Image/StartScene/Quit1.bmp", 158, 47, true, RGB(255, 0, 255));
    IMAGE_MANAGER->AddImage("Image/StartScene/Quit2.bmp", 158, 47, true, RGB(255, 0, 255));
    IMAGE_MANAGER->AddImage("Image/StartScene/BasicCursor.bmp", 27, 33, true, RGB(255, 0, 255));

    // GameScene
    // Cursor
    IMAGE_MANAGER->AddImage("Image/UI/ShootingCursor2.bmp", 63, 63, true, RGB(255, 0, 255));
    // Player
    IMAGE_MANAGER->AddImage("Image/Player/baseCharIdle.bmp", 390, 150, 5, 2, true, RGB(255, 0, 255));
    IMAGE_MANAGER->AddImage("Image/Player/baseCharRun.bmp", 624, 150, 8, 2, true, RGB(255, 0, 255));
    IMAGE_MANAGER->AddImage("Image/Player/baseCharEffect.bmp", 390, 150, 5, 2, true, RGB(255, 0, 255));
    IMAGE_MANAGER->AddImage("Image/Player/basecharDie.bmp", 78, 75, true, RGB(255, 0, 255));
    IMAGE_MANAGER->AddImage("Image/Player/RunEffect.bmp", 240, 96, 5, 2, true, RGB(255, 0, 255));

    // UI
    // DashCount
    IMAGE_MANAGER->AddImage("Image/UI/DashCount/DashBaseLeftEnd.bmp", 6, 24, true, RGB(255, 0, 255));
    IMAGE_MANAGER->AddImage("Image/UI/DashCount/DashBaseRightEnd.bmp", 6, 24, true, RGB(255, 0, 255));
    IMAGE_MANAGER->AddImage("Image/UI/DashCount/DashBase.bmp", 27, 24, true, RGB(255, 0, 255));
    IMAGE_MANAGER->AddImage("Image/UI/DashCount/DashCount.bmp", 27, 12, true, RGB(255, 0, 255));
    // playerHP
    IMAGE_MANAGER->AddImage("Image/UI/HP2/PlayerLifeBase.bmp", 296, 64, true, RGB(255, 0, 255));
    IMAGE_MANAGER->AddImage("Image/UI/HP2/PlayerLifeBack.bmp", 296, 64, true, RGB(255, 0, 255));
    IMAGE_MANAGER->AddImage("Image/UI/HP2/LifeBarFull.bmp", 196, 40, true, RGB(255, 0, 255));

}

void ImageManager::Release()
{
    for (auto& elem : mapImages)
    {
        SAFE_RELEASE(elem.second);
    }
    mapImages.clear();
}

// �������� ���� ��
Image* ImageManager::AddImage(const char* fileName, int width, int height, bool isTrans, COLORREF transColor)
{
    // �̹����� ã�ƺ��� ���� ������ �߰� ����.
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

// �������� �ִ� ��
Image* ImageManager::AddImage(const char* fileName, int width, int height, int maxFrameX, int maxFrameY, bool isTrans, COLORREF transColor)
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

Image* ImageManager::FindImage(const char* fileName)
{
    map<string, Image*>::iterator it = mapImages.find(fileName);
    if (it == mapImages.end())
    {
        return nullptr;
    }

    return it->second;
}

void ImageManager::DeleteImage(const char* fileName)
{
    map<string, Image*>::iterator it = mapImages.find(fileName);
    if (it == mapImages.end())
    {
        return;
    }

    SAFE_RELEASE(it->second);		// Image* // Image �����Ҵ� ����
    mapImages.erase(it);
}
