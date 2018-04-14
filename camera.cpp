//=================================================================================
//	プログラム名	：	
//	処理			：	カメラ処理[camera.cpp]
//	メイン処理		：	main.cpp
//	作成者			：	moriya gaku
//=================================================================================

//=================================================================================
//	インクルードファイル
//=================================================================================
#include "camera.h"
#include "input.h"
#include "manager.h"
#include "renderer.h"
#include "player.h"
#include "debug.h"
#include "debugObj.h"
#include "sceneManager.h"
#include "reticle.h"

//=================================================================================
//	コンストラクタ
//=================================================================================
CCamera::CCamera()
{
	m_Position		= D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	m_LookAt		= D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	m_LookAtTarget	= D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	m_VecDir		= D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	m_Distance		= 0.0f;
	m_fYRot			= 0.0f;
	m_fXRot			= 0.0f;
	m_RotTarget		= 0.0f;
	m_InterpolationEnable = false;
	m_bInterpolationDuring = false;
	D3DXMatrixIdentity( &m_MtxView );
	D3DXMatrixIdentity( &m_MtxProj );
	D3DXMatrixIdentity( &m_MtxViewPort );
	D3DXMatrixIdentity( &m_MtxRot );
	D3DXMatrixIdentity( &m_MtxPos );
}

//=================================================================================
//	デストラクタ
//=================================================================================
CCamera::~CCamera()
{
}

//=================================================================================
//	初期化関数
//=================================================================================
void CCamera::Init( D3DXVECTOR3 pos, float distance, float middleAtOffSetY )
{
	m_Position			= pos;
	m_Distance			= distance;
	m_MiddleAtOffSetY	= middleAtOffSetY; 

	return;
}

//=================================================================================
//	終了関数
//=================================================================================
void CCamera::Uninit( void )
{
	return;
}

//=================================================================================
//	更新関数
//=================================================================================
void CCamera::Update( void )
{
	// CTitle
	if (CTitle* pClassTitle = dynamic_cast<CTitle*>(CSceneManager::GetScene()))
	{
		m_Position = D3DXVECTOR3(0.0f, 50.0f, -135.0f);
		m_LookAt = D3DXVECTOR3(0.0, 0.0, m_Distance);
	}

	// CGame
	if (CGame* pClassGame = dynamic_cast<CGame*>(CSceneManager::GetScene()))
	{
		// オブジェ取得
		CScene* pScene = CScene::GetList(DEFAULT_PRIORITY);

		// スティック入力から回転行列を作成
		CreateRotMatrix();

		// プレイヤー位置から移動行列を作成
		CreatePosMatrix();

		// ＜カメラ座標＞オフセット設定
		D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, -m_Distance);

		// ＜カメラ座標＞回転
		D3DXVec3TransformCoord(&m_Position, &pos, &m_MtxRot);

		// ＜カメラ座標＞移動
		D3DXVec3TransformCoord(&m_Position, &m_Position, &m_MtxPos);

		// ＜注視点＞オフセット設定
		D3DXVECTOR3 at = D3DXVECTOR3(0.0f, 0.0f, m_Distance + 50.0f);

		// ＜注視点＞回転
		D3DXVec3TransformCoord(&m_LookAt, &at, &m_MtxRot);

		// ＜注視点＞移動
		D3DXMATRIX mtxPos = m_MtxPos;
		mtxPos.m[3][1] -= m_MiddleAtOffSetY; // Y成分減らす
		D3DXVec3TransformCoord(&m_LookAt, &m_LookAt, &mtxPos);

		// 注視点用オブジェを移動
		CSceneManager::GetDebugObj()->GetData("cameraLookAt")->SetPosition(m_LookAt);

		// カメラの向きを計算
		// プレイヤー取得
		CGame *pGame = dynamic_cast<CGame*>(CSceneManager::GetScene());
		CPlayer *pPlayer = pGame->GetPlayer();

		m_VecDir = pPlayer->GetPosition() - m_Position;

		//--------キー入力で Distance, middleAtOffSetY 変更---------
#ifdef _DEBUG
		if (CManager::GetInputKeyboard()->GetKeyTrigger(DIK_U))
		{
			m_MiddleAtOffSetY += 1.0f;
		}
		if (CManager::GetInputKeyboard()->GetKeyTrigger(DIK_J))
		{
			m_MiddleAtOffSetY -= 1.0f;
		}
		if (CManager::GetInputKeyboard()->GetKeyTrigger(DIK_I))
		{
			m_Distance += 1.0f;
		}
		if (CManager::GetInputKeyboard()->GetKeyTrigger(DIK_K))
		{
			m_Distance -= 1.0f;
		}

		// パラメータ表示
		ShowParam();
#endif
	}

	// CResult
	if (CResult* pClassResult = dynamic_cast<CResult*>(CSceneManager::GetScene()))
	{
		m_Position = D3DXVECTOR3(0.0f, 50.0f, -135.0f);
		m_LookAt = D3DXVECTOR3(0.0, 0.0, m_Distance);
	}
}

