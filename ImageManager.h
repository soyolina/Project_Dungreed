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

	// 프레임 없는 것
	Image* AddImage(LPCWSTR fileName, int width, int height,
		bool isTrans = false, COLORREF transColor = NULL);

	// 프레임 있는 것
	Image* AddImage(LPCWSTR fileName, int width, int height,
		int maxFrameX, int maxFrameY,
		bool isTrans = false, COLORREF transColor = NULL);
	
	Image* FindImage(LPCWSTR fileName);
	void DeleteImage(LPCWSTR fileName);

};

