#include "pch.h"
#include "PlayerRenderer.h"
#include "SpaceshipPlayer.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PlayerRenderer

PlayerRenderer::PlayerRenderer(std::shared_ptr<Mesh> pMesh, std::vector<std::shared_ptr<Material>> pMaterials)
{
}

PlayerRenderer::PlayerRenderer(const MESHLOADINFO& meshLoadInfo, const std::vector<MATERIALLOADINFO>& materialLoadInfo)
{
	m_eObjectRenderType = OBJECT_RENDER_FORWARD;
	m_pMesh = std::make_shared<TexturedNormalTangentMesh>(meshLoadInfo);

	for (const auto& materialInfo : materialLoadInfo) {
		std::shared_ptr<TexturedNormalMaterial> pMaterial = std::make_shared<TexturedNormalMaterial>(materialInfo);
		m_pMaterials.push_back(pMaterial);
	}

	CreatePipelineStates();
}

void PlayerRenderer::CreatePipelineStates()
{
	m_pd3dEffectPipelineStates.resize(2);

	ComPtr<ID3DBlob> m_pd3dVertexShaderBlob = nullptr;
	ComPtr<ID3DBlob> m_pd3dGeometryShaderBlob = nullptr;
	ComPtr<ID3DBlob> m_pd3dPixelShaderBlob = nullptr;

	// Pipeline #1 : Ray
	D3D12_GRAPHICS_PIPELINE_STATE_DESC d3dPipelineDesc{};
	{
		d3dPipelineDesc.pRootSignature = RenderManager::g_pd3dGlobalRootSignature.Get();
		d3dPipelineDesc.VS = Shader::CompileShader(L"./HLSL/RayShader.hlsl", "VSRay", "vs_5_1", m_pd3dVertexShaderBlob.GetAddressOf());
		d3dPipelineDesc.GS = Shader::CompileShader(L"./HLSL/RayShader.hlsl", "GSRay", "gs_5_1", m_pd3dGeometryShaderBlob.GetAddressOf());
		d3dPipelineDesc.PS = Shader::CompileShader(L"./HLSL/RayShader.hlsl", "PSRay", "ps_5_1", m_pd3dPixelShaderBlob.GetAddressOf());
		d3dPipelineDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		d3dPipelineDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		d3dPipelineDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
		d3dPipelineDesc.DepthStencilState.DepthEnable = true;
		d3dPipelineDesc.DepthStencilState.StencilEnable = false;
		d3dPipelineDesc.InputLayout.NumElements = 0;
		d3dPipelineDesc.InputLayout.pInputElementDescs = nullptr;
		d3dPipelineDesc.SampleMask = UINT_MAX;
		d3dPipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
		d3dPipelineDesc.NumRenderTargets = 1;
		d3dPipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		d3dPipelineDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
		d3dPipelineDesc.SampleDesc.Count = 1;
		d3dPipelineDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	}

	HRESULT hr = D3DCORE->GetDevice()->CreateGraphicsPipelineState(&d3dPipelineDesc, IID_PPV_ARGS(m_pd3dEffectPipelineStates[0].GetAddressOf()));
	if (FAILED(hr)) {
		__debugbreak();
	}

	// Pipeline #2 : Billboard
	{
		d3dPipelineDesc.VS = Shader::CompileShader(L"./HLSL/BillboardShader.hlsl", "VSBillboard", "vs_5_1", m_pd3dVertexShaderBlob.GetAddressOf());
		d3dPipelineDesc.GS = Shader::CompileShader(L"./HLSL/BillboardShader.hlsl", "GSBillboard", "gs_5_1", m_pd3dGeometryShaderBlob.GetAddressOf());
		d3dPipelineDesc.PS = Shader::CompileShader(L"./HLSL/BillboardShader.hlsl", "PSBillboard", "ps_5_1", m_pd3dPixelShaderBlob.GetAddressOf());
		d3dPipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
	}

	hr = D3DCORE->GetDevice()->CreateGraphicsPipelineState(&d3dPipelineDesc, IID_PPV_ARGS(m_pd3dEffectPipelineStates[1].GetAddressOf()));
	if (FAILED(hr)) {
		__debugbreak();
	}
}