//=================================================================================
//	回転行列を作成
//=================================================================================
void CCamera::CreateRotMatrix( void )
{
	D3DXMATRIX mtxRotY;
	D3DXMATRIX mtxRotX;

	// 初期化
	D3DXMatrixIdentity(&mtxRotX);
	D3DXMatrixIdentity(&mtxRotY);

	// パッド入力取得
	CInputGamePad *pad = CManager::GetInputGamePad();

	if (CManager::GetInputGamePad()->CheckInputR())
	{// 右スティック入力がある
		m_fXRot += (float)pad->GetDInput().lRy * 0.00005f;
		m_fYRot += (float)pad->GetDInput().lRx * 0.00005f;
	}

	if (pad->CheckInputL() && !CReticle::GetLockOnMode())
	{// 左スティック入力があるとき かつ ロックオンモードじゃないとき

		// カメラ補間する状態へ
		m_InterpolationEnable = true;
	}
	if (!pad->CheckInputL() && m_InterpolationEnable)
	{// 左スティック入力がない かつ 補間してない かつ　ロックオンモードではない

		// ２ベクトル間の回転量計算
		CGame *pGame = dynamic_cast<CGame*>(CSceneManager::GetScene());
		CPlayer *pPlayer = pGame->GetPlayer();

		D3DXVECTOR3 vec1 = pPlayer->GetVecDir();
		D3DXVECTOR3 vec2 = m_VecDir;
		vec1.y = 0.0f; // 平面ベクトルで計算
		vec2.y = 0.0f;
		m_RotTarget = CalcVecAngle(vec1, vec2);

		// 次フレ―ムまで以上処理は行わない
		m_InterpolationEnable = false;

		// 補間中フラグオン(開始)
		m_bInterpolationDuring = true;
	}


	// キーボード入力があったら
	float move = 0.02f;
	// 上向く
	if (CManager::GetInputKeyboard()->GetKeyPress(DIK_UP)) {
		m_fXRot -= move;
	}
	// 下向く
	if (CManager::GetInputKeyboard()->GetKeyPress(DIK_DOWN)) {
		m_fXRot += move;
	}
	// 右向く
	if (CManager::GetInputKeyboard()->GetKeyPress(DIK_RIGHT)) {
		m_fYRot += move;
	}
	// 左向く
	if (CManager::GetInputKeyboard()->GetKeyPress(DIK_LEFT)) {
		m_fYRot -= move;
	}

	// ロックオン対象へカメラが回転して補間
	if (CReticle::GetLockOnMode()) {

		// 補間中フラグオン(開始)
		m_bInterpolationDuring = true;

		// ２ベクトル間の回転量計算：Y回転
		D3DXVECTOR3 vec1 = CReticle::GetVecDir();
		D3DXVECTOR3 vec2 = m_VecDir;
		vec1.y = 0.0f; // 平面ベクトルで計算
		vec2.y = 0.0f;
		m_RotTarget = CalcVecAngle(vec1, vec2);

	}

	// 回転量補間
	if (m_bInterpolationDuring)
	{
		// Y回転量
		m_fYRot += m_RotTarget * 0.1f;
		m_RotTarget -= m_RotTarget * 0.1f;

		if (fabs(m_RotTarget) < 0.05f)
		{
			// 補間中フラグオフ(終了)
			m_bInterpolationDuring = false;
		}
	}

	// 回転行列作成
	D3DXMatrixRotationY(&mtxRotY, m_fYRot);
	D3DXMatrixRotationX(&mtxRotX, m_fXRot);
	// 合成
	m_MtxRot = mtxRotX * mtxRotY;
}

