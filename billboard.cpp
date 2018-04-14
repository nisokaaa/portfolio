//=================================================================================
//	プログラム名	：	
//	処理			：	ビルボード処理[billboard.cpp]
//	メイン処理		：	main.cpp
//	作成者			：	moriya gaku
//=================================================================================

//=================================================================================
//	インクルードファイル
//=================================================================================
#include "main.h"
#include "scene3D.h"
#include "billboard.h"
#include "manager.h"
#include "renderer.h"
#include "cameraManager.h"
#include "textureManager.h"
#include "sceneManager.h"

//=================================================================================
//	コンストラクタ
//=================================================================================
CBillboard::CBillboard()
{
}

//=================================================================================
//	初期化関数
//=================================================================================
HRESULT CBillboard::Init( void )
{
	CScene3D::Init();

	return S_OK;
}

//=================================================================================
//	終了関数
//=================================================================================
void CBillboard::Uninit( void )
{
	CScene3D::Uninit();
}

//=================================================================================
//	更新関数
//=================================================================================
void CBillboard::Update( void )
{
	CScene3D::Update();
}

//=================================================================================
//	描画関数
//=================================================================================
void CBillboard::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	CCamera *pCamera = CSceneManager::GetCameraManager()->GetCamera(MAIN_CAMERA);
	LPDIRECT3DVERTEXBUFFER9 pVtxBuffer = GetVertexBuffer();

	/*------------------------------------------------------------------------*/
	/*	マトリクス変換
	/*------------------------------------------------------------------------*/
	// 各マトリクスの作成( ワールド・ビュー・ プロジェクション・回転・平行・拡大 )
	D3DXMATRIX mtxWorld, mtxViewInverse;
	D3DXMATRIX mtxScale, mtxRot, mtxTranslate;

	// 行列を単位行列にする(なんでもないただの行列を作成する)
	D3DXMatrixIdentity(&mtxWorld);
	D3DXMatrixIdentity(&mtxViewInverse);

	D3DXMatrixInverse(&mtxViewInverse, NULL, &pCamera->GetViewMtx());	// ビュー行列の逆行列
	mtxViewInverse._41 = mtxViewInverse._42 = mtxViewInverse._43 = 0.0f;	// カメラの平行移動成分は潰す

	// スケールを反映
	D3DXMatrixScaling(&mtxScale, 1.0f, 1.0f, 1.0f);
	D3DXMatrixMultiply(&mtxViewInverse, &mtxViewInverse, &mtxScale);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, D3DXToRadian(GetRotation().y), D3DXToRadian(GetRotation().x), D3DXToRadian(GetRotation().z));
	D3DXMatrixMultiply(&mtxViewInverse, &mtxViewInverse, &mtxRot);

	// 移動を反映
	D3DXVECTOR3 pos = GetPosition();
	D3DXMatrixTranslation(&mtxTranslate, GetPosition().x, GetPosition().y, GetPosition().z);
	D3DXMatrixMultiply(&mtxViewInverse, &mtxViewInverse, &mtxTranslate);

	mtxWorld = mtxViewInverse;

	// デバイスにワールド変換行列を指定
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	if (pDevice != NULL)
	{
		//// ライトをオフにする
		//pDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
		//
		//// 加算合成
		//pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
		//pDevice->SetRenderState( D3DRS_ZENABLE, D3DZB_TRUE );
		//pDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );


		// ストリーム設定
		pDevice->SetStreamSource(0, pVtxBuffer, 0, sizeof(VERTEX_3D));

		// ライティングオフ設定
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

		// フォーマット設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		// テクスチャ貼り付け
		pDevice->SetTexture(0, CTextureManager::GetResource(GetKey()));

		// ポリゴン描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

		// ライティングオフ設定
		pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);


		//// 加算合成をもとに戻す
		//pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
		//pDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
		//
		//// ライトをもとに戻す
		//pDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
	}

}