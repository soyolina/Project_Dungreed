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

