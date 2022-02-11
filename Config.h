#pragma once

// ��� ����
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

// timer�� TimeGetTime �Լ� ������ ���̺귯�� �߰��ؾ���.
#pragma comment(lib, "winmm.lib")

// �� ����
#define WIN_START_POS_X	50
#define WIN_START_POS_Y	50
#define WIN_SIZE_X	1200
#define WIN_SIZE_Y	900

#define TILE_MAP_TOOL_SIZE_X  1800
#define TILE_MAP_TOOL_SIZE_Y  1000

// PI ��
#define PI (3.1415926f)
#define PI2 (PI * 2)

//���߿� �̹��� ȸ���� ����� ��
#define PI_2 (PI * 0.5f) //90��
#define PI_4 (PI * 0.25f) //45��
#define PI_8 (PI * 0.125f) //22.5��


// ��ũ�� �Լ� (Ŭ�������� �����Ҵ� �� �κ� ����)
#define SAFE_RELEASE(p) { if(p) { p->Release(); delete p; p = nullptr; } }
#define SAFE_DELETE(p) { if(p) { delete p; p = nullptr; } }

// ���� ���ϱ� - ������ �������� ��ȯ�ϱ�
#define DEGREE_TO_RADIAN(x)		(x * PI / 180.0f)

//  - Ÿ�ϸ�
// �� �Ӽ�
enum class mapAttribute
{
	Obstacle, NonObstacle, End
};

// Ÿ�ϸ� ����ü
#include "Collider2.h"
typedef struct tagTile
{
	RECT rc;
	// sample tile�� ���̵� (������ X, ������ Y)
	int frameX, frameY;
	mapAttribute mapAttribute;
} TILE_INFO;

// Ÿ�� ����
#define TILE_COUNT_X  25
#define TILE_COUNT_Y  19

// Ÿ�� ������
#define	TILE_SIZE  48

// Ÿ�� �� �ε��� ���� �ε��� ��
enum class mapName { BellialStageBackgournd, BellialStageForeground, End };


extern HWND g_hWnd;
extern HINSTANCE g_hInstance;
extern POINT g_ptClickedMouse;

// ���� ���� �������
#include "ImageManager.h"
#include "SceneManager.h"
#include "TimerManager.h"
#include "Input.h"
#include "ColliderManager.h"

// �̱��� �߰��Ѱ�
#define IMAGE_MANAGER ImageManager::GetSingleton()
#define SCENE_MANAGER SceneManager::GetSingleton()
#define TIMER_MANAGER TimerManager::GetSingleton()
