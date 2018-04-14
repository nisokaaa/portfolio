//=================================================================================
//	プログラム名	：	
//	処理			：	敵処理[enemy.cpp]
//	メイン処理		：	main.cpp
//	作成者			：	moriya gaku
//=================================================================================

//=================================================================================
//	インクルードファイル
//=================================================================================
#include "manager.h"
#include "renderer.h"
#include "meshField.h"
#include "cameraManager.h"
#include "enemy.h"
#include "player.h"
#include "scene.h"
#include "scene2D.h"
#include "input.h"
#include "fade.h"
#include "sceneManager.h"
#include "gauge.h"
#include "reticle.h"

int CEnemy::m_Count = 0;

//=================================================================================
// 初期化処理
//=================================================================================
HRESULT CEnemy::Init(void)
{
	CSceneX::Init();
	
	m_ColLength = 4.0f;

	// ＨＰゲージ
	/*m_pHpGauge = CGauge::Create("data/TEXTURE/gauge3.png", "data/TEXTURE/gauge_back3.png", D3DXVECTOR2(0.0f, 0.0f), D3DXVECTOR2(70.0f, 4.0f), m_Hp, m_Hp, TRUE, 5);
	m_pHpGauge->SetType( CScene::SCENE_TYPE_HP_GAUGE );*/

	return S_OK;
}

//=================================================================================
// 終了処理
//=================================================================================
void CEnemy::Uninit(void)
{
	// モデル
	CSceneX::Uninit();

	CEnemy::SetCount( CEnemy::GetCount() - 1 );
}

//=================================================================================
// 更新処理
//=================================================================================
void CEnemy::Update(void)
{
	CSceneX::Update();

	// カメラ取得
	CCameraManager *pCameraManager = CSceneManager::GetCameraManager();
	CCamera *pCamera = pCameraManager->GetCamera(MAIN_CAMERA);

	D3DXVECTOR3 pos = GetPosition();
	
	static float speedY = 0.1f;
	static float speedZ = 0.1f;

	// 状態判断
	switch( m_State )
	{
	case NEUTRAL:
		// 生成５秒後
		if(m_Time % (60 * 1) == 0 )
		{
			// 攻撃態勢
			m_State = CHASE;
		}
		break;

	case CHASE:
		// 追跡５秒後
		if(m_Time % (60 * 10) == 0 )
		{
			// 攻撃態勢
			//m_State = NEUTRAL;
		}

		// プレイヤーへ直進
		//pos += m_Speed * m_VecDir;
		if( pos.x > 100.0f || pos.x < -100.0f ) m_Speed *= -1.0f;
		if( pos.y > 100.0f || pos.y < 0.0f ) speedY *= -1.0f;
		if( pos.z > 180.0f || pos.z < 80.0f ) speedZ *= -1.0f;
		
		pos.x += m_Speed;
		pos.y += speedY;
		pos.z += speedZ;

		break;

	case ATTACK:
		// 攻撃５秒後
		if((m_Time % (60 * 1)) == 0 )
		{
			// 追跡
			m_State = CHASE;
			break;
		}
		break;

	case DETH:
		// 削除依頼
		Uninit();
		return;

	default:
		break;
	}

	// プレイヤーを取得
	CPlayer *pPlayer = NULL;
	CScene *pScene = CScene::GetList( DEFAULT_PRIORITY );
	while( pScene != NULL )
	{
		CScene::SCENE_TYPE type = pScene->GetType();
		if( type == CScene::SCENE_TYPE_PLAYER )
		{
			pPlayer = dynamic_cast<CPlayer *>(pScene);
			break;
		}
		pScene = pScene->GetNextScene();
	}

	// 敵→プレイヤー　ベクトル設定
	m_VecDir = pPlayer->GetPosition() - pos;

	// ゲージ設定(3次元座標→2次元座
	float offsetY = 3.0f;
	D3DXMATRIX mtxWorld;
	D3DXMatrixIdentity( &mtxWorld );
	D3DXMATRIX mtxView = pCamera->GetViewMtx();
	D3DXMATRIX mtxProj = pCamera->GetProjMtx();
	D3DXMATRIX mtxViewPort = pCamera->GetViewPortMtx();
	D3DXVECTOR4 vecOut = D3DXVECTOR4( 0.0f, 0.0f, 0.0f, 0.0f );
	D3DXVECTOR4 vecPos = D3DXVECTOR4( pos.x, pos.y + offsetY, pos.z, 1.0f );
	Vec4Transform( &vecOut, &vecPos, &mtxWorld );
	Vec4Transform( &vecOut, &vecOut, &mtxView );
	Vec4Transform( &vecOut, &vecOut, &mtxProj );
	Vec4Transform( &vecOut, &vecOut, &mtxViewPort );
	vecOut.x = vecOut.x / vecOut.w;
	vecOut.y = vecOut.y / vecOut.w;
	m_pHpGauge->SetPosition( vecOut.x, vecOut.y, vecOut.z );
	
	// HPをゲージに反映
	m_pHpGauge->SetVal( m_Hp );

	// 座標更新
	SetPosition( pos );

	// 生成２０秒後 死亡
	//if(m_Time == 60 * 60 ) m_State = STATE::DETH;

	// 体力が０ 死亡
	if( m_Hp < 0.0f || m_Hp == 0.0f){
		m_State = DETH;
	}

	// プレイヤーとの当たり判定true 死亡
	//if( CheckColSphere( (D3DXVECTOR3&)pPlayer->GetPosition(), (D3DXVECTOR3&)GetPosition(), 5.0f, 5.0f )){
	//	m_State = DETH;
	//	// HP(val)変更
	//	pPlayer->SetHp( pPlayer->GetHp() - 10.0f );
	//}

	// 経過時間をすすめる
	m_Time += 1;
}

//=================================================================================
// 描画処理
//=================================================================================
void CEnemy::Draw(void)
{
	CSceneX::Draw();
}