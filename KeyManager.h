#pragma once

#include <bitset>
#include "Config.h"
#include "Singleton.h"

#define KEY_MAX_COUNT 256

class KeyManager : public Singleton<KeyManager>
{
private:
	bitset<KEY_MAX_COUNT> keyUp;   // Ű�� �� ���� �������� �ƴ���
	bitset<KEY_MAX_COUNT> keyDown; // Ű�� ���� �������� �ƴ���

public:
	virtual ~KeyManager() {}
	
	void Init();
	void Release();
	
	bool IsOnceKeyDown(int key);	// Ű�� ù��°�� ���ȴ��� �Ǵ�
	bool IsOnceKeyUp(int key);		// Ű�� ù��°�� ���ȴٰ� ���� ������ �Ǵ�
	bool IsStayKeyDown(int key);	// Ű�� ������ �ִ��� �Ǵ�
};

