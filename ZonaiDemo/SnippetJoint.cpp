// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//  * Neither the name of NVIDIA CORPORATION nor the names of its
//    contributors may be used to endorse or promote products derived
//    from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ''AS IS'' AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
// OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Copyright (c) 2008-2023 NVIDIA Corporation. All rights reserved.
// Copyright (c) 2004-2008 AGEIA Technologies, Inc. All rights reserved.
// Copyright (c) 2001-2004 NovodeX AG. All rights reserved.  

// ****************************************************************************
// This snippet illustrates simple use of joints in physx
//
// It creates a chain of objects joined by limited spherical joints, a chain
// joined by fixed joints which is breakable, and a chain of damped D6 joints
// ****************************************************************************

#include <ctype.h>
//#include "PxPhysicsAPI.h"
//#include "../snippets/snippetcommon/SnippetPrint.h"
//#include "../snippets/snippetcommon/SnippetPVD.h"
#include <windows.h>

#include "Eigen/Dense"
#include "ZnCollider.h"
#include "ZnPhysicsBase.h"
#include "ZnRigidBody.h"

//using namespace physx;
//namespace physx
//{
//	class PxJoint;
//}
//
//static PxDefaultAllocator		gAllocator;
//static PxDefaultErrorCallback	gErrorCallback;
//static PxFoundation*			gFoundation = NULL;
//static PxPhysics*				gPhysics = NULL;
//static PxDefaultCpuDispatcher*	gDispatcher = NULL;
//static PxScene*					gScene[3] = { NULL, };
//static PxMaterial*				gMaterial = NULL;
//static PxPvd*					gPvd = NULL;
//static PxRigidDynamic*			last = NULL;
//static PxJoint*					lastj = NULL;
//static PxRigidStatic*			staticRigid[3] = { NULL };
//
//static std::vector<physx::PxJoint*> jointList;

// static PxRigidDynamic* createDynamic(const PxTransform& t, const PxGeometry& geometry, const PxVec3& velocity = PxVec3(0))
// {
// 	PxRigidDynamic* dynamic = PxCreateDynamic(*gPhysics, t, geometry, *gMaterial, 10.0f);
// 	dynamic->setAngularDamping(0.5f);
// 	dynamic->setLinearVelocity(velocity);
// 	gScene->addActor(*dynamic);
// 	return dynamic;
// }

// 각도가 제한된 구형 관절
//static PxJoint* createLimitedSpherical(PxRigidActor* a0, const PxTransform& t0, PxRigidActor* a1, const PxTransform& t1)
//{
//	PxSphericalJoint* j = PxSphericalJointCreate(*gPhysics, a0, t0, a1, t1);
//	j->setLimitCone(PxJointLimitCone(PxPi / 4, PxPi / 4));
//	j->setBreakForce(1000000, 10000000);
//	j->setSphericalJointFlag(PxSphericalJointFlag::eLIMIT_ENABLED, true);
//	return j;
//}
//
//// 파괴 가능한 고정 관절
//static PxJoint* createBreakableFixed(PxRigidActor* a0, const PxTransform& t0, PxRigidActor* a1, const PxTransform& t1)
//{
//	PxFixedJoint* j = PxFixedJointCreate(*gPhysics, a0, t0, a1, t1);
//	j->setBreakForce(1000, 100000);
//	j->setConstraintFlag(PxConstraintFlag::eDRIVE_LIMITS_ARE_FORCES, true);
//	j->setConstraintFlag(PxConstraintFlag::eDISABLE_PREPROCESSING, true);
//	return j;
//}
//
//// 스프링으로 위치가 고정된 D6 관절
//static PxJoint* createDampedD6(PxRigidActor* a0, const PxTransform& t0, PxRigidActor* a1, const PxTransform& t1)
//{
//	PxD6Joint* j = PxD6JointCreate(*gPhysics, a0, t0, a1, t1);
//	j->setBreakForce(1000, 100000);
//	j->setMotion(PxD6Axis::eSWING1, PxD6Motion::eFREE);
//	j->setMotion(PxD6Axis::eSWING2, PxD6Motion::eFREE);
//	j->setMotion(PxD6Axis::eTWIST, PxD6Motion::eFREE);
//	j->setDrive(PxD6Drive::eSLERP, PxD6JointDrive(0, 1000, FLT_MAX, true));
//	return j;
//}
//
//typedef PxJoint* (*JointCreateFunction)(PxRigidActor* a0, const PxTransform& t0, PxRigidActor* a1, const PxTransform& t1);
//
//// create a chain rooted at the origin and extending along the x-axis, all transformed by the argument t.
//
//static void createChain(const PxTransform& t, PxU32 length, const PxGeometry& g, PxReal separation, JointCreateFunction createJoint)
//{
//	PxVec3 offset(separation / 2, 0, 0);
//	PxTransform localTm(offset);
//	PxRigidDynamic* prev = NULL;
//	static int index = 0;
//	
//	for (PxU32 i = 0; i < length; i++)
//	{
//		PxRigidDynamic* current = PxCreateDynamic(*gPhysics, t * localTm, g, *gMaterial, 1.0f);
//		auto j = (*createJoint)(prev, prev ? PxTransform(offset) : t, current, PxTransform(-offset));
//		jointList.push_back(j);
//		if (index == 2)
//		{
//			gScene[0]->addActor(*current);
//		}
//		gScene[index]->addActor(*current);
//		prev = current;
//		localTm.p.x += separation;
//
//		last = current;
//	}
//
//	index++;
//}

