#pragma once
#include "GameObject.h"

class RockObject : public GameObject {
public:
	RockObject();
	virtual ~RockObject();

public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Render(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList) override;

};

