#include "Image.h"

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

	ReleaseDC(g_hWnd, hdc);

	if (imageInfo->hBitmap == NULL)
	{
		Release();
		return E_FAIL;
	}

	return S_OK;
}

// frame�� ���� �̹��� 
HRESULT Image::Init(const char* fileName, int width, int height, bool isTrans, COLORREF transColor)
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
HRESULT Image::Init(const char* fileName, int width, int height, int maxFrameX, int maxFrameY, bool isTrans, COLORREF transColor)
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
void Image::Render(HDC hdc, int destX, int destY)
{
	if (isTransparent)
	{
		GdiTransparentBlt(
			hdc,
			destX - (imageInfo->width / 2),
			destY - (imageInfo->height / 2),
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

// ������ �ִ� �̹���
void Image::Render(HDC hdc, int destX, int destY, int frameX, int frameY)
{
	if (isTransparent)
	{
		GdiTransparentBlt(
			hdc,
			destX - (imageInfo->frameWidth / 2),
			destY - (imageInfo->frameHeight / 2),
			imageInfo->frameWidth, imageInfo->frameHeight,

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
			destX - (imageInfo->frameWidth / 2),		// ����� ��Ʈ���� ���� ��ġ x
			destY - (imageInfo->frameHeight / 2),		// ����� ��Ʈ���� ���� ��ġ y
			imageInfo->frameWidth,	// ���� ������ ���� ũ��
			imageInfo->frameHeight,	// ���� ������ ���� ũ��
			imageInfo->hMemDc,	// ���� DC
			imageInfo->frameWidth * frameX,				// ���� ��Ʈ�� ���� ���� ��ġ x
			imageInfo->frameHeight * frameY,			// ���� ��Ʈ�� ���� ���� ��ġ y
			SRCCOPY);			// ���� �ɼ�
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