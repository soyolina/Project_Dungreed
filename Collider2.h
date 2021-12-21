#pragma once

#include "Config.h"
#include "CommonFunction.h"

enum class ObjectType { Player, Enemy, Item, EnemyAttack, End };

typedef struct ColliderInfo
{
	GameObject* object = nullptr;
	ObjectType objectType = {};
	RECT rect = {};
	//FuncPtr funcPtr;
}COLLID_DATA;


class Collider2
{
private:
	COLLID_DATA m_colliderData = {};

public:
	void CreateCollider(const wstring& name, GameObject* object, const ObjectType& objectType, const RECT& rect);
	void UpdateCollider(const wstring& name, const RECT& rect);
	void Update();
	void Render(HDC hdc);
	void Release();
};

