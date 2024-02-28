#include "Controller.h"

#include "TimeController.h"


namespace PurahEngine
{
	Controller::~Controller()
	= default;

	void Controller::Awake()
	{
		rigidbody = playerBody->GetComponent<RigidBody>();
		transform = gameObject->GetTransform();
		speed = 5.f;
		drag = 0.7f;
	}

	void Controller::Start()
	{
		startPosition = transform->GetWorldPosition();
		startRotation = transform->GetWorldRotation();
		startLinearVelocity = rigidbody->GetLinearVelocity();
		startAngularVelocity = rigidbody->GetAngularVelocity();
	}

	void Controller::Update()
	{
		Move();
	}

	void Controller::Move()
	{
		InputManager& instance = InputManager::Getinstance();
		TimeController& time = TimeController::GetInstance();

		// playerBody 이동
		const bool w = instance.IsKeyPressed('W');
		const bool s = instance.IsKeyPressed('S');
		const bool a = instance.IsKeyPressed('A');
		const bool d = instance.IsKeyPressed('D');

		const bool q = instance.IsKeyPressed('Q');
		const bool e = instance.IsKeyPressed('E');

		const bool up = instance.IsKeyPressed(VK_UP);
		const bool down = instance.IsKeyPressed(VK_DOWN);
		const bool left = instance.IsKeyPressed(VK_LEFT);
		const bool right = instance.IsKeyPressed(VK_RIGHT);

		const bool r = instance.IsKeyDown('R');

		const bool space = instance.IsKeyPressed(VK_SPACE);

		Eigen::Vector3f velo = rigidbody->GetLinearVelocity();

		// 속도로 이동하고 drag로 멈추려고 하는데
		// drag가 크면 떨어지는 속도도 느려짐
		// drag로 멈추는건 수정해야할듯
		if (w || s || a || d || q || e || r)
		{
			Eigen::Vector3f direction{ 0.f, 0.f, 0.f };
			const auto world = transform->GetWorldRotation();

			if (w)
			{
				direction += world * transform->front;
			}

			if (s)
			{
				direction -= world * transform->front;
			}

			if (a)
			{
				direction -= world * transform->right;
			}

			if (d)
			{
				direction += world * transform->right;
			}

			if (q)
			{
				direction -= world * transform->up;
			}

			if (e)
			{
				direction += world * transform->up;
			}

			if (r)
			{
				transform->SetWorldPosition(startPosition);
				transform->SetWorldRotation(startRotation);
				rigidbody->SetLinearVelocity(startLinearVelocity);
				rigidbody->SetAngularVelocity(startAngularVelocity);
			}

			direction.normalize();
			velo += direction * speed;

			const auto playerT = playerBody->GetTransform();
			// playerT->SetWorldRotation({ 0.f, direction.x(), direction.y(), direction.z() });
			transform->SetWorldRotation(world);
		}

		rigidbody->SetLinearVelocity(velo * drag);

		if (up | down | left | right)
		{
			const float dt = time.GetDeltaTime("Simulate");
			const float angle = 45.f * dt;
			const auto world = transform->GetWorldRotation();

			if (up)
			{
				transform->Rotate(world * transform->right, -angle);
			}

			if (down)
			{
				transform->Rotate(world * transform->right, angle);
			}

			if (left)
			{
				transform->Rotate(transform->up, -angle);
			}

			if (right)
			{
				transform->Rotate(transform->up, angle);
			}
		}
	}

	void Controller::HandsUp()
	{

	}

	void Controller::SetPlayer(GameObject* _player)
	{
		assert(_player != nullptr);

		playerBody = _player;
	}
}
