#pragma once

// 헤더 파일
#include <Windows.h>
#include <windowsx.h>
#include <iostream>
#include <math.h>
#include <ctime>
#include <algorithm>
#include <functional>

// STL
#include <vector>
#include <map>
#include <string>

using namespace std;

// enum class
enum class ObjectType { Player, Enemy, Item, EnemyAttack, TileMap, End };

// timer에 TimeGetTime 함수 쓸려고 라이브러리 추가해야함.
#pragma comment(lib, "winmm.lib")

// 값 셋팅
#define WIN_START_POS_X	50
#define WIN_START_POS_Y	50
#define WIN_SIZE_X	1200
#define WIN_SIZE_Y	900

#define TILE_MAP_TOOL_SIZE_X  1800
#define TILE_MAP_TOOL_SIZE_Y  1000

// PI 값
#define PI (3.1415926f)
#define PI2 (PI * 2)

//나중에 이미지 회전시 사용할 값
#define PI_2 (PI * 0.5f) //90도
#define PI_4 (PI * 0.25f) //45도
#define PI_8 (PI * 0.125f) //22.5도


// 매크로 함수 (클래스에서 동적할당 된 부분 해제)
#define SAFE_RELEASE(p) { if(p) { p->Release(); delete p; p = nullptr; } }
#define SAFE_DELETE(p) { if(p) { delete p; p = nullptr; } }

// 각도 구하기 - 도에서 라디안으로 변환하기
#define DEGREE_TO_RADIAN(x)		(x * PI / 180.0f)

//  - 타일맵
// 맵 속성
enum class mapAttribute
{
	Obstacle, NonObstacle, End
};

// 타일맵 구조체
#include "Collider2.h"
typedef struct tagTile
{
	RECT rc;
	// sample tile의 아이디 (프레임 X, 프레임 Y)
	int frameX, frameY;
	mapAttribute mapAttribute;
} TILE_INFO;

// 타일 갯수
#define TILE_COUNT_X  25
#define TILE_COUNT_Y  19

// 타일 사이즈
#define	TILE_SIZE  48

// 타일 맵 로드할 때의 인덱스 용
enum class mapName { BellialStageBackgournd, BellialStageForeground, End };


extern HWND g_hWnd;
extern HINSTANCE g_hInstance;
extern POINT g_ptClickedMouse;

// 내가 만든 헤더파일
#include "ImageManager.h"
#include "SceneManager.h"
#include "TimerManager.h"
#include "Input.h"
#include "ColliderManager.h"

// 싱글톤 추가한것
#define IMAGE_MANAGER ImageManager::GetSingleton()
#define SCENE_MANAGER SceneManager::GetSingleton()
#define TIMER_MANAGER TimerManager::GetSingleton()
