#include "stdafx.h"
#include "Image.h"
#include <bitset>


HRESULT Image::Init(int width, int height)
{
	HDC hdc = GetDC(g_hWnd); // ������ ������ ���� �� ������
	
	// �� ��Ʈ�� ����
	imageInfo = new IMAGE_INFO;
	imageInfo->width = width;
	imageInfo->height = height;
	imageInfo->loadType = ImageLoadType::Empty;
	imageInfo->hBitmap = CreateCompatibleBitmap(hdc, width, height);
	imageInfo->hMemDc = CreateCompatibleDC(hdc); // ���� ������ DC
			// �⺻������ Bitmap�� ����Ǿ� �ִ�.
	imageInfo->hOldBit =
		(HBITMAP)SelectObject(imageInfo->hMemDc, imageInfo->hBitmap);

	// ��������
	reverseDc = CreateCompatibleDC(hdc);
	hReverseBitmap = CreateCompatibleBitmap(hdc, width, height);
	hOldReverseBitmap = (HBITMAP)SelectObject(reverseDc, hReverseBitmap);

	// ������Ʈ
	rotateImageInfo = nullptr;
	// ���ĺ���
	alphablendImageInfo = nullptr;

	ReleaseDC(g_hWnd, hdc);

	if (imageInfo->hBitmap == NULL)
	{
		Release();
		return E_FAIL;
	}

	return S_OK;
}

// frame�� ���� �̹��� 
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
	
	// ��������
	reverseDc = CreateCompatibleDC(hdc);
	hReverseBitmap = (HBITMAP)LoadImage(g_hInstance, fileName, IMAGE_BITMAP,
		width, height, LR_LOADFROMFILE);
	hOldReverseBitmap = (HBITMAP)SelectObject(reverseDc, hReverseBitmap);

	// ������Ʈ
	rotateImageInfo = nullptr;
	// ���ĺ���
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

// �������� �ִ� �̹���
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

	// ��������
	reverseDc = CreateCompatibleDC(hdc);
	hReverseBitmap = (HBITMAP)LoadImage(g_hInstance, fileName, IMAGE_BITMAP,
		width, height, LR_LOADFROMFILE);
	hOldReverseBitmap = (HBITMAP)SelectObject(reverseDc, hReverseBitmap);

	// ������Ʈ
	rotateImageInfo = nullptr;
	// ���ĺ���
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
	BitBlt(hdc,				// ���� ������ DC
		0,					// ����� ��Ʈ���� ���� ��ġ x
		0,					// ����� ��Ʈ���� ���� ��ġ y
		imageInfo->width,	// ���� ������ ���� ũ��
		imageInfo->height,	// ���� ������ ���� ũ��
		imageInfo->hMemDc,	// ���� DC
		0,					// ���� ��Ʈ�� ���� ���� ��ġ x
		0,					// ���� ��Ʈ�� ���� ���� ��ġ y
		SRCCOPY);			// ���� �ɼ�
}

// ������ ���� �̹���
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
			BitBlt(hdc,				// ���� ������ DC
				destX - (imageInfo->width / 2),				// ����� ��Ʈ���� ���� ��ġ x
				destY - (imageInfo->height / 2),			// ����� ��Ʈ���� ���� ��ġ y
				imageInfo->width,	// ���� ������ ���� ũ��
				imageInfo->height,	// ���� ������ ���� ũ��
				imageInfo->hMemDc,	// ���� DC
				0,					// ���� ��Ʈ�� ���� ���� ��ġ x
				0,					// ���� ��Ʈ�� ���� ���� ��ġ y
				SRCCOPY);			// ���� �ɼ�
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

			BitBlt(hdc,				// ���� ������ DC
				destX - (imageInfo->width / 2),				// ����� ��Ʈ���� ���� ��ġ x
				destY - (imageInfo->height / 2),			// ����� ��Ʈ���� ���� ��ġ y
				imageInfo->width,	// ���� ������ ���� ũ��
				imageInfo->height,	// ���� ������ ���� ũ��
				reverseDc,	// ���� DC
				0,					// ���� ��Ʈ�� ���� ���� ��ġ x
				0,					// ���� ��Ʈ�� ���� ���� ��ġ y
				SRCCOPY);			// ���� �ɼ�
		}
	}

	
}

