//=================================================================================
//	プログラム名	：	
//	処理			：	ゲーム管理[manager.cpp]
//	メイン処理		：	main.cpp
//	作成者			：	moriya gaku
//=================================================================================

//=================================================================================
//	インクルードファイル
//=================================================================================
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "input.h"
#include "textureManager.h"
#include "modelManager.h"
#include "sceneManager.h"
#include "fade.h"
#include "debug.h"
#include "sound.h"

//=================================================================================
//　静的メンバ変数宣言
//=================================================================================
CInputKeyboard *CManager::m_pInputKeyboard = NULL;
CInputGamePad *CManager::m_pInputGamePad = NULL;
CInputMouse *CManager::m_pInputMouse = NULL;

//=================================================================================
//	コンストラクタ
//=================================================================================
CManager::CManager()
{
}

//=================================================================================
//	デストラクタ
//=================================================================================
CManager::~CManager()
{
}

//=================================================================================
//	初期化関数
//=================================================================================
HRESULT CManager::Init( HINSTANCE hInstance, HWND hWnd, bool bWindow )
{
	CRenderer::Init( hWnd, bWindow );

	// 入力処理インターフェースの生成
	m_pInputKeyboard = new CInputKeyboard;
	m_pInputKeyboard->Init( hInstance, hWnd );
	m_pInputGamePad = new CInputGamePad;
	if( SUCCEEDED( m_pInputGamePad->Init( hInstance, hWnd ))){
		OutputDebugString( "\n_/_/_/_/_/_/_/_/_/_/_/ GAMEPAD _/_/_/_/_/_/_/_/_/_/_/_/\n\n" );
		OutputDebugString( "Acquired.." );
		OutputDebugString( "\n\n_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/\n\n" );
	}
	m_pInputMouse = new CInputMouse;
	m_pInputMouse->Init( hInstance, hWnd );

	// シード値生成
	srand((unsigned)time(NULL));

	// フェード
	CFade::Init();

	//サウンドの初期化
	InitSound(hWnd);

	CSceneManager::Init();

	// テクスチャ情報全表示
	CTextureManager::Show();


	return S_OK;
}

//=================================================================================
//	終了関数
//=================================================================================
void CManager::Uninit(void)
{
	// フェード
	CFade::Uninit();

	CSceneManager::Uninit();

	// テクスチャの破棄
	CTextureManager::ReleaseAll();

	// モデルの破棄
	CModelManager::ReleaseAll();

	// オブジェクトの破棄
	CScene::ReleaseAll();

	// レンダラー終了
	CRenderer::Uninit();
		

	// 入力処理の破棄
	if( m_pInputKeyboard != NULL )
	{
		m_pInputKeyboard->Uninit();
		m_pInputKeyboard = NULL;
	}
	if( m_pInputGamePad != NULL )
	{
		m_pInputGamePad->Uninit();
		m_pInputGamePad = NULL;
	}
	if( m_pInputMouse != NULL )
	{
		m_pInputMouse->Uninit();
		m_pInputMouse = NULL;
	}

	//サウンド
	UninitSound();

	return;
}

//=================================================================================
//	更新関数
//=================================================================================
void CManager::Update(void)
{
	CDebug::ClearText();

	// フェード
	CFade::Update();

	// 入力処理の更新
	if( m_pInputKeyboard != NULL )
	{
		m_pInputKeyboard->Update();
	}
	if( m_pInputGamePad != NULL )
	{
		m_pInputGamePad->Update();
	}
	if( m_pInputMouse != NULL )
	{
		m_pInputMouse->Update();
	}

	CSceneManager::Update();

	UpdateSound();

	return;
}

//=================================================================================
//	描画関数
//=================================================================================
void CManager::Draw(void)
{
	// Direct3Dによる描画の開始
	if(SUCCEEDED(CRenderer::DrawBegin()))
	{
		CSceneManager::Draw();

		CFade::Draw();

#ifdef _DEBUG
		// FPS表示
		CRenderer::DrawFPS();

		// デバッグ表示
		CDebug::Draw();
#endif
	}

	// 描画終了
	CRenderer::DrawEnd();

	return;
}