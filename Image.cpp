#include "stdafx.h"
#include "Image.h"
#include <bitset>


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

	// 로테이트
	rotateImageInfo = nullptr;
	// 알파블렌드
	alphablendImageInfo = nullptr;

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

	imageInfo->frameWidth = width;
	imageInfo->frameHeight = height;

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

	// 로테이트
	rotateImageInfo = nullptr;
	// 알파블렌드
	alphablendImageInfo = nullptr;


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

	// 로테이트
	rotateImageInfo = nullptr;
	// 알파블렌드
	alphablendImageInfo = nullptr;


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

	if (rotateImageInfo != nullptr)
	{
		SelectObject(rotateImageInfo->hMemDc, rotateImageInfo->hOldBit);
		DeleteObject(rotateImageInfo->hBitmap);
		DeleteDC(rotateImageInfo->hMemDc);

		SAFE_DELETE(rotateImageInfo);
	}

	if (alphablendImageInfo != nullptr)
	{
		SelectObject(alphablendImageInfo->hMemDc, alphablendImageInfo->hOldBit);
		DeleteObject(alphablendImageInfo->hBitmap);
		DeleteDC(alphablendImageInfo->hMemDc);

		SAFE_DELETE(alphablendImageInfo);
	}
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
				static_cast<int>(imageInfo->frameWidth * scale), 
				static_cast<int>(imageInfo->frameHeight * scale),

				imageInfo->hMemDc,
				imageInfo->frameWidth * m_frameX ,
				imageInfo->frameHeight * m_frameY,
				imageInfo->frameWidth, imageInfo->frameHeight,
				transColor
			);
		}
		else
		{
			BitBlt(hdc,				// 복사 목적지 DC
				destX - (imageInfo->frameWidth / 2),	// 복사될 비트맵의 시작 위치 x
				destY - (imageInfo->frameHeight / 2),	// 복사될 비트맵의 시작 위치 y
				imageInfo->frameWidth,	// 원본 복사할 가로 크기
				imageInfo->frameHeight,	// 원본 복사할 세로 크기
				imageInfo->hMemDc,	// 원본 DC
				imageInfo->frameWidth * m_frameX,		// 원본 비트맵 복사 시작 위치 x
				imageInfo->frameHeight * m_frameY,	// 원본 비트맵 복사 시작 위치 y
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

void Image::RenderBasic(HDC hdc, int destX, int destY, int frameX, int frameY, float scale)
{
	if (isTransparent)
	{
		GdiTransparentBlt(
			hdc,
			destX,
			destY,
			static_cast<int>(imageInfo->frameWidth * scale), 
			static_cast<int>(imageInfo->frameHeight * scale),

			imageInfo->hMemDc,
			imageInfo->frameWidth * frameX,
			imageInfo->frameHeight * frameY,
			imageInfo->frameWidth, imageInfo->frameHeight,
			transColor
		);
	}
	else
	{
		BitBlt(hdc,				// 복사 목적지 DC
			destX ,				// 복사될 비트맵의 시작 위치 x
			destY ,				// 복사될 비트맵의 시작 위치 y
			imageInfo->frameWidth,	// 원본 복사할 가로 크기
			imageInfo->frameHeight,	// 원본 복사할 세로 크기
			imageInfo->hMemDc,	// 원본 DC
			imageInfo->frameWidth * frameX,		// 원본 비트맵 복사 시작 위치 x
			imageInfo->frameHeight * frameY,	// 원본 비트맵 복사 시작 위치 y
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

void Image::HpRender2(HDC hdc, int destX, int destY, float remainHp)
{
	if (isTransparent)
	{
		GdiTransparentBlt
		(
			hdc,
			destX - static_cast<int>(imageInfo->width * 0.5f),
			destY - static_cast<int>(imageInfo->height * 0.5f),
			static_cast<int>(remainHp), imageInfo->height,

			imageInfo->hMemDc,
			0, 0,
			static_cast<int>(remainHp), imageInfo->height,
			transColor
		);
	}
}

//void Image::RotateRender2(HDC hdc, const POINT* rect)
//{
//	/*Image white;
//	white.Init(imageInfo->width, imageInfo->height);
//	PatBlt(white.GetMemDC(), 0, 0, imageInfo->width, imageInfo->height, WHITENESS);*/
//
//	//HBITMAP hMask = CreateBitmap(imageInfo->width, imageInfo->height, 1, 1, NULL);
//	HDC tempDC = CreateCompatibleDC(hdc);
//	HBITMAP tempBitmap = CreateCompatibleBitmap(hdc, imageInfo->width, imageInfo->height);
//	SelectObject(tempDC, tempBitmap);
//
//	GdiTransparentBlt(tempDC, 0, 0, imageInfo->width, imageInfo->height,
//	imageInfo->hMemDc, 0, 0, imageInfo->width, imageInfo->height, transColor);
//
//	for (int y = 0; y < imageInfo->height; ++y)
//	{
//		for (int x = 0; x < imageInfo->width; ++x)
//		{
//			if (0 != GetPixel(tempDC, x, y))
//			{
//				SetPixel(tempDC, x, y, RGB(255, 255, 255));
//			}
//		}
//	}
//
//	int bits = GetDeviceCaps(tempDC, BITSPIXEL);
//	
//	// 32bit 비트맵 가지고 1bit 비트맵으로 전환
//	//HBITMAP hMask = CreateBitmap(imageInfo->width, imageInfo->height, 1, 1, &(*bit));
//	
//	auto ret = PlgBlt(
//		hdc,
//		rect,
//		imageInfo->hMemDc,
//		0, 0,
//		imageInfo->width, imageInfo->height,
//
//		NULL,
//		0,
//		0
//	);
//
//	if (ret == 0)
//	{
//		printf("오류");
//	}
//}


HBITMAP Image::GetRotatedBitmap(HDC hdc, float angle, int m_frameX, int m_frameY)
{
	HDC destDC = CreateCompatibleDC(hdc);											// 회전할 비트맵을 출력받을 DC   
	HBITMAP hbmResult = CreateCompatibleBitmap(hdc, imageInfo->width, imageInfo->height);		// 회전할 비트맵을 출력받을 메모리비트맵 핸들   


	HBITMAP hbmOldDest = (HBITMAP)SelectObject(destDC, hbmResult);                  // 회전할 비트맵을 출력받을 DC에, 회전할 비트맵을 출력받을 메모리비트맵 선택   

	HBRUSH hbrBack = CreateSolidBrush(RGB(255, 0, 255));                            // 회전으로 인한, 공백영역을 칠할 브러시핸들 생성   
	HBRUSH hbrOld = (HBRUSH)SelectObject(destDC, hbrBack);							// 브러시핸들 선택   
	PatBlt(destDC, 0, 0, imageInfo->width, imageInfo->height, PATCOPY);				// 선택된 브러시로, 회전할 비트맵을 출력받을 DC에, 미리 색상을 채워 지움   
	DeleteObject(SelectObject(destDC, hbrOld));                                     // 브러시 해제   
	

	float cosine = cosf(angle);												        // 회전이동변환 행렬에 들어갈 cos세타 값을 구함          
	float sine = sinf(angle);											         	// 회전이동변환 행렬에 들어갈 sin세타 값을 구함   

	SetGraphicsMode(destDC, GM_ADVANCED);                                           // 윈도우 좌표계의 회전을 위하여, 그래픽모드를 확장모드로 변경합니다.(요구사항:윈98,NT이상)   

	XFORM xform;                                                                    // 방정식을 표현하는 3행3열의 행렬 선언   
	xform.eM11 = cosine;                                                            // 1행 1열 성분 설정 (회전성분)   
	xform.eM12 = sine;                                                              // 1행 2열 성분 설정 (회전성분)   
	xform.eM21 = -sine;                                                             // 2행 1열 성분 설정 (회전성분)   
	xform.eM22 = cosine;                                                            // 2행 2열 성분 설정 (회전성분)   
	xform.eDx =  (FLOAT)imageInfo->frameWidth * 0.5f + m_frameX * imageInfo->frameWidth;	// 3행 1열 성분 설정 (X축 이동 성분)   
	xform.eDy =  (FLOAT)imageInfo->frameHeight * 0.5f + m_frameY * imageInfo->frameHeight;	// 3행 2열 성분 설정 (Y축 이동 성분)   

	SetWorldTransform(destDC, &xform);

	// 회전된 메모리DC에, 회전할 이미지를 출력   
	BitBlt(destDC,
		static_cast<int>(-(imageInfo->frameWidth * 0.5f)),
		static_cast<int>(-(imageInfo->frameHeight * 0.5f)),

		imageInfo->frameWidth,
		imageInfo->frameHeight,

		imageInfo->hMemDc,
		m_frameX * imageInfo->frameWidth,
		m_frameY * imageInfo->frameHeight,
		SRCCOPY);

	SelectObject(destDC, hbmOldDest);
	DeleteObject(destDC);

	return hbmResult;
}

void Image::RotateHDC(HDC hdc, float angle, int m_frameX, int m_frameY)
{
	HBITMAP hTempBitmap;

	if (rotateImageInfo == nullptr)
	{
		HDC hdc = GetDC(g_hWnd);

		rotateImageInfo = new IMAGE_INFO;
		rotateImageInfo->loadType = ImageLoadType::File;
		rotateImageInfo->hMemDc = CreateCompatibleDC(hdc);
		rotateImageInfo->hBitmap = (HBITMAP)LoadImage(g_hInstance, L"Image/Item/BasicShortSword_New.bmp", IMAGE_BITMAP, imageInfo->width, imageInfo->height, LR_LOADFROMFILE);
		rotateImageInfo->hOldBit = (HBITMAP)SelectObject(rotateImageInfo->hMemDc, rotateImageInfo->hBitmap);
		rotateImageInfo->width = imageInfo->width;
		rotateImageInfo->height = imageInfo->height;

		ReleaseDC(g_hWnd, hdc);
	};

	hTempBitmap = GetRotatedBitmap(hdc, angle, m_frameX, m_frameY);

	(HBITMAP)SelectObject(rotateImageInfo->hMemDc, hTempBitmap);

	DeleteObject(hTempBitmap);
}

void Image::ImgRotateRender(HDC hdc, int destX, int destY, float angle, float scale)
{
	if (isTransparent)//배경색 없애고 출력
	{
		RotateHDC(hdc, angle);
		
		GdiTransparentBlt(
			hdc,									//복사할 장소의 DC
			destX - static_cast<int>(imageInfo->width * scale * 0.5f),		//복사할 좌표 시작X
			destY - static_cast<int>(imageInfo->height * scale * 0.5f),		//복사할 좌표 시작Y
			static_cast<int>(imageInfo->width * scale),						//복사할 이미지 가로크기
			static_cast<int>(imageInfo->height * scale),						//복사할 이미지 세로크기
			rotateImageInfo->hMemDc,
			0, 0,									//복사될 대상의 시작지점
			imageInfo->width,						//복사 영역 가로크기
			imageInfo->height,						//복사 영역 세로크기
			transColor);							//복사할때 제외할 색상 (일반적으로 마젠타 색상을 사용함)
	}
	else//원본 이미지 그대로 출력
	{
		BitBlt(hdc, static_cast<int>(destX - imageInfo->width * 0.5f), static_cast<int>(destY - imageInfo->height * 0.5f), imageInfo->width, imageInfo->height,
			imageInfo->hMemDc, 0, 0, SRCCOPY);
	}
}

void Image::ImgRotateFrameRender(HDC hdc, int destX, int destY, int frameX, int frameY, float angle, float scale)
{
	if (isTransparent)//배경색 없애고 출력
	{
		RotateHDC(hdc, angle, frameX, frameY);

		GdiTransparentBlt(
			hdc,									//복사할 장소의 DC
			static_cast<int>(destX - imageInfo->frameWidth * scale * 0.5f),			//복사할 좌표 시작X
			static_cast<int>(destY - imageInfo->frameHeight * scale * 0.5f),		//복사할 좌표 시작Y
			static_cast<int>(imageInfo->frameWidth * scale),						//복사할 이미지 가로크기
			static_cast<int>(imageInfo->frameHeight * scale),						//복사할 이미지 세로크기
			rotateImageInfo->hMemDc,
			frameX * imageInfo->frameWidth,		//복사될 대상의 시작지점
			frameY * imageInfo->frameHeight,
			imageInfo->frameWidth,						//복사 영역 가로크기
			imageInfo->frameHeight,						//복사 영역 세로크기
			transColor);							//복사할때 제외할 색상 (일반적으로 마젠타 색상을 사용함)
	}
	else//원본 이미지 그대로 출력
	{
		BitBlt(hdc, 
			destX - static_cast<int>(imageInfo->frameWidth * 0.5f), destY - static_cast<int>(imageInfo->frameHeight * 0.5f),
			
			imageInfo->frameWidth, imageInfo->frameHeight,
			imageInfo->hMemDc, 
			0, 0, 
			SRCCOPY);
	}

}

void Image::ImgAlphaBlendRender(HDC hdc, int destX, int destY, BYTE transparancy)
{
	bf.AlphaFormat = 0;						// 비트맵 종류로 일반 비트맵의 경우 0, 32비트 비트맵의 경우 AC_SRC_ALPHA
	bf.BlendFlags = 0;						// 무조건 0이어야 한다
	bf.BlendOp = AC_SRC_OVER;				// 무조건 AC_SRC_OVER이어야 하고 원본과 대상 이미지를 합친다는 의미
	bf.SourceConstantAlpha = transparancy;  // 투명도(투명 0 - 불투명 255)

	if (isTransparent)
	{
		AlphaBlendHDC();

		StretchBlt(
			alphablendImageInfo->hMemDc, 0, 0,
			imageInfo->width, imageInfo->height,
			hdc, 
			static_cast<int>(destX - WIN_SIZE_X * 0.5f),
			static_cast<int>(destY - WIN_SIZE_Y * 0.5f),
			WIN_SIZE_X, WIN_SIZE_Y,
			SRCCOPY
		);

		GdiTransparentBlt(
			alphablendImageInfo->hMemDc,
			0,
			0,
			imageInfo->width,
			imageInfo->height,

			imageInfo->hMemDc,
			0,
			0,
			imageInfo->width,
			imageInfo->height,
			transColor
		);

		GdiAlphaBlend(
			hdc,
			static_cast<int>(destX - WIN_SIZE_X * 0.5f),
			static_cast<int>(destY - WIN_SIZE_Y * 0.5f),
			WIN_SIZE_X,
			WIN_SIZE_Y,

			alphablendImageInfo->hMemDc,
			0,
			0,
			imageInfo->width,
			imageInfo->height,
			bf
		);
	}
}

void Image::ImgAlphaBlendFrameRender(HDC hdc, int destX, int destY, int frameX, int frameY, BYTE transparancy)
{
	bf.AlphaFormat = 0;						// 비트맵 종류로 일반 비트맵의 경우 0, 32비트 비트맵의 경우 AC_SRC_ALPHA
	bf.BlendFlags = 0;						// 무조건 0이어야 한다
	bf.BlendOp = AC_SRC_OVER;				// 무조건 AC_SRC_OVER이어야 하고 원본과 대상 이미지를 합친다는 의미
	bf.SourceConstantAlpha = transparancy;  // 투명도(투명 0 - 불투명 255)
	
	if (isTransparent)
	{
		AlphaBlendHDC();

		BitBlt(alphablendImageInfo->hMemDc, 0, 0, imageInfo->frameWidth, imageInfo->frameHeight,
			hdc, 
			static_cast<int>(destX - imageInfo->frameWidth * 0.5f),
			static_cast<int>(destY - imageInfo->frameHeight * 0.5f), 
			SRCCOPY);

		GdiTransparentBlt(
			alphablendImageInfo->hMemDc,
			0,
			0,
			imageInfo->frameWidth, 
			imageInfo->frameHeight,
			
			imageInfo->hMemDc,
			imageInfo->frameWidth* frameX,
			imageInfo->frameHeight* frameY,
			imageInfo->frameWidth,
			imageInfo->frameHeight,
			transColor
		);
		
		GdiAlphaBlend(
			hdc,
			static_cast<int>(destX - imageInfo->frameWidth * 0.5f),
			static_cast<int>(destY - imageInfo->frameHeight * 0.5f),
			imageInfo->frameWidth,
			imageInfo->frameHeight,

			alphablendImageInfo->hMemDc,
			0, 
			0,
			imageInfo->frameWidth,
			imageInfo->frameHeight,
			bf
		);
	}
}

void Image::AlphaBlendHDC()
{
	if (alphablendImageInfo == nullptr)
	{
		HDC hdc = GetDC(g_hWnd);

		alphablendImageInfo = new IMAGE_INFO;
		alphablendImageInfo->loadType = ImageLoadType::Empty;
		alphablendImageInfo->hMemDc = CreateCompatibleDC(hdc);
		alphablendImageInfo->hBitmap = (HBITMAP)CreateCompatibleBitmap(hdc, imageInfo->frameWidth, imageInfo->frameHeight);
		alphablendImageInfo->hOldBit = (HBITMAP)SelectObject(alphablendImageInfo->hMemDc, alphablendImageInfo->hBitmap);

		alphablendImageInfo->width = imageInfo->width;
		alphablendImageInfo->height = imageInfo->height;
		alphablendImageInfo->frameWidth = imageInfo->frameWidth;
		alphablendImageInfo->frameHeight = imageInfo->frameHeight;

		ReleaseDC(g_hWnd, hdc);
	};
}

