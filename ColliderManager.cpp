#include "stdafx.h"
#include "ColliderManager.h"
#include "Collider2.h"
#include "GameObject.h"

Collider2* ColliderManager::m_player = nullptr;
vector<Collider2*> ColliderManager::m_enemies = {};
vector<Collider2*> ColliderManager::m_enemyAttack = {};
vector<Collider2*> ColliderManager::m_items = {};


Collider2* ColliderManager::CreateCollider(GameObject* object, RECT rect, ObjectType objType)
{
	Collider2* newCollider = new Collider2(object, rect, objType);

	switch (objType)
	{
	case ObjectType::Player:
		m_player = newCollider;
		break;
	case ObjectType::Enemy:
		m_enemies.push_back(newCollider);
		break;
	case ObjectType::Item:
		m_items.push_back(newCollider);
		break;
	case ObjectType::EnemyAttack:
		m_enemyAttack.push_back(newCollider);
		break;
	case ObjectType::End:
		cout << "형이 여기서 왜 나와...?!" << endl;
		break;
	}
	
	return newCollider;
}

void ColliderManager::CheckCollision()
{
	RECT tempRc = {};
	// 적 공격과 플레이어 충돌
	RECT playerRect = m_player->GetColliderData().rect;
	RECT enemyAttackRect = {};

	for (size_t i = 0; i < m_enemyAttack.size(); ++i)
	{
		enemyAttackRect = m_enemyAttack[i]->GetColliderData().rect;
		if (IntersectRect(&tempRc, &playerRect, &enemyAttackRect) && m_player->GetColliderData().object->GetIsHit() == false)
		{
			m_player->GetColliderData().object->SetIsHit(true);
			m_enemyAttack[i]->GetColliderData().object->SetIsHit(true);
			m_enemyAttack[i]->GetColliderData().object->SetIsHit2(true);

			int damage = m_enemyAttack[i]->GetColliderData().object->GetAttackDamage();
			m_player->GetColliderData().object->SetHp(damage);
		}
	}

	// 아이템과 보스 충돌
	RECT itemRect = {};
	RECT enemyRect = {};

	for (size_t i = 0; i < m_items.size(); ++i)
	{
		for (size_t j = 0; j < m_enemies.size(); ++j)
		{
			itemRect = m_items[i]->GetColliderData().rect;
			enemyRect = m_enemies[i]->GetColliderData().rect;

			if (IntersectRect(&tempRc, &enemyRect, &itemRect) && m_enemies[i]->GetColliderData().object->GetIsHit() == false)
			{
				m_enemies[i]->GetColliderData().object->SetIsHit(true);
				m_enemies[i]->GetColliderData().object->SetIsHit2(true);

				int damage = m_items[i]->GetColliderData().object->GetAttackDamage();
				m_enemies[i]->GetColliderData().object->SetHp(damage);
			}
		}
	}
}

void ColliderManager::Render(HDC hdc)
{
	size_t size = 0;

	Rectangle(hdc, m_player->GetColliderData().rect.left, m_player->GetColliderData().rect.top,
		m_player->GetColliderData().rect.right, m_player->GetColliderData().rect.bottom);

	size = m_enemies.size();
	for (size_t i = 0; i < size; ++i)
	{
		Rectangle(hdc, m_enemies[i]->GetColliderData().rect.left, m_enemies[i]->GetColliderData().rect.top,
			m_enemies[i]->GetColliderData().rect.right, m_enemies[i]->GetColliderData().rect.bottom);
		
	}

	size = m_enemyAttack.size();
	for (size_t i = 0; i < size; ++i)
	{
		Rectangle(hdc, m_enemyAttack[i]->GetColliderData().rect.left, m_enemyAttack[i]->GetColliderData().rect.top,
			m_enemyAttack[i]->GetColliderData().rect.right, m_enemyAttack[i]->GetColliderData().rect.bottom);

	}
	
	size = m_items.size();
	for (size_t i = 0; i < size; ++i)
	{
		Rectangle(hdc, m_items[i]->GetColliderData().rect.left, m_items[i]->GetColliderData().rect.top,
			m_items[i]->GetColliderData().rect.right, m_items[i]->GetColliderData().rect.bottom);
	}
}

void ColliderManager::Release()
{
	SAFE_DELETE(m_player);

	for (auto elem : m_enemies)
	{
		SAFE_DELETE(elem);
	}

	for (auto elem : m_enemyAttack)
	{
		SAFE_DELETE(elem);
	}

	for (auto elem : m_items)
	{
		SAFE_DELETE(elem);
	}
}

void ColliderManager::DeleteCollider(ObjectType objType, Collider2* collider)
{
	size_t size = 0;

	switch (objType)
	{
	case ObjectType::Player:
		break;
	case ObjectType::Enemy:
		size = m_enemies.size();
		for (size_t i = 0; i < size; ++i)
		{
			if (m_enemies[i] == collider)
			{
				delete m_enemies[i];
				m_enemies.erase(m_enemies.begin() + i);
				break;
			}
		}
		break;
	case ObjectType::Item:
		break;
	case ObjectType::EnemyAttack:
		size = m_enemyAttack.size();
		for (size_t i = 0; i < size; ++i)
		{
			if (m_enemyAttack[i] == collider)
			{
				delete m_enemyAttack[i];
				m_enemyAttack.erase(m_enemyAttack.begin() + i);
				break;
			}
		}

		break;
	case ObjectType::End:
		break;
	}
}
