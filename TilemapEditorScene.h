#pragma once
#include "Scene.h"
#include "Config.h"
#include "CommonFunction.h"

#define SAMPLE_TILE_COUNT_X  7
#define SAMPLE_TILE_COUNT_Y  4

#define SAMPLE_IMG_POS_X  1300
#define SAMPLE_IMG_POS_Y  800

typedef struct tagSampleTile
{
	RECT rc;
	int frameX, frameY;
	mapAttribute mapAttribute;

} SAMPLE_TILE_INFO;


class Image;
class TilemapEditorScene : public Scene
{
private:
	Image* m_taeyeon = nullptr;

	SAMPLE_TILE_INFO m_sampleTileInfo[SAMPLE_TILE_COUNT_Y][SAMPLE_TILE_COUNT_X] = {};	// 샘플 타일 이미지 영역
	TILE_INFO m_tileInfo[TILE_COUNT_Y][TILE_COUNT_X] = {};								// 그리는 영역 타일
	Image* m_sampleImg = nullptr;														// 샘플 타일 이미지

	SAMPLE_TILE_INFO m_selectedSampleTile = {};											// 선택한 타일 이미지


public:
	virtual ~TilemapEditorScene() {}

	virtual HRESULT Init();
	virtual void Update();
	virtual void Render(HDC hdc);
	virtual void Release();

	void Save();
	void Load();
};

