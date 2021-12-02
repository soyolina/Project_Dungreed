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
	SceneType* scene;
	FuncPtr function;
	Image* btnImage;


public:
	Button(SceneType* scenetype, FuncPtr functionAddress)
		: scene{ scenetype }, function{ functionAddress } {}

	HRESULT Init(POINT pos, int width, int height, string imgName);
	void Update();
	void Render(HDC hdc);
	void Release();
};

template<typename SceneType>
inline HRESULT Button<SceneType>::Init(POINT pos, int width, int height, string imgName)
{
}

template<typename SceneType>
inline void Button<SceneType>::Update()
{
}

template<typename SceneType>
inline void Button<SceneType>::Render(HDC hdc)
{
}

template<typename SceneType>
inline void Button<SceneType>::Release()
{
}
