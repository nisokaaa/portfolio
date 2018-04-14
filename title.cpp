//=================================================================================
//	�v���O������	�F	
//	����			�F	�^�C�g���V�[������[title.cpp]
//	���C������		�F	main.cpp
//	�쐬��			�F	moriya gaku
//=================================================================================

//=================================================================================
//	�C���N���[�h�t�@�C��
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
//	�R���X�g���N�^
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
//	�f�X�g���N�^
//=================================================================================
CTitle::~CTitle()
{
}

//=================================================================================
//	�������֐�
//=================================================================================
HRESULT CTitle::Init( void )
{
	// �}�l�[�W���擾
	CCameraManager *pCameraManager = CSceneManager::GetCameraManager();
	CLightManager *pLightManager = CSceneManager::GetLightManager();
	CMeshFieldManager *pMeshFieldManager = CSceneManager::GetMeshFieldManager();
	CDebugObj *pDebugManager = CSceneManager::GetDebugObj();

	// �J��������
	pCameraManager->Create(MAIN_CAMERA, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 10.0f, 4.0f);

	// ���C�g����
	pLightManager->Create(MAIN_LIGHT, D3DXVECTOR3(0.0f, -0.7f, 0.0f), D3DXVECTOR3(0.0f, 30.0f, -30.0f), COLOR_WHITE, D3DXVECTOR3(0.5f, 0.5f, 0.5f), D3DXVECTOR3(0.3f, 0.3f, 0.3f));

	// ���j���[����
	m_pMenu = new CMenu(new CTitleMenu1);
	m_pMenu->Init();
	m_pMenu->CreateMenuItem("data/TEXTURE/title_menu1.png", TITLE_MENU1_POS, 1612.0f, 89.0f, TITLE_MENU_FONT_SIZE, TRUE, 7);
	m_pMenu->CreateMenuItem("data/TEXTURE/title_menu2.png", TITLE_MENU2_POS, 1360.0f, 89.0f, TITLE_MENU_FONT_SIZE, TRUE, 7);
	m_pMenu->CreateMenuItem("data/TEXTURE/title_menu3.png", TITLE_MENU3_POS, 1612.0f, 89.0f, TITLE_MENU_FONT_SIZE, TRUE, 7);
	
	// �^�C�g��
	CScene2D::Create("data/TEXTURE/title.png", D3DXVECTOR2(SCREEN_CENTER_X, SCREEN_CENTER_Y - 100.0f), 2271.0f, 360.0f, 0.5f, TRUE);

	// copylight
	CScene2D::Create("data/TEXTURE/copylight.png", D3DXVECTOR2(SCREEN_CENTER_X, SCREEN_HEIGHT - 100.0f), 1800.0f, 94.0f, 0.225f, TRUE);

	// �L�����摜
	CScene2D *pPattern = CScene2D::Create("data/TEXTURE/pattern2.png", D3DXVECTOR2(SCREEN_CENTER_X - 700, SCREEN_CENTER_Y), 1030.0f, 1252.0f, 0.9f, TRUE, 1);
	pPattern->SetAlpha(1.0f);

	// �w�i
	m_pBackGround = CScene2D::Create("data/TEXTURE/background.png", D3DXVECTOR2(SCREEN_CENTER_X, SCREEN_CENTER_Y), 1920.0f, 1080.0f, 1.0f, TRUE, 0);
	
	CFade::SetAlpha(1.0f);
	CFade::SetFade(CFade::FADE_IN, new CTitle, 1.5f);

	// SE�Đ�
	//PlaySound(SOUND_LABEL_BGM_TITLE);

	return S_OK;
}

//=================================================================================
//	�I���֐�
//=================================================================================
void CTitle::Uninit( void )
{
	CSceneManager::GetCameraManager()->Release(MAIN_CAMERA);

	CSceneManager::GetLightManager()->Release(MAIN_LIGHT);

	m_pMenu->Uninit();

	// �I�u�W�F�N�g�̔j��
	CScene::ReleaseAll();

	//StopSound(SOUND_LABEL_BGM_TITLE);
}

//=================================================================================
//	�`�揈��
//=================================================================================
void CTitle::Draw( void )
{
	// �J�����Z�b�g(���ׂẴI�u�W�F�N�g��`�悷��O�ɃJ�����u��)
	CCameraManager *pCameraManager = CSceneManager::GetCameraManager();
	CCamera *pCamera = pCameraManager->GetCamera(MAIN_CAMERA);
	if( pCamera != NULL ) pCamera->Set();

	// �I�u�W�F�N�g�̕`��
	CScene::DrawAll();
}

//=================================================================================
//	�X�V�֐�
//=================================================================================
void CTitle::Update( void )
{
	m_pMenu->Update();

	// �I�u�W�F�N�g�̍X�V
	CScene::UpdateAll();

	// �_��
	if (m_nTime % 150 == 0) op *= -1;
	alpha += op * 0.01f;
	if (alpha > 1.0f) alpha = 1.0f;
	if (alpha < 0.0f) alpha = 0.0f;
	m_pBackGround->SetAlpha(alpha);
	
	// �^�C�}�[�J�E���g�A�b�v
	m_nTime += 1;
}