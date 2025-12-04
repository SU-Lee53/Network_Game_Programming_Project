#include "Logic.h"
#include "Rock.h"
#include "Player.h"
#include "ObjectCollections.h"
#include <random>

//////////////////////////////////////////////////////////////////////////////////////////////
// 2025.11.25
// rock을 반원형태의 공간에서 랜덤한 위치에 생성하게 변경 By 민정원
// 2025.12.03
// 반구형 랜덤 위치 생성 구현

std::uniform_real_distribution<float> angleDist(0.0f, 1.0f);  // 0~1 범위로 각도 선택
std::uniform_real_distribution<float> yDist(0.0f, 100.0f);  // Y 높이 범위

std::unique_ptr<Rock> CreateRock(const Player* Player)
{
	std::unique_ptr<Rock> rock = std::make_unique<Rock>();


	// 반원 각도 생성 (270° ~ 360° + 0° ~ 90°)
	float angleChoice = angleDist(dre);
	float theta;
	if (angleChoice < 0.5f) {
		// 270° ~ 360° 구간 (1.5π ~ 2π)
		std::uniform_real_distribution<float> theta1Dist(XM_PI + XM_PIDIV2, XM_2PI);
		theta = theta1Dist(dre);
	}
	else {
		// 0° ~ 90° 구간 (0 ~ 0.5π)
		std::uniform_real_distribution<float> theta2Dist(0.0f, XM_PIDIV2);
		theta = theta2Dist(dre);
	}

	float radius = 1000.f;
	float y = yDist(dre);

	// 극좌표를 직교 좌표로 변환 (XZ 평면)
	float x = radius * cosf(theta);
	float z = radius * sinf(theta);

	rock->SetPosition(x, y, z);

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
	using namespace DirectX;

	std::map<Rock*, std::vector<Player*>> pIntersectionResultMap;

	for (int i = 0; i < 3; ++i) {
		if (Players[i]->IsAlive() == false) {
			continue;
		}

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
			pIntersectionResultMap[pCollidedRock].push_back(Players[i].get());
		}
	}

	// 점수를 줄 플레이어를 선정하여 점수를 준다
	const int nScoreBase = 10;
	for (const auto& [pRock, pPlayers] : pIntersectionResultMap) {
		if (pPlayers.size() == 1) {
			XMFLOAT3 xmf3PlayerPos = pPlayers[0]->GetPosition();
			XMFLOAT3 xmf3RockPos = pRock->GetPosition();
			float fDistance = XMVectorGetX(XMVector3Length(XMVectorSubtract(XMLoadFloat3(&xmf3RockPos), XMLoadFloat3(&xmf3PlayerPos))));
			pPlayers[0]->GiveScore(nScoreBase * (fDistance * 0.1));
		}
		else {
			float fMinDistance = std::numeric_limits<float>::max();
			XMFLOAT3 xmf3RockPos = pRock->GetPosition();
			XMVECTOR xmvRockPos = XMLoadFloat3(&xmf3RockPos);
			Player* pScoringPlayer = nullptr;		// 점수를 얻을 플레이어
			for (auto pPlayer : pPlayers) {
				XMFLOAT3 xmf3PlayerPos = pPlayer->GetPosition();
				float fDistance = XMVectorGetX(XMVector3Length(XMVectorSubtract(XMLoadFloat3(&xmf3RockPos), XMLoadFloat3(&xmf3PlayerPos))));
				if (fDistance < fMinDistance) {
					pScoringPlayer = pPlayer;
				}
			}

			if (pScoringPlayer) {
				pScoringPlayer->GiveScore(nScoreBase * (fMinDistance * 0.1));
			}
		}

	}

}

//////////////////////////////////////////////////////////////////////////////////////////////
// 2025.11.28
// CheckPlayerIntersection() By 최명준
// Player와 Player간의 충돌 검사

void CheckPlayerIntersection()
{
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			if (i == j)
				continue;
			
			if (Players[i]->GetBoundingSphere().Intersects(Players[j]->GetBoundingSphere())&& Players[i]->GetInvincibleTimer() <= 0)
			{
				Players[i]->SetDamagefromPlayer();
			}
		}
	}
}

void CheckRockIntersection()
{
	for (int i = 0; i < 3; ++i)
	{
		for (auto iter = Rocks.begin(); iter != Rocks.end(); ++iter)
		{
			if (Players[i]->GetBoundingSphere().Intersects(iter->get()->GetBoundingSphere()) && Players[i]->GetInvincibleTimer() <= 0)
			{
				Players[i]->SetDamagefromRock();
			}
		}
	}
}
