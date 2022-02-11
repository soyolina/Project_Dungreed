#pragma once

#include "Config.h"

//RECT �߽���ǥ�� �����
inline RECT RectCenter(int x, int y, int width, int height)
{
	RECT rc = { x - width / 2, y - height / 2, x + width / 2, y + height / 2 };
	return rc;
}


// �� �� ������ ��ǥ ���ϱ�
inline float GetDistance(const POINTFLOAT& pos1, const POINTFLOAT& pos2)
{
	return sqrtf((pos1.x - pos2.x) * (pos1.x - pos2.x)
		+ (pos1.y - pos2.y) * (pos1.y - pos2.y));
}

// �� �� ������ ���� ���ϱ�
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

// �� �� ������ ���� ���ϱ�
inline float GetAngle(const POINTFLOAT& pos1, const POINT& pos2)
{
	float x = pos2.x - pos1.x;
	float y = -(pos2.y - pos1.y);
	float angle = atan2f(y, x);
	return angle;
}

// �� �� ������ ���� ���ϱ� - ���ø�
template<typename T1, typename T2>
inline float GetAngle2(const T1& pos1, const T2& pos2)
{
	float x = static_cast<float>(pos2.x - pos1.x);
	float y = static_cast<float>(-(pos2.y - pos1.y));
	float angle = atan2f(y, x);
	return angle;
}

// ������ ������ ����
inline void SetWindowSize(int startX, int startY, int sizeX, int sizeY)
{
	// ���ϴ� ������ �۾����� ����
	RECT rc;
	rc.left = 0; rc.top = 0;
	rc.right = sizeX;	rc.bottom = sizeY;

	// ��Ÿ���� ���Ե� ���� ������ ũ�� ���
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, false);

	// ���� ������ �����츦 �̵���Ű�鼭 ũ�⵵ �Բ� ��ȯ
	MoveWindow(g_hWnd, startX, startY,
		rc.right - rc.left, rc.bottom - rc.top, true);
}