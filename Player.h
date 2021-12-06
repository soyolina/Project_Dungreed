#pragma once

#include "GameObject.h"

#define JUMP_STRENGTH 600.0f

enum class PlayerStatus {Idle, Run, Jump, End};

class Image;
class Player : public GameObject
{
private:
	Image* m_playerEffectImg = nullptr;
	Image* m_runEffectImg = nullptr;

	int m_frameX = 0;
	int m_frameY = 0;
	float m_elapsedCount = 0.0f;

	typedef struct AnimationData
	{
		Image* playerImage = nullptr;
		int maxFrameX = 0;
		float totalTime = 0.0f;
		
	}ANI_DATA;

	ANI_DATA m_statusAniData[3] = {};

	PlayerStatus me_PlayerStatus = {};

	bool mb_isJump = false;
	bool mb_isDownJump = false;
	int m_jumpCount = 0;
	float m_jumpStrength = 0.0f;
	float m_gravity = 0.0f;

	bool mb_isCollide = false;

	// 테스트 상자
	RECT testRC = {};
	RECT testRC2 = {};
	RECT testRC3 = {};
	RECT testRC4 = {};
	RECT testRC5 = {};
	RECT rectArr[5] = {};

	RECT intersectRect = {};
	RECT collidedRect = {};
	
public:
	virtual ~Player() {}

	virtual HRESULT Init();
	virtual void Update();
	virtual void Render(HDC hdc);
	virtual void Release();

	void SetShape(POINTFLOAT playerPos, int bodyWidth, int bodyHeight);

	void Animation(HDC hdc, PlayerStatus playerStatus);

	// 임시 테스트 상자 생성
	void makeTestRect();
};

