#include "Transform.h"
#include "RigidBody.h"
#include "GameObject.h"

#define M_PI       3.14159265358979323846

PurahEngine::Transform::Transform() :
	position(Eigen::Vector3f::Zero()),
	rotation(Eigen::Quaternionf::Identity()),
	scale(1, 1, 1),
	parentTransform(nullptr),
	rigidbody(nullptr)
{

}

PurahEngine::Transform::~Transform()
{

}

void PurahEngine::Transform::Rotate(Eigen::Vector3f axis, float angle)
{
	// axis�� UnitX(), UnitY(), UnitZ()�� �Ұ�
	rotation = Eigen::AngleAxisf(angle * (M_PI / 180.f), axis) * rotation;

	if (rigidbody != nullptr)
	{
		rigidbody->SetRotation(rotation);
	}
}

Eigen::Vector3f PurahEngine::Transform::GetLocalPosition() const
{
	return position;
}

Eigen::Quaternionf PurahEngine::Transform::GetLocalRotation() const
{
	return rotation;
}

Eigen::Vector3f PurahEngine::Transform::GetLocalScale() const
{
	return scale;
}

Eigen::Vector3f PurahEngine::Transform::GetWorldPosition() const
{
	Eigen::Vector3f worldPosition = GetWorldMatrix().block<3, 1>(0, 3);
	return worldPosition;
}

Eigen::Quaternionf PurahEngine::Transform::GetWorldRotation() const
{
	if (parentTransform != nullptr)
	{
		Eigen::Quaternionf parentRotation = parentTransform->GetWorldRotation();
		return parentRotation * rotation;
	}
	else
	{
		return rotation;
	}
}

Eigen::Vector3f PurahEngine::Transform::GetWorldScale() const
{
	if (parentTransform != nullptr)
	{
		Eigen::Vector3f parentScale = parentTransform->GetWorldScale();

		return parentScale.cwiseProduct(scale);
	}
	else
	{
		return scale;
	}
}

Eigen::Matrix4f PurahEngine::Transform::GetLocalMatrix() const
{
	// Translation	
	Eigen::Affine3f localTransform = Eigen::Affine3f::Identity();
	localTransform.translation() = position;
	// rotation
	localTransform.linear() = rotation.toRotationMatrix();
	// scale
	localTransform.scale(scale);

	return localTransform.matrix();
}

//Eigen::Vector3f PurahEngine::Transform::GetFront() const
//{
//	Eigen::Vector3f frontdir = front.normalized();
//}

Eigen::Matrix4f PurahEngine::Transform::GetWorldMatrix() const
{
	if (parentTransform != nullptr)
	{
		Eigen::Affine3f worldTransform = Eigen::Affine3f::Identity();
		worldTransform.translation() = position;
		worldTransform.linear() = rotation.toRotationMatrix();
		worldTransform.scale(scale);

		return parentTransform->GetWorldMatrix() * GetLocalMatrix();
	}
	else
	{
		return GetLocalMatrix();
	}
}

PurahEngine::Transform* PurahEngine::Transform::GetParent()
{
	return parentTransform;
}

std::vector<PurahEngine::Transform*> PurahEngine::Transform::GetChildren() const
{
	return children;
}

void PurahEngine::Transform::SetLocalPosition(Eigen::Vector3f setPosition)
{
	position = setPosition;

	if (rigidbody != nullptr)
	{
		rigidbody->SetPosition(position);
	}
}

void PurahEngine::Transform::SetLocalRotation(Eigen::Quaternionf setRotation)
{
	rotation = setRotation;
}

void PurahEngine::Transform::SetLocalScale(Eigen::Vector3f setScale)
{
	scale = setScale;
}

void PurahEngine::Transform::SetWorldPosition(Eigen::Vector3f setPosition)
{
	/// �̷� : WorldMatrix ���� Position �κи� ��ü�Ѵ�.
	// Eigen::Matrix4f worldMatrix = GetWorldMatrix();
	// worldMatrix.block<3, 1>(0, 3) = setPosition;


	// matrix inverse �� �ٲ��
	if (parentTransform != nullptr)
	{
		Eigen::Matrix4f parentWorldMatrix = parentTransform->GetWorldMatrix();
		Eigen::Matrix4f parentInverse = parentWorldMatrix.inverse();
		Eigen::Vector4f localPosition = parentInverse * Eigen::Vector4f(setPosition.x(), setPosition.y(), setPosition.z(), 1.0f);
		localPosition.w() = 1.0f; // Ensure it's a position, not a direction
		SetLocalPosition(localPosition.head<3>());
	}
	else
	{
		position = setPosition;
	}

	if (rigidbody != nullptr)
	{
		rigidbody->SetPosition(GetWorldPosition());
	}
}

