#pragma once

#include <Windows.h>

//RECT 중심좌표로 만들기
inline RECT RectCenter(int x, int y, int width, int height)
{
	RECT rc = { x - width / 2, y - height / 2, x + width / 2, y + height / 2 };
	return rc;
}
