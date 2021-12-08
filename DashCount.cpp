#include "DashCount.h"
#include "Image.h"
#include "Player.h"

void DashCount::Init()
{
	m_DashBarBaseLeft = IMAGE_MANAGER->FindImage("Image/UI/DashCount/DashBaseLeftEnd.bmp");
	m_DashBarBaseRight = IMAGE_MANAGER->FindImage("Image/UI/DashCount/DashBaseRightEnd.bmp");
	m_DashBarBase = IMAGE_MANAGER->FindImage("Image/UI/DashCount/DashBase.bmp");

	m_DashCountGage = IMAGE_MANAGER->FindImage("Image/UI/DashCount/DashCount.bmp");

	m_player = nullptr;
	m_playerMaxDashCount = 0;
	m_playerDashCount = 0;
}

void DashCount::Update()
{
	m_playerMaxDashCount = m_player->GetPlayerMaxDashCount();
	m_playerDashCount = m_player->GetPlayerDashCount();
}

void DashCount::Render(HDC hdc)
{
	// 대쉬 바 에 관한 이미지 렌더
	m_DashBarBaseLeft->RenderBasic(hdc, DASHBAR_LEFTEND_POSX, DASHBAR_LEFTEND_POSY);

	int DashBarBasePosX = DASHBAR_LEFTEND_POSX + m_DashBarBaseLeft->GetWidth();
	for (int i = 0; i < m_playerMaxDashCount; ++i)
	{
		m_DashBarBase->RenderBasic(hdc, DashBarBasePosX, DASHBAR_LEFTEND_POSY);
		DashBarBasePosX += m_DashBarBase->GetWidth();
	}

	m_DashBarBaseRight->RenderBasic(hdc, DashBarBasePosX, DASHBAR_LEFTEND_POSY);
	

	// 대쉬 게이지 에 관한 이미지 렌더
	int DashGagePosX = DASHBAR_LEFTEND_POSX + m_DashBarBaseLeft->GetWidth() + static_cast<int>(m_DashBarBase->GetWidth() * 0.5f);
	int DashGagePosY = DASHBAR_LEFT_POSY + m_DashBarBaseLeft->GetHeight() * 0.5;
	for (int i = 0; i < m_playerDashCount; ++i)
	{
		m_DashCountGage->Render(hdc, DashGagePosX, DashGagePosY);
		DashGagePosX += m_DashBarBase->GetWidth();
	}
}

void DashCount::Release()
{
	m_DashBarBase = nullptr;
	m_DashCountGage = nullptr;
}
