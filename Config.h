#pragma once

// 헤더 파일
#include <Windows.h>
#include <windowsx.h>
#include <iostream>
#include <math.h>

// STL
#include <vector>
#include <map>
#include <string>

using namespace std;

// 내가 만든 헤더파일
#include "ImageManager.h"
#include "KeyManager.h"
#include "SceneManager.h"
#include "TimerManager.h"

// 싱글톤 추가한것
#define IMAGE_MANAGER ImageManager::GetSingleton()
#define KEY_MANAGER KeyManager::GetSingleton()
#define SCENE_MANAGER SceneManager::GetSingleton()
#define TIMER_MANAGER TimerManager::GetSingleton()


// timer에 TimeGetTime 함수 쓸려고 라이브러리 추가해야함.
#pragma comment(lib, "winmm.lib")

// 값 셋팅
#define WIN_START_POS_X	50
#define WIN_START_POS_Y	50
#define WIN_SIZE_X	1200
#define WIN_SIZE_Y	900


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


extern HWND g_hWnd;
extern HINSTANCE g_hInstance;

extern POINT g_ptMouse;
extern POINT g_ptClickedMouse;