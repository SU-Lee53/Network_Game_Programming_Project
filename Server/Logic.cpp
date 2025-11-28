#include "Logic.h"
#include "Rock.h"
#include "Player.h"
#include "ObjectCollections.h"


std::unique_ptr<Rock> CreateRock(const Player* Player)
{
	std::unique_ptr<Rock> rock = std::make_unique<Rock>();
	auto PlayerTransform = Player->GetWorldMatrix();
	XMFLOAT3 PlayerPosition = XMFLOAT3(PlayerTransform._41, PlayerTransform._42, PlayerTransform._43);
	rock->SetDirection(PlayerPosition);
	return rock;
}

//////////////////////////////////////////////////////////////////////////////////////////////
// 2025.11.25
// CheckRayIntersection() By 이승욱
// Rock 과 Player 의 Ray 간의 충돌 검사

void CheckRayIntersection()
{
	for (int i = 0; i < 3; ++i) {
		const Ray& ray = Players[i]->GetRayData();
		XMVECTOR xmvRayDirection = XMLoadFloat3(&ray.xmf3RayDirection);

		// 방향이 0,0,0 -> Ray 발사가 일어나지 않은 것이므로 넘어감
		if (XMVector3Equal(xmvRayDirection, XMVectorZero())) continue;

		float fMinDistance = std::numeric_limits<float>::max();
		Rock* pCollidedRock = nullptr;	// Ray 교차한 Rock 중 가장 가까운 Rock
		for (const auto& pRock : Rocks) {
			float fDistance = 0;
			if (pRock->CheckCollision(ray, fDistance)) {
				if (fDistance < fMinDistance) {
					fMinDistance = fDistance;
					pCollidedRock = pRock.get();
				}
			}
		}

		if (pCollidedRock) {
			pCollidedRock->SetDead();
		}
	}
}
