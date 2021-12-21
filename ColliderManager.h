#pragma once

#include "Config.h"



class GameObject;
class Collider2;
class ColliderManager
{
private:
	static Collider2* m_player;
	static vector<Collider2*> m_enemies;
	static vector<Collider2*> m_enemyAttack;
	static vector<Collider2*> m_items;

public:
	static Collider2* CreateCollider(GameObject* object, RECT rect, ObjectType objType);
	static void CheckCollision();
	static void Render(HDC hdc);
	static void Release();

	static void DeleteCollider(ObjectType objType, Collider2* collider);
};

