//=================================================================================
//	�v���O������	�F	
//	����			�F	X�t�@�C���`��I�u�W�F�N�g����[sceneX.cpp]
//	���C������		�F	main.cpp
//	�쐬��			�F	moriya gaku
//=================================================================================

//=================================================================================
//	�C���N���[�h�t�@�C��
//=================================================================================
#include "sceneX.h"
#include "manager.h"
#include "renderer.h"
#include "modelManager.h"
#include "textureManager.h"

//=================================================================================
//	�������֐�
//=================================================================================
HRESULT CSceneX::Init(void)
{
	// ���f���̃e�N�X�`���ǂݍ���
	LoadTexture();

	return S_OK;
}

//=================================================================================
//	���f�����̃e�N�X�`���ǂݍ��݊֐�
//=================================================================================
HRESULT CSceneX::LoadTexture( void )
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	LPD3DXBUFFER pBuffMat = CModelManager::GetResource( m_Key )->pBuffMat;
	int numMat = CModelManager::GetResource( m_Key )->nNumMat;

	// ���f���ǂݍ���
	D3DXMATERIAL *pMat = (D3DXMATERIAL*)pBuffMat->GetBufferPointer();
	
	for(int nCntMat = 0; nCntMat < (int)numMat; nCntMat++)
	{
		if( pMat[ nCntMat ].pTextureFilename )
		{ 
			string path = pMat[ nCntMat ].pTextureFilename;
			CTextureManager::Load( path );
		}
	}

	return S_OK;
}

//=================================================================================
//	�I���֐�
//=================================================================================
void CSceneX::Uninit(void)
{
	// �e�N���X�֍폜�˗�
	Release();
}

//=================================================================================
//	�X�V�֐�
//=================================================================================
void CSceneX::Update(void)
{
}

//=================================================================================
//	�`��֐�
//=================================================================================
void CSceneX::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	D3DXMATRIX mtxScale;
	D3DMATERIAL9 matDef;
	D3DXMATERIAL *pMat;
	LPD3DXBUFFER pBuffMat = CModelManager::GetResource( m_Key )->pBuffMat;
	LPD3DXMESH pMesh = CModelManager::GetResource( m_Key )->pMesh;
	int numMat = CModelManager::GetResource( m_Key )->nNumMat;

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_Matrix);

	// �X�P�[��
	D3DXMatrixScaling(&mtxScale, m_Scale.y, m_Scale.x, m_Scale.z);
	D3DXMatrixMultiply(&m_Matrix, &m_Matrix, &mtxScale);

	// ��]
	D3DXMatrixRotationYawPitchRoll( &m_MtxRot, GetRotation().y, GetRotation().x, GetRotation().z );
	D3DXMatrixMultiply(&m_Matrix, &m_Matrix, &m_MtxRot);

	// �ړ�
	D3DXMatrixTranslation(&m_MtxPos, GetPosition().x, GetPosition().y, GetPosition().z);
	D3DXMatrixMultiply(&m_Matrix, &m_Matrix, &m_MtxPos);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_Matrix);

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// �}�e���A���ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)pBuffMat->GetBufferPointer();


	for(int nCntMat = 0; nCntMat < (int)numMat; nCntMat++)
	{
		// �t�@�C�������������}�e���A���͓ǂ�
		if( pMat[ nCntMat ].pTextureFilename != NULL )
		{
			// �}�e���A���̐ݒ�
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			// �e�N�X�`���̐ݒ�
			string path = pMat[ nCntMat ].pTextureFilename;
			if( SUCCEEDED( pDevice->SetTexture(0, CTextureManager::GetResource( path ))))
			{
				// �`��
				pMesh->DrawSubset( nCntMat );
			}
		}
		else
		{
			// �}�e���A���̐ݒ�
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, NULL );

			// �`��
			pMesh->DrawSubset( nCntMat );
		}
	}

	// �}�e���A�������ɖ߂�
	pDevice->SetMaterial(&matDef);
}

