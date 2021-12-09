#include "stdafx.h"
#include "BasicShortSword.h"
#include "Image.h"
#include "Player.h"

void BasicShortSword::Init(Player* player)
{
	m_img = IMAGE_MANAGER->FindImage(L"Image/Item/BasicShortSword_New.bmp");

	m_player = player;
	m_playerPos = m_player->GetPlayerPos();
	m_imgRenderPos = {};
	mb_isReverse = false;
}

void BasicShortSword::Update()
{
	m_playerPos = m_player->GetPlayerPos();

	// 이미지 반전 위해서
	mb_isReverse = m_player->GetPlayerReverse();
	
	if (mb_isReverse == false)
	{
		m_imgRenderPos.x = m_playerPos.x + 10;
		m_imgRenderPos.y = m_playerPos.y + 12;
	}
	else
	{
		m_imgRenderPos.x = m_playerPos.x - 10;
		m_imgRenderPos.y = m_playerPos.y + 12;
	}

}

void BasicShortSword::Render(HDC hdc)
{
	m_img->Render(hdc, m_imgRenderPos.x, m_imgRenderPos.y, 1.0f, mb_isReverse);
}

void BasicShortSword::Release()
{
	m_img = nullptr;
}
