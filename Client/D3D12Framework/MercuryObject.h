#pragma once
#include "GameObject.h"

class MercuryObject : public GameObject {
public:
	MercuryObject();
	virtual ~MercuryObject();

public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Render(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList) override;

};

