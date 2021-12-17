#include "stdafx.h"
#include "Collider.h"

unordered_map<wstring, COLLID_DATA> Collider::m_collider = {};

void Collider::CreateCollider(const wstring& name, GameObject* object, const ObjectType& objectType, const RECT& rect)
{	
	m_collider.insert(make_pair(name, COLLID_DATA{ object, objectType, rect } ));
}

void Collider::UpdateCollider(const wstring& name, const RECT& rect)
{
	auto iter = m_collider.find(name);
	iter->second.rect = rect;
}

void Collider::Update()
{
	for (auto iter = m_collider.begin(); iter != m_collider.end(); ++iter)
	{
		for (auto iter2 = m_collider.begin(); iter2 != m_collider.end(); ++iter2)
		{
			if (iter == iter2) continue;

			if (iter->second.objectType == ObjectType::Player && iter2->second.objectType == ObjectType::EnemyAttack)
			{
				RECT tempRc = {};
				if (IntersectRect(&tempRc, &iter->second.rect, &iter2->second.rect) && iter->second.object->GetIsHit() == false)
				{
					iter->second.object->SetIsHit(true);
					int damage = iter2->second.object->GetAttackDamage();
					iter->second.object->SetHp(damage);
				}
			}

			if (iter->second.objectType == ObjectType::Item && iter2->second.objectType == ObjectType::Enemy)
			{
				RECT tempRc = {};
				if (IntersectRect(&tempRc, &iter->second.rect, &iter2->second.rect) && iter2->second.object->GetIsHit() == false)
				{
					iter2->second.object->SetIsHit(true);
					int damage = iter->second.object->GetAttackDamage();
					iter2->second.object->SetHp(damage);
				}
			}
		}
	}
}

void Collider::Render(HDC hdc)
{
	for (auto& elem : m_collider)
	{
		Rectangle(hdc, elem.second.rect.left, elem.second.rect.top, elem.second.rect.right, elem.second.rect.bottom);
	}
}

void Collider::Release()
{
	m_collider.clear();
}
