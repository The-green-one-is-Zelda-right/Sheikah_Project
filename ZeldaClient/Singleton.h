#pragma once

template <typename T>
class Singleton
{
public:
	static T* Instance()
	{
		static T* instance = nullptr;

		if (!instance)
		{
			instance = new T;
		}

		return instance;
	}

	static void Release()
	{
		auto instance = Instance();

		delete instance;
	}

private:
	Singleton();
	~Singleton();
	Singleton(const Singleton&) = delete;
	Singleton(Singleton&&) noexcept = delete;
	Singleton& operator=(const Singleton&) = delete;
	Singleton& operator=(Singleton&&) noexcept = delete;
};

