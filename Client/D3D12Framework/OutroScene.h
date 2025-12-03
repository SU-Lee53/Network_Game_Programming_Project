#pragma once
#include "Scene.h"

class OutroScene : public Scene {
public:
	virtual void OnEnterScene() override {};
	virtual void OnLeaveScene() override {};

public:
	void BuildObjects() override;
	void ProcessInput() override;
	void Update() override;
	void Render(ComPtr<ID3D12GraphicsCommandList> pd3dCommansList) override;

	virtual void SyncSceneWithServer() override;


};

