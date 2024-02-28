#pragma once

#include <vector>

#include "IConstantBuffer.h"
#include "ZeldaGraphicsDefine.h"

class ConstantBufferManager
{
public:
	void Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	void Finalize();

	void SetBuffer();

protected:
	void RegisterBuffer(IConstantBuffer* buffer);
	void DeRegisterBuffer(IConstantBuffer* buffer);

	template<typename bufferType, ShaderType shaderType> requires std::is_same_v<decltype(bufferType::registerNumB), const unsigned int>
	friend class ConstantBuffer;

private:
	std::vector<IConstantBuffer*> bufferList;
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;


	//singleton
public:
	static ConstantBufferManager& GetInstance();

private:
	ConstantBufferManager();
	~ConstantBufferManager();
	ConstantBufferManager(const ConstantBufferManager& ref) = delete;
	ConstantBufferManager& operator=(const ConstantBufferManager& ref) = delete;
};

