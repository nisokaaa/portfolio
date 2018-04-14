#include <algorithm>

#include "motion.h"
#include "manager.h"
#include "renderer.h"
#include "sceneManager.h"
#include "textureManager.h"

CMotion::CMotion(){}
CMotion::~CMotion(){}


bool CMotion::LoadData(const char* file){

	m_bMotionEndFrame = false;

	//ここでファイル開いてファイルポインターを渡す
	m_filename = file;
	FILE* fp;
	fopen_s(&fp, file, "r");

	if (fp == NULL) {
		std::string str = file;
		str += "の読み込みに失敗";
		MessageBox(NULL, str.c_str(), "motion.cpp", MB_OK);
		return false;
	}

	if (!LoadOffsetData(fp)) return false;	//オフセットを読み込む
	if (!LoadMotionData(fp)) return false;	//モーションデータを読み込む

	fclose(fp);

	SetNowData();

	return true;
}
void CMotion::UnloadData(void){
	UnloadOffsetData();
	UnloadMotionData();
	//ReleaseData();
}

bool CMotion::LoadOffsetData(FILE* fp) {
	//デバイスを取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	//データを読み込んで格納

	int nNumParts;	//パーツ数
	fscanf_s(fp, "%d", &nNumParts);

	for (int nCnt = 0; nCnt < nNumParts; nCnt++) {
		m_vecParts.push_back(new PART);

		char filepass[256];	//ファイルパス
		fscanf_s(fp, "%s", filepass, sizeof(filepass));

		if (FAILED(D3DXLoadMeshFromX(filepass,	//ここも外部読み込み
			D3DXMESH_MANAGED,
			pDevice,
			NULL,
			&m_vecParts[nCnt]->MatBuff,
			NULL,
			&m_vecParts[nCnt]->nNumMat,
			&m_vecParts[nCnt]->Mesh)))
		{
			MessageBox(NULL, "Xファイル(モデル)の読み込み失敗", "motion.cpp", MB_OK);
			return false;
		}

		//座標
		fscanf_s(fp, "%f %f %f", &m_vecParts[nCnt]->Positon.x, &m_vecParts[nCnt]->Positon.y, &m_vecParts[nCnt]->Positon.z);

		//回転量
		fscanf_s(fp, "%f %f %f", &m_vecParts[nCnt]->Rotation.x, &m_vecParts[nCnt]->Rotation.y, &m_vecParts[nCnt]->Rotation.z);

		//拡大率
		fscanf_s(fp, "%f %f %f", &m_vecParts[nCnt]->Scaling.x, &m_vecParts[nCnt]->Scaling.y, &m_vecParts[nCnt]->Scaling.z);

		//親ID
		int nParent;
		fscanf_s(fp, "%d", &nParent);
		if (-1 == nParent) {
			m_vecParts[nCnt]->Parent = NULL;
		}
		else {
			m_vecParts[nCnt]->Parent = m_vecParts[nParent];
		}

		// テクスチャ
		D3DXMATERIAL *pMat = (D3DXMATERIAL*)m_vecParts[nCnt]->MatBuff->GetBufferPointer();
		for (int i = 0; i < (int)m_vecParts[nCnt]->nNumMat; i++)
		{
			if (pMat[i].pTextureFilename != NULL)
			{
				CTextureManager::Load(pMat[i].pTextureFilename);
			}
		}

		//マトリクスの初期化
		D3DXMatrixIdentity(&m_vecParts[nCnt]->Matrix);
	}

	return true;
}

void CMotion::UnloadOffsetData(void) {
	m_vecParts.erase(remove_if(m_vecParts.begin(), m_vecParts.end(),
		[](PART* p){
		if (NULL != p->Mesh) {
			p->Mesh->Release();
			p->Mesh = NULL;
		}
		D3DXMATERIAL *pMat = (D3DXMATERIAL*)p->MatBuff->GetBufferPointer();
		for (int i = 0; i < (int)p->nNumMat; i++)
		{
			if (pMat[i].pTextureFilename != NULL)
			{
				CTextureManager::Unload(pMat[i].pTextureFilename);
			}
		}
		if (NULL != p->MatBuff) {
			p->MatBuff->Release();
			p->MatBuff = NULL;
		}
		delete p;
		return true;
	}), m_vecParts.end());
}

