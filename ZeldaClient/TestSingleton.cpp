#include "TestSingleton.h"

PurahEngine::TestSingleton& PurahEngine::TestSingleton::Getinstance()
{
	static TestSingleton instance;
	return instance;
}

PurahEngine::TestSingleton::TestSingleton()
{

}

PurahEngine::TestSingleton::~TestSingleton()
{

}
