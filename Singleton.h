#pragma once

template <typename T>
class Singleton
{
public:
	virtual ~Singleton() = default;
	static T* GetSingleton()
	{
		// ó�� �� �Լ��� ó�� ȣ��� ��, instance ��ü�� �� Ÿ�Կ� ���� ���������
		// �ι�°�� ȣ��� ���� �� ������ ���ð� �ȴ�. �׸��� �׳� �ּ� ��ȯ
		static T instance;
		return &instance;
	}

protected:
	Singleton() {}
};