bool CMotion::LoadMotionData(FILE* fp) {

	//データを格納

	//モーションの数
	int nNumMotion;
	fscanf_s(fp, "%d", &nNumMotion);

	for (int nCnt = 0; nCnt < nNumMotion; nCnt++) {
		m_vecMotion.push_back(new MOTION);

		//ループ
		int nLoop;
		fscanf_s(fp, "%d", &nLoop);
		if (0 == nLoop) {
			m_vecMotion[nCnt]->bLoop = false;
		} else {
			m_vecMotion[nCnt]->bLoop = true;
		}

		//キーセットの数
		int nKeyset;
		fscanf_s(fp, "%d", &nKeyset);

		for (int nCnt2 = 0; nCnt2 < nKeyset; nCnt2++) {
			m_vecMotion[nCnt]->vecKeySet.push_back(new KEY_SET);

			//キーセットのフレーム数
			//int nFrame;
			fscanf_s(fp, "%d", &m_vecMotion[nCnt]->vecKeySet[nCnt2]->nFrame);

			//パーツの数だけループ
			for (int nCnt3 = 0 ; nCnt3 < m_vecParts.size() ; nCnt3++) {
				m_vecMotion[nCnt]->vecKeySet[nCnt2]->vecKey.push_back(new KEY);

				fscanf_s(fp, "%f %f %f", &m_vecMotion[nCnt]->vecKeySet[nCnt2]->vecKey[nCnt3]->Positon.x, &m_vecMotion[nCnt]->vecKeySet[nCnt2]->vecKey[nCnt3]->Positon.y, &m_vecMotion[nCnt]->vecKeySet[nCnt2]->vecKey[nCnt3]->Positon.z);
				fscanf_s(fp, "%f %f %f", &m_vecMotion[nCnt]->vecKeySet[nCnt2]->vecKey[nCnt3]->Rotation.x, &m_vecMotion[nCnt]->vecKeySet[nCnt2]->vecKey[nCnt3]->Rotation.y, &m_vecMotion[nCnt]->vecKeySet[nCnt2]->vecKey[nCnt3]->Rotation.z);

				m_vecMotion[nCnt]->vecKeySet[nCnt2]->vecKey[nCnt3]->Positon  += m_vecParts[nCnt3]->Positon;
				m_vecMotion[nCnt]->vecKeySet[nCnt2]->vecKey[nCnt3]->Rotation += m_vecParts[nCnt3]->Rotation;
			}
		}
	}

	return true;
}

void CMotion::UnloadMotionData(void){

	m_vecMotion.erase( remove_if(m_vecMotion.begin(),m_vecMotion.end(),
		[](MOTION* m) {
		m->vecKeySet.erase(remove_if(m->vecKeySet.begin(), m->vecKeySet.end(),
			[](KEY_SET* kf) {
			kf->vecKey.erase(remove_if(kf->vecKey.begin(), kf->vecKey.end(),
				[](KEY* k) {
				//k->Positon = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				//k->Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				delete k;
				return true;
			}), kf->vecKey.end());
			//kf->nFrame = 0;
			delete kf;
			return true;
		}), m->vecKeySet.end());
		//m->bLoop = false;
		delete m;
		return true;
	}),m_vecMotion.end());
}

void CMotion::SetNowData(void) {

	m_cNowMotion = 0;		//初期モーション
	m_cNowKeyset = 0;		//初期キーセット
	m_cNextKeyset = (m_cNowKeyset + 1) % (m_vecMotion[m_cNowMotion]->vecKeySet.size());
	m_nMotionCounter = m_vecMotion[m_cNowMotion]->vecKeySet[m_cNowKeyset]->nFrame;

	for (unsigned int nCnt = 0; nCnt < m_vecParts.size(); nCnt++) {
		m_vecParts[nCnt]->Positon  = m_vecMotion[m_cNowMotion]->vecKeySet[m_cNowKeyset]->vecKey[nCnt]->Positon;
		m_vecParts[nCnt]->Rotation = m_vecMotion[m_cNowMotion]->vecKeySet[m_cNowKeyset]->vecKey[nCnt]->Rotation;

	}
}

void CMotion::Update(void) {
	//m_cNextKeyset = (m_cNowKeyset + 1) % (m_vecMotion[m_cNowMotion]->vecKeySet.size());
	m_bMotionEndFrame = false;

	Interpolation();

	if (m_nMotionCounter > 1) {
		m_nMotionCounter--;
	} else {
		//現在のキーセットが最後&&ループモーションの場合
		if (m_cNowKeyset == m_vecMotion[m_cNowMotion]->vecKeySet.size() - 1 &&
			false == m_vecMotion[m_cNowMotion]->bLoop) {
			m_cNowMotion = 0;
			m_bMotionEndFrame = true;
		}
		SetMotionInfo();
	}
}

