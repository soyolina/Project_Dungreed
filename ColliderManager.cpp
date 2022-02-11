#include "stdafx.h"
#include "ColliderManager.h"
#include "Collider2.h"
#include "GameObject.h"

vector<Collider*> ColliderManager::_colliders;

void ColliderManager::AddCollider(Collider* collider)
{
	_colliders.push_back(collider);
}

void ColliderManager::RemoveCollider(Collider* collider)
{
	remove(_colliders.begin(), _colliders.end(), collider);
}

void ColliderManager::CheckCollision()
{
	// 충돌 처리
	for (auto collider : _colliders)
	{
		for (auto other : _colliders)
		{
			collider->CheckCollision(other);
		}
	}
}

void ColliderManager::Render(HDC hdc)
{
	size_t size = 0;

	size = _colliders.size();
	for (size_t i = 0; i < size; ++i)
	{
		if (_colliders[i]->GetOwner() == nullptr)
		{
			continue;
		}
		Rectangle(hdc, _colliders[i]->GetHitbox().left, 
			_colliders[i]->GetHitbox().top,
			_colliders[i]->GetHitbox().right,
			_colliders[i]->GetHitbox().bottom);
	}
}


