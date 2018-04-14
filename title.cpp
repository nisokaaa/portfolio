//=================================================================================
//	プログラム名	：	
//	処理			：	タイトルシーン処理[title.cpp]
//	メイン処理		：	main.cpp
//	作成者			：	moriya gaku
//=================================================================================

//=================================================================================
//	インクルードファイル
//=================================================================================
#include "manager.h"
#include "sceneManager.h"
#include "textureManager.h"
#include "cameraManager.h"
#include "lightManager.h"
#include "input.h"
#include "scene.h"
#include "scene2D.h"
#include "scene3D.h"
#include "sceneX.h"
#include "meshField.h"
#include "fade.h"
#include "doom.h"
#include "billboard.h"
#include "debug.h"
#include "menu.h"
#include "player.h"
#include "motionActor.h"
#include "sound.h"

//=================================================================================
//	コンストラクタ
//=================================================================================
CTitle::CTitle()
{
	m_pMenu = nullptr;
	m_nTime = 0;
	op = -1.0f;
	alpha = 0.0f;
	m_pBackGround = NULL;
}

//=================================================================================
//	デストラクタ
//=================================================================================
CTitle::~CTitle()
{
}

//=================================================================================
//	初期化関数
//=================================================================================
HRESULT CTitle::Init( void )
{
	// マネージャ取得
	CCameraManager *pCameraManager = CSceneManager::GetCameraManager();
	CLightManager *pLightManager = CSceneManager::GetLightManager();
	CMeshFieldManager *pMeshFieldManager = CSceneManager::GetMeshFieldManager();
	CDebugObj *pDebugManager = CSceneManager::GetDebugObj();

	// カメラ生成
	pCameraManager->Create(MAIN_CAMERA, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 10.0f, 4.0f);

	// ライト生成
	pLightManager->Create(MAIN_LIGHT, D3DXVECTOR3(0.0f, -0.7f, 0.0f), D3DXVECTOR3(0.0f, 30.0f, -30.0f), COLOR_WHITE, D3DXVECTOR3(0.5f, 0.5f, 0.5f), D3DXVECTOR3(0.3f, 0.3f, 0.3f));

	// メニュー生成
	m_pMenu = new CMenu(new CTitleMenu1);
	m_pMenu->Init();
	m_pMenu->CreateMenuItem("data/TEXTURE/title_menu1.png", TITLE_MENU1_POS, 1612.0f, 89.0f, TITLE_MENU_FONT_SIZE, TRUE, 7);
	m_pMenu->CreateMenuItem("data/TEXTURE/title_menu2.png", TITLE_MENU2_POS, 1360.0f, 89.0f, TITLE_MENU_FONT_SIZE, TRUE, 7);
	m_pMenu->CreateMenuItem("data/TEXTURE/title_menu3.png", TITLE_MENU3_POS, 1612.0f, 89.0f, TITLE_MENU_FONT_SIZE, TRUE, 7);
	
	// タイトル
	CScene2D::Create("data/TEXTURE/title.png", D3DXVECTOR2(SCREEN_CENTER_X, SCREEN_CENTER_Y - 100.0f), 2271.0f, 360.0f, 0.5f, TRUE);

	// copylight
	CScene2D::Create("data/TEXTURE/copylight.png", D3DXVECTOR2(SCREEN_CENTER_X, SCREEN_HEIGHT - 100.0f), 1800.0f, 94.0f, 0.225f, TRUE);

	// キャラ画像
	CScene2D *pPattern = CScene2D::Create("data/TEXTURE/pattern2.png", D3DXVECTOR2(SCREEN_CENTER_X - 700, SCREEN_CENTER_Y), 1030.0f, 1252.0f, 0.9f, TRUE, 1);
	pPattern->SetAlpha(1.0f);

	// 背景
	m_pBackGround = CScene2D::Create("data/TEXTURE/background.png", D3DXVECTOR2(SCREEN_CENTER_X, SCREEN_CENTER_Y), 1920.0f, 1080.0f, 1.0f, TRUE, 0);
	
	CFade::SetAlpha(1.0f);
	CFade::SetFade(CFade::FADE_IN, new CTitle, 1.5f);

	// SE再生
	//PlaySound(SOUND_LABEL_BGM_TITLE);

	return S_OK;
}

//=================================================================================
//	終了関数
//=================================================================================
void CTitle::Uninit( void )
{
	CSceneManager::GetCameraManager()->Release(MAIN_CAMERA);

	CSceneManager::GetLightManager()->Release(MAIN_LIGHT);

	m_pMenu->Uninit();

	// オブジェクトの破棄
	CScene::ReleaseAll();

	//StopSound(SOUND_LABEL_BGM_TITLE);
}

//=================================================================================
//	描画処理
//=================================================================================
void CTitle::Draw( void )
{
	// カメラセット(すべてのオブジェクトを描画する前にカメラ置く)
	CCameraManager *pCameraManager = CSceneManager::GetCameraManager();
	CCamera *pCamera = pCameraManager->GetCamera(MAIN_CAMERA);
	if( pCamera != NULL ) pCamera->Set();

	// オブジェクトの描画
	CScene::DrawAll();
}

//=================================================================================
//	更新関数
//=================================================================================
void CTitle::Update( void )
{
	m_pMenu->Update();

	// オブジェクトの更新
	CScene::UpdateAll();

	// 点滅
	if (m_nTime % 150 == 0) op *= -1;
	alpha += op * 0.01f;
	if (alpha > 1.0f) alpha = 1.0f;
	if (alpha < 0.0f) alpha = 0.0f;
	m_pBackGround->SetAlpha(alpha);
	
	// タイマーカウントアップ
	m_nTime += 1;
}