#pragma once
#include "GameObject.h"

class SunObject : public GameObject {
public:
	SunObject();
	virtual ~SunObject();

public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Render(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList) override;


};

