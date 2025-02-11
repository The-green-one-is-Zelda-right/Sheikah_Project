#define NOMINMAX
#include <algorithm>

#include "PlayerCamera.h"
#include "Player.h"
#include "PlayerController.h"

namespace Phyzzle
{
	PlayerCamera::PlayerCamera(Player* player)
		: player(player)
	{
		armDefaultPosition = player->GetCameraArmTrnasform()->GetLocalPosition();
		armDefaultRotation = player->GetCameraArmTrnasform()->GetLocalRotation();

		coreDefaultPosition = player->GetCameraCoreTrnasform()->GetLocalPosition();
		coreDefaultRotation = player->GetCameraCoreTrnasform()->GetLocalRotation();

		coreTargetRotation = Eigen::Quaternionf::Identity();

		coreSelectPosition = coreDefaultPosition + Eigen::Vector3f{ 0.5f, 0.5f, 0.f };

		highPosition = Eigen::Vector3f(0.f, 0.f, -10.f);
		lowPosition = Eigen::Vector3f(0.f, 0.f, -2.f);
		differenceHigh = highPosition - coreDefaultPosition;
		differenceLow = lowPosition - coreDefaultPosition;
	}

	void PlayerCamera::UpdateCamera()
	{
		UpdateCameraPositionLerp();
		UpdateCameraRotationLerp();

		CharacterDisable();
	}

	void PlayerCamera::UpdateDefaultCamera()
	{
		RotateCameraArm();
		UpdateDefaultCameraCore();
	}

	void PlayerCamera::UpdateSelectCamera()
	{
		RotateCameraArm();
		UpdateSelectCameraCore();
	}

	void PlayerCamera::UpdateDefaultCameraCore()
	{
		using namespace Eigen;

		Vector3f localPosition = Vector3f::Zero();
		Vector3f worldPosition = Vector3f::Zero();
		// 각도에 따라 위치를 우선 제한함
		CalculateDefaultCameraCorePosition(localPosition, worldPosition, false);

		// 오브젝트에 충돌하면 처리함
		if (ResolveCameraCollision(localPosition, worldPosition))
		{
			SetCameraCoreWorldTargetPosition(worldPosition);
		}
		else
		{
			SetCameraCoreLocalTargetPosition(localPosition);
		}
	}

	void PlayerCamera::UpdateSelectCameraCore()
	{
		using namespace Eigen;

		Vector3f localPosition = Vector3f::Zero();
		Vector3f worldPosition = Vector3f::Zero();
		// 각도에 따라 위치를 우선 제한함
		CalculateDefaultCameraCorePosition(localPosition, worldPosition, true);

		// 오브젝트에 충돌하면 처리함
		if (ResolveCameraCollision(localPosition, worldPosition))
		{
			SetCameraCoreWorldTargetPosition(worldPosition);
		}
		else
		{
			SetCameraCoreLocalTargetPosition(localPosition);
		}
	}

	void PlayerCamera::CalculateDefaultCameraCorePosition(Eigen::Vector3f& localOut, Eigen::Vector3f& worldOut, bool _isSelect)
	{
		using namespace Eigen;

		const float ease = xAngle >= 0.f ? xAngle / limitHighAngle : xAngle / limitLowAngle;
		auto easingFunc = xAngle >= 0.f ?
			[](float x) { return 1.0f - (1.0f - x) * (1.0f - x); } :
			[](float x) { return 1.0f - std::pow(1.0f - x, 5.0f); };

		const Vector3f& diff = xAngle >= 0.f ? differenceHigh : differenceLow;

		Vector3f offset = _isSelect ? Vector3f{ 0.5f, 0.5f, 0.f } : Vector3f::Zero();
		Vector3f pos = coreDefaultPosition + offset;
		// Vector3f pos = data.coreTargetPosition;
		pos.z() = coreDefaultPosition.z() + diff.z() * easingFunc(ease);

		Affine3f world{ player->GetCameraArmTrnasform()->GetWorldMatrix() };
		world.translate(pos);

		localOut = pos;
		worldOut = world.translation();
	}

