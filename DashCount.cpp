#include "stdafx.h"
#include "DashCount.h"
#include "Image.h"
#include "Player.h"

void DashCount::Init()
{
	m_dashBarBaseLeft = IMAGE_MANAGER->FindImage(L"Image/UI/DashCount/DashBaseLeftEnd.bmp");
	m_dashBarBaseRight = IMAGE_MANAGER->FindImage(L"Image/UI/DashCount/DashBaseRightEnd.bmp");
	m_dashBarBase = IMAGE_MANAGER->FindImage(L"Image/UI/DashCount/DashBase.bmp");

	m_dashCountGage = IMAGE_MANAGER->FindImage(L"Image/UI/DashCount/DashCount.bmp");

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
	m_dashBarBaseLeft->RenderBasic(hdc, DASHBAR_LEFTEND_POSX, DASHBAR_LEFTEND_POSY);

	int dashBarBasePosX = DASHBAR_LEFTEND_POSX + m_dashBarBaseLeft->GetWidth();
	for (int i = 0; i < m_playerMaxDashCount; ++i)
	{
		m_dashBarBase->RenderBasic(hdc, dashBarBasePosX, DASHBAR_LEFTEND_POSY);
		dashBarBasePosX += m_dashBarBase->GetWidth();
	}

	m_dashBarBaseRight->RenderBasic(hdc, dashBarBasePosX, DASHBAR_LEFTEND_POSY);
	

	// 대쉬 게이지 에 관한 이미지 렌더
	int dashGagePosX = DASHBAR_LEFTEND_POSX + m_dashBarBaseLeft->GetWidth() + static_cast<int>(m_dashBarBase->GetWidth() * 0.5f);
	int dashGagePosY = static_cast<int>(DASHBAR_LEFTEND_POSY + m_dashBarBaseLeft->GetHeight() * 0.5f);
	for (int i = 0; i < m_playerDashCount; ++i)
	{
		m_dashCountGage->Render(hdc, dashGagePosX, dashGagePosY);
		dashGagePosX += m_dashBarBase->GetWidth();
	}
}

void DashCount::Release()
{
	m_dashBarBaseLeft = nullptr;
	m_dashBarBaseRight = nullptr;
	m_dashBarBase = nullptr;
	m_dashCountGage = nullptr;
}
