#pragma once

#include <Windows.h>

//RECT �߽���ǥ�� �����
inline RECT RectCenter(int x, int y, int width, int height)
{
	RECT rc = { x - width / 2, y - height / 2, x + width / 2, y + height / 2 };
	return rc;
}
