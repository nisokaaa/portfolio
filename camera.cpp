//=================================================================================
//	�v���O������	�F	
//	����			�F	�J��������[camera.cpp]
//	���C������		�F	main.cpp
//	�쐬��			�F	moriya gaku
//=================================================================================

//=================================================================================
//	�C���N���[�h�t�@�C��
//=================================================================================
#include "camera.h"
#include "input.h"
#include "manager.h"
#include "renderer.h"
#include "player.h"
#include "debug.h"
#include "debugObj.h"
#include "sceneManager.h"
#include "reticle.h"

//=================================================================================
//	�R���X�g���N�^
//=================================================================================
CCamera::CCamera()
{
	m_Position		= D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	m_LookAt		= D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	m_LookAtTarget	= D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	m_VecDir		= D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	m_Distance		= 0.0f;
	m_fYRot			= 0.0f;
	m_fXRot			= 0.0f;
	m_RotTarget		= 0.0f;
	m_InterpolationEnable = false;
	m_bInterpolationDuring = false;
	D3DXMatrixIdentity( &m_MtxView );
	D3DXMatrixIdentity( &m_MtxProj );
	D3DXMatrixIdentity( &m_MtxViewPort );
	D3DXMatrixIdentity( &m_MtxRot );
	D3DXMatrixIdentity( &m_MtxPos );
}

//=================================================================================
//	�f�X�g���N�^
//=================================================================================
CCamera::~CCamera()
{
}

//=================================================================================
//	�������֐�
//=================================================================================
void CCamera::Init( D3DXVECTOR3 pos, float distance, float middleAtOffSetY )
{
	m_Position			= pos;
	m_Distance			= distance;
	m_MiddleAtOffSetY	= middleAtOffSetY; 

	return;
}

//=================================================================================
//	�I���֐�
//=================================================================================
void CCamera::Uninit( void )
{
	return;
}

//=================================================================================
//	�X�V�֐�
//=================================================================================
void CCamera::Update( void )
{
	// CTitle
	if (CTitle* pClassTitle = dynamic_cast<CTitle*>(CSceneManager::GetScene()))
	{
		m_Position = D3DXVECTOR3(0.0f, 50.0f, -135.0f);
		m_LookAt = D3DXVECTOR3(0.0, 0.0, m_Distance);
	}

	// CGame
	if (CGame* pClassGame = dynamic_cast<CGame*>(CSceneManager::GetScene()))
	{
		// �I�u�W�F�擾
		CScene* pScene = CScene::GetList(DEFAULT_PRIORITY);

		// �X�e�B�b�N���͂����]�s����쐬
		CreateRotMatrix();

		// �v���C���[�ʒu����ړ��s����쐬
		CreatePosMatrix();

		// ���J�������W���I�t�Z�b�g�ݒ�
		D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, -m_Distance);

		// ���J�������W����]
		D3DXVec3TransformCoord(&m_Position, &pos, &m_MtxRot);

		// ���J�������W���ړ�
		D3DXVec3TransformCoord(&m_Position, &m_Position, &m_MtxPos);

		// �������_���I�t�Z�b�g�ݒ�
		D3DXVECTOR3 at = D3DXVECTOR3(0.0f, 0.0f, m_Distance + 50.0f);

		// �������_����]
		D3DXVec3TransformCoord(&m_LookAt, &at, &m_MtxRot);

		// �������_���ړ�
		D3DXMATRIX mtxPos = m_MtxPos;
		mtxPos.m[3][1] -= m_MiddleAtOffSetY; // Y�������炷
		D3DXVec3TransformCoord(&m_LookAt, &m_LookAt, &mtxPos);

		// �����_�p�I�u�W�F���ړ�
		CSceneManager::GetDebugObj()->GetData("cameraLookAt")->SetPosition(m_LookAt);

		// �J�����̌������v�Z
		// �v���C���[�擾
		CGame *pGame = dynamic_cast<CGame*>(CSceneManager::GetScene());
		CPlayer *pPlayer = pGame->GetPlayer();

		m_VecDir = pPlayer->GetPosition() - m_Position;

		//--------�L�[���͂� Distance, middleAtOffSetY �ύX---------
#ifdef _DEBUG
		if (CManager::GetInputKeyboard()->GetKeyTrigger(DIK_U))
		{
			m_MiddleAtOffSetY += 1.0f;
		}
		if (CManager::GetInputKeyboard()->GetKeyTrigger(DIK_J))
		{
			m_MiddleAtOffSetY -= 1.0f;
		}
		if (CManager::GetInputKeyboard()->GetKeyTrigger(DIK_I))
		{
			m_Distance += 1.0f;
		}
		if (CManager::GetInputKeyboard()->GetKeyTrigger(DIK_K))
		{
			m_Distance -= 1.0f;
		}

		// �p�����[�^�\��
		ShowParam();
#endif
	}

	// CResult
	if (CResult* pClassResult = dynamic_cast<CResult*>(CSceneManager::GetScene()))
	{
		m_Position = D3DXVECTOR3(0.0f, 50.0f, -135.0f);
		m_LookAt = D3DXVECTOR3(0.0, 0.0, m_Distance);
	}
}

