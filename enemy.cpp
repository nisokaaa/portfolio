//=================================================================================
//	�v���O������	�F	
//	����			�F	�G����[enemy.cpp]
//	���C������		�F	main.cpp
//	�쐬��			�F	moriya gaku
//=================================================================================

//=================================================================================
//	�C���N���[�h�t�@�C��
//=================================================================================
#include "manager.h"
#include "renderer.h"
#include "meshField.h"
#include "cameraManager.h"
#include "enemy.h"
#include "player.h"
#include "scene.h"
#include "scene2D.h"
#include "input.h"
#include "fade.h"
#include "sceneManager.h"
#include "gauge.h"
#include "reticle.h"

int CEnemy::m_Count = 0;

//=================================================================================
// ����������
//=================================================================================
HRESULT CEnemy::Init(void)
{
	CSceneX::Init();
	
	m_ColLength = 4.0f;

	// �g�o�Q�[�W
	/*m_pHpGauge = CGauge::Create("data/TEXTURE/gauge3.png", "data/TEXTURE/gauge_back3.png", D3DXVECTOR2(0.0f, 0.0f), D3DXVECTOR2(70.0f, 4.0f), m_Hp, m_Hp, TRUE, 5);
	m_pHpGauge->SetType( CScene::SCENE_TYPE_HP_GAUGE );*/

	return S_OK;
}

//=================================================================================
// �I������
//=================================================================================
void CEnemy::Uninit(void)
{
	// ���f��
	CSceneX::Uninit();

	CEnemy::SetCount( CEnemy::GetCount() - 1 );
}

//=================================================================================
// �X�V����
//=================================================================================
void CEnemy::Update(void)
{
	CSceneX::Update();

	// �J�����擾
	CCameraManager *pCameraManager = CSceneManager::GetCameraManager();
	CCamera *pCamera = pCameraManager->GetCamera(MAIN_CAMERA);

	D3DXVECTOR3 pos = GetPosition();
	
	static float speedY = 0.1f;
	static float speedZ = 0.1f;

	// ��Ԕ��f
	switch( m_State )
	{
	case NEUTRAL:
		// �����T�b��
		if(m_Time % (60 * 1) == 0 )
		{
			// �U���Ԑ�
			m_State = CHASE;
		}
		break;

	case CHASE:
		// �ǐՂT�b��
		if(m_Time % (60 * 10) == 0 )
		{
			// �U���Ԑ�
			//m_State = NEUTRAL;
		}

		// �v���C���[�֒��i
		//pos += m_Speed * m_VecDir;
		if( pos.x > 100.0f || pos.x < -100.0f ) m_Speed *= -1.0f;
		if( pos.y > 100.0f || pos.y < 0.0f ) speedY *= -1.0f;
		if( pos.z > 180.0f || pos.z < 80.0f ) speedZ *= -1.0f;
		
		pos.x += m_Speed;
		pos.y += speedY;
		pos.z += speedZ;

		break;

	case ATTACK:
		// �U���T�b��
		if((m_Time % (60 * 1)) == 0 )
		{
			// �ǐ�
			m_State = CHASE;
			break;
		}
		break;

	case DETH:
		// �폜�˗�
		Uninit();
		return;

	default:
		break;
	}

	// �v���C���[���擾
	CPlayer *pPlayer = NULL;
	CScene *pScene = CScene::GetList( DEFAULT_PRIORITY );
	while( pScene != NULL )
	{
		CScene::SCENE_TYPE type = pScene->GetType();
		if( type == CScene::SCENE_TYPE_PLAYER )
		{
			pPlayer = dynamic_cast<CPlayer *>(pScene);
			break;
		}
		pScene = pScene->GetNextScene();
	}

	// �G���v���C���[�@�x�N�g���ݒ�
	m_VecDir = pPlayer->GetPosition() - pos;

	// �Q�[�W�ݒ�(3�������W��2������
	float offsetY = 3.0f;
	D3DXMATRIX mtxWorld;
	D3DXMatrixIdentity( &mtxWorld );
	D3DXMATRIX mtxView = pCamera->GetViewMtx();
	D3DXMATRIX mtxProj = pCamera->GetProjMtx();
	D3DXMATRIX mtxViewPort = pCamera->GetViewPortMtx();
	D3DXVECTOR4 vecOut = D3DXVECTOR4( 0.0f, 0.0f, 0.0f, 0.0f );
	D3DXVECTOR4 vecPos = D3DXVECTOR4( pos.x, pos.y + offsetY, pos.z, 1.0f );
	Vec4Transform( &vecOut, &vecPos, &mtxWorld );
	Vec4Transform( &vecOut, &vecOut, &mtxView );
	Vec4Transform( &vecOut, &vecOut, &mtxProj );
	Vec4Transform( &vecOut, &vecOut, &mtxViewPort );
	vecOut.x = vecOut.x / vecOut.w;
	vecOut.y = vecOut.y / vecOut.w;
	m_pHpGauge->SetPosition( vecOut.x, vecOut.y, vecOut.z );
	
	// HP���Q�[�W�ɔ��f
	m_pHpGauge->SetVal( m_Hp );

	// ���W�X�V
	SetPosition( pos );

	// �����Q�O�b�� ���S
	//if(m_Time == 60 * 60 ) m_State = STATE::DETH;

	// �̗͂��O ���S
	if( m_Hp < 0.0f || m_Hp == 0.0f){
		m_State = DETH;
	}

	// �v���C���[�Ƃ̓����蔻��true ���S
	//if( CheckColSphere( (D3DXVECTOR3&)pPlayer->GetPosition(), (D3DXVECTOR3&)GetPosition(), 5.0f, 5.0f )){
	//	m_State = DETH;
	//	// HP(val)�ύX
	//	pPlayer->SetHp( pPlayer->GetHp() - 10.0f );
	//}

	// �o�ߎ��Ԃ������߂�
	m_Time += 1;
}

//=================================================================================
// �`�揈��
//=================================================================================
void CEnemy::Draw(void)
{
	CSceneX::Draw();
}