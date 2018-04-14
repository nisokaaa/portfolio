//=================================================================================
//	�v���O������	�F	
//	����			�F	�e����[bullet.cpp]
//	���C������		�F	main.cpp
//	�쐬��			�F	moriya gaku
//=================================================================================

//=================================================================================
//	�C���N���[�h�t�@�C��
//=================================================================================
#include "bullet.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "player.h"
#include "enemy.h"
#include "sound.h"
#include "reticle.h"
#include "debug.h"

//=================================================================================
// ����������
//=================================================================================
HRESULT CBullet::Init( void )
{
	// �e�N���X�̏�����
	CBillboard::Init();

	return S_OK;
}

//=================================================================================
// �I������
//=================================================================================
void CBullet::Uninit( void )
{
	// �e�N���X�̏I������
	CBillboard::Uninit();

}

//=================================================================================
// �X�V����
//=================================================================================
void CBullet::Update( void )
{
	// �e�N���X�̍X�V����
	CBillboard::Update();

	if( m_Life > 0 )
	{
		
		// ���W�ړ�
		Move();

		// �����蔻��
		CEnemy *pEnemy;
		CScene *pScene = CScene::GetList( DEFAULT_PRIORITY );
		while( pScene != NULL )
		{
			CScene::SCENE_TYPE type = pScene->GetType();
			if( type == CScene::SCENE_TYPE_ENEMY )
			{
				pEnemy = dynamic_cast<CEnemy *>(pScene);
			
				if( CheckColSphere( (D3DXVECTOR3&)pEnemy->GetPosition(), (D3DXVECTOR3&)GetPosition(), 5.0f, 5.0f ) )
				{
					// SE�Đ�
					/*PlaySound( SOUND_LABEL_SE_EXPLOSION );*/
				
					// HP�����炷
					pEnemy->SetHp( pEnemy->GetHp() - m_Attack );

					// �����s����
					m_Life = 0;

					// �e�̔j��
					Uninit();

					break;
				}

			}
			pScene = pScene->GetNextScene();
		}
		
		// �������k�߂�
		m_Life -= 1;
	}
	else
	{
		// �����蔻�肪�s��ꂸ�A�������s�����ꍇ
		Uninit();
	}

	/*char str[256];
	CDebug::AddText( "BULLET _/_/_/_/_/_/_/\n" );
	sprintf_s(str, "m_Life : %d\n", m_Life );
	CDebug::AddText( str );
	sprintf_s(str, "m_VecDir( %f, %f, %f )\n", m_VecDir.x, m_VecDir.y, m_VecDir.z );
	CDebug::AddText( str );
	sprintf_s(str, "m_Position( %f, %f, %f )\n", GetPosition().x, GetPosition().y, GetPosition().z );
	CDebug::AddText( str );*/

	return;
}

//=================================================================================
// �`�揈��
//=================================================================================
void CBullet::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CRenderer::GetDevice();

	if( pDevice != NULL )
	{
		// ���Z�������g�p����ݒ�
		pDevice -> SetRenderState( D3DRS_BLENDOP, D3DBLENDOP_ADD );
		pDevice -> SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
		pDevice -> SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );

		// �e�N���X�̕`�揈��
 		CBillboard::Draw();

		// ���Z�������g�p����ݒ�߂�
		pDevice -> SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );			
		pDevice -> SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
		pDevice -> SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
	}
	else
	{
		// �f�o�C�X���擾�ł��Ȃ������ꍇ
		MessageBox( NULL, "�f�o�C�X�̎擾�Ɏ��s���܂���", "GET_DEVICE_ERROR", MB_OK );
		return;
	}

	return;
}