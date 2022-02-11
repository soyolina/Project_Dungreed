#include "stdafx.h"
#include "DrawTilemap.h"
#include "Image.h"

HRESULT DrawTilemap::Init(int loadIndex)
{
    m_sampleImg = IMAGE_MANAGER->FindImage(L"Image/TileMapImage/TileMapImage2.bmp");

    Load(loadIndex);

    // 콜라이더 추가용
    for (int i = 0; i < TILE_COUNT_Y; i++)
    {
        for (int j = 0; j < TILE_COUNT_X; j++)
        {
            if (m_tileInfo[i][j].mapAttribute == mapAttribute::Obstacle)
            {
                // 진짜 콜라이더
                //m_tileInfo[i][j].collider.Init(this, &m_tileInfo[i][j].rc, ObjectType::TileMap, [](auto, auto) {});
                m_colliders[i][j].Init(this, &m_tileInfo[i][j].rc, ObjectType::TileMap, [](auto, auto) {});
            }
        }
    }
	return S_OK;
}

void DrawTilemap::Render(HDC hdc)
{
    // 메인 영역
    for (int i = 0; i < TILE_COUNT_Y; i++)
    {
        for (int j = 0; j < TILE_COUNT_X; j++)
        {
            m_sampleImg->RenderBasic(hdc,
                m_tileInfo[i][j].rc.left,
                m_tileInfo[i][j].rc.top,
                m_tileInfo[i][j].frameX,
                m_tileInfo[i][j].frameY);
        }
    }
}

void DrawTilemap::Release()
{
    m_sampleImg = nullptr;
}

void DrawTilemap::Load(int loadIndex)
{
    wstring loadFileName = L"Save/saveMapData_" + to_wstring(loadIndex);
    loadFileName += L".map";

    HANDLE hFile = CreateFile(loadFileName.c_str(),
        GENERIC_READ,           // 읽기, 쓰기
        0, NULL,                // 공유, 보안 모드
        OPEN_EXISTING,          // 파일 만들거나 읽을 때 옵션
        FILE_ATTRIBUTE_NORMAL,  // 파일 속성(읽기 전용, 숨김 등등)
        NULL);

    // 읽기
    DWORD readByte;
    if (ReadFile(hFile, m_tileInfo, sizeof(tagTile) * TILE_COUNT_X * TILE_COUNT_Y,
        &readByte, NULL) == false)
    {
        MessageBox(g_hWnd, L"맵 데이터 로드에 실패했습니다.", L"에러", MB_OK);
    }

    CloseHandle(hFile);
}
