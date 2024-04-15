#pragma once
#include <cstdint>
#include <Eigen/Dense>


namespace ZonaiPhysics
 {
    struct ZnContact
    {
        Eigen::Vector3f point{ Eigen::Vector3f::Zero() };         // ������
        Eigen::Vector3f normal{ Eigen::Vector3f::Zero() };        // �������� �븻
        Eigen::Vector3f impulse{ Eigen::Vector3f::Zero() };       // ������������ ��ݷ�
        float separation{ 0.f };                                    // ħ�� ����
    };

	struct ZnCollision
 	{
        ZnCollision() = default;
        ~ZnCollision() = default;

        Eigen::Vector3f impulses{ Eigen::Vector3f::Zero() };           // ��ݷ� �ѷ�

        // Eigen::Vector3f thisPreLinearVelocity{ Eigen::Vector3f::Zero() };
        // Eigen::Vector3f otherPreLinearVelocity{ Eigen::Vector3f::Zero() };
        // Eigen::Vector3f thisPreAngularVelocity{ Eigen::Vector3f::Zero() };
        // Eigen::Vector3f otherPreAngularVelocity{ Eigen::Vector3f::Zero() };

        Eigen::Vector3f thisPostLinearVelocity{ Eigen::Vector3f::Zero() };
        Eigen::Vector3f otherPostLinearVelocity{ Eigen::Vector3f::Zero() };
        Eigen::Vector3f thisPostAngularVelocity{ Eigen::Vector3f::Zero() };
        Eigen::Vector3f otherPostAngularVelocity{ Eigen::Vector3f::Zero() };

        ZnContact* contacts{ nullptr };                // ������ �迭 ������
        uint32_t contactCount{ 0 };              // ������ ����
 	};
 }