void CMotion::Draw(D3DXMATRIX* mtxWorld){
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	D3DXMATRIX mtxPartScale, mtxPartRot, mtxPartTrans;

	//モデル
	for (unsigned int nCnt = 0; nCnt < m_vecParts.size(); nCnt++) {
		D3DXMatrixIdentity(&mtxPartScale);
		D3DXMatrixIdentity(&mtxPartRot);
		D3DXMatrixIdentity(&mtxPartTrans);
		D3DXMatrixIdentity(&m_vecParts[nCnt]->Matrix);

		D3DXMatrixScaling(&mtxPartScale,
			m_vecParts[nCnt]->Scaling.x,
			m_vecParts[nCnt]->Scaling.y,
			m_vecParts[nCnt]->Scaling.z);
		D3DXMatrixRotationYawPitchRoll(&mtxPartRot,
			m_vecParts[nCnt]->Rotation.y,
			m_vecParts[nCnt]->Rotation.x,
			m_vecParts[nCnt]->Rotation.z);
		D3DXMatrixTranslation(&mtxPartTrans,
			m_vecParts[nCnt]->Positon.x,
			m_vecParts[nCnt]->Positon.y,
			m_vecParts[nCnt]->Positon.z);

		D3DXMatrixMultiply(&m_vecParts[nCnt]->Matrix, &m_vecParts[nCnt]->Matrix, &mtxPartScale);			//拡大
		D3DXMatrixMultiply(&m_vecParts[nCnt]->Matrix, &m_vecParts[nCnt]->Matrix, &mtxPartRot);				//回転
		D3DXMatrixMultiply(&m_vecParts[nCnt]->Matrix, &m_vecParts[nCnt]->Matrix, &mtxPartTrans);			//平行移動

		if (m_vecParts[nCnt]->Parent == NULL) {
			D3DXMatrixMultiply(&m_vecParts[nCnt]->Matrix, &m_vecParts[nCnt]->Matrix, mtxWorld);
		}
		else {
			D3DXMatrixMultiply(&m_vecParts[nCnt]->Matrix, &m_vecParts[nCnt]->Matrix, &m_vecParts[nCnt]->Parent->Matrix);
		}

		pDevice->SetTransform(D3DTS_WORLD, &m_vecParts[nCnt]->Matrix);

		// モデル描画処理
		D3DMATERIAL9 matDef;

		//マテリアル情報の取得
		pDevice->GetMaterial(&matDef);

		//マテリアル情報のポインタとしてバッファのアドレスを取得
		D3DXMATERIAL* pMat = (D3DXMATERIAL*)m_vecParts[nCnt]->MatBuff->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)m_vecParts[nCnt]->nNumMat; nCntMat++)
		{
			// テクスチャ
			if (pMat[nCntMat].pTextureFilename != NULL)
			{
				LPDIRECT3DTEXTURE9 pTexture = CTextureManager::GetResource(pMat[nCntMat].pTextureFilename);

				pDevice->SetTexture(0, pTexture);
			}
			else
			{
				pDevice->SetTexture(0, NULL);
			}

			// マテリアル
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			// メッシュ
			m_vecParts[nCnt]->Mesh->DrawSubset(nCntMat); //nCnt番目のモデルのメッシュを描画
		}

		// マテリアル設定戻す
		pDevice->SetMaterial(&matDef);

	}
}

void CMotion::Interpolation(void) {

	for ( unsigned int nCnt = 0 ; nCnt < m_vecParts.size() ; nCnt++ ) {
		// 現在 += ((オフセット+モーションキーセット) - 現在) / カウンター
		// = 現在に目標への差分をフレーム分の一加算する
		m_vecParts[nCnt]->Positon  += (m_vecMotion[m_cNowMotion]->vecKeySet[m_cNextKeyset]->vecKey[nCnt]->Positon  - m_vecParts[nCnt]->Positon ) / m_nMotionCounter;
		m_vecParts[nCnt]->Rotation += (m_vecMotion[m_cNowMotion]->vecKeySet[m_cNextKeyset]->vecKey[nCnt]->Rotation - m_vecParts[nCnt]->Rotation) / m_nMotionCounter;
	}
}

void CMotion::SetMotion(char No) {
	if (m_cNowMotion != No) {
		m_cNowMotion = No;

		m_cNowKeyset = 0;
		m_cNextKeyset = (m_cNowKeyset + 1) % (m_vecMotion[m_cNowMotion]->vecKeySet.size());
		m_nMotionCounter = m_vecMotion[m_cNowMotion]->vecKeySet[m_cNextKeyset]->nFrame;
	}
}

void CMotion::SetMotionInfo(void){
	m_cNowKeyset = m_cNextKeyset;
	m_cNextKeyset = (m_cNowKeyset + 1) % (m_vecMotion[m_cNowMotion]->vecKeySet.size());
	m_nMotionCounter = m_vecMotion[m_cNowMotion]->vecKeySet[m_cNextKeyset]->nFrame;
}


float CMotion::GetTopPosY(void){
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	float fTopPos = 0.0f;

	for (unsigned int nCnt = 0; nCnt < m_vecParts.size(); nCnt++) {
		D3DXMATERIAL* pMat = (D3DXMATERIAL*)m_vecParts[nCnt]->MatBuff->GetBufferPointer();
		for (int nCntMat = 0; nCntMat < (int)m_vecParts[nCnt]->nNumMat; nCntMat++)
		{
			D3DXVECTOR3* pVtx;
			m_vecParts[nCnt]->Mesh->LockVertexBuffer(D3DLOCK_READONLY,(void**)&pVtx);
			//頂点数ループ
			for (int nCntVtx = 0; nCntVtx < m_vecParts[nCnt]->Mesh->GetNumVertices(); nCntVtx++) {
				D3DXVECTOR3 localPos = pVtx[nCntVtx];

				//座標変換
				D3DXVec3TransformCoord(&localPos, &localPos, &m_vecParts[nCnt]->Matrix);

				fTopPos = max(fTopPos, localPos.y);	//より大きい座標を取得
			}
			m_vecParts[nCnt]->Mesh->UnlockVertexBuffer();
		}
	}

	return fTopPos;
}
