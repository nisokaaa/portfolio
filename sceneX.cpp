//=================================================================================
//	プログラム名	：	
//	処理			：	Xファイル描画オブジェクト処理[sceneX.cpp]
//	メイン処理		：	main.cpp
//	作成者			：	moriya gaku
//=================================================================================

//=================================================================================
//	インクルードファイル
//=================================================================================
#include "sceneX.h"
#include "manager.h"
#include "renderer.h"
#include "modelManager.h"
#include "textureManager.h"

//=================================================================================
//	初期化関数
//=================================================================================
HRESULT CSceneX::Init(void)
{
	// モデルのテクスチャ読み込み
	LoadTexture();

	return S_OK;
}

//=================================================================================
//	モデル内のテクスチャ読み込み関数
//=================================================================================
HRESULT CSceneX::LoadTexture( void )
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	LPD3DXBUFFER pBuffMat = CModelManager::GetResource( m_Key )->pBuffMat;
	int numMat = CModelManager::GetResource( m_Key )->nNumMat;

	// モデル読み込み
	D3DXMATERIAL *pMat = (D3DXMATERIAL*)pBuffMat->GetBufferPointer();
	
	for(int nCntMat = 0; nCntMat < (int)numMat; nCntMat++)
	{
		if( pMat[ nCntMat ].pTextureFilename )
		{ 
			string path = pMat[ nCntMat ].pTextureFilename;
			CTextureManager::Load( path );
		}
	}

	return S_OK;
}

//=================================================================================
//	終了関数
//=================================================================================
void CSceneX::Uninit(void)
{
	// 親クラスへ削除依頼
	Release();
}

//=================================================================================
//	更新関数
//=================================================================================
void CSceneX::Update(void)
{
}

//=================================================================================
//	描画関数
//=================================================================================
void CSceneX::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	D3DXMATRIX mtxScale;
	D3DMATERIAL9 matDef;
	D3DXMATERIAL *pMat;
	LPD3DXBUFFER pBuffMat = CModelManager::GetResource( m_Key )->pBuffMat;
	LPD3DXMESH pMesh = CModelManager::GetResource( m_Key )->pMesh;
	int numMat = CModelManager::GetResource( m_Key )->nNumMat;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_Matrix);

	// スケール
	D3DXMatrixScaling(&mtxScale, m_Scale.y, m_Scale.x, m_Scale.z);
	D3DXMatrixMultiply(&m_Matrix, &m_Matrix, &mtxScale);

	// 回転
	D3DXMatrixRotationYawPitchRoll( &m_MtxRot, GetRotation().y, GetRotation().x, GetRotation().z );
	D3DXMatrixMultiply(&m_Matrix, &m_Matrix, &m_MtxRot);

	// 移動
	D3DXMatrixTranslation(&m_MtxPos, GetPosition().x, GetPosition().y, GetPosition().z);
	D3DXMatrixMultiply(&m_Matrix, &m_Matrix, &m_MtxPos);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_Matrix);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアルへのポインタを取得
	pMat = (D3DXMATERIAL*)pBuffMat->GetBufferPointer();


	for(int nCntMat = 0; nCntMat < (int)numMat; nCntMat++)
	{
		// ファイル名を持ったマテリアルは読む
		if( pMat[ nCntMat ].pTextureFilename != NULL )
		{
			// マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			// テクスチャの設定
			string path = pMat[ nCntMat ].pTextureFilename;
			if( SUCCEEDED( pDevice->SetTexture(0, CTextureManager::GetResource( path ))))
			{
				// 描画
				pMesh->DrawSubset( nCntMat );
			}
		}
		else
		{
			// マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			// テクスチャの設定
			pDevice->SetTexture(0, NULL );

			// 描画
			pMesh->DrawSubset( nCntMat );
		}
	}

	// マテリアルを元に戻す
	pDevice->SetMaterial(&matDef);
}

