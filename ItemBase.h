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

	// �������� �ƴ��� ����
	bool mb_isAttack = false;
	bool mb_isAttack2 = false;

public:
	virtual void Init();
	virtual void Update();
	virtual void Render(HDC hdc);
	virtual void Release();

	// ������ �̹��� ������ ���Ű
	void SetImgToggle() 
	{ 
		if (GetImgToggle() == false)
		{
			this->mb_imgToggle = true;
		}
		else
		{
			this->mb_imgToggle = false;
		}
	}
	bool GetImgToggle() const { return mb_imgToggle; }

	// ���������� �ƴ��� ����
	void SetIsAttack(bool isAttack) { this->mb_isAttack = isAttack; }
	void SetIsAttack2(bool isAttack) { this->mb_isAttack2 = isAttack; }

	// ������ ����Ʈ �̹��� ���� ��ġ �����ϱ�
	//void SetEffectImgData(POINTFLOAT playerPos, int posNumX, int posNumY, int axisNumX, int axisNumY, float angle);
};

