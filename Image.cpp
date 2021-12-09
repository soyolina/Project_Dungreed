#include "stdafx.h"
#include "Image.h"

HRESULT Image::Init(int width, int height)
{
	HDC hdc = GetDC(g_hWnd); // 권한이 굉장히 많은 총 지배인
	
	// 빈 비트맵 생성
	imageInfo = new IMAGE_INFO;
	imageInfo->width = width;
	imageInfo->height = height;
	imageInfo->loadType = ImageLoadType::Empty;
	imageInfo->hBitmap = CreateCompatibleBitmap(hdc, width, height);
	imageInfo->hMemDc = CreateCompatibleDC(hdc); // 새로 생성된 DC
			// 기본적으로 Bitmap에 연결되어 있다.
	imageInfo->hOldBit =
		(HBITMAP)SelectObject(imageInfo->hMemDc, imageInfo->hBitmap);

	// 리버스용
	reverseDc = CreateCompatibleDC(hdc);
	hReverseBitmap = CreateCompatibleBitmap(hdc, width, height);
	hOldReverseBitmap = (HBITMAP)SelectObject(reverseDc, hReverseBitmap);

	ReleaseDC(g_hWnd, hdc);

	if (imageInfo->hBitmap == NULL)
	{
		Release();
		return E_FAIL;
	}

	return S_OK;
}

// frame이 없는 이미지 
HRESULT Image::Init(LPCWSTR fileName, int width, int height, bool isTrans, COLORREF transColor)
{
	HDC hdc = GetDC(g_hWnd);

	imageInfo = new IMAGE_INFO;
	imageInfo->width = width;
	imageInfo->height = height;
	imageInfo->loadType = ImageLoadType::File;
	imageInfo->hBitmap = (HBITMAP)LoadImage(g_hInstance, fileName, IMAGE_BITMAP,
		width, height, LR_LOADFROMFILE);
	imageInfo->hMemDc = CreateCompatibleDC(hdc);
	imageInfo->hOldBit =
		(HBITMAP)SelectObject(imageInfo->hMemDc, imageInfo->hBitmap);
	
	// 리버스용
	reverseDc = CreateCompatibleDC(hdc);
	hReverseBitmap = (HBITMAP)LoadImage(g_hInstance, fileName, IMAGE_BITMAP,
		width, height, LR_LOADFROMFILE);
	hOldReverseBitmap = (HBITMAP)SelectObject(reverseDc, hReverseBitmap);

	ReleaseDC(g_hWnd, hdc);

	this->isTransparent = isTrans;
	this->transColor = transColor;

	if (imageInfo->hBitmap == NULL)
	{
		Release();
		return E_FAIL;
	}

	return S_OK;
}

// 프레임이 있는 이미지
HRESULT Image::Init(LPCWSTR fileName, int width, int height, int maxFrameX, int maxFrameY, bool isTrans, COLORREF transColor)
{
	HDC hdc = GetDC(g_hWnd);

	imageInfo = new IMAGE_INFO;
	imageInfo->width = width;
	imageInfo->height = height;
	imageInfo->loadType = ImageLoadType::File;
	imageInfo->hBitmap = (HBITMAP)LoadImage(g_hInstance, fileName, IMAGE_BITMAP,
		width, height, LR_LOADFROMFILE);
	imageInfo->hMemDc = CreateCompatibleDC(hdc);
	imageInfo->hOldBit =
		(HBITMAP)SelectObject(imageInfo->hMemDc, imageInfo->hBitmap);

	// 리버스용
	reverseDc = CreateCompatibleDC(hdc);
	hReverseBitmap = (HBITMAP)LoadImage(g_hInstance, fileName, IMAGE_BITMAP,
		width, height, LR_LOADFROMFILE);
	hOldReverseBitmap = (HBITMAP)SelectObject(reverseDc, hReverseBitmap);


	ReleaseDC(g_hWnd, hdc);

	this->isTransparent = isTrans;
	this->transColor = transColor;

	imageInfo->maxFrameX = maxFrameX;
	imageInfo->maxFrameY = maxFrameY;
	imageInfo->currFrameX = 0;
	imageInfo->currFrameY = 0;
	imageInfo->frameWidth = width / maxFrameX;
	imageInfo->frameHeight = height / maxFrameY;

	if (imageInfo->hBitmap == NULL)
	{
		Release();
		return E_FAIL;
	}

	return S_OK;
}

void Image::Release()
{
	if (imageInfo)
	{
		SelectObject(imageInfo->hMemDc, imageInfo->hOldBit);
		DeleteObject(imageInfo->hBitmap);
		DeleteDC(imageInfo->hMemDc);

		delete imageInfo;
		imageInfo = nullptr;
	}

	SelectObject(reverseDc, hOldReverseBitmap);
	DeleteObject(hReverseBitmap);
	DeleteDC(reverseDc);
}

