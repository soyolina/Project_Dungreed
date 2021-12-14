#pragma once

#include "UIBase.h"

#define DASHBAR_LEFTEND_POSX (30)
#define DASHBAR_LEFTEND_POSY (100)

class Image;
class DashCount : public UIBase
{
private:
	Image* m_dashBarBaseLeft = nullptr;
	Image* m_dashBarBaseRight = nullptr;
	Image* m_dashBarBase = nullptr;

	Image* m_dashCountGage = nullptr;

	int m_playerMaxDashCount = 0;
	int m_playerDashCount = 0;

public:
	virtual ~DashCount() {}

	virtual void Init();
	virtual void Update();
	virtual void Render(HDC hdc);
	virtual void Release();

	void SetPlayerMaxDashCount(int maxCount) { this->m_playerMaxDashCount = maxCount; }
	void SetPlayerDashCount(int count) { this->m_playerDashCount = count; }
};

