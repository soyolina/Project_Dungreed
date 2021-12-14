#pragma once

#include "Config.h"
#include "CommonFunction.h"

class Player;
class ItemBase
{
protected:
	Player* m_player = nullptr;
	POINTFLOAT m_playerPos = {};

	// �⺻�� �̹��� ��ġ���̶� �÷��̾�� ������ ���� ������
	bool mb_imgToggle = false;

	bool mb_isAttack = false;
	bool mb_isAttack2 = false;
	

public:
	virtual void Init();
	virtual void Update();
	virtual void Render(HDC hdc);
	virtual void Release();

	// ������ �̹��� ������ ���Ű
	inline void SetImgToggle(bool toggle) { this->mb_imgToggle = toggle; }
	inline bool GetImgToggle() { return mb_imgToggle; }

	// ������ ����Ʈ �̹��� ���� ��ġ �����ϱ�
	//void SetEffectImgData(POINTFLOAT playerPos, int posNumX, int posNumY, int axisNumX, int axisNumY, float angle);
	

	// ���������� �ƴ��� ����
	inline void SetIsAttack(bool isAttack) { this->mb_isAttack = isAttack; }
	inline void SetIsAttack2(bool isAttack) { this->mb_isAttack2 = isAttack; }

	
};

