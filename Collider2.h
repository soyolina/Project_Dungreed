#pragma once

//#include "Config.h"

class GameObject;

typedef struct ColliderInfo
{
	GameObject* object = nullptr;
	RECT rect = {};
	ObjectType objectType = {};
}COLLIDER_DATA;

class Collider2
{
private:
	COLLIDER_DATA m_colliderData = {};

public:
	Collider2(GameObject* object, const RECT& rect, const ObjectType& objType)
		: m_colliderData{ object, rect, objType} {};

	void Update(const RECT& rect);

	COLLIDER_DATA GetColliderData() const { return m_colliderData; }
};