void Image::Render(HDC hdc)
{
	BitBlt(hdc,				// 복사 목적지 DC
		0,					// 복사될 비트맵의 시작 위치 x
		0,					// 복사될 비트맵의 시작 위치 y
		imageInfo->width,	// 원본 복사할 가로 크기
		imageInfo->height,	// 원본 복사할 세로 크기
		imageInfo->hMemDc,	// 원본 DC
		0,					// 원본 비트맵 복사 시작 위치 x
		0,					// 원본 비트맵 복사 시작 위치 y
		SRCCOPY);			// 복사 옵션
}

// 프레임 없는 이미지
void Image::Render(HDC hdc, int destX, int destY, float scale, bool reverse)
{
	if (reverse == false)
	{
		if (isTransparent)
		{
			GdiTransparentBlt(
				hdc,
				destX - (imageInfo->width / 2),
				destY - (imageInfo->height / 2),
				static_cast<int>(imageInfo->width * scale), static_cast<int>(imageInfo->height * scale),

				imageInfo->hMemDc,
				0, 0,
				imageInfo->width, imageInfo->height,
				transColor
			);
		}
		else
		{
			BitBlt(hdc,				// 복사 목적지 DC
				destX - (imageInfo->width / 2),				// 복사될 비트맵의 시작 위치 x
				destY - (imageInfo->height / 2),			// 복사될 비트맵의 시작 위치 y
				imageInfo->width,	// 원본 복사할 가로 크기
				imageInfo->height,	// 원본 복사할 세로 크기
				imageInfo->hMemDc,	// 원본 DC
				0,					// 원본 비트맵 복사 시작 위치 x
				0,					// 원본 비트맵 복사 시작 위치 y
				SRCCOPY);			// 복사 옵션
		}
	}
	else
	{
		if (isTransparent)
		{
			StretchBlt
			(
				reverseDc,
				0, 0,
				imageInfo->width, imageInfo->height,

				imageInfo->hMemDc,
				imageInfo->width, 0,
				-imageInfo->width, imageInfo->height,
				SRCCOPY
			);

			GdiTransparentBlt(
				hdc,
				destX - (imageInfo->width / 2),
				destY - (imageInfo->height / 2),
				static_cast<int>(imageInfo->width * scale), static_cast<int>(imageInfo->height * scale),

				reverseDc,
				0, 0,
				imageInfo->width, imageInfo->height,
				transColor
			);
		}
		else
		{
			StretchBlt
			(
				reverseDc,
				0, 0,
				imageInfo->width, imageInfo->height,

				imageInfo->hMemDc,
				imageInfo->width, 0,
				-imageInfo->width, imageInfo->height,
				SRCCOPY
			);

			BitBlt(hdc,				// 복사 목적지 DC
				destX - (imageInfo->width / 2),				// 복사될 비트맵의 시작 위치 x
				destY - (imageInfo->height / 2),			// 복사될 비트맵의 시작 위치 y
				imageInfo->width,	// 원본 복사할 가로 크기
				imageInfo->height,	// 원본 복사할 세로 크기
				reverseDc,	// 원본 DC
				0,					// 원본 비트맵 복사 시작 위치 x
				0,					// 원본 비트맵 복사 시작 위치 y
				SRCCOPY);			// 복사 옵션
		}
	}

	
}

// 프레임 있는 이미지
void Image::Render(HDC hdc, int destX, int destY, int m_frameX, int m_frameY, float scale, bool reverse)
{
	if (reverse == false)
	{
		if (isTransparent)
		{
			GdiTransparentBlt(
				hdc,
				destX - (imageInfo->frameWidth / 2),
				destY - (imageInfo->frameHeight / 2),
				imageInfo->frameWidth, imageInfo->frameHeight,

				imageInfo->hMemDc,
				imageInfo->frameWidth * m_frameX,
				imageInfo->frameHeight * m_frameY,
				imageInfo->frameWidth, imageInfo->frameHeight,
				transColor
			);
		}
		else
		{
			BitBlt(hdc,				// 복사 목적지 DC
				destX - (imageInfo->frameWidth / 2),		// 복사될 비트맵의 시작 위치 x
				destY - (imageInfo->frameHeight / 2),		// 복사될 비트맵의 시작 위치 y
				imageInfo->frameWidth,	// 원본 복사할 가로 크기
				imageInfo->frameHeight,	// 원본 복사할 세로 크기
				imageInfo->hMemDc,	// 원본 DC
				imageInfo->frameWidth * m_frameX,				// 원본 비트맵 복사 시작 위치 x
				imageInfo->frameHeight * m_frameY,			// 원본 비트맵 복사 시작 위치 y
				SRCCOPY);			// 복사 옵션
		}
	}
	else
	{
		if (isTransparent)
		{
			StretchBlt
			(
				reverseDc,
				imageInfo->frameWidth * m_frameX,	// 위치
				imageInfo->frameHeight * m_frameY,
				imageInfo->frameWidth,				// 크기
				imageInfo->frameHeight,

				imageInfo->hMemDc,
				imageInfo->frameWidth * (m_frameX + 1),	// 위치
				imageInfo->frameHeight,
				-imageInfo->frameWidth,					// 크기
				imageInfo->frameHeight,
				SRCCOPY
			);

			GdiTransparentBlt(
				hdc,
				destX - (imageInfo->frameWidth / 2),
				destY - (imageInfo->frameHeight / 2),
				imageInfo->frameWidth, imageInfo->frameHeight,

				reverseDc,
				imageInfo->frameWidth * m_frameX,
				imageInfo->frameHeight * m_frameY,
				imageInfo->frameWidth, imageInfo->frameHeight,
				transColor
			);
		}
		else
		{
			StretchBlt
			(
				reverseDc,
				imageInfo->frameWidth * m_frameX,	// 위치
				imageInfo->frameHeight * m_frameY,
				imageInfo->frameWidth,				// 크기
				imageInfo->frameHeight,

				imageInfo->hMemDc,
				imageInfo->frameWidth * (m_frameX + 1),	// 위치
				imageInfo->frameHeight,
				-imageInfo->frameWidth,					// 크기
				imageInfo->frameHeight,
				SRCCOPY
			);

			BitBlt(hdc,				// 복사 목적지 DC
				destX - (imageInfo->frameWidth / 2),		// 복사될 비트맵의 시작 위치 x
				destY - (imageInfo->frameHeight / 2),		// 복사될 비트맵의 시작 위치 y
				imageInfo->frameWidth,	// 원본 복사할 가로 크기
				imageInfo->frameHeight,	// 원본 복사할 세로 크기
				reverseDc,	// 원본 DC
				imageInfo->frameWidth * m_frameX,				// 원본 비트맵 복사 시작 위치 x
				imageInfo->frameHeight * m_frameY,			// 원본 비트맵 복사 시작 위치 y
				SRCCOPY);			// 복사 옵션
		}
	}
	
}