/*
void initPhysics(bool interactive)
{
	gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);
	gPvd = PxCreatePvd(*gFoundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
	gPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(), true, gPvd);
	PxInitExtensions(*gPhysics, gPvd);

	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
	gDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;
	gScene = gPhysics->createScene(sceneDesc);

	PxPvdSceneClient* pvdClient = gScene->getScenePvdClient();
	if (pvdClient)
	{
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}

	gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);

	PxRigidStatic* groundPlane = PxCreatePlane(*gPhysics, PxPlane(0, 1, 0, 0), *gMaterial);
	gScene->addActor(*groundPlane);

	// 각도가 제한된 관절
	createChain(PxTransform(PxVec3(0.0f, 20.0f, 0.0f)), 5, PxBoxGeometry(2.0f, 0.5f, 0.5f), 4.0f, createLimitedSpherical);
	
	// 부숴지는 관절
	createChain(PxTransform(PxVec3(0.0f, 20.0f, -10.0f)), 5, PxBoxGeometry(2.0f, 0.5f, 0.5f), 4.0f, createBreakableFixed);
	
	// 스프링으로 연결된 관절
	createChain(PxTransform(PxVec3(0.0f, 20.0f, -20.0f)), 5, PxBoxGeometry(2.0f, 0.5f, 0.5f), 4.0f, createDampedD6);
}
*/

/*
void stepPhysics()// interactive
{
	gScene->simulate(1.0f / 600.0f);
	gScene->fetchResults(true);
}
*/

//void cleanupPhysics(bool /*interactive*/)
//{
//	PX_RELEASE(gScene[0]);
//	PX_RELEASE(gScene[1]);
//	PX_RELEASE(gScene[2]);
//	PX_RELEASE(gDispatcher);
//	PxCloseExtensions();
//	PX_RELEASE(gPhysics);
//	if (gPvd)
//	{
//		PxPvdTransport* transport = gPvd->getTransport();
//		gPvd->release();	gPvd = NULL;
//		PX_RELEASE(transport);
//	}
//	PX_RELEASE(gFoundation);
//
//	printf("SnippetJoint done.\n");
//}
//
//bool keyPress(unsigned char key)
//{
//	return GetAsyncKeyState(key);
//}

#include "ZnJoint.h"
#include "ZnTransform.h"

