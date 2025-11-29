#pragma once
#include "Scene.h"

class SpaceshipPlayer;
class RockObject;

class TestScene : public Scene {
public:
	virtual void OnEnterScene() override;
	virtual void OnLeaveScene() override;

public:
	void BuildObjects() override;
	virtual void BuildLights() override;
	void Update() override;
	void Render(ComPtr<ID3D12GraphicsCommandList> pd3dCommansList) override;

	virtual void SyncSceneWithServer() override;

protected:
	void ProcessInput() override;

	void InitializeOtherPlayers();


	//pointLightPositions.emplace_back(0.7f, 0.2f, 2.0f);
	//pointLightPositions.emplace_back(2.3f, -3.3f, -4.0f);
	//pointLightPositions.emplace_back(-4.0f, 2.0f, -12.0f);
	//pointLightPositions.emplace_back(0.0f, 0.0f, -3.0f);

	std::string strReceived;

	ULONGLONG m_ullDataReceived = 0;

	// 11.15 이승욱
	std::array<std::shared_ptr<SpaceshipPlayer>, 2> m_pOtherPlayers;

	std::shared_ptr<RockObject> m_pRockObj;


};