// ������ �ִ� �̹���
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
			BitBlt(hdc,				// ���� ������ DC
				destX - (imageInfo->frameWidth / 2),	// ����� ��Ʈ���� ���� ��ġ x
				destY - (imageInfo->frameHeight / 2),	// ����� ��Ʈ���� ���� ��ġ y
				imageInfo->frameWidth,	// ���� ������ ���� ũ��
				imageInfo->frameHeight,	// ���� ������ ���� ũ��
				imageInfo->hMemDc,	// ���� DC
				imageInfo->frameWidth * m_frameX,		// ���� ��Ʈ�� ���� ���� ��ġ x
				imageInfo->frameHeight * m_frameY,	// ���� ��Ʈ�� ���� ���� ��ġ y
				SRCCOPY);			// ���� �ɼ�
		}
	}
	else
	{
		if (isTransparent)
		{
			StretchBlt
			(
				reverseDc,
				imageInfo->frameWidth * m_frameX,	// ��ġ
				imageInfo->frameHeight * m_frameY,
				imageInfo->frameWidth,				// ũ��
				imageInfo->frameHeight,

				imageInfo->hMemDc,
				imageInfo->frameWidth * (m_frameX + 1),	// ��ġ
				imageInfo->frameHeight,
				-imageInfo->frameWidth,					// ũ��
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
				imageInfo->frameWidth * m_frameX,	// ��ġ
				imageInfo->frameHeight * m_frameY,
				imageInfo->frameWidth,				// ũ��
				imageInfo->frameHeight,

				imageInfo->hMemDc,
				imageInfo->frameWidth * (m_frameX + 1),	// ��ġ
				imageInfo->frameHeight,
				-imageInfo->frameWidth,					// ũ��
				imageInfo->frameHeight,
				SRCCOPY
			);

			BitBlt(hdc,				// ���� ������ DC
				destX - (imageInfo->frameWidth / 2),		// ����� ��Ʈ���� ���� ��ġ x
				destY - (imageInfo->frameHeight / 2),		// ����� ��Ʈ���� ���� ��ġ y
				imageInfo->frameWidth,	// ���� ������ ���� ũ��
				imageInfo->frameHeight,	// ���� ������ ���� ũ��
				reverseDc,	// ���� DC
				imageInfo->frameWidth * m_frameX,				// ���� ��Ʈ�� ���� ���� ��ġ x
				imageInfo->frameHeight * m_frameY,			// ���� ��Ʈ�� ���� ���� ��ġ y
				SRCCOPY);			// ���� �ɼ�
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
		BitBlt(hdc,				// ���� ������ DC
			destX - (imageInfo->width / 2),				// ����� ��Ʈ���� ���� ��ġ x
			destY - (imageInfo->height / 2),			// ����� ��Ʈ���� ���� ��ġ y
			imageInfo->width,	// ���� ������ ���� ũ��
			imageInfo->height,	// ���� ������ ���� ũ��
			imageInfo->hMemDc,	// ���� DC
			0,					// ���� ��Ʈ�� ���� ���� ��ġ x
			0,					// ���� ��Ʈ�� ���� ���� ��ġ y
			SRCCOPY);			// ���� �ɼ�
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
		BitBlt(hdc,				// ���� ������ DC
			destX ,				// ����� ��Ʈ���� ���� ��ġ x
			destY ,				// ����� ��Ʈ���� ���� ��ġ y
			imageInfo->frameWidth,	// ���� ������ ���� ũ��
			imageInfo->frameHeight,	// ���� ������ ���� ũ��
			imageInfo->hMemDc,	// ���� DC
			imageInfo->frameWidth * frameX,		// ���� ��Ʈ�� ���� ���� ��ġ x
			imageInfo->frameHeight * frameY,	// ���� ��Ʈ�� ���� ���� ��ġ y
			SRCCOPY);			// ���� �ɼ�
	}
}

// �̹��� ���� ����
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
//	// 32bit ��Ʈ�� ������ 1bit ��Ʈ������ ��ȯ
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
//		printf("����");
//	}
//}


