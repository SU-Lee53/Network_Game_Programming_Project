#pragma once
#include "GameObject.h"

class MarsObject : public GameObject {
public:
	MarsObject();
	virtual ~MarsObject();

public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Render(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList) override;


};

