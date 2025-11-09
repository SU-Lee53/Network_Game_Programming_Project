#pragma once
#include "Scene.h"

class TestScene : public Scene {
public:
	void BuildObjects() override;
	virtual void BuildLights() override;
	void Update() override;
	void Render(ComPtr<ID3D12GraphicsCommandList> pd3dCommansList) override;

protected:
	void ProcessInput() override;


	//pointLightPositions.emplace_back(0.7f, 0.2f, 2.0f);
	//pointLightPositions.emplace_back(2.3f, -3.3f, -4.0f);
	//pointLightPositions.emplace_back(-4.0f, 2.0f, -12.0f);
	//pointLightPositions.emplace_back(0.0f, 0.0f, -3.0f);

	std::string strReceived;



};