HBITMAP Image::GetRotatedBitmap(HDC hdc, float angle, int m_frameX, int m_frameY)
{
	HDC destDC = CreateCompatibleDC(hdc);											// ȸ���� ��Ʈ���� ��¹��� DC   
	HBITMAP hbmResult = CreateCompatibleBitmap(hdc, imageInfo->width, imageInfo->height);		// ȸ���� ��Ʈ���� ��¹��� �޸𸮺�Ʈ�� �ڵ�   


	HBITMAP hbmOldDest = (HBITMAP)SelectObject(destDC, hbmResult);                  // ȸ���� ��Ʈ���� ��¹��� DC��, ȸ���� ��Ʈ���� ��¹��� �޸𸮺�Ʈ�� ����   

	HBRUSH hbrBack = CreateSolidBrush(RGB(255, 0, 255));                            // ȸ������ ����, ���鿵���� ĥ�� �귯���ڵ� ����   
	HBRUSH hbrOld = (HBRUSH)SelectObject(destDC, hbrBack);							// �귯���ڵ� ����   
	PatBlt(destDC, 0, 0, imageInfo->width, imageInfo->height, PATCOPY);				// ���õ� �귯�÷�, ȸ���� ��Ʈ���� ��¹��� DC��, �̸� ������ ä�� ����   
	DeleteObject(SelectObject(destDC, hbrOld));                                     // �귯�� ����   
	

	float cosine = cosf(angle);												        // ȸ���̵���ȯ ��Ŀ� �� cos��Ÿ ���� ����          
	float sine = sinf(angle);											         	// ȸ���̵���ȯ ��Ŀ� �� sin��Ÿ ���� ����   

	SetGraphicsMode(destDC, GM_ADVANCED);                                           // ������ ��ǥ���� ȸ���� ���Ͽ�, �׷��ȸ�带 Ȯ����� �����մϴ�.(�䱸����:��98,NT�̻�)   

	XFORM xform;                                                                    // �������� ǥ���ϴ� 3��3���� ��� ����   
	xform.eM11 = cosine;                                                            // 1�� 1�� ���� ���� (ȸ������)   
	xform.eM12 = sine;                                                              // 1�� 2�� ���� ���� (ȸ������)   
	xform.eM21 = -sine;                                                             // 2�� 1�� ���� ���� (ȸ������)   
	xform.eM22 = cosine;                                                            // 2�� 2�� ���� ���� (ȸ������)   
	xform.eDx =  (FLOAT)imageInfo->frameWidth * 0.5f + m_frameX * imageInfo->frameWidth;	// 3�� 1�� ���� ���� (X�� �̵� ����)   
	xform.eDy =  (FLOAT)imageInfo->frameHeight * 0.5f + m_frameY * imageInfo->frameHeight;	// 3�� 2�� ���� ���� (Y�� �̵� ����)   

	SetWorldTransform(destDC, &xform);

	// ȸ���� �޸�DC��, ȸ���� �̹����� ���   
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
	if (isTransparent)//���� ���ְ� ���
	{
		RotateHDC(hdc, angle);
		
		GdiTransparentBlt(
			hdc,									//������ ����� DC
			destX - static_cast<int>(imageInfo->width * scale * 0.5f),		//������ ��ǥ ����X
			destY - static_cast<int>(imageInfo->height * scale * 0.5f),		//������ ��ǥ ����Y
			static_cast<int>(imageInfo->width * scale),						//������ �̹��� ����ũ��
			static_cast<int>(imageInfo->height * scale),						//������ �̹��� ����ũ��
			rotateImageInfo->hMemDc,
			0, 0,									//����� ����� ��������
			imageInfo->width,						//���� ���� ����ũ��
			imageInfo->height,						//���� ���� ����ũ��
			transColor);							//�����Ҷ� ������ ���� (�Ϲ������� ����Ÿ ������ �����)
	}
	else//���� �̹��� �״�� ���
	{
		BitBlt(hdc, static_cast<int>(destX - imageInfo->width * 0.5f), static_cast<int>(destY - imageInfo->height * 0.5f), imageInfo->width, imageInfo->height,
			imageInfo->hMemDc, 0, 0, SRCCOPY);
	}
}

void Image::ImgRotateFrameRender(HDC hdc, int destX, int destY, int frameX, int frameY, float angle, float scale)
{
	if (isTransparent)//���� ���ְ� ���
	{
		RotateHDC(hdc, angle, frameX, frameY);

		GdiTransparentBlt(
			hdc,									//������ ����� DC
			static_cast<int>(destX - imageInfo->frameWidth * scale * 0.5f),			//������ ��ǥ ����X
			static_cast<int>(destY - imageInfo->frameHeight * scale * 0.5f),		//������ ��ǥ ����Y
			static_cast<int>(imageInfo->frameWidth * scale),						//������ �̹��� ����ũ��
			static_cast<int>(imageInfo->frameHeight * scale),						//������ �̹��� ����ũ��
			rotateImageInfo->hMemDc,
			frameX * imageInfo->frameWidth,		//����� ����� ��������
			frameY * imageInfo->frameHeight,
			imageInfo->frameWidth,						//���� ���� ����ũ��
			imageInfo->frameHeight,						//���� ���� ����ũ��
			transColor);							//�����Ҷ� ������ ���� (�Ϲ������� ����Ÿ ������ �����)
	}
	else//���� �̹��� �״�� ���
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
	bf.AlphaFormat = 0;						// ��Ʈ�� ������ �Ϲ� ��Ʈ���� ��� 0, 32��Ʈ ��Ʈ���� ��� AC_SRC_ALPHA
	bf.BlendFlags = 0;						// ������ 0�̾�� �Ѵ�
	bf.BlendOp = AC_SRC_OVER;				// ������ AC_SRC_OVER�̾�� �ϰ� ������ ��� �̹����� ��ģ�ٴ� �ǹ�
	bf.SourceConstantAlpha = transparancy;  // ����(���� 0 - ������ 255)

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
	bf.AlphaFormat = 0;						// ��Ʈ�� ������ �Ϲ� ��Ʈ���� ��� 0, 32��Ʈ ��Ʈ���� ��� AC_SRC_ALPHA
	bf.BlendFlags = 0;						// ������ 0�̾�� �Ѵ�
	bf.BlendOp = AC_SRC_OVER;				// ������ AC_SRC_OVER�̾�� �ϰ� ������ ��� �̹����� ��ģ�ٴ� �ǹ�
	bf.SourceConstantAlpha = transparancy;  // ����(���� 0 - ������ 255)
	
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