	bool PlayerCamera::ResolveCameraCollision(Eigen::Vector3f& localIn, Eigen::Vector3f& worldIn)
	{
		using namespace Eigen;

		static float prevDistance = FLT_MAX;
		static bool prevHit = false;

		float radius = cameraCollisionRadius;
		const Vector3f coreLocalPos = localIn;
		const Vector3f coreWorldPos = worldIn;

		Affine3f world(player->GetCameraArmTrnasform()->GetWorldMatrix());
		const Vector3f worldStart = world.translation();

		Vector3f worldDir = coreWorldPos - worldStart;
		const float dis = worldDir.norm();
		worldDir.normalize();

		if (worldDir.isZero())
		{
			worldDir = player->GetCameraCoreTrnasform()->GetWorldRotation() * -Vector3f::UnitZ();
		}

		unsigned int layers = cameraCollisionLayers;
		ZonaiPhysics::ZnQueryInfo info;

		bool hit = PurahEngine::Physics::Spherecast(
			radius,
			worldStart, Quaternionf::Identity(),
			worldDir, dis,
			layers,
			info
		);

		float targetDistance = dis;

		if (hit)
			targetDistance = info.distance;

		// 보간을 안하는 경우
		if (hit && (targetDistance <= prevDistance))
		{
			// 충돌 했지만 이전 보다 가까운 경우
			prevDistance = targetDistance;

			Vector3f newPosition = worldStart + worldDir * prevDistance;
			Affine3f worldT{ player->GetCameraArmTrnasform()->GetWorldMatrix() };
			Vector3f localPos = worldT.inverse() * newPosition;

			// 보간을 안하는 경우 타겟포지션과 카메라 포지션을 같게함
			player->GetCameraCoreTrnasform()->SetLocalPosition(localPos);
			coreTargetPosition = localPos;

			localIn = localPos;
			worldIn = newPosition;
		}
		// 보간을 하는 경우
		else
		{
			if (hit && (targetDistance > prevDistance))
			{
				// 충돌 했지만 이전보다 먼 경우
				prevDistance = std::min(prevDistance + smoothingSpeed, targetDistance);
			}
			else if (!hit)
			{
				// 충돌 하지 않은 경우
				prevDistance = std::min(prevDistance + smoothingSpeed, dis);
			}

			Vector3f newPosition = worldStart + worldDir * prevDistance;
			Affine3f worldT{ player->GetCameraArmTrnasform()->GetWorldMatrix() };
			Vector3f localPos = worldT.inverse() * newPosition;

			coreTargetPosition = localPos;

			localIn = localPos;
			worldIn = newPosition;
		}

		prevHit = hit;

		return hit;
	}

	void PlayerCamera::UpdateCameraPositionLerp()
	{
		using namespace Eigen;

		float dt = PurahEngine::TimeController::GetInstance().GetDeltaTime();
		Vector3f currPos = player->GetCameraCoreTrnasform()->GetLocalPosition();
		Vector3f target = coreTargetPosition;
		Vector3f diff = target - currPos;
		float dis = diff.norm();

		Vector3f pos = Vector3f::Zero();
		if (dis > 1e-6)
		{
			float inv = dt / cameraLerpTime;
			// float inv = 0.001f / data.cameraLerpTime;
			inv = std::clamp(inv, 0.f, 1.f);
			pos = Lerp(currPos, target, inv);
		}
		else
		{
			pos = target;
		}

		player->GetCameraCoreTrnasform()->SetLocalPosition(pos);
	}

	void PlayerCamera::UpdateCameraRotationLerp()
	{
		using namespace Eigen;

		float dt = PurahEngine::TimeController::GetInstance().GetDeltaTime();
		Quaternionf currRot =player->GetCameraCoreTrnasform()->GetLocalRotation();
		Quaternionf currRotM = Quaternionf(-currRot.w(), -currRot.x(), -currRot.y(), -currRot.z());
		Quaternionf curr = currRot;
		Quaternionf target = coreTargetRotation;

		float costheta0 = currRot.dot(target);
		float costheta1 = currRotM.dot(target);

		if (costheta0 < costheta1)
		{
			curr = currRotM;
		}

		float inv = dt / cameraLerpTime;
		// float inv = 0.001f / data.cameraLerpTime;
		inv = std::clamp(inv, 0.f, 1.f);
		Quaternionf newRot = curr.slerp(inv, target);

		player->GetCameraCoreTrnasform()->SetLocalRotation(newRot);
	}

