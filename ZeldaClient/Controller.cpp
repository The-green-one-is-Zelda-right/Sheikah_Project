


#include "Controller.h"


namespace PurahEngine
{
	Controller::~Controller()
	= default;

	void Controller::Awake()
	{
		rigidbody = gameObject->GetComponent<RigidBody>();
		transform = gameObject->GetComponent<Transform>();
		speed = 10.f;
		drag = 0.1f;
	}

	void Controller::Start()
	{

	}

	void Controller::Update()
	{
		InputManager& instance = InputManager::Getinstance();

		const bool w = instance.IsKeyDown('W');
		const bool s = instance.IsKeyDown('S');
		const bool a = instance.IsKeyDown('A');
		const bool d = instance.IsKeyDown('D');

		const bool q = instance.IsKeyDown('Q');
		const bool e = instance.IsKeyDown('E');

		const bool space = instance.IsKeyDown('E');

		auto velo = rigidbody->GetLinearVelocity();
		rigidbody->SetLinearVelocity(velo * drag);

		if (w || s || a || d || q || e)
		{
			Eigen::Vector3f direction{ 0.f, 0.f, 0.f };

			if (w)
			{
				direction += transform->front;
			}

			if (s)
			{
				direction -= transform->front;
			}

			if (a)
			{
				direction -= transform->right;
			}

			if (d)
			{
				direction += transform->right;
			}

			if (q)
			{
				direction -= transform->up;
			}

			if (e)
			{
				direction += transform->up;
			}

			rigidbody->SetLinearVelocity(direction * speed);
		}
	}

	void Controller::FixedUpdate()
	{

	}
}
