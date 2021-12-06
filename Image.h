#pragma once
#include "Config.h"

class Image
{
public:
	enum ImageLoadType
	{
		Resource,
		File,
		Empty,
		End
	};

	typedef struct tagImageInfo
	{
		HDC hMemDc = NULL;
		HBITMAP hBitmap = NULL;
		HBITMAP hOldBit = NULL;
		int width = 0;
		int height = 0;
		BYTE loadType = ImageLoadType::Empty;

		// �ִϸ��̼� �� ������
		int maxFrameX = 0;
		int maxFrameY = 0;
		int frameWidth = 0;
		int frameHeight = 0;
		int currFrameX = 0;
		int currFrameY = 0;
	} IMAGE_INFO, *LPIMAGE_INFO;

private:
	LPIMAGE_INFO imageInfo = nullptr;
	bool isTransparent = false;
	COLORREF transColor = 0;

public:
	HRESULT Init(int width, int hegiht); // �� ��Ʈ�� �̹��� ����(Empty)
	HRESULT Init(const char* fileName, int width, int height,
		bool isTrans = false, COLORREF transColor = NULL); // File �������� ���� �̹���
	HRESULT Init(const char* fileName, int width, int height,
		int amxFrameX, int maxFrameY,
		bool isTrans = false, COLORREF transColor = NULL); // File �������� �ִ� �̹���

	void Release(); // �޸� ����

	void Render(HDC hdc);
	void Render(HDC hdc, int destX, int destY); // �������� ���� �̹��� �����Ͱ� dest��ǥ�� ����! ���� ȭ�鿡 ����
	void Render(HDC hdc, int destX, int destY, int m_frameX, int m_frameY); // �������� �ִ� �̹��� �����͸� ȭ�鿡 ���� // dest��ǥ�� ����! ���� �̹����� ������

	void RenderBasic(HDC hdc, int destX, int destY); // �������� ���� �̹��� �����Ͱ� dest��ǥ�� ������! ���� ȭ�鿡 ����

	// ���� ����(��ŸƮ ��)
	void loopRender(HDC hdc, int sourX);


	inline int GetWidth() { return imageInfo->width; };
	inline int GetHeight() { return imageInfo->height; }

	inline HDC GetMemDC() { if (imageInfo) return imageInfo->hMemDc; return NULL; };
};