int snippetMain(int, const char* const*)
{
	std::wstring path = L"../x64/Debug/ZonaiPhysicsX.dll";

	HMODULE physicsDLL = LoadLibraryW(path.c_str());
	if (!physicsDLL)
	{
		// MessageBox(_hwnd, L"해당 경로에 Physics DLL 파일이 존재하지 않습니다.", L"DLL 오류", MB_OK | MB_ICONWARNING);
		return false;
	}

	using ImportFunction = ZonaiPhysics::ZnPhysicsBase* (*) ();
	ImportFunction CreateInstance{ reinterpret_cast<ImportFunction>(GetProcAddress(physicsDLL, "CreatePhysics")) };

	if (CreateInstance == nullptr)
	{
		// MessageBox(_hwnd, L"Physics DLL에서 함수 포인터를 받아오지 못했습니다.", L"DLL 오류", MB_OK | MB_ICONWARNING);
		return false;
	}

	ZonaiPhysics::ZnPhysicsBase* physicsEngine = CreateInstance();

	if (physicsEngine == nullptr)
	{
		// MessageBox(_hwnd, L"Graphics Engine 객체 생성 실패", L"DLL 오류", NULL);
		return false;
	}

	physicsEngine->Initialize();


	auto collider = physicsEngine->CreateBoxCollider(L"rigidBody", 2, 2, 2);
	auto rigid = physicsEngine->CreateRigidBody(L"rigidBody");
	Eigen::Vector3f pos1{ 0, 50, -10 };
	rigid->SetPosition(pos1);

	auto collider2 = physicsEngine->CreateBoxCollider(L"rigidBody2", 2, 2, 2);
	auto rigid2 = physicsEngine->CreateRigidBody(L"rigidBody2");
	Eigen::Vector3f pos2{ 0, 55, -5 };
	rigid2->SetPosition({ 0, 55, -5 });

	auto fixedjoint = physicsEngine->CreateFixedJoint(rigid, ZonaiPhysics::ZnTransform{{},{}}, rigid2, ZonaiPhysics::ZnTransform{{},{}});
	fixedjoint->SetLocalPosition(ZonaiPhysics::ZnJoint::eOBJECT::eJOINT_OBJECT0, { 0, 0, 10 });

	auto groundCollider = physicsEngine->CreateBoxCollider(L"ground", 30, 1, 30);
	auto ground = physicsEngine->CreateRigidBody(L"ground");
	ground->SetPosition({ 0, 0, -10 });
	ground->SetKinematic(true);

	while (true)
	{
		physicsEngine->Simulation(1.0f / 1000.0f);
	}

	//static const PxU32 frameCount = 100;

	// initPhysics(false);

	//gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);
	//gPvd = PxCreatePvd(*gFoundation);
	//PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
	//gPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

	//gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(), true, gPvd);
	//PxInitExtensions(*gPhysics, gPvd);

	//PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	//sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
	//gDispatcher = PxDefaultCpuDispatcherCreate(2);
	//sceneDesc.cpuDispatcher = gDispatcher;
	//sceneDesc.filterShader = PxDefaultSimulationFilterShader;
	//gScene[0] = gPhysics->createScene(sceneDesc);
	//gScene[1] = gPhysics->createScene(sceneDesc);
	//gScene[2] = gPhysics->createScene(sceneDesc);
	//
	//for (auto e : gScene)
	//{
	//	PxPvdSceneClient* pvdClient = e->getScenePvdClient();
	//	if (pvdClient)
	//	{
	//		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
	//		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
	//		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	//	}
	//}

	//gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);

	//PxRigidStatic* groundPlane = PxCreatePlane(*gPhysics, PxPlane(0, 1, 0, 0), *gMaterial);
	//gScene[0]->addActor(*groundPlane);

	//// 각도가 제한된 관절
	//createChain(PxTransform(PxVec3(0.0f, 20.0f, 0.0f)), 5, PxBoxGeometry(2.0f, 0.5f, 0.5f), 4.0f, createLimitedSpherical);

	//// 부숴지는 관절
	//createChain(PxTransform(PxVec3(0.0f, 20.0f, -10.0f)), 5, PxBoxGeometry(2.0f, 0.5f, 0.5f), 4.0f, createBreakableFixed);

	//// 스프링으로 연결된 관절
	//createChain(PxTransform(PxVec3(0.0f, 20.0f, -20.0f)), 5, PxBoxGeometry(2.0f, 0.5f, 0.5f), 4.0f, createDampedD6);

	//for (int i = 0; i < 3; i++) 
	//{
	//	staticRigid[i] = PxCreateStatic(
	//		*gPhysics,
	//		PxTransform(PxVec3(0.f, 150.f, -10.f * i)),
	//		PxBoxGeometry(2.f, 2.f, 2.f),
	//		*gMaterial
	//	);

	//	PxShapeFlags shapeFlags = PxShapeFlag::eSCENE_QUERY_SHAPE;

	//	PxActorFlags flags = 
	//		// PxActorFlag::eVISUALIZATION | 
	//		// PxActorFlag::eDISABLE_GRAVITY | 
	//		// PxActorFlag::eSEND_SLEEP_NOTIFIES | 
	//		PxActorFlag::eDISABLE_SIMULATION;

	//	staticRigid[i]->setActorFlags(flags);

	//	gScene[i]->addActor(*(staticRigid[i]));
	//}

	//auto shape = gPhysics->createShape(PxSphereGeometry(4.f), *gMaterial);

	//while (1)
	//{
	//	// gScene->simulate(1.0f / 600.0f);
	//	for (auto e : gScene)
	//	{
	//		e->simulate(1.f / 500.f);
	//		e->fetchResults(true);
	//	}

	//	if (GetAsyncKeyState(0x4A))  // J
	//	{
 //			last->attachShape(*shape);
	//	}

	//	if (GetAsyncKeyState(0x49))  // J
	//	{
	//		auto t = shape->getLocalPose();
	//		t.p.x += 10.f;
	//		t.p.y += 10.f;
	//		t.p.z += 10.f;
	//		shape->setLocalPose(t);
	//	}

	//	if (GetAsyncKeyState(0x4B))  // K
	//	{
	//		for (auto e : staticRigid)
	//		{
	//			PxTransform t = e->getGlobalPose();
	//			t.p.y -= 1.f/100.f;
	//			e->setGlobalPose(t);
	//		}
	//	}
	//	
	//	static bool first = true;

	//	if (GetAsyncKeyState(0x4C) && first)  // L
	//	{
	//		first = false;
	//		for (auto e : jointList)
	//		{
	//			e->release();
	//		}
	//	}
	//}

	//cleanupPhysics(false);

	return 0;
}