//=================================================================================
//	�����֐�
//=================================================================================
CSceneX *CSceneX::Create(string key, int priority)
{
	CSceneX *p = new CSceneX(priority);
	p->SetModel(key);
	p->SetScale(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	p->Init();

	return p;
}

//=================================================================================
//	�L�����N�^�����p���s��Z�o�֐�
//=================================================================================
D3DXMATRIX* CSceneX::CalcLookAtMatrixAxisFix(
   D3DXMATRIX* pout,
   D3DXVECTOR3* pPos,
   D3DXVECTOR3* pLook,
   D3DXVECTOR3* pUp
) {
   D3DXVECTOR3 X, Y, Z, D;
   D = *pLook - *pPos;
   D3DXVec3Normalize( &D, &D );
   D3DXVec3Cross( &X, D3DXVec3Normalize(&Y, pUp), &D );
   D3DXVec3Normalize( &X, &X );
   D3DXVec3Normalize( &Z, D3DXVec3Cross( &Z, &X, &Y ));

   pout->_11 = X.x; pout->_12 = X.y; pout->_13 = X.z; pout->_14 = 0;
   pout->_21 = Y.x; pout->_22 = Y.y; pout->_23 = Y.z; pout->_24 = 0;
   pout->_31 = Z.x; pout->_32 = Z.y; pout->_33 = Z.z; pout->_34 = 0;
   pout->_41 = 0.0f; pout->_42 = 0.0f; pout->_43 = 0.0f; pout->_44 = 1.0f;

   return pout;
}

//=================================================================================
// ���ʐ��`��Ԋ֐�
// out   : ��ԃx�N�g���i�o�́j
// start : �J�n�x�N�g��
// end : �I���x�N�g��
// t : ��Ԓl�i0�`1�j
//=================================================================================
D3DXVECTOR3* CSceneX::SphereLinear( D3DXVECTOR3* out, D3DXVECTOR3* start, D3DXVECTOR3* end, float t )
{
	D3DXVECTOR3 s, e;
	D3DXVec3Normalize( &s, start );
	D3DXVec3Normalize( &e, end );

	// 2�x�N�g���Ԃ̊p�x�i�s�p���j
	float angle = acos( D3DXVec3Dot( &s, &e ) );

	// sin��
	float SinTh = sin( angle );

	// ��ԌW��
	float Ps = sin( angle * ( 1 - t ) );
	float Pe = sin( angle * t );

	*out = ( Ps * s + Pe * e ) / SinTh;

	// �ꉞ���K�����ċ��ʐ��`��Ԃ�
	D3DXVec3Normalize( out, out );

	return out;
}

//=================================================================================
// ���ʐ��`��Ԃɂ���Ԏp���Z�o�֐�
// out : ��Ԏp���i�o�́j
// start : �J�n�p��
// end : �ڕW�p��
// t : ��ԌW���i0�`1
//=================================================================================
D3DXMATRIX* CSceneX::CalcInterPause( D3DXMATRIX* out, D3DXMATRIX* start, D3DXMATRIX* end, float t )
{
   // �e�p���x�N�g�����o
   D3DXVECTOR3 Sy, Sz;
   D3DXVECTOR3 Ey, Ez;

   memcpy( &Sy, start->m[1], sizeof( float ) * 3 );
   memcpy( &Sz, start->m[2], sizeof( float ) * 3 );
   memcpy( &Ey, end->m[1], sizeof( float ) * 3 );
   memcpy( &Ez, end->m[2], sizeof( float ) * 3 );

   // ���ԃx�N�g���Z�o
   D3DXVECTOR3 IY, IZ;
   SphereLinear( &IY, &Sy, &Ey, t );
   SphereLinear( &IZ, &Sz, &Ez, t );

   // ���ԃx�N�g������p���x�N�g�����ĎZ�o
   D3DXVECTOR3 IX;
   D3DXVec3Cross( &IX, &IY, &IZ );
   D3DXVec3Cross( &IY, &IZ, &IX );
   D3DXVec3Normalize( &IX, &IX );
   D3DXVec3Normalize( &IY, &IY );
   D3DXVec3Normalize( &IZ, &IZ );

   memset( out, 0, sizeof( D3DXMATRIX ) );
   memcpy( out->m[0], &IX, sizeof( float ) * 3 );
   memcpy( out->m[1], &IY, sizeof( float ) * 3 );
   memcpy( out->m[2], &IZ, sizeof( float ) * 3 );
   out->_44 = 1.0f;

   return out;
}