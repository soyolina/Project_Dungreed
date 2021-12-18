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

	// �̹��� �������
	HDC reverseDc = NULL;
	HBITMAP hReverseBitmap = NULL;
	HBITMAP hOldReverseBitmap = NULL;

	// Alphablend�Լ� ������� BLENDFUNCTION ����ü
	BLENDFUNCTION bf; 
	//bf.AlphaFormat = 0;		// ��Ʈ�� ������ �Ϲ� ��Ʈ���� ��� 0, 32��Ʈ ��Ʈ���� ��� AC_SRC_ALPHA
	//bf.BlendFlags = 0;		// ������ 0�̾�� �Ѵ�
	//bf.BlendOp = AC_SRC_OVER; // ������ AC_SRC_OVER�̾�� �ϰ� ������ ��� �̹����� ��ģ�ٴ� �ǹ�
	//bf.SourceConstantAlpha = 127; // ����(���� 0 - ������ 255)

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
	LPIMAGE_INFO rotateImageInfo = nullptr;		// ������Ʈ �̹���
	LPIMAGE_INFO alphablendImageInfo = nullptr; // ������ �̹���

	bool isTransparent = false;
	COLORREF transColor = 0;

public:
	// �� ��Ʈ�� �̹��� ����(Empty)
	HRESULT Init(int width, int hegiht); 
	
	// File �������� ���� �̹���
	HRESULT Init(LPCWSTR fileName, int width, int height, bool isTrans = false, COLORREF transColor = NULL); 
	
	// File �������� �ִ� �̹���
	HRESULT Init(LPCWSTR fileName, int width, int height, int amxFrameX, int maxFrameY, bool isTrans = false, COLORREF transColor = NULL); 


	void Release(); // �޸� ����


	void Render(HDC hdc);
	// �ؿ� �� ������ reverse ���� ���� �̹����� �����Ǿ� ��µ� �� �ְ� �س���. 
	void Render(HDC hdc, int destX, int destY, float scale = 1.0f, bool reverse = false); // �������� ���� �̹��� �����Ͱ� dest��ǥ�� ����! ���� ȭ�鿡 ����
	void Render(HDC hdc, int destX, int destY, int m_frameX, int m_frameY, float scale = 1.0f, bool reverse = false); // �������� �ִ� �̹��� �����͸� ȭ�鿡 ���� // dest��ǥ�� ����! ���� �̹����� ������

	void RenderBasic(HDC hdc, int destX, int destY); // �������� ���� �̹��� �����Ͱ� dest��ǥ�� ������! ���� ȭ�鿡 ����
	void RenderBasic(HDC hdc, int destX, int destY, int frameX, int frameY); // �������� �ִ� �̹��� �����Ͱ� dest��ǥ�� ������! ���� ȭ�鿡 ����

	// ���� ����(��ŸƮ ��)
	void loopRender(HDC hdc, int sourX);
	
	// �÷��̾� HP ���� - dest��ǥ�� �������� �̹��� ����
	void HpRender(HDC hdc, int destX, int destY, float remainHp);
	// ���� HP ���� - dest��ǥ�� �������� �̹��� ����
	void HpRender2(HDC hdc, int destX, int destY, float remainHp);
	
	// ������ ȸ�� ����  - ����
	void RotateRender2(HDC hdc, const POINT* rect);
	
	// ������ ȸ�� ���� ��¥
	HBITMAP GetRotatedBitmap(HDC hdc, float angle, int m_frameX = 0, int m_frameY = 0);
	void RotateHDC(HDC hdc, float angle, int m_frameX = 0, int m_frameY = 0);
	// ������ ���� ��
	void ImgRotateRender(HDC hdc, int destX, int destY, float angle);
	// ������ �ִ� ��
	void ImgRotateFrameRender(HDC hdc, int destX, int destY, int frameX, int frameY, float angle);


	// �÷��̾� ��Ʈ�� �������̹����� ���� Alphablend ����
	void AlphaBlendHDC();
	void ImgAlphaBlendFrameRender(HDC hdc, int destX, int destY, int frameX, int frameY, BYTE transparancy);

	// �÷��̾� ��Ʈ�� �ߴ� �׵θ� ���� ����� ���� ���ķ���
	void ImgAlphaBlendRender(HDC hdc, int destX, int destY, BYTE transparancy);
	


	// Getter ��
	int GetWidth() const { return imageInfo->width; };
	int GetHeight() const { return imageInfo->height; }

	int GetFrameWidth() const { return imageInfo->frameWidth; }
	int GetFrameHeight() const { return imageInfo->frameHeight; }

	HDC GetMemDC() const { if (imageInfo) return imageInfo->hMemDc; return NULL; };

	// Ammo��
	int GetMaxFrameX() const { return imageInfo->maxFrameX; }


};

