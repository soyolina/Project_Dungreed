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
	SceneType* m_scene = nullptr;
	FuncPtr m_function = nullptr;

	POINT m_btnPos = {};
	RECT m_btnRect = {};
	Image* m_btnImage = {};
	const char* m_btnImgName[2] = {};
	eButtonState me_BtnState = {};

public:
	Button(SceneType* scenetype, FuncPtr functionAddress)
		: m_scene{ scenetype }, m_function{ functionAddress } {}

	HRESULT Init(POINT pos, int width, int height, const char* imgName1, const char* imgName2 = nullptr);
	void Update();
	void Render(HDC hdc);
	void Release();
};

template<typename SceneType>
inline HRESULT Button<SceneType>::Init(POINT pos, int width, int height, const char* imgName1, const char* imgName2)
{
	m_btnPos = pos;
	m_btnRect = RectCenter(pos.x, pos.y, width, height);
	m_btnImage = IMAGE_MANAGER->FindImage(imgName1);
	m_btnImgName[0] = imgName1;
	m_btnImgName[1] = imgName2;

	me_BtnState = eButtonState::Idle;

	return S_OK;
}

template<typename SceneType>
inline void Button<SceneType>::Update()
{
	switch (me_BtnState)
	{
	case eButtonState::Idle:
		if (PtInRect(&m_btnRect, g_ptMouse))
		{
			me_BtnState = eButtonState::Put;
		}
		break;

	case eButtonState::Put:
		if (KEY_MANAGER->IsOnceKeyDown(VK_LBUTTON))
		{
			me_BtnState = eButtonState::Click;
		}

		if (PtInRect(&m_btnRect, g_ptMouse))
		{
			if (m_btnImgName[1] != nullptr)
			{
				m_btnImage = IMAGE_MANAGER->FindImage(m_btnImgName[1]);
			}
		}
		else
		{
			m_btnImage = IMAGE_MANAGER->FindImage(m_btnImgName[0]);
			me_BtnState = eButtonState::Idle;
		}
		break;

	case eButtonState::Click:
		if (KEY_MANAGER->IsOnceKeyUp(VK_LBUTTON))
		{
			if (PtInRect(&m_btnRect, g_ptMouse))
			{
				(m_scene->*m_function)();
			}
			me_BtnState = eButtonState::Idle;
		}
		break;
	}

}

template<typename SceneType>
inline void Button<SceneType>::Render(HDC hdc)
{
	Rectangle(hdc, m_btnRect.left, m_btnRect.top, m_btnRect.right, m_btnRect.bottom);
	m_btnImage->Render(hdc, m_btnPos.x, m_btnPos.y);
}

template<typename SceneType>
inline void Button<SceneType>::Release()
{
}
