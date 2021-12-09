#pragma once
#include "Singleton.h"
#include "Config.h"

class Image;
class ImageManager : public Singleton<ImageManager>
{
private:
	map<wstring, Image*> mapImages;

public:
	virtual ~ImageManager() {}
	void Init();
	void Release();

	// ������ ���� ��
	Image* AddImage(LPCWSTR fileName, int width, int height,
		bool isTrans = false, COLORREF transColor = NULL);

	// ������ �ִ� ��
	Image* AddImage(LPCWSTR fileName, int width, int height,
		int maxFrameX, int maxFrameY,
		bool isTrans = false, COLORREF transColor = NULL);
	
	Image* FindImage(LPCWSTR fileName);
	void DeleteImage(LPCWSTR fileName);

};

