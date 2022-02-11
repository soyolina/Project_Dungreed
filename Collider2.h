#pragma once

//#include "Config.h"

class GameObject;

class Collider
{
	using callback_t = function<void(Collider*, RECT)>;
public:
	Collider();
	~Collider();

	GameObject*		GetOwner() const;
	RECT			GetHitbox() const;
	ObjectType		GetType() const;

	void			Init(GameObject* owner, const RECT* hitbox, ObjectType type, const callback_t& callback);
	void			CheckCollision(Collider* other);
private:
	GameObject* _owner = nullptr;
	const RECT*	_hitbox = nullptr;
	ObjectType	_type = ObjectType::End;
	callback_t	_callback;
};