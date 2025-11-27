#pragma once
#include "MeshRenderer.h"

class SpaceshipPlayer;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PlayerRenderer

struct CB_PLAYER_DATA {
	Vector3 v3PositionW;
	float pad0 = 0.f;
	Vector3 v3LookW;             // ray direction
	float pad1 = 0.f;
	Vector3 v3UpW;               // ray Up -> 축 구성을 위함
	float pad2 = 0.f;
	float fRayLength;          // ray length
	float fRayHalfWidth;       // ray halfWidth
	Vector2 v2BillboardSizeW;    // billboard (width, height)
	Vector3 gvRayPositionW;
	float pad3 = 0.f;
	Vector3 gvRaDirectionW; // ray direction
	float pad4 = 0.f;
};

class PlayerRenderer : public MeshRenderer {
public:
	PlayerRenderer(std::shared_ptr<Mesh> pMesh, std::vector<std::shared_ptr<Material>> pMaterials);
	PlayerRenderer(const MESHLOADINFO& meshLoadInfo, const std::vector<MATERIALLOADINFO>& materialLoadInfo);
	virtual ~PlayerRenderer() {}

public:
	void SetPlayer(std::shared_ptr<SpaceshipPlayer> pPlayerRef);

public:
	virtual void Render(ComPtr<ID3D12Device> pd3dDevice, ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, 
		DescriptorHandle& descHandle, int nInstanceCount, OUT int& refnInstanceBase) override;

private:
	void CreatePipelineStates();

	std::weak_ptr<SpaceshipPlayer> m_wpPlayerRef;

};

