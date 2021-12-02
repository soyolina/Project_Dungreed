#pragma once

#include "Config.h"
#include <time.h>

class Timer
{
private:
	bool isHardware = false;		// ���� Ÿ�̸Ӹ� �� �� �ִ��� ����
	__int64 periodFrequency{};		// ���� Ÿ�̸��� ���ļ�( �ʴ� ������ ), �������� ������ 0
	float timeScale{};				// 1�ʸ� �ʴ� �������� ������ ��� �������� �ð������� ��ȯ
	__int64 currTime{};				// ���� ���� ������ (���� �ð�)
	__int64 lastTime{};				// ���� ���� ������ (���� �ð�)
	float deltaTime{};				// ��� �ð� (currTime - lastTime) * timeScale

	float fpsTimeElapsed{};			// ������ ��� �ð�
	unsigned long fpsFrameCount{};	// �ʴ� ������ ��
	unsigned long fps{};

public:
	void Init();
	void Tick();

	inline unsigned long GetFPS() { return fps; }
	inline float GetDeltaTime() { return deltaTime; }

};

