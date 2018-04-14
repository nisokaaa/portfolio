//=================================================================================
//	�v���O������	�F	
//	����			�F	�v���C���[����[player.cpp]
//	���C������		�F	main.cpp
//	�쐬��			�F	moriya gaku
//=================================================================================

//=================================================================================
//	�C���N���[�h�t�@�C��
//=================================================================================
#include "manager.h"
#include "renderer.h"
#include "meshField.h"
#include "player.h"
#include "enemy.h"
#include "scene.h"
#include "input.h"
#include "cameraManager.h"
#include "debug.h"
#include "debugObj.h"
#include "sceneManager.h"
#include "meshFieldManager.h"
#include "scene2D.h"
#include "sound.h"
#include "bullet.h"
#include "fade.h"
#include "reticle.h"
#include "motionActor.h"
#include "playerState.h"

//=================================================================================
//	�������֐�
//=================================================================================
HRESULT CPlayer::Init(string key)
{
	// �v���C���[���璍���_�܂ł̋���
	m_Distance = 10.0f;

	m_pModel = new MotionActor(key.c_str());
	m_pModel->Init();
	m_pModel->SetScl(D3DXVECTOR3(0.05f, 0.05f, 0.05f));

	m_pState = new CPlayerState;

	return S_OK;
}

//=================================================================================
//	�I���֐�
//=================================================================================
void CPlayer::Uninit(void)
{	
	m_pModel->Uninit();

	CScene::Release();

	m_pState->Uninit();
}

//=================================================================================
//	�X�V�֐�
//=================================================================================
void CPlayer::Update(void)
{
	m_pState->Update();

	// �v���C���[�ړ�
	MovePlayer();

	// �����_�ړ�
	MoveLookAt();

	// �v���C���[��](���f��)
	RotatePlayer();
	
	CalcPositionToVector(&m_VecDir, m_LookAt, GetPosition());

	// �v���C���[HP���O�ŃQ�[���I�[�o�[�B�V�[���J��
	if( m_Hp <= 0.0f ){
		CFade::SetFade( CFade::FADE_OUT, new CResult, 0.5f );
	}

	// ���f������]
	m_pModel->Update();
	m_pModel->SetPos(GetPosition());
	m_pModel->SetLookAtVec(GetLookAt());

#ifdef _DEBUG
	ShowParam();
#endif
}

//=================================================================================
//	�`��֐�
//=================================================================================
void CPlayer::Draw(void)
{
	m_pModel->Draw();
}

//=================================================================================
//	�����֐�
//=================================================================================
CPlayer *CPlayer::Create(string key, float hp, float mp, float speed)
{
	CPlayer *p = new CPlayer();
	p->SetType(CScene::SCENE_TYPE_PLAYER);
	p->m_Hp = hp;
	p->m_Mp = mp;
	p->m_Speed = D3DXVECTOR3(speed, speed, speed);
	p->Init(key);

	return p;
}

//=================================================================================
//	�����_�ړ��֐�
//=================================================================================
void CPlayer::MoveLookAt( void )
{
	// �J�����擾
	CCameraManager *pCameraManager = CSceneManager::GetCameraManager();
	CCamera *pCamera = pCameraManager->GetCamera(MAIN_CAMERA);

	D3DXVECTOR3 pos = GetPosition();

	// ���X�e�b�N���͂���������
	if (CManager::GetInputGamePad()->CheckInputL())
	{
		m_Angle = 0;
		m_Angle = atan2((float)CManager::GetInputGamePad()->GetDInput().lY * 0.000001f,
			(float)CManager::GetInputGamePad()->GetDInput().lX * 0.000001f) + D3DX_PI / 2;

		// playePos��CameraAt �x�N�g��
		D3DXVECTOR3 cVecDir = (D3DXVECTOR3 &)pCamera->GetVecDir();
		cVecDir.y = 0.0f;	// Y����������

							// �J������angle����playerAt�ɑ���
		float cAngle = 0.0f;
		cAngle = -(float)atan2(cVecDir.z, cVecDir.x) + D3DX_PI / 2;

		m_Angle += cAngle;

		// 0 <= x <= 6.28�ɒ���
		if (2 * D3DX_PI < m_Angle)
		{
			m_Angle -= 2 * D3DX_PI;
			cAngle -= 2 * D3DX_PI;
		}
		if (0 > m_Angle)
		{
			m_Angle += 2 * D3DX_PI;
			cAngle += 2 * D3DX_PI;
		}

		// �ړ��s��X�V
		D3DXMatrixTranslation(&m_MtxPos, pos.x, pos.y, pos.z);

		// ��]�s��X�V
		D3DXMatrixRotationY(&m_MtxRot, m_Angle);

		// �����_
		D3DXVECTOR3 at = D3DXVECTOR3(0.0f, 0.0f, m_Distance);

		// ���_�����ɂP�O���ꂽ�_���璍���_����]
		D3DXVec3TransformCoord(&m_LookAt, &at, &m_MtxRot);
		D3DXVec3TransformCoord(&m_LookAt, &m_LookAt, &m_MtxPos);
	}

	// ���X�e�B�b�N���͂��Ȃ�������
	if (!CManager::GetInputGamePad()->CheckInputL())
	{
		m_LookAt.y = GetPosition().y;
		
		// �ړ��s��X�V
		D3DXMatrixTranslation(&m_MtxPos, pos.x, pos.y, pos.z);
	}

	if (CReticle::GetLockOnMode())
	{
	}

#ifdef _DEBUG
	// AT�p�I�u�W�F�Ɍ��ʂ𔽉f

	if (CSceneManager::GetDebugObj()->GetData("playerLookAt"))
	{
		CSceneManager::GetDebugObj()->GetData("playerLookAt")->SetPosition(m_LookAt);
	}
#endif

}

