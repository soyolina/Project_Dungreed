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

	// 플레이어 히트시 배경
	Image* m_hitBackGround = nullptr;
	BYTE m_hitBackGroundTransparancy = {};

	// 플레이어가 바라보고 있는 방향
	bool mb_isLeft = false;
	
	// - 플레이어 상태
	PlayerStatus me_PlayerStatus = {};

	// - 플레이어 이미지 애니메이션 렌더용
	// 변수
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
	// 함수
	void PlayAnimation(HDC hdc, PlayerStatus playerStatus);


	// - 점프 관련 ( + 중력, 충돌 후 보정처리까지 관련)
	// 변수
	bool mb_isJump = false;
	bool mb_isDownJump = false;
	int m_jumpCount = 0;
	float m_jumpStrength = 0.0f;
	float m_gravity = 0.0f;

	bool mb_isCollide = false;
	long m_beforePlayerBottom = 0;
	// 함수
	// -> 이동과 점프관련 함수
	void Move();
	// -> 중력적용 함수
	void ApplyGravity();


	// - 플레이어가 이동시 발생하는 먼지 이펙트 관련
	// 변수
	Image* m_runEffectImg = nullptr;
	bool mb_isMove = false;
	float m_dustEffectCount = 0.0f;
	int m_dustFrameX = 0;
	int m_dustMaxFrameX = 0;
	// 함수
	void RunEffectAnimation(HDC hdc);


	// - 대쉬관련
	// 변수
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
	// 함수
	void Dash();
	void RegenDash();


	// - 아이템 관련
	// 변수
	ItemBase* m_weapon[2] = {};
	int m_selectedWeaponIndex = 0;

public:
	// 함수
	void SetItem(int index, ItemBase* item) { this->m_weapon[index] = item; }
private:
	// 공격
	void Attack();

public:
	virtual ~Player() {}

	virtual HRESULT Init();
	virtual void Update();
	virtual void Render(HDC hdc);
	virtual void Release();

	// 히트박스 셋팅
	void SetShape(const POINTFLOAT& playerPos, const int& bodyWidth, const int& bodyHeight);
	
	// 히트시 관련
	void Hit();


	// DashCount UI에서 쓰기위해서
	int GetPlayerMaxDashCount() const { return m_maxDashCount; }
	int GetPlayerDashCount() const { return m_dashCount; }
	
	// HP UI에서 쓰기 위해서
	int GetPlayerHP() const { return m_hp; }

	// BasicShortSword - 검 이미지 렌더 좌표 위해서, Bellial - 플레이어 공격위해서
	POINTFLOAT GetPlayerPos() const { return m_pos; }
	// BasicShortSword에서 - 이미지 렌더 반대로 하는거 또는 렌더 위치를 설정하기 위해서
	bool GetPlayerReverse() const { return mb_isLeft; }
};

