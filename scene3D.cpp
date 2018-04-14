//=================================================================================
//	�v���O������	�F	
//	����			�F	3D�`��I�u�W�F�N�g����[scene3D.cpp]
//	���C������		�F	main.cpp
//	�쐬��			�F	moriya gaku
//=================================================================================

//=================================================================================
//	�C���N���[�h�t�@�C��
//=================================================================================
#include "scene3D.h"
#include "renderer.h"
#include "textureManager.h"

//=================================================================================
//	�R���X�g���N�^
//=================================================================================
CScene3D::CScene3D( int priority ) : CScene( priority )
{
	m_pVtxBuffer = NULL;
	m_Key = "";
	m_Scale = D3DXVECTOR3( 1.0f, 1.0f, 1.0f );
	m_Color = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );
	m_Length = 0.0f;								// �|���S���̑Ίp���̒���
	m_Angle = 0.0f;									// �|���S���̑Ίp���̊p�x
	m_Width = 0.0f;									// ��
	m_Height = 0.0f;								// ����	
	m_TexU = 0.0f;									// U�l
	m_TexV = 0.0f;									// V�l
	m_TexWidth = 0.0f;								// �e�N�X�`����
	m_TexHeight = 0.0f;								// �e�N�X�`������
	D3DXMatrixIdentity(&m_mtxWorld);
}

//=================================================================================
//	�f�X�g���N�^
//=================================================================================
CScene3D::~CScene3D()
{
}

//=================================================================================
//�@����������
//=================================================================================
HRESULT CScene3D::Init(void)
{
	// ���_�o�b�t�@����
	MakeVertexBuffer();

	// ���_���Z�b�g
	SetVertex();

	return S_OK;
}

//=================================================================================
//�@�I������
//=================================================================================
void CScene3D::Uninit(void)
{
	// ���_�o�b�t�@�̊J��
	if(m_pVtxBuffer)
	{
		m_pVtxBuffer->Release();
		m_pVtxBuffer = NULL;
	}

	// �e�N���X�֍폜�˗�
	Release();
}

//=================================================================================
//�@�X�V����
//=================================================================================
void CScene3D::Update(void)
{
	// ���_���Z�b�g
	SetVertex();
}

//=================================================================================
//�@�`�揈��
//=================================================================================
void CScene3D::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	D3DXMATRIX mtxScale, mtxRot, mtxTranslate;

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// �X�P�[���𔽉f
	D3DXMatrixScaling(&mtxScale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, D3DXToRadian(GetRotation().y), D3DXToRadian(GetRotation().x), D3DXToRadian(GetRotation().z));
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&mtxTranslate, GetPosition().x, GetPosition().y, GetPosition().z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTranslate);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���_�o�b�t�@�������_�����O�p�C�v���C���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuffer, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, CTextureManager::GetResource( m_Key ));

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
}

//=================================================================================
//�@���_�o�b�t�@�쐬
//=================================================================================
HRESULT CScene3D::MakeVertexBuffer( void )
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

    if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
												D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
												FVF_VERTEX_3D,				// �g�p���钸�_�t�H�[�}�b�g
												D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
												&m_pVtxBuffer,				// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
												NULL)))						// NULL�ɐݒ�
	{
        return E_FAIL;
	}

	return S_OK;
}

//=================================================================================
//�@���_�����Z�b�g
//=================================================================================
void CScene3D::SetVertex( void )
{
	//���_�o�b�t�@�̒��g�𖄂߂�
	VERTEX_3D *pVtx;

	// �|���S���̑Ίp���̒�����ݒ�
	m_Length = sqrtf((float)(((m_Width / 2) * (m_Width / 2)) + ((m_Height / 2) * (m_Height / 2))));

	// �|���S���̑Ίp���̊p�x��ݒ�
	m_Angle = atan2f((m_Width / 2), (m_Height / 2));

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	pVtx[0].pos.x = -sinf(m_Angle) * m_Length;
	pVtx[0].pos.y = cosf(m_Angle) * m_Length;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = sinf(m_Angle) * m_Length;
	pVtx[1].pos.y = cosf(m_Angle) * m_Length;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = -sinf(m_Angle) * m_Length;
	pVtx[2].pos.y = -cosf(m_Angle) * m_Length;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = sinf(m_Angle) * m_Length;
	pVtx[3].pos.y = -cosf(m_Angle) * m_Length;
	pVtx[3].pos.z = 0.0f;

	// �@���̐ݒ�
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	// ���_�J���[�̐ݒ�
	pVtx[0].col = m_Color;
	pVtx[1].col = m_Color;
	pVtx[2].col = m_Color;
	pVtx[3].col = m_Color;

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�f�[�^���A�����b�N����
	m_pVtxBuffer->Unlock();

}