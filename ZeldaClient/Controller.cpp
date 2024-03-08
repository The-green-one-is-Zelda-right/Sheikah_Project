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

		// playerBody �̵�
		const bool w = instance.IsKeyPressed(eKey::eKEY_W);
		const bool s = instance.IsKeyPressed(eKey::eKEY_S);
		const bool a = instance.IsKeyPressed(eKey::eKEY_A);
		const bool d = instance.IsKeyPressed(eKey::eKEY_D);

		const bool q = instance.IsKeyPressed(eKey::eKEY_Q);
		const bool e = instance.IsKeyPressed(eKey::eKEY_E);

		const bool up = instance.IsKeyPressed(eKey::eKEY_UP);
		const bool down = instance.IsKeyPressed(eKey::eKEY_DOWN);
		const bool left = instance.IsKeyPressed(eKey::eKEY_LEFT);
		const bool right = instance.IsKeyPressed(eKey::eKEY_RIGHT);

		const bool r = instance.IsKeyDown(eKey::eKEY_R);

		const bool space = instance.IsKeyPressed(eKey::eKEY_SPACE);

		Eigen::Vector3f velo = rigidbody->GetLinearVelocity();

		// �ӵ��� �̵��ϰ� drag�� ���߷��� �ϴµ�
		// drag�� ũ�� �������� �ӵ��� ������
		// drag�� ���ߴ°� �����ؾ��ҵ�
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
