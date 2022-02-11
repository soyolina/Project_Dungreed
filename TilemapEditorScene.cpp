#include "stdafx.h"
#include "TilemapEditorScene.h"
#include "Image.h"

HRESULT TilemapEditorScene::Init()
{
    SetWindowSize(WIN_START_POS_X, WIN_START_POS_Y, TILE_MAP_TOOL_SIZE_X, TILE_MAP_TOOL_SIZE_Y);

    m_taeyeon = IMAGE_MANAGER->FindImage(L"Image/Taeyeon.bmp");

    //m_sampleImg = IMAGE_MANAGER->FindImage(L"Image/TileMapImage/TileMapImage.bmp");
    m_sampleImg = IMAGE_MANAGER->FindImage(L"Image/TileMapImage/TileMapImage2.bmp");


    // 왼쪽 상단 메인 영역 초기화
    for (int y = 0; y < TILE_COUNT_Y; y++)    // y축
    {
        for (int x = 0; x < TILE_COUNT_X; x++)    // x축
        {
            RECT& rc = m_tileInfo[y][x].rc;
            rc.left = x * TILE_SIZE;
            rc.top = y * TILE_SIZE;
            rc.right = x * TILE_SIZE + TILE_SIZE;
            rc.bottom = y * TILE_SIZE + TILE_SIZE;
            
            m_tileInfo[y][x].frameX = 6;
            m_tileInfo[y][x].frameY = 3;

            m_tileInfo[y][x].mapAttribute = mapAttribute::NonObstacle;
        }
    }

    // 오른쪽 상단 샘플 영역 초기화
    for (int i = 0; i < SAMPLE_TILE_COUNT_Y; i++)    // y축
    {
        for (int j = 0; j < SAMPLE_TILE_COUNT_X; j++)    // x축
        {
            SetRect(&(m_sampleTileInfo[i][j].rc),
                TILE_MAP_TOOL_SIZE_X - m_sampleImg->GetWidth() + j * TILE_SIZE,
                i * TILE_SIZE,
                TILE_MAP_TOOL_SIZE_X - m_sampleImg->GetWidth() + j * TILE_SIZE + TILE_SIZE,
                i * TILE_SIZE + TILE_SIZE);

            m_sampleTileInfo[i][j].frameX = j;
            m_sampleTileInfo[i][j].frameY = i;

            if (i <= 1)
            {
                m_sampleTileInfo[i][j].mapAttribute = mapAttribute::Obstacle;
            }
            else
            {
                m_sampleTileInfo[i][j].mapAttribute = mapAttribute::NonObstacle;
            }
        }
    }

    // 선택된 타일
    m_selectedSampleTile.frameX = 6;
    m_selectedSampleTile.frameY = 3;
    m_selectedSampleTile.mapAttribute = mapAttribute::NonObstacle;


    return S_OK;
}

void TilemapEditorScene::Update()
{
    // 샘플영역에서 샘플을 선택
    RECT sampleArea;
    sampleArea.left = TILE_MAP_TOOL_SIZE_X - m_sampleImg->GetWidth();
    sampleArea.right = TILE_MAP_TOOL_SIZE_X;
    sampleArea.top = 0;
    sampleArea.bottom = m_sampleImg->GetHeight();

    if ( PtInRect( &(sampleArea), Input::GetMousePosition() ) )
    {
        if (Input::GetButtonDown(VK_LBUTTON))
        {
            int posX = Input::GetMousePosition().x - sampleArea.left;
            int selectedIdX = posX / TILE_SIZE;

            int posY = Input::GetMousePosition().y - sampleArea.top;
            int selectedIdY = posY / TILE_SIZE;

            m_selectedSampleTile.frameX =
                m_sampleTileInfo[selectedIdY][selectedIdX].frameX;

            m_selectedSampleTile.frameY =
                m_sampleTileInfo[selectedIdY][selectedIdX].frameY;

            m_selectedSampleTile.mapAttribute = m_sampleTileInfo[selectedIdY][selectedIdX].mapAttribute;
        }
    }


    // 메인영역에서 선택된 샘플 정보로 수정
    sampleArea.left = 0;
    sampleArea.right = TILE_COUNT_X * TILE_SIZE;
    sampleArea.top = 0;
    sampleArea.bottom = TILE_COUNT_Y * TILE_SIZE;
    if (PtInRect( &(sampleArea), Input::GetMousePosition()))
    {
        if (Input::GetButton(VK_LBUTTON))
        {
            int posX = Input::GetMousePosition().x - sampleArea.left;
            int selectedIdX = posX / TILE_SIZE;

            int posY = Input::GetMousePosition().y - sampleArea.top;
            int selectedIdY = posY / TILE_SIZE;

            m_tileInfo[selectedIdY][selectedIdX].frameX = m_selectedSampleTile.frameX;
            m_tileInfo[selectedIdY][selectedIdX].frameY = m_selectedSampleTile.frameY;

            m_tileInfo[selectedIdY][selectedIdX].mapAttribute = m_selectedSampleTile.mapAttribute;
        }
    }

    if (Input::GetButtonDown('S'))
    {
        Save();
    }

    if (Input::GetButtonDown('L'))
    {
        Load();
    }
}

