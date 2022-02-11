#pragma once

#include "Config.h"

//RECT 중심좌표로 만들기
inline RECT RectCenter(int x, int y, int width, int height)
{
	RECT rc = { x - width / 2, y - height / 2, x + width / 2, y + height / 2 };
	return rc;
}


// 두 점 사이의 좌표 구하기
inline float GetDistance(const POINTFLOAT& pos1, const POINTFLOAT& pos2)
{
	return sqrtf((pos1.x - pos2.x) * (pos1.x - pos2.x)
		+ (pos1.y - pos2.y) * (pos1.y - pos2.y));
}

// 두 점 사이의 각도 구하기
//float getAngle(float startX, float startY, float endX, float endY)
//{
//	float x = endX - startX;
//	float y = endY - startY;
//	float distance = sqrtf(x * x + y * y);
//	float angle = acosf(x / distance);
//	if (endY > startY)
//	{
//		angle = PI2 - angle;
//	}
//	return angle;
//}

// 두 점 사이의 각도 구하기
inline float GetAngle(const POINTFLOAT& pos1, const POINT& pos2)
{
	float x = pos2.x - pos1.x;
	float y = -(pos2.y - pos1.y);
	float angle = atan2f(y, x);
	return angle;
}

// 두 점 사이의 각도 구하기 - 템플릿
template<typename T1, typename T2>
inline float GetAngle2(const T1& pos1, const T2& pos2)
{
	float x = static_cast<float>(pos2.x - pos1.x);
	float y = static_cast<float>(-(pos2.y - pos1.y));
	float angle = atan2f(y, x);
	return angle;
}

// 윈도우 사이즈 조정
inline void SetWindowSize(int startX, int startY, int sizeX, int sizeY)
{
	// 원하는 윈도우 작업영역 설정
	RECT rc;
	rc.left = 0; rc.top = 0;
	rc.right = sizeX;	rc.bottom = sizeY;

	// 스타일이 포함된 실제 윈도우 크기 계산
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, false);

	// 계산된 값으로 윈도우를 이동시키면서 크기도 함께 변환
	MoveWindow(g_hWnd, startX, startY,
		rc.right - rc.left, rc.bottom - rc.top, true);
}