//=================================================================================
//	プログラム名	：	
//	処理			：	メッシュフィールド処理[meshField.cpp]
//	メイン処理		：	main.cpp
//	作成者			：	moriya gaku
//=================================================================================

//=================================================================================
//	インクルードファイル
//=================================================================================
#include "main.h"
#include "meshField.h"
#include "manager.h"
#include "renderer.h"
#include "textureManager.h"
#include "shaderManager.h"
#include "sceneManager.h"
#include "cameraManager.h"
#include "lightManager.h"

//=================================================================================
//	終了関数
//=================================================================================
void CMeshField::Uninit(void)
{
	for (int i = 0; i < (int)m_vHeight.size(); i++)
	{
		for (int j = 0; j < (int)m_vHeight[i].size(); j++)
		{
			m_vHeight[i][j] = 0.0f;
		}
	}
	m_vHeight.clear();

	CMesh::Uninit();
}

//=================================================================================
//	頂点バッファ作成
//=================================================================================
HRESULT CMeshField::MakeVertexBuffer( void )
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
			// (2 * 2 + ヨコマス) * タテマス + (タテマス - 1) * 2
			// sizeof( VERTEX_3D ) * ( numHeight + 1 ) * ( numWidth + 1 ), 
			sizeof( VERTEX_3D ) * ( 2 * 2 * numWidth ) * numHeight + ( numHeight - 1 ) * 2,
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
			pDevice -> CreateVertexBuffer( 
				//sizeof( VERTEX_3D ) * ( numHeight + 1 ) * ( numWidth + 1 ),
				sizeof( VERTEX_3D ) * ( 2 * 2 * numWidth ) * numHeight + ( numHeight - 1 ) * 2,
				D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &pVtxBuffer, NULL );	
		}

	}

	// 頂点バッファ保存
	SetVertexBuffer( pVtxBuffer );

	return S_OK;
}

//=================================================================================
//	頂点情報のセット
//=================================================================================
void CMeshField::SetVertex( void )
{
	// 仮想メモリアクセス用
	VERTEX_3D *pVtx;
	LPDIRECT3DVERTEXBUFFER9 pVtxBuffer = GetVertexBuffer();
	float width = GetWidth();
	float height = GetHeight();
	int numWidth = GetNumWidth();
	int numHeight = GetNumHeight();

	// 基準設定
	D3DXVECTOR3 basePos = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	D3DXVECTOR3 offsetPos = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );

	// フィールド中心から角までの角度
	float fTheta = atan2f( float(height * height), float(width * width) );

	float allWidth = float(width * numWidth);
	float allHeight = float(height * numHeight);
	float length = sqrtf( allWidth * allWidth + allHeight * allHeight ) * 0.5f;
	basePos.x = -cosf( fTheta ) * length;
	basePos.z = sin( fTheta ) * length;

	// テクスチャ座標UV
	float offsetTexU = 0.0f;
	float offsetTexV = 0.0f;

	// ループ用添字
	int index = 0;

	// 仮想メモリに書き込み
	pVtxBuffer->Lock( 0, 0, ( void** )&pVtx, 0 );

	// 座標・UV値・色の決定
	for( int i = 0; i <= numHeight; i++ )
	{
		// ループ用添字更新
		index = i * (numWidth + 1);

		for( int j = 0; j <= numWidth; j++ )
		{
			pVtx[ j + index ].pos.x = basePos.x + offsetPos.x;
			pVtx[ j + index ].pos.y = m_vHeight[i][j];
			pVtx[ j + index ].pos.z = basePos.z + offsetPos.z;
			pVtx[ j + index ].tex	= D3DXVECTOR2( 0.0f + offsetTexU, 0.0f + offsetTexV );
			pVtx[ j + index ].col	= GetColor();

			// X軸シフト
			offsetPos.x += width;
			// U値シフト
			offsetTexU += 1.0f;
		}
		// X軸クリア
		offsetPos.x = 0.0f;
		// Z軸シフト
		offsetPos.z -= height;
		// U値クリア
		offsetTexU = 0.0f;
		// V値シフト
		offsetTexV += 1.0f;
	}

	// 法線の決定
	for( int i = 1; i <= numHeight; i++ )
	{
		// ループ用添字更新
		index = i * (numWidth + 1);

		for( int j = 1; j <= numWidth; j++ )
		{
			// ベクトルX求める(終点から始点を引く)
			D3DXVECTOR3 directionX = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
			directionX = pVtx[ j + index + 1 ].pos - pVtx[ j + index - 1 ].pos;
			
			//// ベクトルZ求める(終点から始点を引く)
			D3DXVECTOR3 directionZ = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
			directionZ =  pVtx[ j + index - numWidth ].pos - pVtx[ j + index + numWidth ].pos;			

			// ｙとｘに対する法線ベクトルnormalX
			D3DXVECTOR3 normalX = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
			normalX.x = -directionX.y;
			normalX.y = directionX.x;
			normalX.z = 0.0f;

			// ｙとｚに対する法線ベクトルnormalZ
			D3DXVECTOR3 normalZ = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
			normalZ.x = 0.0f;
			normalZ.y = directionZ.z;
			normalZ.z = -directionZ.y;

			// normalXとnormalZの中間値
			D3DXVECTOR3 normal = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
			normal = normalX + normalZ;
			D3DXVec3Normalize( &normal, &normal );
			pVtx[ j + index ].nor = normal;
		}
	}

	// 仮想メモリをアンロック
	pVtxBuffer->Unlock();

	// シェーダーの値セット
	CCameraManager *pCameraManager = CSceneManager::GetCameraManager();
	CCamera *pCamera = pCameraManager->GetCamera(MAIN_CAMERA);
	CLightManager *pLightManager = CSceneManager::GetLightManager();
	CLight *pLight = pLightManager->GetLight(MAIN_LIGHT);
	CSMeshField *pShader = dynamic_cast<CSMeshField*>(GetShader());
	D3DXVECTOR3 dir = pLight->GetVecDir();
	pShader->SetParam(pCamera->GetViewMtx(), pCamera->GetProjMtx(), 5, 6, dir, pCamera->GetPosition());
	
	return;
}

