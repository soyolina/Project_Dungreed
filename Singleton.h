#pragma once

template <typename T>
class Singleton
{
public:
	virtual ~Singleton() = default;
	static T* GetSingleton()
	{
		// 처음 이 함수가 처음 호출될 때, instance 객체가 그 타입에 따라 만들어지고
		// 두번째로 호출될 때는 밑 문장이 무시가 된다. 그리고 그냥 주소 반환
		static T instance;
		return &instance;
	}

protected:
	Singleton() {}
};