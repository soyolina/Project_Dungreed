#pragma once

#include "GameObject.h"

#define JUMP_STRENGTH 600.0f

enum class PlayerStatus {Idle, Run, Dead, End};

class Collider2;
class ItemBase;
class Image;
class Player : public GameObject
{
private:

	// �÷��̾� ��Ʈ�� ���
	Image* m_hitBackGround = nullptr;
	BYTE m_hitBackGroundTransparancy = {};

	// �÷��̾ �ٶ󺸰� �ִ� ����
	bool mb_isLeft = false;
	
	// - �÷��̾� ����
	PlayerStatus me_PlayerStatus = {};

	// - �÷��̾� �̹��� �ִϸ��̼� ������
	// ����
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
	// �Լ�
	void PlayAnimation(HDC hdc, PlayerStatus playerStatus);


	// - ���� ���� ( + �߷�, �浹 �� ����ó������ ����)
	// ����
	bool mb_isJump = false;
	bool mb_isDownJump = false;
	int m_jumpCount = 0;
	float m_jumpStrength = 0.0f;
	float m_gravity = 0.0f;

	bool mb_isCollide = false;
	long m_beforePlayerBottom = 0;
	// �Լ�
	// -> �̵��� �������� �Լ�
	void Move();
	// -> �߷����� �Լ�
	void ApplyGravity();


	// - �÷��̾ �̵��� �߻��ϴ� ���� ����Ʈ ����
	// ����
	Image* m_runEffectImg = nullptr;
	bool mb_isMove = false;
	float m_dustEffectCount = 0.0f;
	int m_dustFrameX = 0;
	int m_dustMaxFrameX = 0;
	// �Լ�
	void RunEffectAnimation(HDC hdc);


	// - �뽬����
	// ����
	Image* m_dashEffectImg = nullptr;
	bool mb_isDash = false;
	float m_angle = 0.0f;
	int m_dashCount = 0;
	int m_maxDashCount = 0;
	float m_dashSpeed = 0.0f;
	float m_dashRegenTime = 0.0f;
	
	int m_dashTimer = 0;
	int m_dashFrameX = 0;
	POINTFLOAT m_beforePlayerPos = {};
	// �Լ�
	void Dash();
	void RegenDash();


	// - ������ ����
	// ����
	ItemBase* m_weapon[2] = {};
	int m_selectedWeaponIndex = 0;

public:
	// �Լ�
	void SetItem(int index, ItemBase* item) { this->m_weapon[index] = item; }
private:
	// ����
	void Attack();

public:
	virtual ~Player() {}

	virtual HRESULT Init();
	virtual void Update();
	virtual void Render(HDC hdc);
	virtual void Release();

	// ��Ʈ�ڽ� ����
	void SetShape(const POINTFLOAT& playerPos, const int& bodyWidth, const int& bodyHeight);
	
	// ��Ʈ�� ����
	void Hit();


	// DashCount UI���� �������ؼ�
	int GetPlayerMaxDashCount() const { return m_maxDashCount; }
	int GetPlayerDashCount() const { return m_dashCount; }
	
	// HP UI���� ���� ���ؼ�
	int GetPlayerHP() const { return m_hp; }

	// BasicShortSword - �� �̹��� ���� ��ǥ ���ؼ�, Bellial - �÷��̾� �������ؼ�
	POINTFLOAT GetPlayerPos() const { return m_pos; }
	// BasicShortSword���� - �̹��� ���� �ݴ�� �ϴ°� �Ǵ� ���� ��ġ�� �����ϱ� ���ؼ�
	bool GetPlayerReverse() const { return mb_isLeft; }
};

