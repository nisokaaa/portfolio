//=================================================================================
//	�v���O������	�F	
//	����			�F	���b�V����񏈗�[mesh.cpp]
//	���C������		�F	main.cpp
//	�쐬��			�F	moriya gaku
//=================================================================================

//=================================================================================
//	�C���N���[�h�t�@�C��
//=================================================================================
#include "mesh.h"
#include "scene3D.h"
#include "renderer.h"
#include "manager.h"
#include "textureManager.h"
#include "shaderManager.h"

//=================================================================================
//	�R���X�g���N�^
//=================================================================================
CMesh::CMesh( int priority ) : CScene3D( priority )
{
	m_pIdxBuffer = NULL;	
	m_NumWidth = 0;			
	m_NumHeight = 0;		
	m_NumPolygon = 0;
	m_NumVertex = 0;
}

//=================================================================================
//	�������֐�
//=================================================================================
HRESULT CMesh::Init( void )
{
	CScene3D::Init();

	// ���_�C���f�b�N�X�쐬
	MakeIndexBuffer();

	return S_OK;
}

//=================================================================================
//	�I���֐�
//=================================================================================
void CMesh::Uninit( void )
{	
	CScene3D::Uninit();

	if( m_pIdxBuffer != NULL )
	{
		m_pIdxBuffer->Release();		// �C���f�b�N�X�o�b�t�@�C���^�[�t�F�[�X�̉��
		m_pIdxBuffer = NULL;
	}

	return;
}

//=================================================================================
//	�X�V�֐�
//=================================================================================
void CMesh::Update( void )
{
	CScene3D::Update();

	return;
}

//=================================================================================
//	�`��֐�
//=================================================================================
void CMesh::Draw( void )
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	LPDIRECT3DVERTEXBUFFER9 pVtxBuffer = GetVertexBuffer();
	int numWidth = GetNumWidth();
	int numHeight = GetNumHeight();
	int numPolygon = GetNumPolygon();

	if( pDevice != NULL )
	{
		SetMatrix();

		// VRAM��GPU�Ԃœ`������ݒ�̏���
		pDevice->SetStreamSource( 0, pVtxBuffer, 0, sizeof( VERTEX_3D ) );

		// �t�H�[�}�b�g�ݒ�
		pDevice -> SetFVF( FVF_VERTEX_3D );

		// �f�o�C�X�փC���f�b�N�X�o�b�t�@��ݒ�
		pDevice -> SetIndices( m_pIdxBuffer );

		// �|���S���`��( �v���~�e�B�u���{�k�ރ|���S���S���� )
		pDevice -> DrawIndexedPrimitive( D3DPT_TRIANGLESTRIP, 0, 0, ( numWidth + 1 ) * ( numHeight + 1 ), 0, numPolygon );
	}

	return;
}

//=================================================================================
//	���_�C���f�b�N�X�쐬
//=================================================================================
void CMesh::MakeIndexBuffer( void )
{
	// 16�r�b�g�Y��
	WORD *pIdx;
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// �k�ރ|���S���܂߂������_��
	m_NumVertex = (( m_NumWidth + 2 ) * ( m_NumHeight -1 ) + m_NumWidth ) * 2 + 2;

	/*------------------------------------------------------------------------*/
	/*	�C���f�b�N�X�o�b�t�@�̍쐬
	/*------------------------------------------------------------------------*/
	if( FAILED( pDevice->CreateIndexBuffer(
		sizeof( WORD ) * m_NumVertex,			// �o�b�t�@��
		D3DUSAGE_WRITEONLY,						// �g�p���@�t���O
		D3DFMT_INDEX16,							// �C���f�b�N�X�t�H�[�}�b�g
		D3DPOOL_MANAGED,						// �������Ǘ����@
		&m_pIdxBuffer,							// �C���f�b�N�X�o�b�t�@�C���^�t�F�[�X�ւ̃|�C���^�̃A�h���X
		NULL
		) ) )
	{
		MessageBox( NULL, "�C���f�b�N�X�o�b�t�@�̍쐬�Ɏ��s���܂����B", "INDEX_BUFFER_ERROR", MB_OK );
	}
	else
	{
		pDevice->CreateIndexBuffer( sizeof( WORD ) * m_NumVertex, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIdxBuffer, NULL );
	}

	/*------------------------------------------------------------------------*/
	/*	���b�V���t�B�[���h���_���Z�b�g(1��x�̂�)
	/*------------------------------------------------------------------------*/
	// ���z�������ɏ�������
	m_pIdxBuffer->Lock( 0, 0, (void**)&pIdx, 0 );

	int nLoop = 0;

	for( int i = 0; i < m_NumHeight; i++ )
	{
		for( int j = 0; j < m_NumWidth + 1; j++ )
		{
			pIdx[ nLoop ] = ( m_NumWidth + 1 ) * ( i + 1 ) + j;
			nLoop++;
			pIdx[ nLoop ] = ( m_NumWidth + 1 ) * i + j;
			nLoop++;
		}
		if( i != m_NumHeight -1 )
		{
			pIdx[ nLoop ] = pIdx[ nLoop -1 ];
			nLoop++;
			pIdx[ nLoop ] = ( m_NumWidth + 1 ) * ( i + 2 );
			nLoop++;
		}
	}
	// ���z���������A�����b�N
	m_pIdxBuffer->Unlock();

	return;
}