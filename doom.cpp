//=================================================================================
//	プログラム名	：	
//	処理			：	メッシュドーム処理[doom.cpp]
//	メイン処理		：	main.cpp
//	作成者			：	moriya gaku
//=================================================================================

//=================================================================================
//	インクルードファイル
//=================================================================================
#include "main.h"
#include "doom.h"
#include "manager.h"
#include "renderer.h"
#include "textureManager.h"
#include "shaderManager.h"
#include "sceneManager.h"
#include "cameraManager.h"
#include "lightManager.h"

//=================================================================================
//	コンストラクタ
//=================================================================================
CDoom::CDoom()
{
	m_AngleXY = 0.0f;
	m_AngleXZ = 0.0f;
}

//=================================================================================
//	頂点バッファ作成
//=================================================================================
HRESULT CDoom::MakeVertexBuffer( void )
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	LPDIRECT3DVERTEXBUFFER9 pVtxBuffer = NULL;
	int numWidth = GetNumWidth();
	int numHeight = GetNumHeight();

    if( pDevice != NULL )
	{
		/*------------------------------------------------------------------------*/
		/*	頂点バッファの作成
		/*------------------------------------------------------------------------*/
		if( FAILED( pDevice -> CreateVertexBuffer( 
			sizeof( VERTEX_3D ) * ( numHeight + 1 ) * ( numWidth + 1 ), 
			D3DUSAGE_WRITEONLY, 
			FVF_VERTEX_3D, 
			D3DPOOL_MANAGED, 
			&pVtxBuffer, 
			NULL ) ) )
		{
			MessageBox( NULL, "頂点バッファの作成に失敗しました。", "VERTEX_BUFFER_ERROR", MB_OK );
		}
		else
		{
			pDevice -> CreateVertexBuffer( sizeof( VERTEX_3D ) * ( numHeight + 1 ) * ( numWidth + 1 ), D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &pVtxBuffer, NULL );	
		}

	}

	// 頂点バッファ保存
	SetVertexBuffer( pVtxBuffer );

	return S_OK;
}

//=================================================================================
//	頂点情報のセット
//=================================================================================
void CDoom::SetVertex( void )
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// 仮想メモリアクセス用
	VERTEX_3D *pVtx;
	LPDIRECT3DVERTEXBUFFER9 pVtxBuffer = GetVertexBuffer();
	float width = GetWidth();
	float height = GetHeight();
	int numWidth = GetNumWidth();
	int numHeight = GetNumHeight();

	// テクスチャ座標UV
	float fOffSetTexU = 1.0f / numWidth;
	float fTexU = 0.0f;
	float fOffSetTexV = 1.0f / numHeight;
	float fTexV = 0.0f;
	static float MoveU = 0.0f;

	// ループ用添字
	int nIndex = 0;
	
	// 仮想メモリに書き込み
	pVtxBuffer->Lock( 0, 0, ( void** )&pVtx, 0 );
	
	// うえから見た移動角
	float fAngleValXZ = m_AngleXZ / numWidth;
	float fAngleXZ = 0.0f;
	
	// よこから見た移動角
	float fAngleValXY = m_AngleXY / numHeight;
	float fAngleXY = 90.0f;

	// 球の半径
	float fr = (width * numWidth) * 0.5f;
	
	// 移動量X
	float fLength = 0.0f;

	// 座標保存用(前)
	D3DXVECTOR3 fPos = D3DXVECTOR3( 0.0f, fr, 0.0f );

	// フィールドの縦×横ポリゴン枚数分繰り返し
	for( int nCntDoomY = 0; nCntDoomY <= numHeight; nCntDoomY++ )
	{
		// ループ用添字更新
		nIndex = nCntDoomY * (numWidth + 1);

		for( int nCntDoomX = 0; nCntDoomX <= numWidth; nCntDoomX++ )
		{	
			// XZ座標回転
			fPos.x = fLength * cosf( AngleToRadian(fAngleXZ) );
			fPos.z = fLength * sinf( AngleToRadian(fAngleXZ) );

			// 頂点バッファへ格納
			pVtx[ nCntDoomX + nIndex ].pos	= fPos;
			pVtx[ nCntDoomX + nIndex ].tex	= D3DXVECTOR2( fTexU + MoveU, fTexV );
			pVtx[ nCntDoomX + nIndex ].nor	= D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
			pVtx[ nCntDoomX + nIndex ].col	= D3DCOLOR_RGBA( 255, 255, 255, 255 );
	
			//MyOutputDebugString( ("pVtx[ %d ] %f, %f, %f )\n"),nCntDoomX + nIndex, fPos.x, fPos.y, fPos.z );
			
			// U値シフト
			fTexU += fOffSetTexU;

			// 時計回りに移動
			fAngleXZ += fAngleValXZ;
		}

		// U値クリア
		fTexU = 0.0f;

		// V値シフト
		fTexV += fOffSetTexV;

		// 角度を下げる移動
		fAngleXY -= fAngleValXY;
		
		// Y移動
		float fPrevPosY = fPos.y;
		float diffY = fPrevPosY - (fr * sinf( AngleToRadian(fAngleXY) ));
		fPos.y -= diffY;
		
		// X移動
		fLength = fr * cosf( AngleToRadian(fAngleXY) );

		// XZ初期化
		fAngleXZ = 0.0f;
	}

	MoveU += 0.00008f;

	// 仮想メモリをアンロック
	pVtxBuffer->Unlock();

	// シェーダーの値セット
	CCameraManager *pCameraManager = CSceneManager::GetCameraManager();
	CCamera *pCamera = pCameraManager->GetCamera(MAIN_CAMERA);
	CLightManager *pLightManager = CSceneManager::GetLightManager();
	CLight *pLight = pLightManager->GetLight(MAIN_LIGHT);
	CSBasic *pShader = dynamic_cast<CSBasic*>(GetShader());
	D3DXVECTOR3 dir = pLight->GetVecDir();
	pShader->SetParam(pCamera->GetViewMtx(), pCamera->GetProjMtx(), dir, pCamera->GetPosition(), D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f), "data/TEXTURE/skydome/309.JPG");

	return;
}