//=================================================================================
//	���W�ړ��֐�
//=================================================================================
void CPlayer::MovePlayer( void )
{
	// �J�����擾
	CCameraManager *pCameraManager = CSceneManager::GetCameraManager();
	CCamera *pCamera = pCameraManager->GetCamera(MAIN_CAMERA);

	D3DXVECTOR3 pos = GetPosition();
	m_prevPos = pos;

	// �Q�[���p�b�h���͂���������
	// X���W�X�V
	if (fabs((float)CManager::GetInputGamePad()->GetDInput().lX) > 250)
	{
		pos.x += fabs((float)CManager::GetInputGamePad()->GetDInput().lX) * 0.00125f * m_VecDir.x * m_Speed.x;
		pos.z += fabs((float)CManager::GetInputGamePad()->GetDInput().lX) * 0.00125f * m_VecDir.z * m_Speed.z;

		m_pState->SetState(new CMovePlayer);
	}
	// Z���W�X�V
	if (fabs((float)CManager::GetInputGamePad()->GetDInput().lY) > 250)
	{
		pos.x += fabs((float)CManager::GetInputGamePad()->GetDInput().lY) * 0.00125f * m_VecDir.x * m_Speed.x;
		pos.z += fabs((float)CManager::GetInputGamePad()->GetDInput().lY) * 0.00125f * m_VecDir.z * m_Speed.z;
		pos.y -= (float)CManager::GetInputGamePad()->GetDInput().lY * 0.003f * m_VecDir.y * m_Speed.y;

		m_pState->SetState(new CMovePlayer);
	}
	// A�{�^�����������Ƃ�
	if (CManager::GetInputGamePad()->GetPress(DIJOFS_BUTTON0))
	{
		pos.y -= 5.0f;
		m_pState->SetState(new CFallPlayer);
	}
	// B�{�^�����������Ƃ�
	if (CManager::GetInputGamePad()->GetPress(DIJOFS_BUTTON1))
	{
		pos = m_prevPos;
		m_pState->SetQuickState(new CGardPlayer);
	}
	// X�{�^�����������Ƃ�
	if (CManager::GetInputGamePad()->GetPress(DIJOFS_BUTTON2))
	{
		m_pState->SetQuickState(new CAttackPlayer);
	}
	// Y�{�^�����������Ƃ�
	if (CManager::GetInputGamePad()->GetPress(DIJOFS_BUTTON3))
	{
		pos.y += 5.0f;
		m_pState->SetQuickState(new CRisePlayer);
	}
	// R�{�^�����������Ƃ�
	if (CManager::GetInputGamePad()->GetTrigger(DIJOFS_BUTTON5) && (m_Mp >= 10.0f))
	{
		m_pState->SetQuickState(new CDashPlayer);

#ifdef SE_PLAY_PROC
		// BGM�Đ�
		PlaySound(SOUND_LABEL_SE_BOOST);
#endif
		// �u�[�X�g
		m_Speed.x = 7.0f;
		m_Speed.z = 7.0f;
		m_Speed.y = 7.0f;
	}

	if (m_Speed.x > 1.0f) m_Speed.x *= 0.92f;
	if (m_Speed.z > 1.0f) m_Speed.z *= 0.92f;
	if (m_Speed.y > 1.0f) m_Speed.y *= 0.92f;

	// X�{�^�����������Ƃ�
	if (CManager::GetInputGamePad()->GetTrigger(DIJOFS_BUTTON2) ||
		CManager::GetInputKeyboard()->GetKeyTrigger(DIK_RETURN))
	{
		if (CReticle::GetLockOnMode())
		{
			/*CNormalBullet* pNormalBullet = new CNormalBullet;
			pNormalBullet->SetPosition( D3DXVECTOR3( pos.x, pos.y, pos.z ) );
			pNormalBullet->SetHeight( 1.65f );
			pNormalBullet->SetWidth( 1.65f );
			pNormalBullet->SetTexHeight( 1.0f );
			pNormalBullet->SetTexWidth( 1.0f );
			pNormalBullet->SetTexture( "data/TEXTURE/effect000.jpg" ) ;
			pNormalBullet->SetScale( D3DXVECTOR3( 1.0f, 1.0f, 1.0f ) );
			pNormalBullet->SetColor( D3DXCOLOR( 1.0f, 1.0f, 0.3f, 0.85f ) );
			pNormalBullet->SetSpeed( 3.5f );
			pNormalBullet->SetAttack( 5.0f );
			pNormalBullet->SetLife( 100 );
			pNormalBullet->Init();*/
			CHomingBullet* pHomingBullet = new CHomingBullet;
			pHomingBullet->SetPosition(D3DXVECTOR3(pos.x, pos.y, pos.z));
			pHomingBullet->SetHeight(1.65f);
			pHomingBullet->SetWidth(1.65f);
			pHomingBullet->SetTexHeight(1.0f);
			pHomingBullet->SetTexWidth(1.0f);
			pHomingBullet->SetTexture("data/TEXTURE/effect000.jpg");
			pHomingBullet->SetScale(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
			pHomingBullet->SetColor(D3DXCOLOR(1.0f, 1.0f, 0.3f, 0.85f));
			pHomingBullet->SetSpeed(2.5f);
			pHomingBullet->SetAttack(1.0f);
			pHomingBullet->SetLife(300);
			pHomingBullet->SetVecDir(D3DXVECTOR3(GetVecDir().x, GetVecDir().y, GetVecDir().z));
			pHomingBullet->Init();
		}
	}
	// �L�[�{�[�h���͂���������
	float move = 1.0f;
	// X���W�X�V
	if (CManager::GetInputKeyboard()->GetKeyPress(DIK_D))
	{
		D3DXVECTOR3 vecDir = pCamera->GetVecDir();
		pos.x += move * vecDir.z;
		pos.z -= move * vecDir.x;
	}
	// X���W�X�V
	if (CManager::GetInputKeyboard()->GetKeyPress(DIK_A))
	{
		D3DXVECTOR3 vecDir = pCamera->GetVecDir();
		pos.x -= move * vecDir.z;
		pos.z += move * vecDir.x;
	}
	// Z���W�X�V
	if (CManager::GetInputKeyboard()->GetKeyPress(DIK_W))
	{
		D3DXVECTOR3 vecDir = pCamera->GetVecDir();
		pos.z += move * vecDir.z;
		pos.x += move * vecDir.x;
	}
	// Z���W�X�V
	if (CManager::GetInputKeyboard()->GetKeyPress(DIK_S))
	{
		D3DXVECTOR3 vecDir = pCamera->GetVecDir();
		pos.z -= move * vecDir.z;
		pos.x -= move * vecDir.x;
	}
	// Y���W�X�V
	if (CManager::GetInputKeyboard()->GetKeyPress(DIK_SPACE))
	{
		pos.y += 1.0f;
	}
	// Y���W�X�V
	if (CManager::GetInputKeyboard()->GetKeyPress(DIK_LSHIFT))
	{
		pos.y -= 1.0f;
	}

	// �n�ʂɂ��Ă���Ƃ�
	if (m_bOnGround)
	{
		// �n�ʂɐڒn������(����)
		pos.y -= 1.0f;
	}

	// �n�ʂƂ̐ڒn����
	CMeshField *pMeshField = CSceneManager::GetMeshFieldManager()->GetData(GAME_MAIN_STAGE);

	if(pMeshField != NULL)
	{
		if (pos.y < pMeshField->GetPosHeight(pos))
		{
			pos.y = pMeshField->GetPosHeight(pos);
			m_bOnGround = true;
		}
		else
		{
			m_bOnGround = false;
		}
	}

	SetPosition(pos);
}

//=================================================================================
//	���f����]
//=================================================================================
void CPlayer::RotatePlayer( void )
{
	// �v���C���[�ʒu
	D3DXVECTOR3 pos = GetPosition();
	
	// �J���������_
	D3DXVECTOR3 at = CSceneManager::GetCameraManager()->GetCamera(MAIN_CAMERA)->GetLookAt();

	// �v���C���[���J���������_�̃x�N�g��
	m_CameraLookAtVecDir = at - pos;

	// �J�����̒����_�����Ɍ�������
	CalcLookAtMatrix( &m_MtxRot, &pos, &at, &D3DXVECTOR3(0.0f, 1.0f, 0.0f) );
}

//=================================================================================
//	�f�o�b�O�\��
//=================================================================================
#ifdef _DEBUG
void CPlayer::ShowParam( void )
{
	//-------------------------�f�o�b�O���-------------------------
	char str[256];
	CDebug::AddText( "PLAYER _/_/_/_/_/_/_/\n" );
	sprintf_s(str, "m_Position( %f, %f, %f )\n", GetPosition().x, GetPosition().y, GetPosition().z );
	CDebug::AddText( str );
	sprintf_s(str, "m_VecDir( %f, %f, %f )\n", m_VecDir.x, m_VecDir.y, m_VecDir.z );
	CDebug::AddText( str );
	sprintf_s(str, "m_LookAt( %f, %f, %f )\n", m_LookAt.x, m_LookAt.y, m_LookAt.z );
	CDebug::AddText( str );
	sprintf_s(str, "m_fXRot : %f\n", m_fXRot );
	CDebug::AddText( str );
	sprintf_s(str, "m_fYRot : %f\n", m_fYRot );
	CDebug::AddText( str );
}
#endif