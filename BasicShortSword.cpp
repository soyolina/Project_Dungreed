#include "BasicShortSword.h"
#include "Image.h"
#include "Player.h"

void BasicShortSword::Init(Player* player)
{
	m_img = IMAGE_MANAGER->FindImage(L"Image/Item/BasicShortSword_New.bmp");

	m_direction = 1;
	m_player = player;
	m_playerPos = m_player->GetPlayerPos();
}

void BasicShortSword::Update()
{
	m_playerPos = m_player->GetPlayerPos();

	if (m_playerPos.x < Input::GetMousePosition().x) { m_direction = 1; }
	else { m_direction = -1; }
}

void BasicShortSword::Render(HDC hdc)
{
	m_img->Render(hdc, BSS_POSX(static_cast<int>(m_playerPos.x), m_direction), BSS_POSY(static_cast<int>(m_playerPos.y)));
}

void BasicShortSword::Release()
{
}
