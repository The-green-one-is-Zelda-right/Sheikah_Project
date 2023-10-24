#pragma once

namespace PurahEngine
{
	class SceneManager
	{
	public:

	private:
		SceneManager();
		~SceneManager();
		SceneManager(const SceneManager& ref) = delete;
		// Ŭ������ �����ϰ� �� ���, �⺻������ = �� ���� ������ �����ε��� �����ȴ�.
		// �̱����� ��ü�� �ϳ����� �ϹǷ� �װ��� �����ϱ� ����, ��������� delete�� ����Ͽ� ����� ���´�.
		SceneManager& operator=(const SceneManager& ref) = delete;
	public:
		static SceneManager& GetInstance();
	};
}

