#pragma once

#include "Config.h"

class Collider;
class GameObject;
class ColliderManager
{
private:
	static vector<Collider*> _colliders;

public:
	static void	AddCollider(Collider* collider);
	static void RemoveCollider(Collider* collider);

	static void CheckCollision();
	static void Render(HDC hdc);
};

