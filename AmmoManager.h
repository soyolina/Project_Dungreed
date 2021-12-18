#pragma once

#include "Config.h"
#include "CommonFunction.h"
#include "Ammo.h"

class AmmoManager
{
private:
	vector<Ammo*> m_vecAmmos = {};
	size_t m_size = 0;

public:
	HRESULT Init();
	void Update();
	void Render(HDC hdc);
	void Release();

	void MakeAmmo(LPCWSTR imgName, POINTFLOAT pos, float angle, float attackDamage, float moveSpeed, float changeAngle = 0);

};