#include "CameraController.h"

namespace Phyzzle
{

	CameraController::~CameraController()
	{

	}

	void CameraController::Start()
	{

	}

	void CameraController::Update()
	{
		if (newTransform)
		{

		}
	}

	void CameraController::PreSerialize(json& jsonData) const
	{
	}

	void CameraController::PreDeserialize(const json& jsonData)
	{
	}

	void CameraController::PostSerialize(json& jsonData) const
	{
	}

	void CameraController::PostDeserialize(const json& jsonData)
	{
	}

	Eigen::Vector3f CameraController::GetArmWorldDirection()
	{
		return cameraArm->GetWorldRotation() * Eigen::Vector3f::UnitZ();
	}

	Eigen::Vector3f CameraController::GetCoreWorldDirection()
	{
		return cameraCore->GetWorldRotation() * Eigen::Vector3f::UnitZ();
	}

	void CameraController::ArmReset()
	{

	}

	void CameraController::CoreReset()
	{
	}

	void CameraController::ArmRotateY(float _angle)
	{
		cameraArm->Rotate(Eigen::Vector3f::UnitY(), _angle);
	}

	void CameraController::ArmRotateX(float _angle)
	{
		float deltaAngle = _angle;
		camera_xAngle += _angle;

		if (armRotateLimit)
		{
			if (camera_xAngle > pitchMaxAngle)
			{
				deltaAngle -= (camera_xAngle - pitchMaxAngle);
				camera_xAngle = pitchMaxAngle;
			}
			else if (camera_xAngle < pitchMinAngle)
			{
				deltaAngle -= (camera_xAngle - pitchMinAngle);
				camera_xAngle = pitchMinAngle;
			}
		}

		Eigen::Vector3f cameraRight = cameraArm->GetWorldRotation() * Eigen::Vector3f::UnitX();
		cameraRight.normalize();

		cameraArm->Rotate(cameraRight, _angle);
	}

	void CameraController::CoreRotateY(float _angle)
	{
		cameraCore->Rotate(Eigen::Vector3f::UnitY(), _angle);
	}

	void CameraController::CoreRotateX(float _angle)
	{
		const Eigen::Vector3f cameraRight = cameraCore->GetWorldRotation() * Eigen::Vector3f::UnitX();

		cameraCore->Rotate(cameraRight, _angle);
	}

	float CameraController::GetDistance()
	{
		Eigen::Vector3f armPosition = cameraArm->GetWorldPosition();
		Eigen::Vector3f corePosition = cameraCore->GetWorldPosition();
		Eigen::Vector3f diff = armPosition - corePosition;
		
		return diff.norm();
	}

	void CameraController::ArmRotate(const Eigen::Vector3f& _axis, float _angle)
	{
		assert(!_axis.isZero());


	}

	void CameraController::CoreRotate(const Eigen::Vector3f& _axis, float _angle)
	{
		assert(!_axis.isZero());

	}

	void CameraController::LookToWorld(const Eigen::Vector3f& _worldDirection)
	{
		assert(!_worldDirection.isZero());


	}

	void CameraController::LookAtWorld(const Eigen::Vector3f& _worldPosition)
	{
		assert(!_worldPosition.isZero());


	}

	void CameraController::LookToLocal(const Eigen::Vector3f& _localDirection)
	{
		assert(!_localDirection.isZero());


	}

	void CameraController::LookAtLocal(const Eigen::Vector3f& _localPosition)
	{
		assert(!_localPosition.isZero());


	}
}