void TilemapEditorScene::Render(HDC hdc)
{
    m_taeyeon->Render(hdc);

    // 메인 영역
    for (int i = 0; i < TILE_COUNT_Y; i++)
    {
        for (int j = 0; j < TILE_COUNT_X; j++)
        {
            Rectangle(hdc, m_tileInfo[i][j].rc.left,
                m_tileInfo[i][j].rc.top,
                m_tileInfo[i][j].rc.right,
                m_tileInfo[i][j].rc.bottom);

            m_sampleImg->Render(hdc,
                m_tileInfo[i][j].rc.left + TILE_SIZE / 2,
                m_tileInfo[i][j].rc.top + TILE_SIZE / 2,
                m_tileInfo[i][j].frameX,
                m_tileInfo[i][j].frameY);
        }
    }

    // 샘플 영역
    for (int i = 0; i < SAMPLE_TILE_COUNT_Y; i++)
    {
        for (int j = 0; j < SAMPLE_TILE_COUNT_X; ++j)
        {
			Rectangle(hdc, m_sampleTileInfo[i][j].rc.left,
				m_sampleTileInfo[i][j].rc.top,
				m_sampleTileInfo[i][j].rc.right,
				m_sampleTileInfo[i][j].rc.bottom);
        }
    }

    m_sampleImg->RenderBasic(hdc, TILE_MAP_TOOL_SIZE_X - m_sampleImg->GetWidth(), 0);


    // 선택된 타일
    m_sampleImg->RenderBasic(hdc, SAMPLE_IMG_POS_X, SAMPLE_IMG_POS_Y, m_selectedSampleTile.frameX, m_selectedSampleTile.frameY, 2.0f);

}

void TilemapEditorScene::Release()
{
    m_taeyeon = nullptr;
    m_sampleImg = nullptr;
}

void TilemapEditorScene::Save()
{
    int saveIndex;
    cout << "저장할 맵 번호 입력" << '\n';
    cin >> saveIndex;

    wstring saveFileName = L"Save/saveMapData_" + to_wstring(saveIndex);
    saveFileName += L".map";

    HANDLE hFile = CreateFile(saveFileName.c_str(),
        GENERIC_WRITE,          // 읽기, 쓰기
        0, NULL,                // 공유, 보안 모드
        CREATE_ALWAYS,          // 파일 만들거나 읽을 때 옵션
        FILE_ATTRIBUTE_NORMAL,  // 파일 속성(읽기 전용, 숨김 등등)
        NULL);
    
    // 쓰기
    DWORD byteSize = sizeof(tagTile) * TILE_COUNT_X * TILE_COUNT_Y;
    
    DWORD writtenByte;
    if (static_cast<bool>
        (WriteFile(hFile,    // 파일 핸들
         m_tileInfo,       // 메모리 시작주소
        sizeof(tagTile) * TILE_COUNT_X * TILE_COUNT_Y,  // 메모리 크기
        &writtenByte,   // 실제 쓰여진 파일 용량
        NULL)) == true)          // ???
    {
        MessageBox(g_hWnd, L"맵 데이터 저장에 성공했습니다.", L"성공", MB_OK);
    }
    else
    {
        MessageBox(g_hWnd, L"맵 데이터 저장에 실패했습니다.", L"에러", MB_OK);
    }


    CloseHandle(hFile);
}

void TilemapEditorScene::Load()
{
    int loadIndex;
    cout << "불러올 맵 번호 입력" << '\n';
    cin >> loadIndex;

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
    if (static_cast<bool>(ReadFile(hFile, m_tileInfo, sizeof(tagTile) * TILE_COUNT_X * TILE_COUNT_Y, &readByte, NULL)) == true)
    {
        MessageBox(g_hWnd, L"맵 데이터 로드에 성공했습니다.", L"성공", MB_OK);
    }
    else
    {
        MessageBox(g_hWnd, L"맵 데이터 로드에 실패했습니다.", L"에러", MB_OK);
    }

    CloseHandle(hFile);
}
