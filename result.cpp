//=================================================================================
//	�v���O������	�F	
//	����			�F	���U���g�V�[������[result.cpp]
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
#include "doom.h"
#include "meshField.h"
#include "fade.h"
#include "doom.h"
#include "sound.h"
#include "score.h"
#include "gauge.h"

//=================================================================================
//	�}�N����`
//=================================================================================
// ��g�G���A
#define WRAP_POS_H					(100)
#define WRAP_POS_W					(100)

// ���o���G���A
#define HEAD_POS_H					(WRAP_POS_H)
#define HEAD_POS_W					(WRAP_POS_W)

// ���C���R���e���c�G���A
#define MAIN_POS_H					(HEAD_POS_H + 100)
#define MAIN_POS_W					(HEAD_POS_W)

// �t�b�^�R���e���c�G���A
#define FOOTER_POS_H				(MAIN_POS_H + 450)
#define FOOTER_POS_W				(MAIN_POS_W)

// ��
#define THIN_LINE_H					(5)								// �ׂ� �� ����
#define THIN_LINE_W					(1500)							// �ׂ� �� ��
#define MIDDIUM_LINE_H				(30)							// �����炢 �� ����
#define MIDDIUM_LINE_W				(1500)							// �����炢 �� ��

// �i�Ԋu
#define STAGE_INTERVAL				(50)							// �i�̊Ԋu							

// ���o��
#define HEADER_FONT_SCALE			(0.25f)							// ���o�� �����T�C�Y��
#define HEADER_FONT_SIZE_H			(187 * HEADER_FONT_SCALE)		// ���o�� �����T�C�Y����
#define HEADER_FONT_SIZE_W			(1226 * HEADER_FONT_SCALE)		// ���o�� �����T�C�Y��

#define DEF_FONT_SCALE				(0.15f)							// ���̑� �����X�P�[��
#define DEF_FONT_SIZE_H				(100 * DEF_FONT_SCALE)			// ���̑� �����T�C�Y ����

// ���̑�����
#define DIFFICULY_FONT_SIZE_W		(1981 * DEF_FONT_SCALE)			// "��Փx" �����T�C�Y ��

#define SCORE_FONT_SIZE_W			(931 * DEF_FONT_SCALE)			// "�X�R�A" �����T�C�Y ��

#define CLEAR_TIME_FONT_SIZE_W		(1641 * DEF_FONT_SCALE)			// "�N���A����" �����T�C�Y ��

#define ENEMY_KILLS_FONT_SIZE_W		(1831 * DEF_FONT_SCALE)			// "���ސ�" �����T�C�Y ��

#define CONTINUE_FONT_SIZE_W		(1351 * DEF_FONT_SCALE)			// "�R���e�j���[��" �����T�C�Y ��

#define RANK_FONT_SIZE_W			(741 * DEF_FONT_SCALE)			// "RANK" �����T�C�Y ��

#define RANK_ALPHA_FONT_SIZE_W		(378)							// "�����N" �����T�C�Y ��
#define RANK_ALPHA_FONT_SIZE_H		(280)							// "�����N" �����T�C�Y ����

#define PRESS_BUTTON_FONT_SIZE_W	(4071 * DEF_FONT_SCALE)			// "PRESS START BUTTON " �����T�C�Y ��

// ��������
#define TEXT_PADDING				(7)								// �����p�f�B���O

//=================================================================================
//	�R���X�g���N�^
//=================================================================================
CResult::CResult()
{
	m_nTime = 0;
	m_pBackGround = NULL;
	op = -1.0f;
	btnAlpha = 0.0f;
	textAlpha = 0.0f;
}

//=================================================================================
//	�f�X�g���N�^
//=================================================================================
CResult::~CResult()
{
}

