#include "UIManager.h"
#include "DashCount.h"

void UIManager::Init(Player* player)
{
	m_player = player;

	m_allUI.push_back(new DashCount);
	
	for (unsigned int i = 0; i < m_allUI.size(); ++i)
	{
		m_allUI[i]->Init();
		m_allUI[i]->SetPlayer(m_player);
	}
}

void UIManager::Update()
{
	for (unsigned int i = 0; i < m_allUI.size(); ++i)
	{
		m_allUI[i]->Update();
	}
}

void UIManager::Render(HDC hdc)
{
	for (unsigned int i = 0; i < m_allUI.size(); ++i)
	{
		m_allUI[i]->Render(hdc);
	}
}

void UIManager::Release()
{
	for (unsigned int i = 0; i < m_allUI.size(); ++i)
	{
		SAFE_RELEASE(m_allUI[i]);
	}
}
