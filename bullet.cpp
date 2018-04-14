//=================================================================================
//	プログラム名	：	
//	処理			：	弾処理[bullet.cpp]
//	メイン処理		：	main.cpp
//	作成者			：	moriya gaku
//=================================================================================

//=================================================================================
//	インクルードファイル
//=================================================================================
#include "bullet.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "player.h"
#include "enemy.h"
#include "sound.h"
#include "reticle.h"
#include "debug.h"

//=================================================================================
// 初期化処理
//=================================================================================
HRESULT CBullet::Init( void )
{
	// 親クラスの初期化
	CBillboard::Init();

	return S_OK;
}

//=================================================================================
// 終了処理
//=================================================================================
void CBullet::Uninit( void )
{
	// 親クラスの終了処理
	CBillboard::Uninit();

}

//=================================================================================
// 更新処理
//=================================================================================
void CBullet::Update( void )
{
	// 親クラスの更新処理
	CBillboard::Update();

	if( m_Life > 0 )
	{
		
		// 座標移動
		Move();

		// 当たり判定
		CEnemy *pEnemy;
		CScene *pScene = CScene::GetList( DEFAULT_PRIORITY );
		while( pScene != NULL )
		{
			CScene::SCENE_TYPE type = pScene->GetType();
			if( type == CScene::SCENE_TYPE_ENEMY )
			{
				pEnemy = dynamic_cast<CEnemy *>(pScene);
			
				if( CheckColSphere( (D3DXVECTOR3&)pEnemy->GetPosition(), (D3DXVECTOR3&)GetPosition(), 5.0f, 5.0f ) )
				{
					// SE再生
					/*PlaySound( SOUND_LABEL_SE_EXPLOSION );*/
				
					// HPを減らす
					pEnemy->SetHp( pEnemy->GetHp() - m_Attack );

					// 寿命尽きる
					m_Life = 0;

					// 弾の破棄
					Uninit();

					break;
				}

			}
			pScene = pScene->GetNextScene();
		}
		
		// 寿命を縮める
		m_Life -= 1;
	}
	else
	{
		// 当たり判定が行われず、寿命が尽きた場合
		Uninit();
	}

	/*char str[256];
	CDebug::AddText( "BULLET _/_/_/_/_/_/_/\n" );
	sprintf_s(str, "m_Life : %d\n", m_Life );
	CDebug::AddText( str );
	sprintf_s(str, "m_VecDir( %f, %f, %f )\n", m_VecDir.x, m_VecDir.y, m_VecDir.z );
	CDebug::AddText( str );
	sprintf_s(str, "m_Position( %f, %f, %f )\n", GetPosition().x, GetPosition().y, GetPosition().z );
	CDebug::AddText( str );*/

	return;
}

//=================================================================================
// 描画処理
//=================================================================================
void CBullet::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CRenderer::GetDevice();

	if( pDevice != NULL )
	{
		// 加算合成を使用する設定
		pDevice -> SetRenderState( D3DRS_BLENDOP, D3DBLENDOP_ADD );
		pDevice -> SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
		pDevice -> SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );

		// 親クラスの描画処理
 		CBillboard::Draw();

		// 加算合成を使用する設定戻す
		pDevice -> SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );			
		pDevice -> SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
		pDevice -> SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
	}
	else
	{
		// デバイスが取得できなかった場合
		MessageBox( NULL, "デバイスの取得に失敗しました", "GET_DEVICE_ERROR", MB_OK );
		return;
	}

	return;
}