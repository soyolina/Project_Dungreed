#pragma once

#include "Config.h"
#include "CommonFunction.h"

class Ammo;
class AmmoManager
{
private:
	vector<Ammo*> m_vecAmmos = {};

public:
	void Update();
	void Render(HDC hdc);
	void Release();

	void MakeAmmo(LPCWSTR imgName, POINTFLOAT pos, float angle, int attackDamage, float moveSpeed, ObjectType objType, float changeAngle = 0);
};