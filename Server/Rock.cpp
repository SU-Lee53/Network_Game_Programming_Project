#include "Rock.h"

Rock::Rock()
{
}

Rock::~Rock()
{
}

void Rock::SetDirection(const XMFLOAT3& PlayerPosition)
{
	XMFLOAT3 Direction = Vector3::Subtract(PlayerPosition, m_Xmf3Position);
	m_Xmf3Direction = Vector3::Normalize(Direction);
}
