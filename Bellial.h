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

// 레이저 관련 - 머리 이미지 위치
#define BOSS_LEFT_LASER_POSX (BOSS_LEFTHAND_POSX + 50)
#define BOSS_LEFT_LASER_POSY (m_leftHandPosY + 30)

#define BOSS_RIGHT_LASER_POSX (BOSS_RIGHTHAND_POSX - 50)
#define BOSS_RIGHT_LASER_POSY (m_rightHandPosY + 30)

// 미사일 관련
#define BOSS_AMMO_POSX  (BOSS_HITBOX_POSX)
#define BOSS_AMMO_POSY  (BOSS_HITBOX_POSY + 48)

// HP UI 관련
#define BOSS_HP_UI_POSX  (WIN_SIZE_X * 0.5f)
#define BOSS_HP_UI_POSY  (WIN_SIZE_Y - 50)

#define BOSS_HP_GAGE_POSX (WIN_SIZE_X * 0.5f + 46.0f)
#define BOSS_HP_GAGE_POSY (WIN_SIZE_Y - 50)
 
class AmmoManager;
class Player;
class Image;
class Bellial : public Enemy
{
private:
	// 콜라이더
	Collider2* m_laserCollider = nullptr;

	Player* m_player = nullptr;

	// --  보스 애니메이션용
	// - 본체 해골 
	Image* m_bossImg = nullptr;
	Image* m_bossHitImg = nullptr;
	// 함수 - 본체 해골 히트박스
	void SetHitbox();

	// 밑에 변수는 해골본체 idle , 중앙구슬 애니메이션이 쓰고있음 
	int m_frameX = 0;
	int m_frameY = 0;
	int m_maxFrameX = 0;
	float m_elapsedCount = 0.0f;

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

	// -- 손
	// 변수
	// 상태
	enum class LeftHandStatus {Idle, Attack, End};
	enum class RightHandStatus {Idle, Attack, End};
	LeftHandStatus me_leftHandStatus = LeftHandStatus::End;
	RightHandStatus me_rightHandStatus = RightHandStatus::End;

	// 이미지 렌더용
	typedef struct HandImgData
	{
		Image* handImg = nullptr;
		int frameX = 0;
		int frameY = 0;
		int maxFrameX = 0;
		float totalTime = 0.0f;
	}HAND_IMG_DATA;

	HAND_IMG_DATA m_leftHand[static_cast<int>(LeftHandStatus::End)];
	HAND_IMG_DATA m_rightHand[static_cast<int>(RightHandStatus::End)];
	float m_lefthandElapsedCount = 0.0f;
	float m_righthandElapsedCount = 0.0f;

	// 손 y축 위치
	float m_leftHandPosY = 0.0f;
	float m_rightHandPosY = 0.0f;
	// 손 움직일 속도 - 플레이어 추적할 때
	float m_handMoveSpeed = 0.0f;
	
	// 함수
	void RenderLeftHand(HDC hdc, LeftHandStatus status);
	void RenderRightHand(HDC hdc, RightHandStatus status);

	// - laser
	// 변수
	Image* m_laserBody = nullptr;
	Image* m_laserHead = nullptr;

	RECT m_laserHitbox = {};
	int m_leftLaserFrameX = 0;
	int m_rightLaserFrameX = 0;
	int m_rightLaserBodyFrameX = 0;
	int m_LaserMaxFrameX = 0;
	float m_leftLaserElapsedCount = 0.0f;
	float m_rightLaserElapsedCount = 0.0f;
	bool mb_leftLaserAttack = false;
	bool mb_rightLaserAttack = false;
	// 함수
	void RenderLeftHandLaser(HDC hdc);
	void RenderRightHandLaser(HDC hdc);

	void SetLeftLaserHitbox(HDC hdc);
	void SetRightLaserHitbox(HDC hdc);

	// - fireLaser함수에서 쓸 변수들
	float m_handAttackDelay = 0.0f;
	int m_totalHandAttackCount = 0;	// 레이저 공격 가능 총 횟수
	bool mb_isLefthandAttack = false; // 왼손이 이전에 공격을했냐. false면 아직 공격 안한 것. 

	bool mb_readyToFire = false; // 얘는 미사일 공격패턴에서도 쓰임
	// 함수
	void FireLaserbeam();


	// 미사일 쏘는 패턴 용
	// 변수
	AmmoManager* m_ammoManager = nullptr;
	POINTFLOAT m_ammoPos = {};
	float m_ammoInterval = 0.0f;	// 미사일 공격 횟수 간의 시간 간격
	float m_ammoAttackDelay = 0.0f;	// 각각 미사일들의 발사 간격 
	float m_ammoAttackDuration = 0.0f; // 미사일 한번 공격때 얼마동안 미사일 쏠 껀지
	float m_ammoAngle = 0.0f;
	float m_ammoChangeAngle = 0.0f;
	int m_totalAmmoAttackCount = 0;
	// 함수
	void FireMissile();

	// Attack Delay 와 각 어택 관련
	bool mb_isAttack = false;
	float m_attackDelay = 0.0f;

	bool mb_fireLaserbeam = false;
	bool mb_fireAmmo = false;



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


	void SetPlayer(Player* player) { this->m_player = player; }
	void SetAmmoManager(AmmoManager* ammoManager) { this->m_ammoManager = ammoManager; }
};