//=================================================================================
//	移動行列を作成
//=================================================================================
void CCamera::CreatePosMatrix( void )
{
	// プレイヤー取得
	CGame *pGame = dynamic_cast<CGame*>(CSceneManager::GetScene());
	CPlayer *pPlayer = pGame->GetPlayer();

	// プレイヤー位置を元に移動行列を作成
	D3DXMatrixTranslation(&m_MtxPos, pPlayer->GetPosition().x, pPlayer->GetPosition().y + m_MiddleAtOffSetY, pPlayer->GetPosition().z);

	m_MtxPos = m_MtxPos;
}

//=================================================================================
//	デバッグ表示
//=================================================================================
void CCamera::ShowParam( void )
{
	// デバッグ情報
	char str[256];
	CDebug::AddText( "CAMERA _/_/_/_/_/_/_/\n" );
	sprintf_s(str, "m_Position( %f, %f, %f )\n", m_Position.x, m_Position.y, m_Position.z );
	CDebug::AddText( str );
	sprintf_s(str, "m_LookAt( %f, %f, %f )\n", m_LookAt.x, m_LookAt.y, m_LookAt.z );
	CDebug::AddText( str );
	sprintf_s(str, "m_TargetAt( %f, %f, %f )\n", CReticle::Get3DPosition().x, CReticle::Get3DPosition().y, CReticle::Get3DPosition().z );
	CDebug::AddText( str );
	sprintf_s(str, "m_VecDir( %f, %f, %f )\n", m_VecDir.x, m_VecDir.y, m_VecDir.z );
	CDebug::AddText( str );
	sprintf_s(str, "STIC lX,lY( %f, %f )\n", (float)CManager::GetInputGamePad()->GetDInput().lX, (float)CManager::GetInputGamePad()->GetDInput().lY );
	CDebug::AddText( str );
	sprintf_s(str, "m_bInterpolationDuring : %s\n", m_bInterpolationDuring ? "TRUE" : "FALSE" );
	CDebug::AddText( str );
	sprintf_s(str, "m_RotTarget : %f\n", m_RotTarget );
	CDebug::AddText( str );
	CDebug::AddText( "\n" );
}

//=================================================================================
//	プロジェクション・ビューマトリクス設定
//=================================================================================
void CCamera::Set( void )
{
	// Direct3Dデバイス
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	
	if( pDevice != NULL )
	{
		// ビューマトリクス作成
		D3DXMatrixLookAtLH( &m_MtxView, &m_Position, &m_LookAt, &D3DXVECTOR3(0.0f, 1.0f, 0.0f) );
		
		// デバイスにビュー変換行列を指定
		pDevice -> SetTransform( D3DTS_VIEW, &m_MtxView );

		// プロジェクションマトリクスの作成
		D3DXMatrixPerspectiveFovLH( &m_MtxProj,
			D3DX_PI / 3.0f,
			( float )SCREEN_WIDTH / SCREEN_HEIGHT,
			CAMERA_AT_NEAR,
			CAMERA_AT_FAR );

		// プロジェクションマトリクスを指定
		pDevice -> SetTransform( D3DTS_PROJECTION, &m_MtxProj );

		// ビューポート行列の作成
		m_MtxViewPort._11 = SCREEN_WIDTH / 2.0f;
		m_MtxViewPort._12 = 0.0f;
		m_MtxViewPort._13 = 0.0f;
		m_MtxViewPort._14 = 0.0f;

		m_MtxViewPort._21 = 0.0f;
		m_MtxViewPort._22 = -SCREEN_HEIGHT / 2.0f;
		m_MtxViewPort._23 = 0.0f;
		m_MtxViewPort._24 = 0.0f;

		m_MtxViewPort._31 = 0.0f;
		m_MtxViewPort._32 = 0.0f;
		m_MtxViewPort._33 = 1.0f;
		m_MtxViewPort._34 = 0.0f;

		m_MtxViewPort._41 = SCREEN_WIDTH / 2.0f;
		m_MtxViewPort._42 = SCREEN_HEIGHT / 2.0f;
		m_MtxViewPort._43 = 0.0f;
		m_MtxViewPort._44 = 1.0f;
	}
	else
	{
		// デバイスが取得できなかった場合
		MessageBox( NULL, "デバイスの取得に失敗しました", "GET_DEVICE_ERROR", MB_OK );
		return;
	}

	return;

}