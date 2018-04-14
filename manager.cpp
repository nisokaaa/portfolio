//=================================================================================
//	�v���O������	�F	
//	����			�F	�Q�[���Ǘ�[manager.cpp]
//	���C������		�F	main.cpp
//	�쐬��			�F	moriya gaku
//=================================================================================

//=================================================================================
//	�C���N���[�h�t�@�C��
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
//�@�ÓI�����o�ϐ��錾
//=================================================================================
CInputKeyboard *CManager::m_pInputKeyboard = NULL;
CInputGamePad *CManager::m_pInputGamePad = NULL;
CInputMouse *CManager::m_pInputMouse = NULL;

//=================================================================================
//	�R���X�g���N�^
//=================================================================================
CManager::CManager()
{
}

//=================================================================================
//	�f�X�g���N�^
//=================================================================================
CManager::~CManager()
{
}

//=================================================================================
//	�������֐�
//=================================================================================
HRESULT CManager::Init( HINSTANCE hInstance, HWND hWnd, bool bWindow )
{
	CRenderer::Init( hWnd, bWindow );

	// ���͏����C���^�[�t�F�[�X�̐���
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

	// �V�[�h�l����
	srand((unsigned)time(NULL));

	// �t�F�[�h
	CFade::Init();

	//�T�E���h�̏�����
	InitSound(hWnd);

	CSceneManager::Init();

	// �e�N�X�`�����S�\��
	CTextureManager::Show();


	return S_OK;
}

//=================================================================================
//	�I���֐�
//=================================================================================
void CManager::Uninit(void)
{
	// �t�F�[�h
	CFade::Uninit();

	CSceneManager::Uninit();

	// �e�N�X�`���̔j��
	CTextureManager::ReleaseAll();

	// ���f���̔j��
	CModelManager::ReleaseAll();

	// �I�u�W�F�N�g�̔j��
	CScene::ReleaseAll();

	// �����_���[�I��
	CRenderer::Uninit();
		

	// ���͏����̔j��
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

	//�T�E���h
	UninitSound();

	return;
}

//=================================================================================
//	�X�V�֐�
//=================================================================================
void CManager::Update(void)
{
	CDebug::ClearText();

	// �t�F�[�h
	CFade::Update();

	// ���͏����̍X�V
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
//	�`��֐�
//=================================================================================
void CManager::Draw(void)
{
	// Direct3D�ɂ��`��̊J�n
	if(SUCCEEDED(CRenderer::DrawBegin()))
	{
		CSceneManager::Draw();

		CFade::Draw();

#ifdef _DEBUG
		// FPS�\��
		CRenderer::DrawFPS();

		// �f�o�b�O�\��
		CDebug::Draw();
#endif
	}

	// �`��I��
	CRenderer::DrawEnd();

	return;
}