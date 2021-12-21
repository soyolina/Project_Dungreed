#pragma once

#include "Enemy.h"

// - ��������
// ���� ��ü IDLE
#define BOSS_POSX  (WIN_SIZE_X * 0.5f - 23)
#define BOSS_POSY  (WIN_SIZE_Y * 0.5f - 65)

// �߾� ����
#define BOSSBACK_POSX (WIN_SIZE_X * 0.5f)
#define BOSSBACK_POSY (WIN_SIZE_Y * 0.5f)

// ��Ʈ �ڽ�
#define BOSS_HITBOX_POSX (BOSS_POSX + 22)
#define BOSS_HITBOX_POSY (BOSS_POSY + 20)

// ��
#define BOSS_LEFTHAND_POSX (WIN_SIZE_X * 0.18f)
#define BOSS_LEFTHAND_POSY (WIN_SIZE_Y * 0.7f)

#define BOSS_RIGHTHAND_POSX (WIN_SIZE_X * 0.82f)
#define BOSS_RIGHTHAND_POSY (WIN_SIZE_Y * 0.7f)

// ������ ���� - �Ӹ� �̹��� ��ġ
#define BOSS_LEFT_LASER_POSX (BOSS_LEFTHAND_POSX + 50)
#define BOSS_LEFT_LASER_POSY (m_leftHandPosY + 30)

#define BOSS_RIGHT_LASER_POSX (BOSS_RIGHTHAND_POSX - 50)
#define BOSS_RIGHT_LASER_POSY (m_rightHandPosY + 30)

// �̻��� ����
#define BOSS_AMMO_POSX  (BOSS_HITBOX_POSX)
#define BOSS_AMMO_POSY  (BOSS_HITBOX_POSY + 48)

// HP UI ����
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
	// �ݶ��̴�
	Collider2* m_laserCollider = nullptr;

	Player* m_player = nullptr;

	// --  ���� �ִϸ��̼ǿ�
	// - ��ü �ذ� 
	Image* m_bossImg = nullptr;
	Image* m_bossHitImg = nullptr;
	// �Լ� - ��ü �ذ� ��Ʈ�ڽ�
	void SetHitbox();

	// �ؿ� ������ �ذ�ü idle , �߾ӱ��� �ִϸ��̼��� �������� 
	int m_frameX = 0;
	int m_frameY = 0;
	int m_maxFrameX = 0;
	float m_elapsedCount = 0.0f;

	// - ��汸�� 
	// ū �߾� ����
	Image* m_bossBackImg = nullptr;	
	// - ���� ������
	// ����
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
	//�Լ�
	void SetParticleInitialPos(int num);
	void SetParticleImgData();
	void SetParticleImgPos(int num);
	void UpdateParticleAnimation();

	// -- ��
	// ����
	// ����
	enum class LeftHandStatus {Idle, Attack, End};
	enum class RightHandStatus {Idle, Attack, End};
	LeftHandStatus me_leftHandStatus = LeftHandStatus::End;
	RightHandStatus me_rightHandStatus = RightHandStatus::End;

	// �̹��� ������
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

	// �� y�� ��ġ
	float m_leftHandPosY = 0.0f;
	float m_rightHandPosY = 0.0f;
	// �� ������ �ӵ� - �÷��̾� ������ ��
	float m_handMoveSpeed = 0.0f;
	
	// �Լ�
	void RenderLeftHand(HDC hdc, LeftHandStatus status);
	void RenderRightHand(HDC hdc, RightHandStatus status);

	// - laser
	// ����
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
	// �Լ�
	void RenderLeftHandLaser(HDC hdc);
	void RenderRightHandLaser(HDC hdc);

	void SetLeftLaserHitbox(HDC hdc);
	void SetRightLaserHitbox(HDC hdc);

	// - fireLaser�Լ����� �� ������
	float m_handAttackDelay = 0.0f;
	int m_totalHandAttackCount = 0;	// ������ ���� ���� �� Ƚ��
	bool mb_isLefthandAttack = false; // �޼��� ������ �������߳�. false�� ���� ���� ���� ��. 

	bool mb_readyToFire = false; // ��� �̻��� �������Ͽ����� ����
	// �Լ�
	void FireLaserbeam();


	// �̻��� ��� ���� ��
	// ����
	AmmoManager* m_ammoManager = nullptr;
	POINTFLOAT m_ammoPos = {};
	float m_ammoInterval = 0.0f;	// �̻��� ���� Ƚ�� ���� �ð� ����
	float m_ammoAttackDelay = 0.0f;	// ���� �̻��ϵ��� �߻� ���� 
	float m_ammoAttackDuration = 0.0f; // �̻��� �ѹ� ���ݶ� �󸶵��� �̻��� �� ����
	float m_ammoAngle = 0.0f;
	float m_ammoChangeAngle = 0.0f;
	int m_totalAmmoAttackCount = 0;
	// �Լ�
	void FireMissile();

	// Attack Delay �� �� ���� ����
	bool mb_isAttack = false;
	float m_attackDelay = 0.0f;

	bool mb_fireLaserbeam = false;
	bool mb_fireAmmo = false;



	// - ���� HP UI ����
	// ����
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

