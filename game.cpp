//=================================================================================
//	�v���O������	�F	
//	����			�F	�Q�[���V�[������[game.cpp]
//	���C������		�F	main.cpp
//	�쐬��			�F	moriya gaku
//=================================================================================

//=================================================================================
//	�C���N���[�h�t�@�C��
//=================================================================================
#include "manager.h"
#include "sceneManager.h"
#include "textureManager.h"
#include "meshFieldManager.h"
#include "cameraManager.h"
#include "lightManager.h"
#include "input.h"
#include "scene.h"
#include "scene2D.h"
#include "scene3D.h"
#include "sceneX.h"
#include "meshField.h"
#include "billboard.h"
#include "fade.h"
#include "doom.h"
#include "player.h"
#include "enemy.h"
#include "debug.h"
#include "debugObj.h"
#include "gauge.h"
#include "sound.h"
#include "reticle.h"
#include "motionActor.h"
#include "renderer.h"
#include "background.h"
#include "menu.h"

//=================================================================================
//	�R���X�g���N�^
//=================================================================================
CGame::CGame()
{
	m_Timer = 0;		// �o�ߎ���(�t���[��)
	m_bPause = FALSE;	// �|�[�Y���[�h�t���O
}

//=================================================================================
//	�f�X�g���N�^
//=================================================================================
CGame::~CGame()
{
}

//=================================================================================
//	�������֐�
//=================================================================================
HRESULT CGame::Init( void )
{
	// �}�l�[�W���擾
	CCameraManager *pCameraManager = CSceneManager::GetCameraManager();
	CLightManager *pLightManager = CSceneManager::GetLightManager();
	CMeshFieldManager *pMeshFieldManager = CSceneManager::GetMeshFieldManager();
	CDebugObj *pDebugManager = CSceneManager::GetDebugObj();
	CGame *pGame = dynamic_cast<CGame*>(CSceneManager::GetScene());
	
	// �J��������
	//pCameraManager->Create(MAIN_CAMERA, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 10.0f, 4.0f);
	pCameraManager->Create(MAIN_CAMERA, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 5.0f, 4.0f);

	// ���C�g����
	pLightManager->Create(MAIN_LIGHT, D3DXVECTOR3(-0.96f, -0.27f, 0.0f), D3DXVECTOR3(0.0f, 30.0f, -30.0f), COLOR_WHITE, D3DXVECTOR3(0.5f, 0.5f, 0.5f), D3DXVECTOR3(0.3f, 0.3f, 0.3f));

	// ���j���[����
	m_pMenu = new CMenu(new CGameMenu1);
	m_pMenu->Init();
	m_pMenu->CreateMenuItem("data/TEXTURE/game_menu1.png", GAME_MENU1_POS, 1783.0f, 89.0f, GAME_MENU_FONT_SIZE, TRUE, 7);
	m_pMenu->CreateMenuItem("data/TEXTURE/game_menu2.png", GAME_MENU2_POS, 1909.0f, 89.0f, GAME_MENU_FONT_SIZE, TRUE, 7);
	m_pMenu->CreateMenuItem("data/TEXTURE/game_menu3.png", GAME_MENU3_POS, 1306.0f, 89.0f, GAME_MENU_FONT_SIZE, TRUE, 7);
	CScene2D::Create("data/TEXTURE/pause2.png", D3DXVECTOR2(SCREEN_CENTER_X, SCREEN_CENTER_Y - 150), 936.0f, 187.0f, 0.58f, TRUE, 7);

	// �Q�[��UI����
	//CScene2D::Create("data/TEXTURE/score.png", D3DXVECTOR2((SCREEN_WIDTH - 1033 * 0.2f)-300, 20), 1033.0f, 187.0f, 0.2f, FALSE, 5);
	//CScene2D::Create("data/TEXTURE/life.png", D3DXVECTOR2(20, (SCREEN_HEIGHT - 187.0f * 0.2)-20), 805.0f, 187.0f, 0.2f, FALSE, 5);

	// �h�[������
	//CDoom *pMeshDoom = CDoom::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50.0f, 50.0f, 50, 50, 180.0f, 361.0f, "data/TEXTURE/skydome/309.JPG");

	// ���b�V���t�B�[���h����
	//pMeshFieldManager->Load("data/field.txt", GAME_MAIN_STAGE);

	// �v���C���[����
	m_pPlayer = CPlayer::Create("data/player_motion.txt", 100.0f, 100.0f, 2.0f);
	m_pPlayer->SetLookAt(D3DXVECTOR3(0.0f, 20.0f, 100.0f));

	/*CSceneX *pDebugObj1 = pDebugManager->Create("data/MODEL/enemy.x", "playerLookAt");
	pDebugObj1->SetPosition(m_pPlayer->GetLookAt());
	pDebugObj1->SetScale(D3DXVECTOR3(0.5f, 0.5f, 0.5f));*/
	CSceneX *pDebugObj2 = pDebugManager->Create("data/MODEL/enemy.x", "cameraLookAt");
	pDebugObj2->SetPosition(pCameraManager->GetCamera(MAIN_CAMERA)->GetLookAt());
	pDebugObj2->SetScale(D3DXVECTOR3(0.3f, 0.3f, 0.3f));

	// ���`�N��
	CReticle *pReticle = new CReticle(4, true);
	pReticle->SetPosition(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 200.0f, 0.0f));
	pReticle->SetHeight(RETICLE_WIDTH);
	pReticle->SetWidth(RETICLE_HEIGHT);
	pReticle->SetTexHeight(1.0f);
	pReticle->SetTexWidth(1.0f);
	pReticle->SetTexture("data/TEXTURE/reticle.png");
	pReticle->SetType(CScene::SCENE_TYPE_RETICLE);
	pReticle->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
	pReticle->SetViewAngle(60.0f);
	pReticle->Init();

	// SE�Đ�
	//PlaySound(SOUND_LABEL_BGM_GAME);

	m_pBackGround = new CBackGround;
	m_pBackGround->Create();

	return S_OK;
}

