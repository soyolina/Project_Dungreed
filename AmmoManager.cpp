#include "stdafx.h"
#include "AmmoManager.h"
#include "Ammo.h"

void AmmoManager::Update()
{
	size_t size = m_vecAmmos.size();

	for (size_t i = 0; i < size; ++i)
	{
		if (m_vecAmmos[i]->GetIsDead() == false)
		{
			m_vecAmmos[i]->Update();
		}
		else
		{
			SAFE_RELEASE(m_vecAmmos[i]);
			m_vecAmmos.erase(m_vecAmmos.begin() + i);

			--size;
			--i;
		}
	}
}

void AmmoManager::Render(HDC hdc)
{
	size_t size = m_vecAmmos.size();

	for (size_t i = 0; i < size; ++i)
	{
		m_vecAmmos[i]->Render(hdc);
	}
}

void AmmoManager::Release()
{
	size_t size = m_vecAmmos.size();

	for (size_t i = 0; i < size; ++i)
	{
		SAFE_RELEASE(m_vecAmmos[i]);
	}
}

void AmmoManager::MakeAmmo(LPCWSTR imgName, POINTFLOAT pos, float angle, int attackDamage, float moveSpeed, ObjectType objType, float changeAngle)
{
	Ammo* ammo = new Ammo;
	ammo->Init(imgName, pos, angle, attackDamage, moveSpeed, objType, changeAngle);
	m_vecAmmos.push_back(ammo);
}