//=================================================================================
//	��]�s����쐬
//=================================================================================
void CCamera::CreateRotMatrix( void )
{
	D3DXMATRIX mtxRotY;
	D3DXMATRIX mtxRotX;

	// ������
	D3DXMatrixIdentity(&mtxRotX);
	D3DXMatrixIdentity(&mtxRotY);

	// �p�b�h���͎擾
	CInputGamePad *pad = CManager::GetInputGamePad();

	if (CManager::GetInputGamePad()->CheckInputR())
	{// �E�X�e�B�b�N���͂�����
		m_fXRot += (float)pad->GetDInput().lRy * 0.00005f;
		m_fYRot += (float)pad->GetDInput().lRx * 0.00005f;
	}

	if (pad->CheckInputL() && !CReticle::GetLockOnMode())
	{// ���X�e�B�b�N���͂�����Ƃ� ���� ���b�N�I�����[�h����Ȃ��Ƃ�

		// �J������Ԃ����Ԃ�
		m_InterpolationEnable = true;
	}
	if (!pad->CheckInputL() && m_InterpolationEnable)
	{// ���X�e�B�b�N���͂��Ȃ� ���� ��Ԃ��ĂȂ� ���@���b�N�I�����[�h�ł͂Ȃ�

		// �Q�x�N�g���Ԃ̉�]�ʌv�Z
		CGame *pGame = dynamic_cast<CGame*>(CSceneManager::GetScene());
		CPlayer *pPlayer = pGame->GetPlayer();

		D3DXVECTOR3 vec1 = pPlayer->GetVecDir();
		D3DXVECTOR3 vec2 = m_VecDir;
		vec1.y = 0.0f; // ���ʃx�N�g���Ōv�Z
		vec2.y = 0.0f;
		m_RotTarget = CalcVecAngle(vec1, vec2);

		// ���t���\���܂ňȏ㏈���͍s��Ȃ�
		m_InterpolationEnable = false;

		// ��Ԓ��t���O�I��(�J�n)
		m_bInterpolationDuring = true;
	}


	// �L�[�{�[�h���͂���������
	float move = 0.02f;
	// �����
	if (CManager::GetInputKeyboard()->GetKeyPress(DIK_UP)) {
		m_fXRot -= move;
	}
	// ������
	if (CManager::GetInputKeyboard()->GetKeyPress(DIK_DOWN)) {
		m_fXRot += move;
	}
	// �E����
	if (CManager::GetInputKeyboard()->GetKeyPress(DIK_RIGHT)) {
		m_fYRot += move;
	}
	// ������
	if (CManager::GetInputKeyboard()->GetKeyPress(DIK_LEFT)) {
		m_fYRot -= move;
	}

	// ���b�N�I���ΏۂփJ��������]���ĕ��
	if (CReticle::GetLockOnMode()) {

		// ��Ԓ��t���O�I��(�J�n)
		m_bInterpolationDuring = true;

		// �Q�x�N�g���Ԃ̉�]�ʌv�Z�FY��]
		D3DXVECTOR3 vec1 = CReticle::GetVecDir();
		D3DXVECTOR3 vec2 = m_VecDir;
		vec1.y = 0.0f; // ���ʃx�N�g���Ōv�Z
		vec2.y = 0.0f;
		m_RotTarget = CalcVecAngle(vec1, vec2);

	}

	// ��]�ʕ��
	if (m_bInterpolationDuring)
	{
		// Y��]��
		m_fYRot += m_RotTarget * 0.1f;
		m_RotTarget -= m_RotTarget * 0.1f;

		if (fabs(m_RotTarget) < 0.05f)
		{
			// ��Ԓ��t���O�I�t(�I��)
			m_bInterpolationDuring = false;
		}
	}

	// ��]�s��쐬
	D3DXMatrixRotationY(&mtxRotY, m_fYRot);
	D3DXMatrixRotationX(&mtxRotX, m_fXRot);
	// ����
	m_MtxRot = mtxRotX * mtxRotY;
}

