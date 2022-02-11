#include "stdafx.h"
#include "DrawTilemap.h"
#include "Image.h"

HRESULT DrawTilemap::Init(int loadIndex)
{
    m_sampleImg = IMAGE_MANAGER->FindImage(L"Image/TileMapImage/TileMapImage2.bmp");

    Load(loadIndex);

    // �ݶ��̴� �߰���
    for (int i = 0; i < TILE_COUNT_Y; i++)
    {
        for (int j = 0; j < TILE_COUNT_X; j++)
        {
            if (m_tileInfo[i][j].mapAttribute == mapAttribute::Obstacle)
            {
                // ��¥ �ݶ��̴�
                //m_tileInfo[i][j].collider.Init(this, &m_tileInfo[i][j].rc, ObjectType::TileMap, [](auto, auto) {});
                m_colliders[i][j].Init(this, &m_tileInfo[i][j].rc, ObjectType::TileMap, [](auto, auto) {});
            }
        }
    }
	return S_OK;
}

void DrawTilemap::Render(HDC hdc)
{
    // ���� ����
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
        GENERIC_READ,           // �б�, ����
        0, NULL,                // ����, ���� ���
        OPEN_EXISTING,          // ���� ����ų� ���� �� �ɼ�
        FILE_ATTRIBUTE_NORMAL,  // ���� �Ӽ�(�б� ����, ���� ���)
        NULL);

    // �б�
    DWORD readByte;
    if (ReadFile(hFile, m_tileInfo, sizeof(tagTile) * TILE_COUNT_X * TILE_COUNT_Y,
        &readByte, NULL) == false)
    {
        MessageBox(g_hWnd, L"�� ������ �ε忡 �����߽��ϴ�.", L"����", MB_OK);
    }

    CloseHandle(hFile);
}
