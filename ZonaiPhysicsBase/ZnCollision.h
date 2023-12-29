#pragma once
#include <cstdint>
#include "../eigen-3.4.0/Eigen/Dense"


namespace ZonaiPhysics
 {
 	struct ZnCollision
 	{
        Eigen::Vector3f position;
        float separation;
        Eigen::Vector3f normal;
        uint32_t faceIndex0;
	    Eigen::Vector3f impulse;
        uint32_t faceIndex1;
 	};
 }
