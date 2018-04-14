//=================================================================================
//	�v���O������	�F	
//	����			�F	�W�I����[reticle.cpp]
//	���C������		�F	main.cpp
//	�쐬��			�F	moriya gaku
//=================================================================================

//=================================================================================
//	�C���N���[�h�t�@�C��
//=================================================================================
#include "reticle.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "debug.h"
#include "sceneManager.h"
#include "scene.h"
#include "sceneX.h"
#include "player.h"
#include "enemy.h"
#include "cameraManager.h"

// ���b�N�I�����[�h
bool CReticle::m_bLockOn = false;
float CReticle::m_ViewAngle = 0.0f;
D3DXVECTOR3 CReticle::m_VecDir = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
D3DXVECTOR3 CReticle::m_3DPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

//=================================================================================
// ����������
//=================================================================================
HRESULT CReticle::Init(void)
{
	CScene2D::Init();

	return S_OK;
}

//=================================================================================
// �I������
//=================================================================================
void CReticle::Uninit(void)
{
	CScene2D::Uninit();
}

//=================================================================================
// �X�V����
//=================================================================================
void CReticle::Update(void)
{
	CScene2D::Update();

	// �v���C���[�擾
	CGame *pGame = dynamic_cast<CGame*>(CSceneManager::GetScene());
	CPlayer *pPlayer = pGame->GetPlayer();

	// �J�����擾
	CCameraManager *pCameraManager = CSceneManager::GetCameraManager();
	CCamera *pCamera = pCameraManager->GetCamera(MAIN_CAMERA);

	CScene *pScene = NULL;

	// L1�{�^�����������Ƃ�,������̓G�����b�N�I������
	if( CManager::GetInputGamePad()->GetTrigger( DIJOFS_BUTTON4 ))
	{
		pScene = CScene::GetList( DEFAULT_PRIORITY );
		while( pScene != NULL )
		{
			CScene::SCENE_TYPE type = pScene->GetType();
			if( type == CScene::SCENE_TYPE_ENEMY )
			{
				CEnemy *pEnemy = dynamic_cast<CEnemy *>(pScene); 

				// �v���C���[���͂ɂ���G��T��
				if( CheckColSphere( (D3DXVECTOR3&)pEnemy->GetPosition(), 
					(D3DXVECTOR3&)pPlayer->GetPosition(), 5.0f, 500.0f ) )
				{
					// ������ɓG�����Ȃ�������
					// P�����`�N���x�N�g�� �� P���G�x�N�g�� �̂Ȃ��p���v�Z
					float rad = -(fabs( CalcVecAngle( m_VecDir, (D3DXVECTOR3 &)pEnemy->GetVecDir() )) - D3DX_PI);
					float angle = 0.0f;
					if( rad != 0.0f ){
						angle = rad / (D3DX_PI / 180.0f);
					}
					// ����p��������
					if( angle < m_ViewAngle )
					{
						if( !pEnemy->GetLockOnFlg() )
						{
							// ���b�N�I��
							pEnemy->SetLockOnFlg( true );
							// ���b�N�I�����[�h��
							m_bLockOn = true;
						}
						else
						{
							pEnemy->SetLockOnFlg( false );
						}
					}
				}
			}
			pScene = pScene->GetNextScene();
		}
	}

	// L2�{�^�����������Ƃ�
	if( CManager::GetInputGamePad()->GetTrigger( DIJOFS_BUTTON7 ) )
	{
		// ���b�N�I�����[�h�I��
		m_bLockOn = false;
		pScene = CScene::GetList( DEFAULT_PRIORITY );
		while( pScene != NULL )
		{
			CScene::SCENE_TYPE type = pScene->GetType();
			if( type == CScene::SCENE_TYPE_ENEMY )
			{
				CEnemy *pEnemy = dynamic_cast<CEnemy *>(pScene);
				// �S�G�̃��b�N�I���ς݃t���O��؂�
				pEnemy->SetLockOnFlg( false );
			}
			pScene = pScene->GetNextScene();
		}
	}

	// �J���������_���W���Z�b�g(�񃍃b�N�I�����[�h)
	m_3DPosition = pCamera->GetLookAt();
	m_3DPosition.y += 20.0f;

	// ���b�N�I�����̓G���W�����炤(���b�N�I�����[�h)
	if( m_bLockOn )
	{
		// ���b�N�I�����̓G��������Ȃ�������A
		// ���b�N�I�����[�h���I������
		m_bLockOn = false;
		pScene = CScene::GetList( DEFAULT_PRIORITY );
		while( pScene != NULL )
		{
			CScene::SCENE_TYPE type = pScene->GetType();
			if( type == CScene::SCENE_TYPE_ENEMY )
			{
				CEnemy *pEnemy = dynamic_cast<CEnemy *>(pScene);
		
				if( pEnemy->GetLockOnFlg() )
				{// ���b�N�I������Ă�G��������
					m_3DPosition = pEnemy->GetPosition();
					m_bLockOn = true;
				}
			}
			pScene = pScene->GetNextScene();
		}
	}

	// ���W�ϊ� 3������2����
	D3DXMATRIX mtxWorld;
	D3DXMatrixIdentity( &mtxWorld );
	D3DXMATRIX mtxView = pCamera->GetViewMtx();
	D3DXMATRIX mtxProj = pCamera->GetProjMtx();
	D3DXMATRIX mtxViewPort = pCamera->GetViewPortMtx();
	D3DXVECTOR4 vecOut = D3DXVECTOR4( 0.0f, 0.0f, 0.0f, 0.0f );
	D3DXVECTOR4 vecPos = D3DXVECTOR4( m_3DPosition.x, m_3DPosition.y, m_3DPosition.z, 1.0f );
	Vec4Transform( &vecOut, &vecPos, &mtxWorld );
	Vec4Transform( &vecOut, &vecOut, &mtxView );
	Vec4Transform( &vecOut, &vecOut, &mtxProj );
	Vec4Transform( &vecOut, &vecOut, &mtxViewPort );
	vecOut.x = vecOut.x / vecOut.w;
	vecOut.y = vecOut.y / vecOut.w;
	SetPosition( D3DXVECTOR3( vecOut.x - (RETICLE_WIDTH * 0.5f), vecOut.y - (RETICLE_HEIGHT * 0.5f), vecOut.z ) );

	// �����v�Z
	CalcPositionToVector( &m_VecDir, m_3DPosition, pPlayer->GetPosition() );
	D3DXVec3Normalize( &m_VecDir, &m_VecDir );

#ifdef _DEBUG
	ShowParam();
#endif
}

//=================================================================================
// �`�揈��
//=================================================================================
void CReticle::Draw(void)
{
	//CScene2D::Draw();
}

//=================================================================================
// �f�o�b�O�\��
//=================================================================================
#ifdef _DEBUG
void CReticle::ShowParam( void )
{
	//-------------------------�f�o�b�O���-------------------------
	char str[256];
	CDebug::AddText( "RETICLE _/_/_/_/_/_/_/\n" );
	sprintf_s(str, "m_Position( %f, %f, %f )\n", GetPosition().x, GetPosition().y, GetPosition().z );
	CDebug::AddText( str );
	sprintf_s(str, "m_VecDir( %f, %f, %f )\n", m_VecDir.x, m_VecDir.y, m_VecDir.z );
	CDebug::AddText( str );
	sprintf_s(str, "LOCKON_MODE : %s\n", m_bLockOn ? "TRUE" : "FALSE" );
	CDebug::AddText( str );

}
#endif