void PlayerRenderer::SetPlayer(std::shared_ptr<SpaceshipPlayer> pPlayerRef)
{
	m_wpPlayerRef = pPlayerRef;
}

void PlayerRenderer::Render(ComPtr<ID3D12Device> pd3dDevice, ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, DescriptorHandle& descHandle, int nInstanceCount, OUT int& refnInstanceBase)
{
	// Draw Mesh
	for (int i = 0; i < m_pMaterials.size(); ++i) {
		// Per Object CB
		CB_PER_OBJECT_DATA cbData = { m_pMaterials[i]->GetMaterialColors(), refnInstanceBase };
		ConstantBuffer cbuffer = RESOURCE->AllocCBuffer<CB_PER_OBJECT_DATA>();
		cbuffer.WriteData(&cbData);

		pd3dDevice->CopyDescriptorsSimple(1, descHandle.cpuHandle, cbuffer.CBVHandle, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		descHandle.cpuHandle.Offset(1, D3DCore::g_nCBVSRVDescriptorIncrementSize);
		pd3dCommandList->SetGraphicsRootDescriptorTable(ROOT_PARAMETER_OBJ_MATERIAL_DATA, descHandle.gpuHandle);
		descHandle.gpuHandle.Offset(1, D3DCore::g_nCBVSRVDescriptorIncrementSize);

		// Texture (있다면)
		m_pMaterials[i]->UpdateShaderVariables(pd3dDevice, pd3dCommandList, descHandle);	// Texture 가 있다면 Descriptor 가 복사될 것이고 아니면 안될것

		const auto& pipelineStates = m_pMaterials[i]->GetShader()->GetPipelineStates();
		pd3dCommandList->SetPipelineState(pipelineStates[0].Get());

		m_pMesh->Render(pd3dCommandList, i, nInstanceCount);
	}

	refnInstanceBase += nInstanceCount;

	if (m_wpPlayerRef.lock()->IsFiring()) {
		// Data set
		auto pOwnerPlayer = m_wpPlayerRef.lock();

		CB_PLAYER_DATA playerData;
		{
			playerData.v3PositionW = pOwnerPlayer->GetTransform().GetPosition();
			playerData.v3LookW = pOwnerPlayer->GetTransform().GetLook();
			playerData.v3UpW = pOwnerPlayer->GetTransform().GetUp();
			playerData.v2BillboardSizeW = Vector2(0.3, 0.2);
			playerData.fRayLength = 10000.f;
			playerData.fRayHalfWidth = 0.05f;
			playerData.gvRayPositionW = pOwnerPlayer->GetRayPos();
			playerData.gvRaDirectionW = pOwnerPlayer->GetRayDirection();
		}

		ConstantBuffer cbuffer = RESOURCE->AllocCBuffer<CB_PLAYER_DATA>();
		cbuffer.WriteData(&playerData);
		
		pd3dDevice->CopyDescriptorsSimple(1, descHandle.cpuHandle, cbuffer.CBVHandle, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		descHandle.cpuHandle.Offset(1, D3DCore::g_nCBVSRVDescriptorIncrementSize);
		pd3dCommandList->SetGraphicsRootDescriptorTable(ROOT_PARAMETER_PLAYER_DATA, descHandle.gpuHandle);
		descHandle.gpuHandle.Offset(1, D3DCore::g_nCBVSRVDescriptorIncrementSize);

		// Ray
		pd3dCommandList->SetPipelineState(m_pd3dEffectPipelineStates[0].Get());
		pd3dCommandList->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_POINTLIST);
		pd3dCommandList->DrawInstanced(1, 1, 0, 0);

		// Billboard




	}



}
