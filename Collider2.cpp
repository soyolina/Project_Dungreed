#include "stdafx.h"
#include "Collider2.h"
#include "ColliderManager.h"

Collider::Collider()
{
	ColliderManager::AddCollider(this);
}

Collider::~Collider()
{
	_owner = nullptr;
	_hitbox = nullptr;

	ColliderManager::RemoveCollider(this);
}

void Collider::Init(GameObject* owner, const RECT* hitbox, ObjectType type, const callback_t& callback)
{
	_owner = owner;
	_hitbox = hitbox;
	_type = type;
	_callback = callback;
}

void Collider::CheckCollision(Collider* other)
{
	// ���� ��ü�� X
	if (this == other)
	{
		return;
	}

	// _owner == nullptr�� ���� TileMap�ʿ��� ���� �ȵ� �ֵ�
	if (_owner == nullptr || nullptr == other->GetOwner())
	{
		return;
	}

	RECT intersectedRect = {};
	RECT otherHitbox = other->GetHitbox();

	// ��Ʈ �浹 üũ
	if (IntersectRect(&intersectedRect, _hitbox, &otherHitbox))
	{
		// �ݹ��� �ִٸ�
		if (_callback)
		{
			// ����
			// �ݹ��� Init()���� ������.
			_callback(other, intersectedRect);
		}
	}
}

GameObject* Collider::GetOwner() const
{
	return _owner;
}

RECT Collider::GetHitbox() const
{
	return *_hitbox;
}

ObjectType Collider::GetType() const
{
	return _type;
}