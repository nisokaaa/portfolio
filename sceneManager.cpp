//=================================================================================
//	プログラム名	：	
//	処理			：	シーン情報一括管理[sceneManager.cpp]
//	メイン処理		：	manager.cpp
//	作成者			：	moriya gaku
//=================================================================================

//=================================================================================
//	インクルードファイル
//=================================================================================
#include "manager.h"
#include "sceneManager.h"
#include "player.h"
#include "debugObj.h"
#include "meshFieldManager.h"
#include "shaderManager.h"
#include "cameraManager.h"
#include "lightManager.h"
#include "background.h"

//=================================================================================
//	静的メンバ変数の宣言
//=================================================================================
CCameraManager *CSceneManager::m_pCameraManager = NULL;	
CLightManager *CSceneManager::m_pLightManager = NULL;
CSceneBase *CSceneManager::m_pScene = new CGame;
CDebugObj *CSceneManager::m_pDebugObj = NULL;
CShaderManager *CSceneManager::m_pShaderManager = NULL;
CMeshFieldManager *CSceneManager::m_pMeshFieldManager = NULL;
CBackGround *CSceneManager::m_pBackGround = NULL;

//=================================================================================
//	コンストラクタ
//=================================================================================
CSceneManager::CSceneManager() 
{
}

//=================================================================================
//	デストラクタ
//=================================================================================
CSceneManager::~CSceneManager() 
{
}

//=================================================================================
//	初期化関数
//=================================================================================
HRESULT CSceneManager::Init( void )
{
	m_pDebugObj = new CDebugObj();

	m_pShaderManager = new CShaderManager();
	m_pShaderManager->Init();

	m_pMeshFieldManager = new CMeshFieldManager();

	m_pCameraManager = new CCameraManager();

	m_pLightManager = new CLightManager();

	m_pBackGround = new CBackGround;
	m_pBackGround->Create();

	m_pScene->Init();

	return S_OK;
}

//=================================================================================
//	終了関数
//=================================================================================
void CSceneManager::Uninit( void )
{
	CTextureManager::ReleaseAll();

	m_pBackGround->Uninit();

	m_pScene->Uninit();

	m_pDebugObj->Uninit();

	m_pCameraManager->ReleaseAll();

	m_pLightManager->ReleaseAll();

	m_pMeshFieldManager->ReleaseAll();

	m_pShaderManager->ReleaseAll();
}

//=================================================================================
//	描画関数
//=================================================================================
void CSceneManager::Draw( void )
{
	m_pScene->Draw();

	//m_pBackGround->Draw();
}

//=================================================================================
//	更新関数
//=================================================================================
void CSceneManager::Update( void )
{
	m_pScene->Update();

	m_pBackGround->Update();
}