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

		// 애니메이션 용 데이터
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
	HRESULT Init(int width, int hegiht); // 빈 비트맵 이미지 생성(Empty)
	HRESULT Init(const char* fileName, int width, int height,
		bool isTrans = false, COLORREF transColor = NULL); // File 프레임이 없는 이미지
	HRESULT Init(const char* fileName, int width, int height,
		int amxFrameX, int maxFrameY,
		bool isTrans = false, COLORREF transColor = NULL); // File 프레임이 있는 이미지

	void Release(); // 메모리 해제

	void Render(HDC hdc);
	void Render(HDC hdc, int destX, int destY); // 프레임이 없는 이미지 데이터가 dest좌표가 중점! 으로 화면에 복사
	void Render(HDC hdc, int destX, int destY, int m_frameX, int m_frameY); // 프레임이 있는 이미지 데이터를 화면에 복사 // dest좌표가 중점! 으로 이미지가 생성됨

	void RenderBasic(HDC hdc, int destX, int destY); // 프레임이 없는 이미지 데이터가 dest좌표를 시작점! 으로 화면에 복사

	// 루프 렌더(스타트 씬)
	void loopRender(HDC hdc, int sourX);


	inline int GetWidth() { return imageInfo->width; };
	inline int GetHeight() { return imageInfo->height; }

	inline HDC GetMemDC() { if (imageInfo) return imageInfo->hMemDc; return NULL; };
};

