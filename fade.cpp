//=================================================================================
//	�v���O������	�F	
//	����			�F	�t�F�[�h����[fade.cpp]
//	���C������		�F	main.cpp
//	�쐬��			�F	moriya gaku
//=================================================================================

//=================================================================================
//	�C���N���[�h�t�@�C��
//=================================================================================
#include "fade.h"
#include "sceneManager.h"
#include "textureManager.h"
#include "renderer.h"

//=================================================================================
//�@�ÓI�����o�ϐ��錾
//=================================================================================
LPDIRECT3DVERTEXBUFFER9		CFade::m_pVtxBuff = NULL;					// ���_�o�b�t�@�ւ̃|�C���^
string						CFade::m_Key = "";							// �e�N�X�`���֌����L�[

CFade::MODE					CFade::m_FadeMode = FADE_NONE;				// �t�F�[�h���
CSceneBase*					CFade::m_pNextScene = NULL;					// �J�ڐ�

float CFade::m_FadeRate		= 0.05f;									// �t�F�[�h���[�g
float CFade::m_Width		= SCREEN_WIDTH;								// ��
float CFade::m_Height		= SCREEN_HEIGHT;							// ����	
float CFade::m_TexU			= 0.0f;										// U�l
float CFade::m_TexV			= 0.0f;										// V�l
float CFade::m_TexWidth		= 1.0f;										// �e�N�X�`����
float CFade::m_TexHeight	= 1.0f;										// �e�N�X�`������

// ���W
D3DXVECTOR3 CFade::m_Position = D3DXVECTOR3( SCREEN_CENTER_X, SCREEN_CENTER_Y, 0.0f );

// �t�F�[�h�F
D3DXCOLOR					CFade::m_Color = D3DXCOLOR( 255.0f, 255.0f, 255.0f, 0.0f );	

// �|���S���̑Ίp���̒���
float CFade::m_Length		= sqrtf((float)(((m_Width / 2) * (m_Width / 2)) + ((m_Height / 2) * (m_Height / 2))));

// �|���S���̑Ίp���̊p�x
float CFade::m_Angle		= atan2f((m_Width / 2), (m_Height / 2));

//===============================================================================
// ����������
//===============================================================================
HRESULT CFade::Init( void )
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

	// �e�N�X�`���Z�b�g
	SetTexture( "data/TEXTURE/fade.png" );

	return S_OK;
}

//===============================================================================
// �I������
//===============================================================================
void CFade::Uninit( void )
{
	// ���_�o�b�t�@�̊J��
	if( m_pVtxBuff != NULL )
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

}

//===============================================================================
// �X�V����
//===============================================================================
void CFade::Update( void )
{	
	// ���_�̐ݒ�
	VERTEX_2D *pVtx;

	// ���_�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	pVtx[ 0 ].pos.x = m_Position.x - sinf(m_Angle) * m_Length;
	pVtx[ 0 ].pos.y = m_Position.y - cosf(m_Angle) * m_Length;
	pVtx[ 0 ].pos.z = 0.0f;
		    
	pVtx[ 1 ].pos.x = m_Position.x + sinf(m_Angle) * m_Length;
	pVtx[ 1 ].pos.y = m_Position.y - cosf(m_Angle) * m_Length;
	pVtx[ 1 ].pos.z = 0.0f;
		    
	pVtx[ 2 ].pos.x = m_Position.x - sinf(m_Angle) * m_Length;
	pVtx[ 2 ].pos.y = m_Position.y + cosf(m_Angle) * m_Length;
	pVtx[ 2 ].pos.z = 0.0f;
		    
	pVtx[ 3 ].pos.x = m_Position.x + sinf(m_Angle) * m_Length;
	pVtx[ 3 ].pos.y = m_Position.y + cosf(m_Angle) * m_Length;
	pVtx[ 3 ].pos.z = 0.0f;

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


	// �t�F�[�h����
	if( m_FadeMode == FADE_IN )
	{
		// a�l�����Z���Č��̊G�𕂂��オ�点��(a�l����t�F�[�h�l�������Ă���)
		m_Color.a -= m_FadeRate;

		if( m_Color.a < 0.0f )
		{
			m_Color.a = 0.0f;
			m_FadeMode = FADE_NONE;
		}
	}
	else if( m_FadeMode == FADE_OUT )
	{
		// a�l�����Z���ĉ�ʂ������Ă���
		m_Color.a += m_FadeRate;

		if( m_Color.a > 1.0f )
		{
			m_Color.a = 1.0f;
			m_FadeMode = FADE_IN;
			CSceneManager::ChangeScene( m_pNextScene );
		}
	}
}

//===============================================================================
// �`�揈��
//===============================================================================
void CFade::Draw( void )
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���Ƀo�C���h
    pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, CTextureManager::GetResource( m_Key ));

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
}

//=================================================================================
//	�t�F�[�h�̐ݒ�
//=================================================================================
void CFade::SetFade( MODE fadeMode, CSceneBase *pSceneNext, float sec )
{
	m_FadeMode = fadeMode;						// �t�F�[�h���[�h�ݒ�
	m_pNextScene = pSceneNext;					// ���̑J�ڐ�|�C���^
	m_FadeRate = 1.0f / ( 60 * sec );			// �t�F�[�h���[�g(����)�ݒ�
}

//=================================================================================
//	�t�F�[�h��Ԏ擾
//=================================================================================
CFade::MODE CFade::GetFadeMode( void )
{
	return m_FadeMode;
}