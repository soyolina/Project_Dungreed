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
	// 같은 객체면 X
	if (this == other)
	{
		return;
	}

	// _owner == nullptr인 경우는 TileMap쪽에서 세팅 안된 애들
	if (_owner == nullptr || nullptr == other->GetOwner())
	{
		return;
	}

	RECT intersectedRect = {};
	RECT otherHitbox = other->GetHitbox();

	// 렉트 충돌 체크
	if (IntersectRect(&intersectedRect, _hitbox, &otherHitbox))
	{
		// 콜백이 있다면
		if (_callback)
		{
			// 실행
			// 콜백은 Init()에서 설정됨.
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