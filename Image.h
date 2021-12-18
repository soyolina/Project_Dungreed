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

	// 이미지 뒤집기용
	HDC reverseDc = NULL;
	HBITMAP hReverseBitmap = NULL;
	HBITMAP hOldReverseBitmap = NULL;

	// Alphablend함수 사용위해 BLENDFUNCTION 구조체
	BLENDFUNCTION bf; 
	//bf.AlphaFormat = 0;		// 비트맵 종류로 일반 비트맵의 경우 0, 32비트 비트맵의 경우 AC_SRC_ALPHA
	//bf.BlendFlags = 0;		// 무조건 0이어야 한다
	//bf.BlendOp = AC_SRC_OVER; // 무조건 AC_SRC_OVER이어야 하고 원본과 대상 이미지를 합친다는 의미
	//bf.SourceConstantAlpha = 127; // 투명도(투명 0 - 불투명 255)

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
	LPIMAGE_INFO rotateImageInfo = nullptr;		// 로테이트 이미지
	LPIMAGE_INFO alphablendImageInfo = nullptr; // 반투명 이미지

	bool isTransparent = false;
	COLORREF transColor = 0;

public:
	// 빈 비트맵 이미지 생성(Empty)
	HRESULT Init(int width, int hegiht); 
	
	// File 프레임이 없는 이미지
	HRESULT Init(LPCWSTR fileName, int width, int height, bool isTrans = false, COLORREF transColor = NULL); 
	
	// File 프레임이 있는 이미지
	HRESULT Init(LPCWSTR fileName, int width, int height, int amxFrameX, int maxFrameY, bool isTrans = false, COLORREF transColor = NULL); 


	void Release(); // 메모리 해제


	void Render(HDC hdc);
	// 밑에 두 렌더는 reverse 값에 따라 이미지가 반전되어 출력될 수 있게 해놓음. 
	void Render(HDC hdc, int destX, int destY, float scale = 1.0f, bool reverse = false); // 프레임이 없는 이미지 데이터가 dest좌표가 중점! 으로 화면에 복사
	void Render(HDC hdc, int destX, int destY, int m_frameX, int m_frameY, float scale = 1.0f, bool reverse = false); // 프레임이 있는 이미지 데이터를 화면에 복사 // dest좌표가 중점! 으로 이미지가 생성됨

	void RenderBasic(HDC hdc, int destX, int destY); // 프레임이 없는 이미지 데이터가 dest좌표를 시작점! 으로 화면에 복사
	void RenderBasic(HDC hdc, int destX, int destY, int frameX, int frameY); // 프레임이 있는 이미지 데이터가 dest좌표를 시작점! 으로 화면에 복사

	// 루프 렌더(스타트 씬)
	void loopRender(HDC hdc, int sourX);
	
	// 플레이어 HP 렌더 - dest좌표를 시작으로 이미지 렌더
	void HpRender(HDC hdc, int destX, int destY, float remainHp);
	// 보스 HP 렌더 - dest좌표를 중점으로 이미지 렌더
	void HpRender2(HDC hdc, int destX, int destY, float remainHp);
	
	// 아이템 회전 렌더  - 실패
	void RotateRender2(HDC hdc, const POINT* rect);
	
	// 아이템 회전 렌더 진짜
	HBITMAP GetRotatedBitmap(HDC hdc, float angle, int m_frameX = 0, int m_frameY = 0);
	void RotateHDC(HDC hdc, float angle, int m_frameX = 0, int m_frameY = 0);
	// 프레임 없는 것
	void ImgRotateRender(HDC hdc, int destX, int destY, float angle);
	// 프레임 있는 것
	void ImgRotateFrameRender(HDC hdc, int destX, int destY, int frameX, int frameY, float angle);


	// 플레이어 히트시 반투명이미지를 위한 Alphablend 렌더
	void AlphaBlendHDC();
	void ImgAlphaBlendFrameRender(HDC hdc, int destX, int destY, int frameX, int frameY, BYTE transparancy);

	// 플레이어 히트시 뜨는 테두리 빨간 배경을 위한 알파렌더
	void ImgAlphaBlendRender(HDC hdc, int destX, int destY, BYTE transparancy);
	


	// Getter 등
	int GetWidth() const { return imageInfo->width; };
	int GetHeight() const { return imageInfo->height; }

	int GetFrameWidth() const { return imageInfo->frameWidth; }
	int GetFrameHeight() const { return imageInfo->frameHeight; }

	HDC GetMemDC() const { if (imageInfo) return imageInfo->hMemDc; return NULL; };

	// Ammo용
	int GetMaxFrameX() const { return imageInfo->maxFrameX; }


};

