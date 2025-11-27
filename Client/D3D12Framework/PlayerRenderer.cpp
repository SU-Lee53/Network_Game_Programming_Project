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
}