//=================================================================================
//	ワールド行列計算・デバイスにセット
//=================================================================================
void CDoom::SetMatrix(void)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// 各マトリクスの作成( ワールド・ビュー・ プロジェクション・回転・平行・拡大 )
	D3DXMATRIX mtxWorld, mtxRot, mtxPos;

	// 行列を単位行列にする(なんでもないただの行列を作成する)
	D3DXMatrixIdentity(&mtxWorld);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, D3DXToRadian(GetRotation().y), D3DXToRadian(GetRotation().x), D3DXToRadian(GetRotation().z));
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	// 移動を反映
	D3DXMatrixTranslation(&mtxPos, GetPosition().x, GetPosition().y, GetPosition().z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxPos);

	//// デバイスにワールド変換行列を指定
	//pDevice -> SetTransform( D3DTS_WORLD, &mtxWorld );

	//pDevice->SetTexture(0, CTextureManager::GetResource(GetKey()));

	// デバイスにワールド変換行列を指定
	GetShader()->SetMatrix(mtxWorld);
}

//=================================================================================
//	メッシュフィールド生成
//=================================================================================
CDoom *CDoom::Create( D3DXVECTOR3 pos, D3DXVECTOR3 rot, float width, float  height,int numWidth, int numHeight, float angleXY, float angleXZ, const char* texPath )
{
	CDoom *pDoom = NULL;

	pDoom = new CDoom;

	// 座標設定
	pDoom->SetPosition( pos );

	// 回転角度設定
	pDoom->SetRotation( rot );

	// ポリゴンの横サイズ設定
	pDoom->SetWidth( width );
	
	// ポリゴンの縦サイズ設定
	pDoom->SetHeight( height );
	
	// 横ブロック数設定
	pDoom->SetNumWidth( numWidth );													
	
	// 縦ブロック数設定
	pDoom->SetNumHeight( numHeight );
	
	// XY軸間の角度(90で半球)
	pDoom->SetAngleXY( angleXY );
	
	// XZ軸間の角度(361で半球)
	pDoom->SetAngleXZ( angleXZ );
	
	// 縮退ポリゴン含めた総ポリゴン数
	pDoom->SetNumPolygon( (( numWidth + 2 ) * ( numHeight -1 ) + numWidth ) * 2 );

	// テクスチャ読み込み
	pDoom->SetTexture( texPath );

	// シェーダーのセット
	CShaderManager *pShaderManager = CSceneManager::GetShaderManager();
	CSBasic *pShader = dynamic_cast<CSBasic*>(pShaderManager->GetData(SHADER_BASIC));
	pDoom->SetShader(pShader);

	pDoom->Init();

	return pDoom;
}

float CDoom::AngleToRadian( float angle )
{
	return (angle * 3.14f) / 180.0f;
};
