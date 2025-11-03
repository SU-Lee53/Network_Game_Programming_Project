#pragma once
#include "GameObject.h"

class SpaceShipObject : public GameObject {
public:
	SpaceShipObject();
	virtual ~SpaceShipObject();

public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Render(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList) override;

};

