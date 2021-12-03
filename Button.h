#pragma once

#include "CommonFunction.h"
#include "Config.h"

enum class eButtonState {Idle, Put, Click, End};

class Image;
template <typename SceneType>
class Button
{
public:
	using FuncPtr = void(SceneType::*)();

private:
	SceneType* scene = nullptr;
	FuncPtr function = nullptr;

	POINT btnPos = {};
	RECT btnRect = {};
	Image* btnImage = {};
	const char* btnImgName[2] = {};
	eButtonState eBtnState = {};

public:
	Button(SceneType* scenetype, FuncPtr functionAddress)
		: scene{ scenetype }, function{ functionAddress } {}

	HRESULT Init(POINT pos, int width, int height, const char* imgName1, const char* imgName2 = nullptr);
	void Update();
	void Render(HDC hdc);
	void Release();
};

template<typename SceneType>
inline HRESULT Button<SceneType>::Init(POINT pos, int width, int height, const char* imgName1, const char* imgName2)
{
	btnPos = pos;
	btnRect = RectCenter(pos.x, pos.y, width, height);
	btnImage = IMAGE_MANAGER->FindImage(imgName1);
	btnImgName[0] = imgName1;
	btnImgName[1] = imgName2;

	eBtnState = eButtonState::Idle;

	return S_OK;
}

template<typename SceneType>
inline void Button<SceneType>::Update()
{
	switch (eBtnState)
	{
	case eButtonState::Idle:
		if (PtInRect(&btnRect, g_ptMouse))
		{
			eBtnState = eButtonState::Put;
		}
		break;

	case eButtonState::Put:
		if (KEY_MANAGER->IsOnceKeyDown(VK_LBUTTON))
		{
			eBtnState = eButtonState::Click;
		}

		if (PtInRect(&btnRect, g_ptMouse))
		{
			if (btnImgName[1] != nullptr)
			{
				btnImage = IMAGE_MANAGER->FindImage(btnImgName[1]);
			}
		}
		else
		{
			btnImage = IMAGE_MANAGER->FindImage(btnImgName[0]);
			eBtnState = eButtonState::Idle;
		}
		break;

	case eButtonState::Click:
		if (KEY_MANAGER->IsOnceKeyUp(VK_LBUTTON))
		{
			if (PtInRect(&btnRect, g_ptMouse))
			{
				(scene->*function)();
			}
			eBtnState = eButtonState::Idle;
		}
		break;
	}

}

template<typename SceneType>
inline void Button<SceneType>::Render(HDC hdc)
{
	Rectangle(hdc, btnRect.left, btnRect.top, btnRect.right, btnRect.bottom);
	btnImage->Render(hdc, btnPos.x, btnPos.y);
}

template<typename SceneType>
inline void Button<SceneType>::Release()
{
}
