#pragma once

#include "Enemy.h"

// - 보스관련
// 보스 본체 IDLE
#define BOSS_POSX  (WIN_SIZE_X * 0.5f - 23)
#define BOSS_POSY  (WIN_SIZE_Y * 0.5f - 65)

// 중앙 구슬
#define BOSSBACK_POSX (WIN_SIZE_X * 0.5f)
#define BOSSBACK_POSY (WIN_SIZE_Y * 0.5f)

// 히트 박스
#define BOSS_HITBOX_POSX (BOSS_POSX + 22)
#define BOSS_HITBOX_POSY (BOSS_POSY + 20)

// 손
#define BOSS_LEFTHAND_POSX (WIN_SIZE_X * 0.18f)
#define BOSS_LEFTHAND_POSY (WIN_SIZE_Y * 0.7f)

#define BOSS_RIGHTHAND_POSX (WIN_SIZE_X * 0.82f)
#define BOSS_RIGHTHAND_POSY (WIN_SIZE_Y * 0.7f)

// HP UI 관련
#define BOSS_HP_UI_POSX  (WIN_SIZE_X * 0.5f)
#define BOSS_HP_UI_POSY  (WIN_SIZE_Y - 50)

#define BOSS_HP_GAGE_POSX (WIN_SIZE_X * 0.5f + 46.0f)
#define BOSS_HP_GAGE_POSY (WIN_SIZE_Y - 50)
 
class Image;
class Bellial : public Enemy
{
private:
	// --  보스 애니메이션용
	// - 본체 해골 
	Image* m_bossIdleImg = nullptr;

	// - 배경구슬 
	// 큰 중앙 구슬
	Image* m_bossBackImg = nullptr;	
	// - 작은 구슬들
	// 변수
	typedef struct ParticleImageData
	{
		Image* m_bossParticleImg = nullptr;	
		POINTFLOAT m_particleIdlePos = {};
		POINTFLOAT m_particlePos = {};
		float m_particleMoveSpeed = 0.0f;
		int m_particleFrameX = 0;
		int m_particleMaxFrameX = 0;
		float m_particleElapsedCount = 0.0f;
		bool mb_isAnimated = false;
	}PARTICLE_IMG_DATA;
	
	PARTICLE_IMG_DATA m_particleArr[7] = {};
	//함수
	void SetParticleInitialPos(int num);
	void SetParticleImgData();
	void SetParticleImgPos(int num);
	void UpdateParticleAnimation();

	// 손
	Image* m_bossLeftHandImg = nullptr;
	Image* m_bossRightHandImg = nullptr;
	float m_leftHandPosY = 0.0f;
	float m_rightHandPosY = 0.0f;

	int m_frameX = 0;
	int m_frameY = 0;
	int m_maxFrameX = 0;
	float m_elapsedCount = 0.0f;


	// - 보스 HP UI 관련
	// 변수
	Image* m_bossLifeBack = nullptr;
	Image* m_bossLifeGage = nullptr;
	Image* m_bossLifeBar = nullptr;

	float m_bossHpPercentage = 0.0f;

public:
	virtual ~Bellial() {}

	virtual HRESULT Init();
	virtual void Update();
	virtual void Render(HDC hdc);
	virtual void Release();

	void SetHitbox();
};

