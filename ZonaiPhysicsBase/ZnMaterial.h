#pragma once

 namespace ZonaiPhysics
 {
 	struct ZnMaterial
 	{
 		float staticFriction;		// Á¤Àû ¸¶Âû
 		float dynamicFriction;		// µ¿Àû ¸¶Âû
 		float restitution;			// ¹Ý¹ß
        float linearDamping;		// °¨¼è
        float angularDamping;		// °¨¼è
 	};
 }