void Image::RenderBasic(HDC hdc, int destX, int destY)
{
	if (isTransparent)
	{
		GdiTransparentBlt(
			hdc,
			destX,
			destY,
			imageInfo->width, imageInfo->height,

			imageInfo->hMemDc,
			0, 0,
			imageInfo->width, imageInfo->height,
			transColor
		);
	}
	else
	{
		BitBlt(hdc,				// 복사 목적지 DC
			destX - (imageInfo->width / 2),				// 복사될 비트맵의 시작 위치 x
			destY - (imageInfo->height / 2),			// 복사될 비트맵의 시작 위치 y
			imageInfo->width,	// 원본 복사할 가로 크기
			imageInfo->height,	// 원본 복사할 세로 크기
			imageInfo->hMemDc,	// 원본 DC
			0,					// 원본 비트맵 복사 시작 위치 x
			0,					// 원본 비트맵 복사 시작 위치 y
			SRCCOPY);			// 복사 옵션
	}
}

// 이미지 루프 렌더
void Image::loopRender(HDC hdc, int sourX)
{
	//sourX = sourX % imageInfo->width;

	if (isTransparent)
	{
		if (sourX + WIN_SIZE_X < imageInfo->width)
		{
			GdiTransparentBlt
			(
				hdc,
				0, 0,
				WIN_SIZE_X, WIN_SIZE_Y,

				imageInfo->hMemDc,
				sourX, 0,
				WIN_SIZE_X,
				imageInfo->height,
				transColor
			);
		}
		else
		{
			GdiTransparentBlt
			(
				hdc,
				0, 0,
				imageInfo->width - sourX, WIN_SIZE_Y,

				imageInfo->hMemDc,
				sourX, 0,
				imageInfo->width - sourX,
				imageInfo->height,
				transColor
			);

			GdiTransparentBlt
			(
				hdc,
				imageInfo->width - sourX, 0,
				WIN_SIZE_X - (imageInfo->width - sourX), WIN_SIZE_Y,

				imageInfo->hMemDc,
				0, 0,
				WIN_SIZE_X - (imageInfo->width - sourX),
				imageInfo->height,
				transColor
			);
		}
	}
	else
	{
		if (sourX + WIN_SIZE_X < imageInfo->width)
		{
			StretchBlt
			(
				hdc,
				0, 0,
				WIN_SIZE_X, WIN_SIZE_Y,

				imageInfo->hMemDc,
				sourX, 0,
				WIN_SIZE_X,
				imageInfo->height,
				SRCCOPY
			);
		}
		else
		{
			StretchBlt
			(
				hdc,
				0, 0,
				imageInfo->width - sourX, WIN_SIZE_Y,

				imageInfo->hMemDc,
				sourX, 0,
				imageInfo->width - sourX,
				imageInfo->height,
				SRCCOPY
			);

			StretchBlt
			(
				hdc,
				imageInfo->width - sourX, 0,
				WIN_SIZE_X - (imageInfo->width - sourX), WIN_SIZE_Y,

				imageInfo->hMemDc,
				0, 0,
				WIN_SIZE_X - (imageInfo->width - sourX),
				imageInfo->height,
				SRCCOPY
			);
		}
	}

	
}

void Image::HpRender(HDC hdc, int destX, int destY, float remainHp)
{
	if (isTransparent)
	{
		GdiTransparentBlt
		(
			hdc,
			destX,
			destY,
			static_cast<int>(remainHp), imageInfo->height,

			imageInfo->hMemDc,
			0, 0,
			static_cast<int>(remainHp), imageInfo->height,
			transColor
		);
	}
}
