#pragma once
#include "GameObject.h"

class EarthObject : public GameObject {
public:
	EarthObject();
	virtual ~EarthObject();

public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Render(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList) override;

};

