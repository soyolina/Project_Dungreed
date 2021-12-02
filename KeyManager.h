#pragma once

#include <bitset>
#include "Config.h"
#include "Singleton.h"

#define KEY_MAX_COUNT 256

class KeyManager : public Singleton<KeyManager>
{
private:
	bitset<KEY_MAX_COUNT> keyUp;   // 키가 안 눌린 상태인지 아닌지
	bitset<KEY_MAX_COUNT> keyDown; // 키가 눌린 상태인지 아닌지

public:
	virtual ~KeyManager() {}
	
	void Init();
	void Release();
	
	bool IsOnceKeyDown(int key);	// 키가 첫번째로 눌렸는지 판단
	bool IsOnceKeyUp(int key);		// 키가 첫번째로 눌렸다가 떨어 졌는지 판단
	bool IsStayKeyDown(int key);	// 키가 눌려져 있는지 판다
};