	void PlayerCamera::CharacterDisable()
	{
		using namespace Eigen;

		PurahEngine::Camera* mainCamera = PurahEngine::SceneManager::GetInstance().GetMainCamera();
		Vector3f cameraPos = mainCamera->GetGameObject()->GetTransform()->GetWorldPosition();
		Vector3f cameraArmPos = player->GetCameraArmTrnasform()->GetWorldPosition();
		Vector3f vec = cameraArmPos - cameraPos;
		float distance = vec.norm();
		PurahEngine::GameObject* obj = player->GetModelTransform()->GetGameObject();

		if (distance < 3.f)
		{
			obj->SetEnable(false);
		}
		else
		{
			obj->SetEnable(true);
		}
	}

	void PlayerCamera::SetCameraCoreLocalTargetPosition(const Eigen::Vector3f& _localPosision)
	{
		coreTargetPosition = _localPosision;
	}

	void PlayerCamera::SetCameraCoreLocalTargetRotation(const Eigen::Quaternionf& _localRotation)
	{
		coreTargetRotation = _localRotation;
	}

	void PlayerCamera::SetCameraCoreWorldTargetPosition(const Eigen::Vector3f& _worldPosision)
	{
		using namespace Eigen;

		Affine3f world{ player->GetCameraArmTrnasform()->GetWorldMatrix() };
		Vector3f localPos = world.inverse() * _worldPosision;

		coreTargetPosition = localPos;
	}

	void PlayerCamera::SetCameraCoreWorldTargetRotation(const Eigen::Quaternionf& _worldRotation)
	{
		using namespace Eigen;

		Quaternion world{ player->GetCameraArmTrnasform()->GetWorldRotation() };
		Quaternionf localRot = world.inverse() * _worldRotation;

		coreTargetRotation = localRot;
	}

	Eigen::Vector3f PlayerCamera::CalculateCameraFocusPosition(const Eigen::Vector3f& cameraPos, const Eigen::Vector3f direction)
	{
		using namespace Eigen;

		Vector3f xy;
		Vector3f xz;
		bool hasXY = IntersectXYPlane(cameraPos, direction, xy);
		bool hasXZ = IntersectXZPlane(cameraPos, direction, xz);

		if (hasXY && hasXZ)
		{
			if (xy.z() < xz.z())
			{
				return xy;
			}
			else
			{
				return xz;
			}
		}
		else if (hasXY)
		{
			return xy;
		}
		else
		{
			return xz;
		}
	}

	bool PlayerCamera::IntersectXYPlane(const Eigen::Vector3f& cameraPos, const Eigen::Vector3f direction, Eigen::Vector3f& out)
	{
		using namespace Eigen;

		if (std::fabs(direction.z()) < 1e-6)
		{
			return false;
		}

		float t = -cameraPos.z() / direction.z();
		out = cameraPos + t * direction;

		return true;
	}

	bool PlayerCamera::IntersectXZPlane(const Eigen::Vector3f& cameraPos, const Eigen::Vector3f direction, Eigen::Vector3f& out)
	{
		using namespace Eigen;

		if (std::fabs(direction.y()) < 1e-6)
		{
			return false;
		}

		float t = -cameraPos.y() / direction.y();
		out = cameraPos + t * direction;

		return true;
	}

	void PlayerCamera::ResetCameraArmAndCameraCore()
	{
		using namespace Eigen;

		Vector3f corePos = player->GetCameraCoreTrnasform()->GetWorldPosition();
		Quaternionf coreRot = player->GetCameraCoreTrnasform()->GetWorldRotation();

		xAngle = 0.f;
		player->GetCameraArmTrnasform()->SetLocalPosition(armDefaultPosition);
		player->GetCameraArmTrnasform()->SetLocalRotation(player->GetModelTransform()->GetLocalRotation());
		
		player->GetCameraCoreTrnasform()->SetWorldRotation(coreRot);
		player->GetCameraCoreTrnasform()->SetWorldPosition(corePos);
	}

	void PlayerCamera::ResetCameraCoreTarget()
	{
		SetCameraCoreLocalTargetPosition(coreDefaultPosition);
		SetCameraCoreLocalTargetRotation(coreDefaultRotation);
	}

	void PlayerCamera::ResetCamera()
	{
		ResetCameraArm();
		ResetCameraCore();
	}

