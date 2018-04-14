//=================================================================================
//	�v���O������	�F	
//	����			�F	2D�`��I�u�W�F�N�g����[scene2D.h]
//	���C������		�F	main.cpp
//	�쐬��			�F	moriya gaku
//=================================================================================

//=================================================================================
//	�C���N���[�h�t�@�C��
//=================================================================================
#include "scene2D.h"
#include "renderer.h"
#include "textureManager.h"

//=================================================================================
//�@����������
//=================================================================================
HRESULT CScene2D::Init( void )
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
    if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
												D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
												FVF_VERTEX_2D,				// �g�p���钸�_�t�H�[�}�b�g
												D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
												&m_pVtxBuff,				// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
												NULL)))						// NULL�ɐݒ�
	{
        return E_FAIL;
	}

	return S_OK;
}

//=================================================================================
//�@�I������
//=================================================================================
void CScene2D::Uninit( void )
{
	// ���_�o�b�t�@�̊J��
	if( m_pVtxBuff != NULL )
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// �e�N���X�֍폜�˗�
	Release();
}

//=================================================================================
//�@�X�V����
//=================================================================================
void CScene2D::Update( void )
{
	// Z���}�C�i�X�̏ꍇ�͕`�悵�Ȃ�
	if( GetPosition().z < 0 ) return;

	VERTEX_2D *pVtx;
	
	// ���_�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�ݒ�
	pVtx = SetVertex( pVtx );

	pVtx[ 0 ].rhw =
	pVtx[ 1 ].rhw =
	pVtx[ 2 ].rhw =
	pVtx[ 3 ].rhw = 1.0f;

	// ���_�J���[�ݒ�
	pVtx[ 0 ].col = m_Color;
	pVtx[ 1 ].col = m_Color;
	pVtx[ 2 ].col = m_Color;
	pVtx[ 3 ].col = m_Color;

	// �e�N�X�`�����W�ݒ�
	pVtx[ 0 ].tex = D3DXVECTOR2( m_TexU, m_TexV );
	pVtx[ 1 ].tex = D3DXVECTOR2( m_TexU + m_TexWidth, m_TexV );
	pVtx[ 2 ].tex = D3DXVECTOR2( m_TexU, m_TexV + m_TexHeight );
	pVtx[ 3 ].tex = D3DXVECTOR2( m_TexU + m_TexWidth, m_TexV + m_TexHeight );

	// ���_���A�����b�N
	m_pVtxBuff->Unlock();

}

//=================================================================================
//�@�`�揈��
//=================================================================================
void CScene2D::Draw( void )
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// �e�N�X�`���ݒ�
	pDevice->SetTexture(0, CTextureManager::GetResource(m_Key));

	// ���_�t�H�[�}�b�g�ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// ���_�o�b�t�@�ݒ�
    pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// �`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
}

//=================================================================================
//�@�����֐�
//=================================================================================
CScene2D *CScene2D::Create(std::string key, D3DXVECTOR2 pos, float width, float height, float scl, BOOL mode, int priority)
{
	CScene2D *p = new CScene2D(priority);

	p->SetTexture(key);
	p->SetPosition(pos.x, pos.y, 0.0f);
	p->m_Width = width;
	p->m_Height = height;
	p->m_Scale = scl;
	p->m_TexWidth = 1.0f;
	p->m_TexHeight = 1.0f;
	p->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	p->m_bDrawType = mode;

	p->Init();

	return p;
}

//=================================================================================
//�@���_�ݒ�
//=================================================================================
VERTEX_2D* CScene2D::SetVertex( VERTEX_2D *pVtx )
{
	D3DXVECTOR3 position = GetPosition();

	// ���S���_�ŕ`��
	if(m_bDrawType){
		D3DXVECTOR3 rotation = GetRotation();
		float length = GetLength();
		float angle = GetAngle();
		float width = GetWidth();
		float height = GetHeight();
		float scale = GetScale();

		// �|���S���̑Ίp���̒�����ݒ�
		length = sqrtf((float)(((width / 2) * (width / 2)) + ((height / 2) * (height / 2))));
		SetLength( length );

		// �|���S���̑Ίp���̊p�x��ݒ�
		angle = atan2f((width / 2), (height / 2));
		SetAngle( angle );

		//// ���_���W�̐ݒ�
		pVtx[ 0 ].pos.x = ( position.x - sinf(angle - rotation.z) * length * scale);
		pVtx[ 0 ].pos.y = ( position.y - cosf(angle - rotation.z) * length * scale);
		pVtx[ 0 ].pos.z = 0.0f;
		    
		pVtx[ 1 ].pos.x = ( position.x + sinf(angle + rotation.z) * length * scale);
		pVtx[ 1 ].pos.y = ( position.y - cosf(angle + rotation.z) * length * scale);
		pVtx[ 1 ].pos.z = 0.0f;
		    
		pVtx[ 2 ].pos.x = ( position.x - sinf(angle + rotation.z) * length * scale);
		pVtx[ 2 ].pos.y = ( position.y + cosf(angle + rotation.z) * length * scale);
		pVtx[ 2 ].pos.z = 0.0f;
		    
		pVtx[ 3 ].pos.x = ( position.x + sinf(angle - rotation.z) * length * scale);
		pVtx[ 3 ].pos.y = ( position.y + cosf(angle - rotation.z) * length * scale);
		pVtx[ 3 ].pos.z = 0.0f;

		return pVtx;
	}

	// ���_���W�̐ݒ�
	pVtx[ 0 ].pos.x = position.x;
	pVtx[ 0 ].pos.y = position.y;
	pVtx[ 0 ].pos.z = 0.0f;
			    
	pVtx[ 1 ].pos.x = position.x + m_Width * m_Scale;
	pVtx[ 1 ].pos.y = position.y;
	pVtx[ 1 ].pos.z = 0.0f;
			    
	pVtx[ 2 ].pos.x = position.x;
	pVtx[ 2 ].pos.y = position.y + m_Height * m_Scale;
	pVtx[ 2 ].pos.z = 0.0f;
			    
	pVtx[ 3 ].pos.x = position.x + m_Width * m_Scale;
	pVtx[ 3 ].pos.y = position.y + m_Height * m_Scale;
	pVtx[ 3 ].pos.z = 0.0f;

	return pVtx;
}