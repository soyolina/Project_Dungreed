#pragma once

#include "GameObject.h"

#define JUMP_STRENGTH 600.0f

enum class PlayerStatus {Idle, Run, Jump, End};

class Image;
class Player : public GameObject
{
private:
	Image* m_dashEffectImg = nullptr;
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

	ANI_DATA m_statusAniData[static_cast<int>(PlayerStatus::End)] = {};

	PlayerStatus me_PlayerStatus = {};

	bool mb_isJump = false;
	bool mb_isDownJump = false;
	int m_jumpCount = 0;
	float m_jumpStrength = 0.0f;
	float m_gravity = 0.0f;

	bool mb_isCollide = false;

	long m_beforePlayerBottom = 0;

	// �÷��̾ �ٶ󺸰� �ִ� ����
	bool mb_isLeft = false;

	// �÷��̾ �̵��� �߻��ϴ� ���� ����Ʈ
	bool mb_isMove = false;
	float m_dustEffectCount = 0.0f;
	int m_dustFrameX = 0;
	int m_dustMaxFrameX = 0;

	// �뽬����
	bool mb_isDash = false;
	float m_angle = 0.0f;
	int m_dashCount = 0;
	int m_maxDashCount = 0;
	float m_dashSpeed = 0.0f;
	float m_dashRegenTime = 0.0f;
	
	int m_dashTimer = 0;
	int m_dashFrameX = 0;
	POINTFLOAT m_beforePlayerPos = {};

	// �׽�Ʈ ����
	RECT testRC = {};
	RECT testRC2 = {};
	RECT testRC3 = {};
	RECT testRC4 = {};
	RECT testRC5 = {};
	RECT testRC6 = {};
	RECT rectArr[6] = {};

	RECT intersectRect = {};
	RECT collidedRect = {};
	
public:
	virtual ~Player() {}

	virtual HRESULT Init();
	virtual void Update();
	virtual void Render(HDC hdc);
	virtual void Release();

	void SetShape(POINTFLOAT playerPos, int bodyWidth, int bodyHeight);

	void Dash();
	void DashRegen();
	void Move();
	void Animation(HDC hdc, PlayerStatus playerStatus);
	void RunEffectAnimation(HDC hdc);

	// �ӽ� �׽�Ʈ ���� ����
	void makeTestRect();
};

