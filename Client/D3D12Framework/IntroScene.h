#pragma once
#include "Scene.h"

class IntroScene : public Scene {
public:
	virtual void OnEnterScene() override {};
	virtual void OnLeaveScene() override {};

public:
	void ProcessInput() override;
	void BuildObjects() override;
	void Update() override;
	void Render(ComPtr<ID3D12GraphicsCommandList> pd3dCommansList) override;


};