//=================================================================================
//	生成関数
//=================================================================================
CSceneX *CSceneX::Create(string key, int priority)
{
	CSceneX *p = new CSceneX(priority);
	p->SetModel(key);
	p->SetScale(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	p->Init();

	return p;
}

//=================================================================================
//	キャラクタ束縛姿勢行列算出関数
//=================================================================================
D3DXMATRIX* CSceneX::CalcLookAtMatrixAxisFix(
   D3DXMATRIX* pout,
   D3DXVECTOR3* pPos,
   D3DXVECTOR3* pLook,
   D3DXVECTOR3* pUp
) {
   D3DXVECTOR3 X, Y, Z, D;
   D = *pLook - *pPos;
   D3DXVec3Normalize( &D, &D );
   D3DXVec3Cross( &X, D3DXVec3Normalize(&Y, pUp), &D );
   D3DXVec3Normalize( &X, &X );
   D3DXVec3Normalize( &Z, D3DXVec3Cross( &Z, &X, &Y ));

   pout->_11 = X.x; pout->_12 = X.y; pout->_13 = X.z; pout->_14 = 0;
   pout->_21 = Y.x; pout->_22 = Y.y; pout->_23 = Y.z; pout->_24 = 0;
   pout->_31 = Z.x; pout->_32 = Z.y; pout->_33 = Z.z; pout->_34 = 0;
   pout->_41 = 0.0f; pout->_42 = 0.0f; pout->_43 = 0.0f; pout->_44 = 1.0f;

   return pout;
}

//=================================================================================
// 球面線形補間関数
// out   : 補間ベクトル（出力）
// start : 開始ベクトル
// end : 終了ベクトル
// t : 補間値（0～1）
//=================================================================================
D3DXVECTOR3* CSceneX::SphereLinear( D3DXVECTOR3* out, D3DXVECTOR3* start, D3DXVECTOR3* end, float t )
{
	D3DXVECTOR3 s, e;
	D3DXVec3Normalize( &s, start );
	D3DXVec3Normalize( &e, end );

	// 2ベクトル間の角度（鋭角側）
	float angle = acos( D3DXVec3Dot( &s, &e ) );

	// sinθ
	float SinTh = sin( angle );

	// 補間係数
	float Ps = sin( angle * ( 1 - t ) );
	float Pe = sin( angle * t );

	*out = ( Ps * s + Pe * e ) / SinTh;

	// 一応正規化して球面線形補間に
	D3DXVec3Normalize( out, out );

	return out;
}

//=================================================================================
// 球面線形補間による補間姿勢算出関数
// out : 補間姿勢（出力）
// start : 開始姿勢
// end : 目標姿勢
// t : 補間係数（0～1
//=================================================================================
D3DXMATRIX* CSceneX::CalcInterPause( D3DXMATRIX* out, D3DXMATRIX* start, D3DXMATRIX* end, float t )
{
   // 各姿勢ベクトル抽出
   D3DXVECTOR3 Sy, Sz;
   D3DXVECTOR3 Ey, Ez;

   memcpy( &Sy, start->m[1], sizeof( float ) * 3 );
   memcpy( &Sz, start->m[2], sizeof( float ) * 3 );
   memcpy( &Ey, end->m[1], sizeof( float ) * 3 );
   memcpy( &Ez, end->m[2], sizeof( float ) * 3 );

   // 中間ベクトル算出
   D3DXVECTOR3 IY, IZ;
   SphereLinear( &IY, &Sy, &Ey, t );
   SphereLinear( &IZ, &Sz, &Ez, t );

   // 中間ベクトルから姿勢ベクトルを再算出
   D3DXVECTOR3 IX;
   D3DXVec3Cross( &IX, &IY, &IZ );
   D3DXVec3Cross( &IY, &IZ, &IX );
   D3DXVec3Normalize( &IX, &IX );
   D3DXVec3Normalize( &IY, &IY );
   D3DXVec3Normalize( &IZ, &IZ );

   memset( out, 0, sizeof( D3DXMATRIX ) );
   memcpy( out->m[0], &IX, sizeof( float ) * 3 );
   memcpy( out->m[1], &IY, sizeof( float ) * 3 );
   memcpy( out->m[2], &IZ, sizeof( float ) * 3 );
   out->_44 = 1.0f;

   return out;
}