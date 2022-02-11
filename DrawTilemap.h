#pragma once

#include "Config.h"
#include "GameObject.h"

class Image;
class DrawTilemap : public GameObject
{
private:
	TILE_INFO m_tileInfo[TILE_COUNT_Y][TILE_COUNT_X] = {};								// 그리는 영역 타일
	Collider m_colliders[TILE_COUNT_Y][TILE_COUNT_X] = {};
	Image* m_sampleImg = nullptr;														// 샘플 타일 이미지

public:
	HRESULT Init(int loadIndex);
	void Render(HDC hdc);
	void Release();

	void Load(int loadIndex);
};

