//=================================================================================
//	�v���O������	�F	
//	����			�F	�V�[�����ꊇ�Ǘ�[sceneManager.cpp]
//	���C������		�F	manager.cpp
//	�쐬��			�F	moriya gaku
//=================================================================================

//=================================================================================
//	�C���N���[�h�t�@�C��
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
//	�ÓI�����o�ϐ��̐錾
//=================================================================================
CCameraManager *CSceneManager::m_pCameraManager = NULL;	
CLightManager *CSceneManager::m_pLightManager = NULL;
CSceneBase *CSceneManager::m_pScene = new CGame;
CDebugObj *CSceneManager::m_pDebugObj = NULL;
CShaderManager *CSceneManager::m_pShaderManager = NULL;
CMeshFieldManager *CSceneManager::m_pMeshFieldManager = NULL;
CBackGround *CSceneManager::m_pBackGround = NULL;

//=================================================================================
//	�R���X�g���N�^
//=================================================================================
CSceneManager::CSceneManager() 
{
}

//=================================================================================
//	�f�X�g���N�^
//=================================================================================
CSceneManager::~CSceneManager() 
{
}

//=================================================================================
//	�������֐�
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
//	�I���֐�
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
//	�`��֐�
//=================================================================================
void CSceneManager::Draw( void )
{
	m_pScene->Draw();

	//m_pBackGround->Draw();
}

//=================================================================================
//	�X�V�֐�
//=================================================================================
void CSceneManager::Update( void )
{
	m_pScene->Update();

	m_pBackGround->Update();
}