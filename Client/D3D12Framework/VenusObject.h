#pragma once
#include "GameObject.h"

class VenusObject : public GameObject {
public:
	VenusObject();
	virtual ~VenusObject();

public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Render(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList) override;


};