//=================================================================================
//	ワールド行列計算・デバイスにセット
//=================================================================================
void CMeshField::SetMatrix(void)
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

	// デバイスにワールド変換行列を指定
	GetShader()->SetMatrix(mtxWorld);
}

//=================================================================================
//	メッシュフィールド生成
//=================================================================================
CMeshField *CMeshField::Create( D3DXVECTOR3 pos, D3DXVECTOR3 rot, float width, float  height,int numWidth, int numHeight, std::vector<std::vector<float>> vHeight)
{
	CMeshField *pMeshField;

	pMeshField = new CMeshField( 0 );

	// 座標設定
	pMeshField->SetPosition( pos );

	// 回転角度設定
	pMeshField->SetRotation( rot );

	// ポリゴンの横サイズ設定
	pMeshField->SetWidth( width );
	
	// ポリゴンの縦サイズ設定
	pMeshField->SetHeight( height );
	
	// 横ブロック数設定
	pMeshField->SetNumWidth( numWidth );													
	
	// 縦ブロック数設定
	pMeshField->SetNumHeight( numHeight );

	// 凹凸情報設定
	pMeshField->m_vHeight = vHeight;

	// オブジェクトタイプ設定
	pMeshField->SetType( SCENE_TYPE_FIELD );

	// 縮退ポリゴン含めた総ポリゴン数
	pMeshField->SetNumPolygon( (( numWidth + 2 ) * ( numHeight -1 ) + numWidth ) * 2 );

	// 色設定
	pMeshField->SetColor( D3DXCOLOR( 0.9f, 0.85f, 0.85f, 1.0f ) );
	
	// テクスチャ読み込み
	CTextureManager::Load("data/TEXTURE/sand.jpg");
	CTextureManager::Load("data/TEXTURE/rock_surface.jpg");
	pMeshField->SetTexture("data/TEXTURE/sand.jpg");

	// シェーダーのセット
	CShaderManager *pShaderManager = CSceneManager::GetShaderManager();
	CSMeshField *pShader = dynamic_cast<CSMeshField*>(pShaderManager->GetData(SHADER_MESHFIELD));
	pMeshField->SetShader(pShader);

	pMeshField->Init();

	return pMeshField;
}

