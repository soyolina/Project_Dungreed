#pragma once

#include "UIBase.h"

#define DASHBAR_LEFTEND_POSX (30)
#define DASHBAR_LEFTEND_POSY (100)


class Image;
class DashCount : public UIBase
{
private:
	Image* m_DashBarBaseLeft = nullptr;
	Image* m_DashBarBaseRight = nullptr;
	Image* m_DashBarBase = nullptr;

	Image* m_DashCountGage = nullptr;

	int m_playerMaxDashCount = 0;
	int m_playerDashCount = 0;

public:
	virtual ~DashCount() {}

	virtual void Init();
	virtual void Update();
	virtual void Render(HDC hdc);
	virtual void Release();


	inline void SetPlayerMaxDashCount(int maxCount) { this->m_playerMaxDashCount = maxCount; }
	inline void SetPlayerDashCount(int count) { this->m_playerDashCount = count; }
};

