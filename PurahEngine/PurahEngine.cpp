#include "PurahEngine.h"

#include "GameLoop.h"

_In_ int nCmdShow;


void PurahEngine::Initialize(_In_ HINSTANCE hInstance, LPCWSTR gameName, unsigned int width, unsigned int height)
{
	ComponentFactory::GetInstance().RegisterComponent<Transform>("Transform");

	// Physics
	{
		ComponentFactory::GetInstance().RegisterComponent<RigidBody>("RigidBody");

		ComponentFactory::GetInstance().RegisterComponent<BoxCollider>("BoxCollider");
		ComponentFactory::GetInstance().RegisterComponent<SphereCollider>("SphereCollider");
		ComponentFactory::GetInstance().RegisterComponent<CapsuleCollider>("CapsuleCollider");

		ComponentFactory::GetInstance().RegisterComponent<FixedJoint>("FixedJoint");
		ComponentFactory::GetInstance().RegisterComponent<SlideJoint>("SlideJoint");
		ComponentFactory::GetInstance().RegisterComponent<HingeJoint>("HingeJoint");
		ComponentFactory::GetInstance().RegisterComponent<BallJoint>("BallJoint");
		ComponentFactory::GetInstance().RegisterComponent<DistanceJoint>("DistanceJoint");
		ComponentFactory::GetInstance().RegisterComponent<SpringJoint>("SpringJoint");

	}

	ComponentFactory::GetInstance().RegisterComponent<Camera>("Camera");
	ComponentFactory::GetInstance().RegisterComponent<DirectionalLight>("DirectionalLight");
	ComponentFactory::GetInstance().RegisterComponent<ModelRenderer>("ModelRenderer");
	ComponentFactory::GetInstance().RegisterComponent<MeshRenderer>("MeshRenderer");
	ComponentFactory::GetInstance().RegisterComponent<Animator>("Animator");
	ComponentFactory::GetInstance().RegisterComponent<AudioSource>("AudioSource");
	ComponentFactory::GetInstance().RegisterComponent<AudioListener>("AudioListener");

	PurahEngine::GameLoop::GetInstance().Initialize(hInstance, gameName, width, height);
}

void PurahEngine::Run(_In_ int nCmdShow)
{
	PurahEngine::GameLoop::GetInstance().Run(nCmdShow);
}

void PurahEngine::Finalize()
{

}