	void PlayerCamera::ResetCameraArm()
	{
		xAngle = 0.f;
		player->GetCameraArmTrnasform()->SetLocalPosition(armDefaultPosition);
		player->GetCameraArmTrnasform()->SetLocalRotation(player->GetModelTransform()->GetLocalRotation());
	}

	void PlayerCamera::ResetCameraCore()
	{
		player->GetCameraCoreTrnasform()->SetLocalPosition(coreDefaultPosition);
		player->GetCameraCoreTrnasform()->SetLocalRotation(coreDefaultRotation);
	}

	void PlayerCamera::SetCameraArmFoward(const Eigen::Vector3f& _direction)
	{
		using namespace Eigen;

		Eigen::Vector3f up{ Vector3f::UnitY() };
		Eigen::Vector3f right = up.cross(_direction).normalized();

		Eigen::Vector3f newUp = _direction.cross(right).normalized();

		Eigen::Matrix3f rotation;
		rotation.col(0) = right;
		rotation.col(1) = newUp;
		rotation.col(2) = _direction;

		player->GetCameraArmTrnasform()->SetWorldRotation(Eigen::Quaternionf(rotation));

		float cosTheta = _direction.dot(up);
		cosTheta = std::clamp(cosTheta, -1.f, 1.f);
		float theta = std::acosf(cosTheta);
		xAngle = theta * (180.f / std::numbers::pi_v<float>);
	}

	void PlayerCamera::RotateCameraArm()
	{
		PurahEngine::TimeController& time = PurahEngine::TimeController::GetInstance();

		const float deltaTime = time.GetDeltaTime();
		const float angle = sensitivity;

		Phyzzle::PlayerInput inputData = player->GetController()->GetPlayerInputData();

		const float xFactor = inputData.Rstick.X * inputData.Rstick.Size;
		const float yawAngle = xFactor * angle * deltaTime;
		RotateCameraArmYaw(yawAngle);

		const float yFactor = -inputData.Rstick.Y * inputData.Rstick.Size;
		const float pitchAngle = yFactor * angle * deltaTime;
		RotateCameraArmPitch(pitchAngle);
	}

	void PlayerCamera::RotateCameraArmYaw(float yawAngle)
	{
		player->GetCameraArmTrnasform()->Rotate(Eigen::Vector3f(0.f, 1.f, 0.f), yawAngle);
	}

	void PlayerCamera::RotateCameraArmPitch(float pitchAngle)
	{
		float deltaAngle = pitchAngle;
		xAngle += pitchAngle;

		if (xAngle > limitHighAngle)
		{
			deltaAngle -= (xAngle - limitHighAngle);
			xAngle = limitHighAngle;
		}
		else if (xAngle < limitLowAngle)
		{
			deltaAngle -= (xAngle - limitLowAngle);
			xAngle = limitLowAngle;
		}

		const Eigen::Vector3f cameraRight = player->GetCameraArmTrnasform()->GetWorldRotation() * Eigen::Vector3f::UnitX();
		player->GetCameraArmTrnasform()->Rotate(cameraRight, deltaAngle);
	}

	void PlayerCamera::CameraLookTo(const Eigen::Vector3f& _direction)
	{
		const Eigen::Quaternionf targetRotation = Eigen::Quaternionf::FromTwoVectors(Eigen::Vector3f::UnitZ(), _direction);

		player->GetCameraArmTrnasform()->SetWorldRotation(targetRotation);
	}

	void PlayerCamera::CameraLookAt(const Eigen::Vector3f& _position)
	{
		using namespace Eigen;

		Vector3f pos = player->GetCameraCoreTrnasform()->GetWorldPosition();
		Vector3f direction = _position - pos;
		direction.normalize();

		Eigen::Vector3f up(0.0f, 1.0f, 0.0f);
		Eigen::Vector3f right = up.cross(direction).normalized();

		Eigen::Vector3f newUp = direction.cross(right).normalized();

		Eigen::Matrix3f rotation;
		rotation.col(0) = right;
		rotation.col(1) = newUp;
		rotation.col(2) = direction;

		player->GetCameraCoreTrnasform()->SetWorldRotation(Eigen::Quaternionf(rotation));
	}

	Eigen::Vector3f PlayerCamera::Lerp(const Eigen::Vector3f start, const Eigen::Vector3f end, float _t)
	{
		if (start == end)
			return start;

		_t = std::clamp(_t, 0.f, 1.f);

		return start + _t * (end - start);
	}
}
