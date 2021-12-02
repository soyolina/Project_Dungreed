#pragma once
#include "Singleton.h"
#include "Config.h"

class Image;
class ImageManager : public Singleton<ImageManager>
{
private:
	map<string, Image*> mapImages;

public:
	virtual ~ImageManager() {}
	void Init();
	void Release();

	// ������ ���� ��
	Image* AddImage(const char* fileName, int width, int height,
		bool isTrans = false, COLORREF transColor = NULL);

	// ������ �ִ� ��
	Image* AddImage(const char* fileName, int width, int height,
		int maxFrameX, int maxFrameY,
		bool isTrans = false, COLORREF transColor = NULL);
	
	Image* FindImage(const char* fileName);
	void DeleteImage(const char* fileName);

};

