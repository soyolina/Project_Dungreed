#include "stdafx.h"
#include "ItemBase.h"
//#include "Player.h"

HRESULT ItemBase::Init()
{
	return S_OK;
}

void ItemBase::Update()
{
}

void ItemBase::Render(HDC hdc)
{
}

void ItemBase::Release()
{
}

//void ItemBase::SetEffectImgData(POINTFLOAT playerPos, int posNumX, int posNumY, int axisNumX, int axisNumY, float angle)
//{
//		if (m_player->GetPlayerReverse() == true)
//		{
//			this->m_effectImgRenderPos.x = playerPos.x - posNumX;
//		}
//		else
//		{
//			this->m_effectImgRenderPos.x = playerPos.x + posNumX;
//		}
//		this->m_effectImgRenderPos.y = playerPos.y;
//}
