#pragma once

template <typename T>
class Singleton
{
public:
	static T* Instance()
	{
		if (!instance)
		{
			instance = new T();
		}

		return instance;
	}

	static void Release()
	{
		if (instance)
			delete instance;

		instance = nullptr;
	}

private:
	static T* instance;

protected:
	Singleton() {}
	virtual ~Singleton() {}
	Singleton(const Singleton&) = delete;
	Singleton(Singleton&&) noexcept = delete;
	Singleton& operator=(const Singleton&) = delete;
	Singleton&& operator=(Singleton&&) noexcept = delete;
};

template <typename T>
T* Singleton<T>::instance = nullptr;

