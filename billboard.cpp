//=================================================================================
//	�v���O������	�F	
//	����			�F	�r���{�[�h����[billboard.cpp]
//	���C������		�F	main.cpp
//	�쐬��			�F	moriya gaku
//=================================================================================

//=================================================================================
//	�C���N���[�h�t�@�C��
//=================================================================================
#include "main.h"
#include "scene3D.h"
#include "billboard.h"
#include "manager.h"
#include "renderer.h"
#include "cameraManager.h"
#include "textureManager.h"
#include "sceneManager.h"

//=================================================================================
//	�R���X�g���N�^
//=================================================================================
CBillboard::CBillboard()
{
}

//=================================================================================
//	�������֐�
//=================================================================================
HRESULT CBillboard::Init( void )
{
	CScene3D::Init();

	return S_OK;
}

//=================================================================================
//	�I���֐�
//=================================================================================
void CBillboard::Uninit( void )
{
	CScene3D::Uninit();
}

//=================================================================================
//	�X�V�֐�
//=================================================================================
void CBillboard::Update( void )
{
	CScene3D::Update();
}

//=================================================================================
//	�`��֐�
//=================================================================================
void CBillboard::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	CCamera *pCamera = CSceneManager::GetCameraManager()->GetCamera(MAIN_CAMERA);
	LPDIRECT3DVERTEXBUFFER9 pVtxBuffer = GetVertexBuffer();

	/*------------------------------------------------------------------------*/
	/*	�}�g���N�X�ϊ�
	/*------------------------------------------------------------------------*/
	// �e�}�g���N�X�̍쐬( ���[���h�E�r���[�E �v���W�F�N�V�����E��]�E���s�E�g�� )
	D3DXMATRIX mtxWorld, mtxViewInverse;
	D3DXMATRIX mtxScale, mtxRot, mtxTranslate;

	// �s���P�ʍs��ɂ���(�Ȃ�ł��Ȃ������̍s����쐬����)
	D3DXMatrixIdentity(&mtxWorld);
	D3DXMatrixIdentity(&mtxViewInverse);

	D3DXMatrixInverse(&mtxViewInverse, NULL, &pCamera->GetViewMtx());	// �r���[�s��̋t�s��
	mtxViewInverse._41 = mtxViewInverse._42 = mtxViewInverse._43 = 0.0f;	// �J�����̕��s�ړ������ׂ͒�

	// �X�P�[���𔽉f
	D3DXMatrixScaling(&mtxScale, 1.0f, 1.0f, 1.0f);
	D3DXMatrixMultiply(&mtxViewInverse, &mtxViewInverse, &mtxScale);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, D3DXToRadian(GetRotation().y), D3DXToRadian(GetRotation().x), D3DXToRadian(GetRotation().z));
	D3DXMatrixMultiply(&mtxViewInverse, &mtxViewInverse, &mtxRot);

	// �ړ��𔽉f
	D3DXVECTOR3 pos = GetPosition();
	D3DXMatrixTranslation(&mtxTranslate, GetPosition().x, GetPosition().y, GetPosition().z);
	D3DXMatrixMultiply(&mtxViewInverse, &mtxViewInverse, &mtxTranslate);

	mtxWorld = mtxViewInverse;

	// �f�o�C�X�Ƀ��[���h�ϊ��s����w��
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	if (pDevice != NULL)
	{
		//// ���C�g���I�t�ɂ���
		//pDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
		//
		//// ���Z����
		//pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
		//pDevice->SetRenderState( D3DRS_ZENABLE, D3DZB_TRUE );
		//pDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );


		// �X�g���[���ݒ�
		pDevice->SetStreamSource(0, pVtxBuffer, 0, sizeof(VERTEX_3D));

		// ���C�e�B���O�I�t�ݒ�
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

		// �t�H�[�}�b�g�ݒ�
		pDevice->SetFVF(FVF_VERTEX_3D);

		// �e�N�X�`���\��t��
		pDevice->SetTexture(0, CTextureManager::GetResource(GetKey()));

		// �|���S���`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

		// ���C�e�B���O�I�t�ݒ�
		pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);


		//// ���Z���������Ƃɖ߂�
		//pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
		//pDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
		//
		//// ���C�g�����Ƃɖ߂�
		//pDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
	}

}