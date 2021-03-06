#pragma once
class WindMill : public GameObject
{
	BufferManager* m_WindMillBodyBuffer;
	BufferManager* m_WindMillWingBuffer;

	Transform* m_WingTrans;

	bool m_bShoot;
	bool m_bReturn;

	D3DXVECTOR3 reDir;
	D3DXVECTOR3 Dir;
	D3DXMATRIX mTemp;
	D3DXVECTOR3 m_TempPosition;
	D3DXVECTOR3 mDirTemp;

	float m_fDistance;

public:
	WindMill();
	~WindMill();

public:
	void Init();

private:
	void Move();
	void ShootWing();

private:
	void SetWindMillVertex();
	void SetWindMillIndex();

public:
	void Update(float deltaTime) override;
	void Render() override;
};