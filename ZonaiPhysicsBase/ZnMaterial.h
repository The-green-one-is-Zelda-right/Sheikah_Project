#pragma once

 namespace ZonaiPhysics
 {
 	struct ZnMaterial
 	{
 		float staticFriction;		// ���� ����
 		float dynamicFriction;		// ���� ����
 		float restitution;			// �ݹ�
        float linearDamping;		// ����
        float angularDamping;		// ����
 	};
 }