//=================================================================================
//	�������֐�
//=================================================================================
HRESULT CResult::Init( void )
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

	// �w�i
	m_pBackGround = CScene2D::Create("data/TEXTURE/result/bg/test_screen.png", D3DXVECTOR2(SCREEN_CENTER_X, SCREEN_CENTER_Y), 1920.0f, 1080.0f, 1.0f, TRUE, 0);

	// RESULT�^�C�g��
	CScene2D::Create("data/TEXTURE/result/text/result.png", D3DXVECTOR2(HEAD_POS_W, HEAD_POS_H), HEADER_FONT_SIZE_W, HEADER_FONT_SIZE_H, 1.0f, FALSE, 0);

	// ��Փx�\��
	CScene2D::Create("data/TEXTURE/result/text/normal.png", D3DXVECTOR2(HEAD_POS_W + 1150, HEAD_POS_H + (HEADER_FONT_SIZE_H * 0.5f)), DIFFICULY_FONT_SIZE_W, DEF_FONT_SIZE_H, 1.0f, FALSE, 0);

	// RESULT��bar
	CScene2D::Create("data/TEXTURE/result/bg/bar.png", D3DXVECTOR2(HEAD_POS_W, HEAD_POS_H + HEADER_FONT_SIZE_H + 20), THIN_LINE_W, THIN_LINE_H, 1.0f, FALSE, 0);

	// �X�R�A
	m_vpBoxText.push_back(UI(CScene2D::Create("data/TEXTURE/result/text/score.png", D3DXVECTOR2(MAIN_POS_W + 25 + TEXT_PADDING, MAIN_POS_H + STAGE_INTERVAL + TEXT_PADDING), SCORE_FONT_SIZE_W, DEF_FONT_SIZE_H, 1.0f, FALSE, 0)));

	// �N���A����
	m_vpBoxText.push_back(UI(CScene2D::Create("data/TEXTURE/result/text/clear_time.png", D3DXVECTOR2(MAIN_POS_W + 25 + TEXT_PADDING, MAIN_POS_H + STAGE_INTERVAL * 3 + TEXT_PADDING), CLEAR_TIME_FONT_SIZE_W, DEF_FONT_SIZE_H, 1.0f, FALSE, 0)));

	// ���ސ�
	m_vpBoxText.push_back(UI(CScene2D::Create("data/TEXTURE/result/text/enemy_kills.png", D3DXVECTOR2(MAIN_POS_W + 25 + TEXT_PADDING, MAIN_POS_H + STAGE_INTERVAL * 5 + TEXT_PADDING), ENEMY_KILLS_FONT_SIZE_W, DEF_FONT_SIZE_H, 1.0f, FALSE, 0)));

	// �R���e�j���[��
	m_vpBoxText.push_back(UI(CScene2D::Create("data/TEXTURE/result/text/continue.png", D3DXVECTOR2(MAIN_POS_W + 25 + TEXT_PADDING, MAIN_POS_H + STAGE_INTERVAL * 7 + TEXT_PADDING), CONTINUE_FONT_SIZE_W, DEF_FONT_SIZE_H, 1.0f, FALSE, 0)));

	int cnt = 1;
	for (int i = 0; i < 4; i++)
	{
		// BAR
		m_vpBoxBar.push_back(UI(CScene2D::Create("data/TEXTURE/result/bg/bar.png", D3DXVECTOR2(MAIN_POS_W, MAIN_POS_H + STAGE_INTERVAL * cnt), MIDDIUM_LINE_W, MIDDIUM_LINE_H, 1.0f, FALSE, 0)));
		
		// �����N(RANK)
		m_vpBoxText.push_back(UI(CScene2D::Create("data/TEXTURE/result/text/rank.png", D3DXVECTOR2(MAIN_POS_W + 1280 + TEXT_PADDING, MAIN_POS_H + STAGE_INTERVAL * cnt + TEXT_PADDING), RANK_FONT_SIZE_W, DEF_FONT_SIZE_H, 1.0f, FALSE, 0)));

		// �����N
		m_vpBoxText.push_back(UI(CScene2D::Create("data/TEXTURE/result/text/rank_b.png", D3DXVECTOR2(MAIN_POS_W + 1450 + TEXT_PADDING, MAIN_POS_H + STAGE_INTERVAL * cnt + MIDDIUM_LINE_H * 0.5f), RANK_ALPHA_FONT_SIZE_W, RANK_ALPHA_FONT_SIZE_H, 0.15f, TRUE, 0)));

		cnt += 2;
	}

	// ���ʉ�bar
	CScene2D::Create("data/TEXTURE/result/bg/bar.png", D3DXVECTOR2(FOOTER_POS_W, FOOTER_POS_H), THIN_LINE_W, THIN_LINE_H, 1.0f, FALSE, 0);

	// �����N(RANK)
	m_vpBoxText.push_back(UI(CScene2D::Create("data/TEXTURE/result/text/rank.png", D3DXVECTOR2(FOOTER_POS_W + 1050 + TEXT_PADDING, FOOTER_POS_H + 150), RANK_FONT_SIZE_W, DEF_FONT_SIZE_H, 1.0f, FALSE, 0)));

	// �����N
	m_vpBoxText.push_back(UI(CScene2D::Create("data/TEXTURE/result/text/rank_c.png", D3DXVECTOR2(FOOTER_POS_W + 1350 + TEXT_PADDING, FOOTER_POS_H + 150), RANK_ALPHA_FONT_SIZE_W, RANK_ALPHA_FONT_SIZE_H, 0.4, TRUE, 0)));

	// PRESS START BUTTON
	m_pButton = CScene2D::Create("data/TEXTURE/result/text/press_start_button.png", D3DXVECTOR2(FOOTER_POS_W, FOOTER_POS_H + 250), PRESS_BUTTON_FONT_SIZE_W, DEF_FONT_SIZE_H, 1.0f, FALSE, 0);

	CFade::SetAlpha(1.0f);
	CFade::SetFade(CFade::FADE_IN, new CResult, 0.5f);

	// SE�Đ�
	//PlaySound(SOUND_LABEL_BGM_RESULT);

	// �����x��0�ɂ��Ă���
	for (int i = 0; i < (int)m_vpBoxBar.size(); i++)
		m_vpBoxBar[i].ui->SetAlpha(m_vpBoxBar[i].alpha);
	m_vpBoxBar[0].flg = TRUE;

	for (int i = 0; i < (int)m_vpBoxText.size(); i++)
		m_vpBoxText[i].ui->SetAlpha(m_vpBoxText[i].alpha);
	m_vpBoxText[0].flg = TRUE;

	// �X�R�A������
	//m_pScore = CScore::Create("data/TEXTURE/number.png", D3DXVECTOR2(380.0f, 380.0f), D3DXVECTOR2(1425.0f, 92.0f), 3);

	// �Q�[�W������
	m_pHpGauge = CGauge::Create("data/TEXTURE/gauge2.png", D3DXVECTOR2(104.0f, 3.0f), D3DXVECTOR2(423.0f, 14.0f), 1.2f, 100.0f, 100.0f,
								"data/TEXTURE/gauge_back2.png", D3DXVECTOR2(500.0f, SCREEN_HEIGHT - 600.0f), D3DXVECTOR2(531.0f, 20.0f), 1.2f);

	return S_OK;
}

