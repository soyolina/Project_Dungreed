#pragma once

#include "Config.h"
#include "GameObject.h"

class Image;
class DrawTilemap : public GameObject
{
private:
	TILE_INFO m_tileInfo[TILE_COUNT_Y][TILE_COUNT_X] = {};								// �׸��� ���� Ÿ��
	Collider m_colliders[TILE_COUNT_Y][TILE_COUNT_X] = {};
	Image* m_sampleImg = nullptr;														// ���� Ÿ�� �̹���

public:
	HRESULT Init(int loadIndex);
	void Render(HDC hdc);
	void Release();

	void Load(int loadIndex);
};

