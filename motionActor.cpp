#include "motionActor.h"
#include "motion.h"


HRESULT MotionActor::Init(void) {

	m_pMotion = new CMotion();
	m_bMotion = m_pMotion->LoadData(m_FileName);

	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	D3DXMatrixIdentity(&m_mtxWorld);
	D3DXMatrixIdentity(&m_mtxRot);
	D3DXMatrixIdentity(&m_mtxTrans);
	D3DXMatrixIdentity(&m_mtxScale);

	return S_OK;
}

void MotionActor::Uninit(void) {
	m_pMotion->UnloadData();
	delete m_pMotion;
	m_pMotion = NULL;
}

void MotionActor::Update(void) {
	if (m_bMotion) m_pMotion->Update();
}

void MotionActor::Draw(void) {

	// ワールド変換行列処理 ////////
	D3DXMatrixIdentity(&m_mtxWorld);
	D3DXMatrixIdentity(&m_mtxScale);
	D3DXMatrixIdentity(&m_mtxRot);
	D3DXMatrixIdentity(&m_mtxTrans);


	D3DXMatrixScaling(&m_mtxScale, m_Scl.x, m_Scl.y, m_Scl.z);					//拡大行列を作る
	D3DXMatrixRotationYawPitchRoll(&m_mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);		//回転行列を作る

	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	CalcLookAtMatrix(&m_mtxRot, &m_Pos, &m_LookVec, &up);

	D3DXMatrixTranslation(&m_mtxTrans, m_Pos.x, m_Pos.y, m_Pos.z);					//平行移動行列を作る

	D3DXMatrixMultiply(&m_mtxWorld,&m_mtxWorld,&m_mtxScale);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &m_mtxRot);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &m_mtxTrans);


	if (m_bMotion) m_pMotion->Draw(&m_mtxWorld);

}

//void MotionActor::CalcLookAtMatrix(D3DXMATRIX* pout, D3DXVECTOR3* pLook) {
//	D3DXVECTOR3 X, Y, Z;
//	D3DXVECTOR3 pUp(0.0f, 1.0f, 0.0f);
//	Z = *pLook;
//	D3DXVec3Normalize(&Z, &Z);
//	D3DXVec3Cross(&X, D3DXVec3Normalize(&Y, &pUp), &Z);
//	D3DXVec3Normalize(&X, &X);
//	D3DXVec3Normalize(&Y, D3DXVec3Cross(&Y, &Z, &X));
//
//
//	pout->_11 = X.x; pout->_12 = X.y; pout->_13 = X.z; pout->_14 = 0;
//	pout->_21 = Y.x; pout->_22 = Y.y; pout->_23 = Y.z; pout->_24 = 0;
//	pout->_31 = Z.x; pout->_32 = Z.y; pout->_33 = Z.z; pout->_34 = 0;
//	pout->_41 = 0.0f; pout->_42 = 0.0f; pout->_43 = 0.0f; pout->_44 = 1.0f;
//}