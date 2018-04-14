#ifndef _MOTION_ACTOR_H_
#define _MOTION_ACTOR_H_

#include "main.h"
#include "motion.h"

class MotionActor{
public:
	MotionActor(const char* filename) : m_FileName(filename){}
	~MotionActor(){}

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	D3DXVECTOR3& GetPos(void) { return m_Pos; }
	D3DXVECTOR3& GetRot(void) { return m_Rot; }
	D3DXVECTOR3& GetScl(void) { return m_Scl; }

	void SetPos(D3DXVECTOR3 pos) { m_Pos = pos; }
	void SetRot(D3DXVECTOR3 rot) { m_Rot = rot; }
	void SetScl(D3DXVECTOR3 scl) { m_Scl = scl; }

	CMotion* GetMotionData(void) { return m_pMotion; }
	void SetLookAtVec(D3DXVECTOR3 look) { m_LookVec = look; }

private:
	//void CalcLookAtMatrix(D3DXMATRIX* pout, D3DXVECTOR3* pLook);
	D3DXVECTOR3 m_LookVec;
	D3DXVECTOR3 m_Pos;
	D3DXVECTOR3 m_Rot;
	D3DXVECTOR3 m_Scl;

	D3DXMATRIX m_mtxWorld;
	D3DXMATRIX m_mtxRot;
	D3DXMATRIX m_mtxTrans;
	D3DXMATRIX m_mtxScale;

	CMotion* m_pMotion;
	bool m_bMotion;
	
	const char* m_FileName;
};

#endif	//_MOTION_ACTOR_H_
