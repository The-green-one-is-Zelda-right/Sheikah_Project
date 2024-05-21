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

        Eigen::Vector3f thisPostLinearVelocity{ Eigen::Vector3f::Zero() };
        Eigen::Vector3f otherPostLinearVelocity{ Eigen::Vector3f::Zero() };
        Eigen::Vector3f thisPostAngularVelocity{ Eigen::Vector3f::Zero() };
        Eigen::Vector3f otherPostAngularVelocity{ Eigen::Vector3f::Zero() };

        ZnContact* contacts{ nullptr };                // ������ �迭 ������
        uint32_t contactCount{ 0 };              // ������ ����

        ZnCollision Reverse()
        {
            ZnCollision collision;
            collision.impulses = impulses;
            collision.thisPostLinearVelocity = thisPostLinearVelocity;
            collision.otherPostLinearVelocity = otherPostLinearVelocity;
            collision.thisPostAngularVelocity = thisPostAngularVelocity;
            collision.otherPostAngularVelocity = otherPostAngularVelocity;
            collision.contactCount = contactCount;

            if (contactCount)
            {
				collision.contacts = new ZnContact[contactCount];

                for (int i = 0; i < contactCount; i++)
                {
                    collision.contacts[i].normal = contacts[i].normal * -1.f;
                    collision.contacts[i].point = contacts[i].point;
                    collision.contacts[i].impulse = contacts[i].impulse * -1.f;
                    collision.contacts[i].separation = contacts[i].separation;
                }
            }

            return collision;
        }
 	};
 }
