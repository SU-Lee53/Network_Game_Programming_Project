#include "pch.h"
#include "ModelManager.h"
#include "PlayerRenderer.h"

ModelManager::ModelManager(ComPtr<ID3D12Device> pDevice)
{
}

ModelManager::~ModelManager()
{
}

void ModelManager::LoadGameModels()
{
	// 원래 이러라고 만든게 아닌데 모든 행성이 아래 모델을 공유해버려 Texture 를 갈아끼우기가 곤란함
	// 방법은 더 생각해봐야겠지만 일단 이걸 5개 만들어버리는걸로 때워보자
	Add("Sphere_1", MODELLOADER->Load<GameObject, TexturedNormalRenderer>("../Resource/Sphere.obj"));	// Earth
	Add("Sphere_2", MODELLOADER->Load<GameObject, TexturedNormalRenderer>("../Resource/Sphere.obj"));	// Mars
	Add("Sphere_3", MODELLOADER->Load<GameObject, TexturedNormalRenderer>("../Resource/Sphere.obj"));	// Mercury
	Add("Sphere_4", MODELLOADER->Load<GameObject, TexturedNormalRenderer>("../Resource/Sphere.obj"));	// Rock
	Add("Sphere_5", MODELLOADER->Load<GameObject, TexturedRenderer>("../Resource/Sphere.obj"));	// Sun
	Add("Sphere_6", MODELLOADER->Load<GameObject, TexturedNormalRenderer>("../Resource/Sphere.obj"));	// Venus


	Add("Spaceship", MODELLOADER->Load<GameObject, PlayerRenderer>("../Resource/space_ship.obj"));
	Add("Spaceship2", MODELLOADER->Load<GameObject, PlayerRenderer>("../Resource/space_ship.obj"));
	Add("Spaceship3", MODELLOADER->Load<GameObject, PlayerRenderer>("../Resource/space_ship.obj"));
	Add("Rock", MODELLOADER->Load<GameObject, TexturedNormalRenderer>("../Resource/rock/rock.obj"));
}

void ModelManager::Add(const std::string& strModelName, std::shared_ptr<GameObject> pObj)
{
	if (!m_pModelPool.contains(strModelName)) {
		m_pModelPool.insert({ strModelName, pObj });
	}
}

std::shared_ptr<GameObject> ModelManager::Get(const std::string& strObjName)
{
	auto it = m_pModelPool.find(strObjName);
	if (it == m_pModelPool.end()) {
		return nullptr;
	}

	return it->second;
}

