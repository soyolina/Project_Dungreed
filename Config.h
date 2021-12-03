#pragma once

// ��� ����
#include <Windows.h>
#include <windowsx.h>
#include <iostream>

// STL
#include <vector>
#include <map>
#include <string>

using namespace std;

// ���� ���� �������
#include "ImageManager.h"
#include "KeyManager.h"
#include "SceneManager.h"
#include "TimerManager.h"

// �̱��� �߰��Ѱ�
#define IMAGE_MANAGER ImageManager::GetSingleton()
#define KEY_MANAGER KeyManager::GetSingleton()
#define SCENE_MANAGER SceneManager::GetSingleton()
#define TIMER_MANAGER TimerManager::GetSingleton()


// timer�� TimeGetTime �Լ� ������ ���̺귯�� �߰��ؾ���.
#pragma comment(lib, "winmm.lib")

// �� ����
#define WIN_START_POS_X	50
#define WIN_START_POS_Y	50
#define WIN_SIZE_X	1200
#define WIN_SIZE_Y	900


// ��ũ�� �Լ� (Ŭ�������� �����Ҵ� �� �κ� ����)
#define SAFE_RELEASE(p) { if(p) { p->Release(); delete p; p = nullptr; } }
#define SAFE_DELETE(p) { if(p) { delete p; p = nullptr; } }




extern HWND g_hWnd;
extern HINSTANCE g_hInstance;

extern POINT g_ptMouse;
extern POINT g_ptClickedMouse;