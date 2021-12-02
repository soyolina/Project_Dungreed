#pragma once

// 헤더 파일
#include <Windows.h>
#include <windowsx.h>
#include <iostream>

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


// 매크로 함수 (클래스에서 동적할당 된 부분 해제)
#define SAFE_RELEASE(p) { if(p) { p->Release(); delete p; p = nullptr; } }
#define SAFE_DELETE(p) { if(p) { delete p; p = nullptr; } }




extern HWND g_hWnd;
extern HINSTANCE g_hInstance;

extern POINT g_ptMouse;
extern POINT g_ptClickedMouse;