void PurahEngine::Transform::SetWorldRotation(Eigen::Quaternionf setRotation)
{

	if (parentTransform != nullptr)
	{
		rotation = parentTransform->GetWorldRotation().inverse() * setRotation;
	}
	else
	{
		rotation = setRotation;
	}

	if (rigidbody != nullptr)
	{
		rigidbody->SetRotation(GetWorldRotation());
	}
}

void PurahEngine::Transform::SetParent(PurahEngine::Transform* parentObject)
{
	if (parentTransform == nullptr)
	{
		parentTransform = parentObject;
		parentObject->children.push_back(this);
	}
	else
	{
		// erase, remove ���� ���� ���� �� ��� ������ �ִ�.
		// erase �� ���� �����ϸ� size�� �پ���. ������ return ���� �ǹ̰� ����.
		// remove �� ���� �����ص� size�� �پ���� �ʴ´�. return ���� ���� ���� �� ������ ���� �ٷ� �ڸ� ����Ų��.
		// ��� erase(remove())�� �ϰ� �Ǹ� remove�� return���� ������ �ϰ�, erase�� �ϰԵǸ� size���� �پ��� �� �� �ִ�.
		parentTransform->children.erase(remove(parentTransform->children.begin(), parentTransform->children.end(), this), parentTransform->children.end());

		// ���� �������� parentTransform�� nullptr�� ��������.
		parentTransform = parentObject;
		parentObject->children.push_back(this);
	}
}

void PurahEngine::Transform::SetWorldMatrix(Eigen::Matrix4f targetMatrix)
{
	Eigen::Affine3f transformation = Eigen::Affine3f::Identity();
	transformation.translation() << 1.0f, 2.0f, 3.0f; // �̵�
	transformation.rotate(Eigen::AngleAxisf(45.0f * M_PI / 180.0f, Eigen::Vector3f::UnitY())); // ȸ��
	transformation.scale(2.0f); // ũ�� ����

	// ���� ��ȯ ��� decompose
	Eigen::Vector3f translation = transformation.translation();
	Eigen::Quaternionf rotation_quaternion(transformation.linear()); // ȸ�� ��Ʈ������ Quaternionf�� ��ȯ
	Eigen::Vector3f scaling;
	scaling[0] = transformation.linear().col(0).norm(); // x ���� ũ��
	scaling[1] = transformation.linear().col(1).norm(); // y ���� ũ��
	scaling[2] = transformation.linear().col(2).norm(); // z ���� ũ��
}

void PurahEngine::Transform::PreSerialize(json& jsonData) const
{

}

void PurahEngine::Transform::PreDeserialize(const json& jsonData)
{
	PREDESERIALIZE_BASE();
	PREDESERIALIZE_VECTOR3F(position);
	PREDESERIALIZE_QUATERNIONF(rotation);
	PREDESERIALIZE_VECTOR3F(scale);
}

void PurahEngine::Transform::PostSerialize(json& jsonData) const
{

}

void PurahEngine::Transform::PostDeserialize(const json& jsonData)
{
	auto& fManager = FileManager::GetInstance();

	//for (int i = 0; i < jsonData["__ID__children"].size(); i++)
	//{
	//	children.push_back(static_cast<Transform*>(fManager.GetAddress(jsonData["__ID__children"][i])));
	//}
	POSTDESERIALIZE_VECTOR_PTR(children);

	if (jsonData["__ID__parentTransform"].size() != 0)
	{
		parentTransform = static_cast<Transform*>(fManager.GetAddress(jsonData["__ID__parentTransform"][0]));
	}
}

void PurahEngine::Transform::SetRigidBody(RigidBody* rigid)
{
	assert(rigidbody == nullptr || (rigid == nullptr && rigidbody != nullptr));

	rigidbody = rigid;
}