//=================================================================================
//	Pが乗っている凹凸の高さ取得
//=================================================================================
float CMeshField::GetPosHeight( D3DXVECTOR3 position )
{
	VERTEX_3D *pVtx;
	LPDIRECT3DVERTEXBUFFER9 pVtxBuffer = GetVertexBuffer();
	int numWidth = GetNumWidth();
	int numHeight = GetNumHeight();

	// 一行の頂点数
	int numLineVertex = numWidth + 1;

	// ポリゴン3頂点用
	D3DXVECTOR3 p0 = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	D3DXVECTOR3 p1 = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	D3DXVECTOR3 p2 = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );

	// 頂点→頂点
	D3DXVECTOR3 v01 = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	D3DXVECTOR3 v12 = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	D3DXVECTOR3 v20 = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );

	// 頂点→Player
	D3DXVECTOR3 v0p = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	D3DXVECTOR3 v1p = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	D3DXVECTOR3 v2p = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );

	// 法線ベクトル用
	D3DXVECTOR3 vc0 = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	D3DXVECTOR3 vc1 = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	D3DXVECTOR3 vc2 = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );

	pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	for( int z = 0; z <= numHeight; z++  )
	{
		for( int x = 0; x < numWidth; x++  )
		{
			/*=====================================
						  1ポリゴン
			=====================================*/
			// ポリゴン3頂点を求める
			p0 = pVtx[ numLineVertex * ( z + 1 ) + x ].pos;
			p1 = pVtx[ numLineVertex * z + x ].pos;
			p2 = pVtx[ numLineVertex * ( z + 1 ) + ( x + 1 ) ].pos;

			// vc0求める
			v01 = p1 - p0;
			v0p = position - p0;
			D3DXVec3Cross( &vc0, &v01, &v0p );
			if( vc0.y >= 0.0f )
			{
				// vc1求める
				v12 = p2 - p1;
				v1p = position - p1;
				D3DXVec3Cross( &vc1, &v12, &v1p );
				if( vc1.y >= 0.0f )
				{
					// vc2求める
					v20 = p0 - p2;
					v2p = position - p2;
					D3DXVec3Cross( &vc2, &v20, &v2p );
					if( vc2.y >= 0.0f )
					{
						// 高さ計算
						D3DXVECTOR3 N = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
						D3DXVECTOR3 v01 = p1 - p0;
						D3DXVECTOR3 v02 = p2 - p0;
						D3DXVec3Cross( &N, &v01, &v02 );
						D3DXVec3Normalize( &N, &N );
						position.y = p0.y - ( (position.x - p0.x) * N.x + (position.z - p0.z) * N.z) / N.y;
						pVtxBuffer->Unlock();
						return position.y;
					}
				}
			}
			/*=====================================
						  2ポリゴン
			=====================================*/
			// ポリゴン3頂点を求める
			p0 = pVtx[ numLineVertex * z + (x + 1) ].pos;
			p1 = pVtx[ numLineVertex * ( z + 1 ) + ( x + 1 ) ].pos;
			p2 = pVtx[ numLineVertex * z + x ].pos;

			// vc0求める
			v01 = p1 - p0;
			v0p = position - p0;
			D3DXVec3Cross( &vc0, &v01, &v0p );
			if( vc0.y >= 0.0f )
			{
				// vc1求める
				v12 = p2 - p1;
				v1p = position - p1;
				D3DXVec3Cross( &vc1, &v12, &v1p );
				if( vc1.y >= 0.0f )
				{
					// vc2求める
					v20 = p0 - p2;
					v2p = position - p2;
					D3DXVec3Cross( &vc2, &v20, &v2p );
					if( vc2.y >= 0.0f )
					{
						// 高さ計算
						D3DXVECTOR3 N = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
						D3DXVECTOR3 v01 = p1 - p0;
						D3DXVECTOR3 v02 = p2 - p0;
						D3DXVec3Cross( &N, &v01, &v02 );
						D3DXVec3Normalize( &N, &N );
						position.y = p0.y - ( (position.x - p0.x) * N.x + (position.z - p0.z) * N.z) / N.y;
						pVtxBuffer->Unlock();
						return position.y;
					}
				}
			}
		}
	}

	pVtxBuffer->Unlock();

	return 0.0f;
}