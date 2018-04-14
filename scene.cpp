//=================================================================================
//	プログラム名	：	
//	処理			：	全描画オブジェクト処理[scene.cpp]
//	メイン処理		：	main.cpp
//	作成者			：	moriya gaku
//=================================================================================

//=================================================================================
//	インクルードファイル
//=================================================================================
#include "scene.h"
#include "manager.h"
#include "renderer.h"
#include "shader.h"

//=================================================================================
//　静的メンバ変数を宣言
//=================================================================================
CScene *CScene::m_Top[ PRIORITY_MAX ] = {NULL};

//=================================================================================
//	コンストラクタ
//=================================================================================
CScene::CScene( int priority )
{
	// リスト構造の初期化
	if( m_Top[ priority ] == NULL )
	{
		m_Top[ priority ] = this;
	}
	else
	{
		CScene *scene = m_Top[ priority ];
		while( scene->m_Next != NULL )
		{
			scene = scene->m_Next;
		}
		scene->m_Next = this;
	}

	m_Next = NULL;
	m_Position = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	m_Rotation = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	m_Type = SCENE_TYPE_NONE;
	m_Delete = false;
	m_pShader = nullptr;
}

//=================================================================================
// 全描画オブジェクトの更新処理
//=================================================================================
void CScene::UpdateAll(int nOnlyUpdatePriority)
{
	for( int i = 0; i < PRIORITY_MAX; i++ )
	{
		// 指定された優先度のみ更新したいとき
		if (i != nOnlyUpdatePriority && nOnlyUpdatePriority != -1)continue;

		CScene *scene = m_Top[ i ];
		CScene *scenePrev = m_Top[ i ];
		CScene *sceneNext = NULL;

		while( scene != NULL )
		{
			// 削除依頼されてないモノは更新
			if(!scene->m_Delete)scene->Update();
			scene = scene->m_Next;
		}

		scene = m_Top[ i ];
		while( scene != NULL )
		{
			sceneNext = scene->m_Next;

			if( scene->m_Delete == true )
			{
				if( scene == m_Top[ i ] )
				{
					// 先頭を破棄する準備
					m_Top[ i ] = scene->m_Next;
				}
				else
				{
					// リスト構造のつなぎ変え
					scenePrev->m_Next = scene->m_Next;
				}

				// 破棄
				delete scene;
				scene = NULL;
	
			}
			else
			{
				scenePrev = scene;
			}

			// 進める
			scene = sceneNext;
		}
	}
}

//=================================================================================
// 全描画オブジェクトの描画処理
//=================================================================================
void CScene::DrawAll(int nOnlyDrawPriority, int nExcludePriority)
{
	for( int i = 0; i < PRIORITY_MAX; i++ )
	{
		// 指定された優先度のみ描画させたいとき
		if (i != nOnlyDrawPriority && nOnlyDrawPriority != -1)continue;

		CScene *scene = m_Top[ i ];

		// 指定された優先度は描画させたくないとき
		if (i == nExcludePriority && nExcludePriority != -1)scene = NULL;
		
		while( scene != NULL )
		{
			CShader *pShader = scene->GetShader();
			if(pShader)
			{
				scene->GetShader()->Begin();	// シェーダー開始

				scene->GetShader()->SendData();	// データ送信

				scene->Draw();					// 描画

				scene->GetShader()->End();		// シェーダー終了
			}
			else
			{
				scene->Draw();
			}
			
			scene = scene->m_Next;			// 次ポインタへ
		}
	}
}

//=================================================================================
// 派生クラスが自分自身のscene解放
//=================================================================================
void CScene::Release( void )
{
	// 削除フラグオン
	m_Delete = true;

}

//=================================================================================
// 全ての描画オブジェクトの解放
//=================================================================================
void CScene::ReleaseAll( void )
{
	for( int i = 0; i < PRIORITY_MAX; i++ )
	{
		CScene *scene = m_Top[ i ];
		CScene *bufScene = NULL;

		while( scene != NULL )
		{
			bufScene = scene->m_Next;
			scene->Uninit();
			delete scene;
			scene = bufScene;
		}

		m_Top[ i ] = NULL;
	}
}