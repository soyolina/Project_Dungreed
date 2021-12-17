#pragma once

#include <unordered_map>

#include "Config.h"
#include "CommonFunction.h"
#include "GameObject.h"
#include "CollisonFunction.h"

enum class ObjectType { Player, Enemy, Item, EnemyAttack, End };

//using FuncPtr = decltype(&CollisionFunction::OnCollided);

//typedef void(CollisionFunction::*function)();

typedef struct ColliderInfo
{
	GameObject* object = nullptr;
	ObjectType objectType = {};
	RECT rect = {};
	//FuncPtr funcPtr;
}COLLID_DATA;

class Collider
{
private:
	static unordered_map<wstring, COLLID_DATA> m_collider;


public:
	static void CreateCollider(const wstring& name, GameObject* object,const ObjectType& objectType, const RECT& rect);
	static void UpdateCollider(const wstring& name, const RECT& rect);
	static void Update();
	static void Render(HDC hdc);
	static void Release();

};


//struct IBehaviour
//{
//	virtual void PhysicsUpdate() = 0;
//	virtual void OnCollision() = 0;
//};
//
//class GameObject : IBehaviour
//{
//	virtual void PhysicsUpdate()
//	{
//		for (Component* comp : _components)
//		{
//			comp->PhyscisUpdate();
//		}
//	}
//};
//
//class Collider : public Component
//{
//	virtual void PhysicsUpdate()
//	{
//		// 충돌체크
//		if (IsCollided())
//		{
//			_owner->OnCollision();
//		}
//	}
//};
//
//class Player : public GameObject
//{
//	virtual void OnCollision()
//	{
//		PlayerHPComponent* comp = GetComponent<PlayerHPComponent>();
//		comp->TakeDamage(100.0f);	
//	}
//};