//=================================================================================
//	�I���֐�
//=================================================================================
void CGame::Uninit( void )
{
	CSceneManager::GetMeshFieldManager()->Unload(GAME_MAIN_STAGE);

	CSceneManager::GetCameraManager()->Release(MAIN_CAMERA);

	CSceneManager::GetLightManager()->Release(MAIN_LIGHT);

	m_pMenu->Uninit();

	// �I�u�W�F�N�g�̔j��
	CScene::ReleaseAll();

	// �Q�[���֘A������
	m_Timer = 0;

	// SE�Đ�
	//StopSound(SOUND_LABEL_BGM_GAME);
}

//=================================================================================
//	�`��֐�
//=================================================================================
void CGame::Draw( void )
{
	// �J�����Z�b�g(���ׂẴI�u�W�F�N�g��`�悷��O�ɃJ�����u��)
	CCameraManager *pCameraManager = CSceneManager::GetCameraManager();
	CCamera *pCamera = pCameraManager->GetCamera(MAIN_CAMERA);
	if (pCamera != NULL) pCamera->Set();

	// �|�[�Y���[�h����������UI��p�̗D��x�ȏ�݂̂ɍX�V����
	if (m_bPause) {
		//CScene::DrawAll();
		m_pBackGround->Draw();
		//CScene::DrawAll(7);
		return;
	}
	else
	{
		// �I�u�W�F�N�g�̕`��
		CScene::DrawAll(-1, 7);
	}
}

//=================================================================================
//	�X�V�֐�
//=================================================================================
void CGame::Update( void )
{
	// �G����
//#ifdef CREATE_ENEMY
//	if (((m_Timer % (60 * 2)) == 0) && CEnemy::GetCount() < 1)
//	{
//		/*const float MIN_RANGE_WIDTH = -40.0f;
//		const float MAX_RANGE_WIDTH = 40.0f;
//
//		const float MIN_RANGE_HEIGHT = 10.0f;
//		const float MAX_RANGE_HEIGHT = 40.0f;
//
//		const float MIN_RANGE_DEPTH = -40.0f;
//		const float MAX_RANGE_DEPTH = 40.0f;
//
//		float x = GetRandRnage( MIN_RANGE_WIDTH, MAX_RANGE_WIDTH );
//		float y = GetRandRnage( MIN_RANGE_HEIGHT, MAX_RANGE_HEIGHT );
//		float z = GetRandRnage( MIN_RANGE_DEPTH, MAX_RANGE_DEPTH );
//		*/
//
//		CEnemy *pEnemy = new CEnemy(DEFAULT_PRIORITY);
//		//pEnemy->SetPosition( D3DXVECTOR3( x, y, z ) );
//		pEnemy->SetPosition(D3DXVECTOR3(0.0f, 35.0f, 80.0f));
//		pEnemy->SetRotation(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
//		pEnemy->SetScale(D3DXVECTOR3(1.5f, 1.5f, 1.5f));
//		pEnemy->SetModel("data/MODEL/enemy.x");
//		pEnemy->SetSpeed(0.1f);
//		pEnemy->SetHp(200.0f);
//		pEnemy->SetState(CEnemy::NEUTRAL);
//		pEnemy->SetType(CScene::SCENE_TYPE_ENEMY);
//		CEnemy::SetCount(CEnemy::GetCount() + 1); // �G�J�E���g���₷
//		pEnemy->Init();
//	}
//#endif

	// �|�[�Y���[�h�̃X�C�b�`
	if (CManager::GetInputGamePad()->GetTrigger(DIJOFS_BUTTON7))
	{
		BOOL temp = m_bPause;
		m_bPause = m_bPause ? FALSE : TRUE;

		// �|�[�Y���[�h�Ɉڍs�����u��
		if (!temp && m_bPause)
		{
			LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
			pDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), COLOR_BLACK, 1.0f, 0);
			pDevice->SetRenderTarget(0, m_pBackGround->GetSurface());
			CScene::DrawAll(-1, 4);
			m_pBackGround->MakeTexture();
		}
	}

	// �|�[�Y���[�h����������UI��p�̗D��x�ȏ�݂̂ɍX�V����
	if (m_bPause) {
		// ���j���[�̍X�V
		m_pMenu->Update();

		CScene::UpdateAll(7);
		return;
	}
	else
	{
		// �I�u�W�F�N�g�̍X�V(UI��p�̗D��x�����X�V)
		CScene::UpdateAll();

		// �^�C�}�[�X�V
		m_Timer += 1;

		CSceneManager::GetCameraManager()->Update();

		CSceneManager::GetLightManager()->Update();
	}

	if (CManager::GetInputKeyboard()->GetKeyTrigger(DIK_RETURN))
		CFade::SetFade(CFade::FADE_OUT, new CResult, 1.0f);
}