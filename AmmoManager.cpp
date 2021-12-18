#include "stdafx.h"
#include "AmmoManager.h"

HRESULT AmmoManager::Init()
{
	return S_OK;
}

void AmmoManager::Update()
{
	m_size = m_vecAmmos.size();

	for (int i = 0; i < m_size; ++i)
	{
		m_vecAmmos[i]->Update();
	}

}

void AmmoManager::Render(HDC hdc)
{
	m_size = m_vecAmmos.size();

	for (int i = 0; i < m_size; ++i)
	{
		m_vecAmmos[i]->Render(hdc);
	}
}

void AmmoManager::Release()
{
	m_size = m_vecAmmos.size();

	for (int i = 0; i < m_size; ++i)
	{
		SAFE_RELEASE(m_vecAmmos[i]);
	}
}

void AmmoManager::MakeAmmo(LPCWSTR imgName, POINTFLOAT pos, float angle, float attackDamage, float moveSpeed, float changeAngle)
{
	Ammo* ammo = new Ammo;
	ammo->Init(imgName, pos, angle, attackDamage, moveSpeed, changeAngle);
	m_vecAmmos.push_back(ammo);
}
