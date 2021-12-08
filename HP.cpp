#include "HP.h"
#include "Image.h"
#include "Player.h"

void HP::Init()
{
	m_playerLifeBase = IMAGE_MANAGER->FindImage("Image/UI/HP2/PlayerLifeBase.bmp");
	m_playerLifeGray = IMAGE_MANAGER->FindImage("Image/UI/HP2/PlayerLifeBack.bmp");
	m_playerLifeGage = IMAGE_MANAGER->FindImage("Image/UI/HP2/LifeBarFull.bmp");

	m_player = nullptr;
	m_playerHP = 0;
	m_playerHpPercentage = m_playerLifeGage->GetWidth() / 100.0f;
}

void HP::Update()
{
	m_playerHP = m_player->GetPlayerHP();
}

void HP::Render(HDC hdc)
{
	m_playerLifeGray->RenderBasic(hdc, LIFEBASE_POSX, LIFEBASE_POSY);
	m_playerLifeGage->HpRender(hdc, static_cast<int>(LIFEGAGE_POSX), LIFEGAGE_POSY, m_playerHP * m_playerHpPercentage);
	m_playerLifeBase->RenderBasic(hdc, LIFEBASE_POSX, LIFEBASE_POSY);
}

void HP::Release()
{
	m_playerLifeBase = nullptr;
	m_playerLifeGray = nullptr;
	m_playerLifeGage = nullptr;
}
