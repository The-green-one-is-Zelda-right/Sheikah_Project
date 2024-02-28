#include "ConstantBufferManager.h"

#include "ConstantBuffer.h"

void ConstantBufferManager::RegisterBuffer(IConstantBuffer* buffer)
{
	bufferList.push_back(buffer);
}

void ConstantBufferManager::DeRegisterBuffer(IConstantBuffer* buffer)
{
	bufferList.erase(remove(bufferList.begin(), bufferList.end(), buffer), bufferList.end());
}

void ConstantBufferManager::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	this->device = device;
	this->deviceContext = deviceContext;
}

void ConstantBufferManager::Finalize()
{
	this->device = nullptr;
	this->deviceContext = nullptr;
}

void ConstantBufferManager::SetBuffer()
{
	for (unsigned int i = 0; i < bufferList.size(); i++)
	{
		bufferList[i]->SetBuffer(deviceContext);
	}
}

ConstantBufferManager& ConstantBufferManager::GetInstance()
{
	static ConstantBufferManager instance;
	return instance;
}

ConstantBufferManager::ConstantBufferManager() :
	device(nullptr),
	deviceContext(nullptr)
{
}

ConstantBufferManager::~ConstantBufferManager()
{
}
