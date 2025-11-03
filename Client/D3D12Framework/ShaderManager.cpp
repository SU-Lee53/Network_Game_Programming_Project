#include "pch.h"
#include "ShaderManager.h"
#include "PlayerRenderer.h"

ShaderManager::ShaderManager(ComPtr<ID3D12Device> pDevice)
{
	m_pd3dDevice = pDevice;
}

ShaderManager::~ShaderManager()
{
}

void ShaderManager::Initialize()
{
	Load<DiffusedShader>();
	Load<FullScreenShader>();
	Load<TexturedShader>();
	Load<TexturedIlluminatedShader>();
	Load<TexturedNormalShader>();
	//Load<PlayerShader>();
}
