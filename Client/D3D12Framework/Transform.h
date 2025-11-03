#pragma once

class GameObject;

class Transform {
public:
	Transform();

	void Update(std::shared_ptr<GameObject> pParent = nullptr);

public:
	void SetFrameMatrix(const Matrix& mtxFrame);

public:
	void SetPosition(float x, float y, float z);
	void SetPosition(const Vector3& xmf3Position);

	void Move(Vector3 v3MoveDirection, float fAmount);

public:
	// Rotate 이후 이걸 쓰면 기존의 모든 회전이 날아감
	void SetRotation(float fPitch, float fYaw, float fRoll);

	// Rotate 이후 이걸 쓰면 기존의 모든 회전이 날아감
	void SetRotation(const Vector3& xmf3Position);

	void Rotate(const Vector3& v3Rotation);
	void Rotate(float fPitch, float fYaw, float fRoll);
	void Rotate(const Vector3& v3Axis, float fAngle);
	
	void RotateWorld(float fPitch, float fYaw, float fRoll, const Vector3& v3BaseOnWorld = Vector3(0,0,0));
	void RotateWorld(const Vector3& v3Axis, float fAngle, const Vector3& v3BaseOnWorld = Vector3(0, 0, 0));

public:
	void Scale(float fxScale, float fyScale, float fzScale);
	void Scale(const Vector3& v3Scale);
	void Scale(float fScale);

public:
	Vector3 GetPosition() const;
	Vector3 GetRight() const;
	Vector3 GetUp() const;
	Vector3 GetLook() const;
	
	Matrix GetWorldMatrix() const;

private:
	// m_mtxFrameRelative : 계층 모델에서 부모로부터의 상대 변환
	// m_mtxTransform : 오브젝트가 수행할 변환의 누적
	// m_mtxWorld : m_mtxTransform * m_mtxFrameRelative (자식이라면 * (부모의 월드))
	//             월드 원점 기준 최종 변환 행렬

	Matrix m_mtxFrameRelative	= {};
	Matrix m_mtxTransform		= {};
	Matrix m_mtxWorld			= {};

};