//=================================================================================
//	�I���֐�
//=================================================================================
void CResult::Uninit( void )
{
	CSceneManager::GetCameraManager()->Release(MAIN_CAMERA);

	CSceneManager::GetLightManager()->Release(MAIN_LIGHT);

	// �I�u�W�F�N�g�̔j��
	CScene::ReleaseAll();

	//StopSound(SOUND_LABEL_BGM_RESULT);
}

//=================================================================================
//	�`��֐�
//=================================================================================
void CResult::Draw( void )
{
	// �J�����Z�b�g(���ׂẴI�u�W�F�N�g��`�悷��O�ɃJ�����u��)
	CCameraManager *pCameraManager = CSceneManager::GetCameraManager();
	CCamera *pCamera = pCameraManager->GetCamera(MAIN_CAMERA);
	if (pCamera != NULL) pCamera->Set();

	// �I�u�W�F�N�g�̕`��
	CScene::DrawAll();
}

//=================================================================================
//	�X�V�֐�
//=================================================================================
void CResult::Update( void )
{
	// �I�u�W�F�N�g�̍X�V
	CScene::UpdateAll();

	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();
	if (pKeyboard->GetKeyTrigger(DIK_RETURN))
	{
		CFade::SetFade( CFade::FADE_OUT, new CTitle, 1.0f );
	}

	if (pKeyboard->GetKeyPress(DIK_U))
	{
		m_pHpGauge->Add(2.0f);
	}
	if (pKeyboard->GetKeyPress(DIK_D))
	{
		m_pHpGauge->Sub(2.0f);
	}

	// �_��
	if (m_nTime % 65 == 0) op *= -1;
	btnAlpha += op * 0.01f;
	if (btnAlpha > 1.0f) btnAlpha = 1.0f;
	if (btnAlpha < 0.0f) btnAlpha = 0.0f;
	m_pButton->SetAlpha(btnAlpha);

	// �B���Ă������v�f�����X�ɕ\��
	for (int i = 0; i < (int)m_vpBoxBar.size(); i++)
	{
		if (m_vpBoxBar[i].flg)
		{
			m_vpBoxBar[i].ui->SetAlpha(m_vpBoxBar[i].alpha);

			m_vpBoxBar[i].alpha += 0.04f;

			if (m_vpBoxBar[i].alpha > 1.0f)
			{
				m_vpBoxBar[i].flg = FALSE;
				
				if(i + 1 < m_vpBoxBar.size())
					m_vpBoxBar[i + 1].flg = TRUE;
			}
		}
	}

	// �B���Ă������v�f�����X�ɕ\��
	for (int i = 0; i < (int)m_vpBoxText.size(); i++)
	{
		if (m_vpBoxText[i].flg)
		{
			m_vpBoxText[i].ui->SetAlpha(m_vpBoxText[i].alpha);

			m_vpBoxText[i].alpha += 0.04f;

			if (m_vpBoxText[i].alpha > 1.0f)
			{
				m_vpBoxText[i].flg = FALSE;

				if (i + 1 < m_vpBoxText.size())
					m_vpBoxText[i + 1].flg = TRUE;
			}
		}
	}

	// �^�C�}�[�J�E���g�A�b�v
	m_nTime += 1;
}