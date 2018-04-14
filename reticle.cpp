//=================================================================================
//	プログラム名	：	
//	処理			：	標的処理[reticle.cpp]
//	メイン処理		：	main.cpp
//	作成者			：	moriya gaku
//=================================================================================

//=================================================================================
//	インクルードファイル
//=================================================================================
#include "reticle.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "debug.h"
#include "sceneManager.h"
#include "scene.h"
#include "sceneX.h"
#include "player.h"
#include "enemy.h"
#include "cameraManager.h"

// ロックオンモード
bool CReticle::m_bLockOn = false;
float CReticle::m_ViewAngle = 0.0f;
D3DXVECTOR3 CReticle::m_VecDir = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
D3DXVECTOR3 CReticle::m_3DPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

//=================================================================================
// 初期化処理
//=================================================================================
HRESULT CReticle::Init(void)
{
	CScene2D::Init();

	return S_OK;
}

//=================================================================================
// 終了処理
//=================================================================================
void CReticle::Uninit(void)
{
	CScene2D::Uninit();
}

//=================================================================================
// 更新処理
//=================================================================================
void CReticle::Update(void)
{
	CScene2D::Update();

	// プレイヤー取得
	CGame *pGame = dynamic_cast<CGame*>(CSceneManager::GetScene());
	CPlayer *pPlayer = pGame->GetPlayer();

	// カメラ取得
	CCameraManager *pCameraManager = CSceneManager::GetCameraManager();
	CCamera *pCamera = pCameraManager->GetCamera(MAIN_CAMERA);

	CScene *pScene = NULL;

	// L1ボタンを押したとき,視野内の敵をロックオンする
	if( CManager::GetInputGamePad()->GetTrigger( DIJOFS_BUTTON4 ))
	{
		pScene = CScene::GetList( DEFAULT_PRIORITY );
		while( pScene != NULL )
		{
			CScene::SCENE_TYPE type = pScene->GetType();
			if( type == CScene::SCENE_TYPE_ENEMY )
			{
				CEnemy *pEnemy = dynamic_cast<CEnemy *>(pScene); 

				// プレイヤー周囲にいる敵を探す
				if( CheckColSphere( (D3DXVECTOR3&)pEnemy->GetPosition(), 
					(D3DXVECTOR3&)pPlayer->GetPosition(), 5.0f, 500.0f ) )
				{
					// 視野内に敵がいないか判定
					// P→レチクルベクトル と P→敵ベクトル のなす角を計算
					float rad = -(fabs( CalcVecAngle( m_VecDir, (D3DXVECTOR3 &)pEnemy->GetVecDir() )) - D3DX_PI);
					float angle = 0.0f;
					if( rad != 0.0f ){
						angle = rad / (D3DX_PI / 180.0f);
					}
					// 視野角内か判定
					if( angle < m_ViewAngle )
					{
						if( !pEnemy->GetLockOnFlg() )
						{
							// ロックオン
							pEnemy->SetLockOnFlg( true );
							// ロックオンモードへ
							m_bLockOn = true;
						}
						else
						{
							pEnemy->SetLockOnFlg( false );
						}
					}
				}
			}
			pScene = pScene->GetNextScene();
		}
	}

	// L2ボタンを押したとき
	if( CManager::GetInputGamePad()->GetTrigger( DIJOFS_BUTTON7 ) )
	{
		// ロックオンモード終了
		m_bLockOn = false;
		pScene = CScene::GetList( DEFAULT_PRIORITY );
		while( pScene != NULL )
		{
			CScene::SCENE_TYPE type = pScene->GetType();
			if( type == CScene::SCENE_TYPE_ENEMY )
			{
				CEnemy *pEnemy = dynamic_cast<CEnemy *>(pScene);
				// 全敵のロックオン済みフラグを切る
				pEnemy->SetLockOnFlg( false );
			}
			pScene = pScene->GetNextScene();
		}
	}

	// カメラ注視点座標をセット(非ロックオンモード)
	m_3DPosition = pCamera->GetLookAt();
	m_3DPosition.y += 20.0f;

	// ロックオン中の敵座標をもらう(ロックオンモード)
	if( m_bLockOn )
	{
		// ロックオン中の敵が見つからなかったら、
		// ロックオンモードを終了する
		m_bLockOn = false;
		pScene = CScene::GetList( DEFAULT_PRIORITY );
		while( pScene != NULL )
		{
			CScene::SCENE_TYPE type = pScene->GetType();
			if( type == CScene::SCENE_TYPE_ENEMY )
			{
				CEnemy *pEnemy = dynamic_cast<CEnemy *>(pScene);
		
				if( pEnemy->GetLockOnFlg() )
				{// ロックオンされてる敵だったら
					m_3DPosition = pEnemy->GetPosition();
					m_bLockOn = true;
				}
			}
			pScene = pScene->GetNextScene();
		}
	}

	// 座標変換 3次元→2次元
	D3DXMATRIX mtxWorld;
	D3DXMatrixIdentity( &mtxWorld );
	D3DXMATRIX mtxView = pCamera->GetViewMtx();
	D3DXMATRIX mtxProj = pCamera->GetProjMtx();
	D3DXMATRIX mtxViewPort = pCamera->GetViewPortMtx();
	D3DXVECTOR4 vecOut = D3DXVECTOR4( 0.0f, 0.0f, 0.0f, 0.0f );
	D3DXVECTOR4 vecPos = D3DXVECTOR4( m_3DPosition.x, m_3DPosition.y, m_3DPosition.z, 1.0f );
	Vec4Transform( &vecOut, &vecPos, &mtxWorld );
	Vec4Transform( &vecOut, &vecOut, &mtxView );
	Vec4Transform( &vecOut, &vecOut, &mtxProj );
	Vec4Transform( &vecOut, &vecOut, &mtxViewPort );
	vecOut.x = vecOut.x / vecOut.w;
	vecOut.y = vecOut.y / vecOut.w;
	SetPosition( D3DXVECTOR3( vecOut.x - (RETICLE_WIDTH * 0.5f), vecOut.y - (RETICLE_HEIGHT * 0.5f), vecOut.z ) );

	// 向き計算
	CalcPositionToVector( &m_VecDir, m_3DPosition, pPlayer->GetPosition() );
	D3DXVec3Normalize( &m_VecDir, &m_VecDir );

#ifdef _DEBUG
	ShowParam();
#endif
}

//=================================================================================
// 描画処理
//=================================================================================
void CReticle::Draw(void)
{
	//CScene2D::Draw();
}

//=================================================================================
// デバッグ表示
//=================================================================================
#ifdef _DEBUG
void CReticle::ShowParam( void )
{
	//-------------------------デバッグ情報-------------------------
	char str[256];
	CDebug::AddText( "RETICLE _/_/_/_/_/_/_/\n" );
	sprintf_s(str, "m_Position( %f, %f, %f )\n", GetPosition().x, GetPosition().y, GetPosition().z );
	CDebug::AddText( str );
	sprintf_s(str, "m_VecDir( %f, %f, %f )\n", m_VecDir.x, m_VecDir.y, m_VecDir.z );
	CDebug::AddText( str );
	sprintf_s(str, "LOCKON_MODE : %s\n", m_bLockOn ? "TRUE" : "FALSE" );
	CDebug::AddText( str );

}
#endif