//=================================================================================
//	�ړ��s����쐬
//=================================================================================
void CCamera::CreatePosMatrix( void )
{
	// �v���C���[�擾
	CGame *pGame = dynamic_cast<CGame*>(CSceneManager::GetScene());
	CPlayer *pPlayer = pGame->GetPlayer();

	// �v���C���[�ʒu�����Ɉړ��s����쐬
	D3DXMatrixTranslation(&m_MtxPos, pPlayer->GetPosition().x, pPlayer->GetPosition().y + m_MiddleAtOffSetY, pPlayer->GetPosition().z);

	m_MtxPos = m_MtxPos;
}

//=================================================================================
//	�f�o�b�O�\��
//=================================================================================
void CCamera::ShowParam( void )
{
	// �f�o�b�O���
	char str[256];
	CDebug::AddText( "CAMERA _/_/_/_/_/_/_/\n" );
	sprintf_s(str, "m_Position( %f, %f, %f )\n", m_Position.x, m_Position.y, m_Position.z );
	CDebug::AddText( str );
	sprintf_s(str, "m_LookAt( %f, %f, %f )\n", m_LookAt.x, m_LookAt.y, m_LookAt.z );
	CDebug::AddText( str );
	sprintf_s(str, "m_TargetAt( %f, %f, %f )\n", CReticle::Get3DPosition().x, CReticle::Get3DPosition().y, CReticle::Get3DPosition().z );
	CDebug::AddText( str );
	sprintf_s(str, "m_VecDir( %f, %f, %f )\n", m_VecDir.x, m_VecDir.y, m_VecDir.z );
	CDebug::AddText( str );
	sprintf_s(str, "STIC lX,lY( %f, %f )\n", (float)CManager::GetInputGamePad()->GetDInput().lX, (float)CManager::GetInputGamePad()->GetDInput().lY );
	CDebug::AddText( str );
	sprintf_s(str, "m_bInterpolationDuring : %s\n", m_bInterpolationDuring ? "TRUE" : "FALSE" );
	CDebug::AddText( str );
	sprintf_s(str, "m_RotTarget : %f\n", m_RotTarget );
	CDebug::AddText( str );
	CDebug::AddText( "\n" );
}

//=================================================================================
//	�v���W�F�N�V�����E�r���[�}�g���N�X�ݒ�
//=================================================================================
void CCamera::Set( void )
{
	// Direct3D�f�o�C�X
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	
	if( pDevice != NULL )
	{
		// �r���[�}�g���N�X�쐬
		D3DXMatrixLookAtLH( &m_MtxView, &m_Position, &m_LookAt, &D3DXVECTOR3(0.0f, 1.0f, 0.0f) );
		
		// �f�o�C�X�Ƀr���[�ϊ��s����w��
		pDevice -> SetTransform( D3DTS_VIEW, &m_MtxView );

		// �v���W�F�N�V�����}�g���N�X�̍쐬
		D3DXMatrixPerspectiveFovLH( &m_MtxProj,
			D3DX_PI / 3.0f,
			( float )SCREEN_WIDTH / SCREEN_HEIGHT,
			CAMERA_AT_NEAR,
			CAMERA_AT_FAR );

		// �v���W�F�N�V�����}�g���N�X���w��
		pDevice -> SetTransform( D3DTS_PROJECTION, &m_MtxProj );

		// �r���[�|�[�g�s��̍쐬
		m_MtxViewPort._11 = SCREEN_WIDTH / 2.0f;
		m_MtxViewPort._12 = 0.0f;
		m_MtxViewPort._13 = 0.0f;
		m_MtxViewPort._14 = 0.0f;

		m_MtxViewPort._21 = 0.0f;
		m_MtxViewPort._22 = -SCREEN_HEIGHT / 2.0f;
		m_MtxViewPort._23 = 0.0f;
		m_MtxViewPort._24 = 0.0f;

		m_MtxViewPort._31 = 0.0f;
		m_MtxViewPort._32 = 0.0f;
		m_MtxViewPort._33 = 1.0f;
		m_MtxViewPort._34 = 0.0f;

		m_MtxViewPort._41 = SCREEN_WIDTH / 2.0f;
		m_MtxViewPort._42 = SCREEN_HEIGHT / 2.0f;
		m_MtxViewPort._43 = 0.0f;
		m_MtxViewPort._44 = 1.0f;
	}
	else
	{
		// �f�o�C�X���擾�ł��Ȃ������ꍇ
		MessageBox( NULL, "�f�o�C�X�̎擾�Ɏ��s���܂���", "GET_DEVICE_ERROR", MB_OK );
		return;
	}

	return;

}