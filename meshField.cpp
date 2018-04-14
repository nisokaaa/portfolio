//=================================================================================
//	�v���O������	�F	
//	����			�F	���b�V���t�B�[���h����[meshField.cpp]
//	���C������		�F	main.cpp
//	�쐬��			�F	moriya gaku
//=================================================================================

//=================================================================================
//	�C���N���[�h�t�@�C��
//=================================================================================
#include "main.h"
#include "meshField.h"
#include "manager.h"
#include "renderer.h"
#include "textureManager.h"
#include "shaderManager.h"
#include "sceneManager.h"
#include "cameraManager.h"
#include "lightManager.h"

//=================================================================================
//	�I���֐�
//=================================================================================
void CMeshField::Uninit(void)
{
	for (int i = 0; i < (int)m_vHeight.size(); i++)
	{
		for (int j = 0; j < (int)m_vHeight[i].size(); j++)
		{
			m_vHeight[i][j] = 0.0f;
		}
	}
	m_vHeight.clear();

	CMesh::Uninit();
}

//=================================================================================
//	���_�o�b�t�@�쐬
//=================================================================================
HRESULT CMeshField::MakeVertexBuffer( void )
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	LPDIRECT3DVERTEXBUFFER9 pVtxBuffer = NULL;
	int numWidth = GetNumWidth();
	int numHeight = GetNumHeight();

    if( pDevice != NULL )
	{
		/*------------------------------------------------------------------------*/
		/*	���_�o�b�t�@�̍쐬
		/*------------------------------------------------------------------------*/
		if( FAILED( pDevice -> CreateVertexBuffer( 
			// (2 * 2 + ���R�}�X) * �^�e�}�X + (�^�e�}�X - 1) * 2
			// sizeof( VERTEX_3D ) * ( numHeight + 1 ) * ( numWidth + 1 ), 
			sizeof( VERTEX_3D ) * ( 2 * 2 * numWidth ) * numHeight + ( numHeight - 1 ) * 2,
			D3DUSAGE_WRITEONLY, 
			FVF_VERTEX_3D, 
			D3DPOOL_MANAGED, 
			&pVtxBuffer, 
			NULL ) ) )
		{
			MessageBox( NULL, "���_�o�b�t�@�̍쐬�Ɏ��s���܂����B", "VERTEX_BUFFER_ERROR", MB_OK );
		}
		else
		{
			pDevice -> CreateVertexBuffer( 
				//sizeof( VERTEX_3D ) * ( numHeight + 1 ) * ( numWidth + 1 ),
				sizeof( VERTEX_3D ) * ( 2 * 2 * numWidth ) * numHeight + ( numHeight - 1 ) * 2,
				D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &pVtxBuffer, NULL );	
		}

	}

	// ���_�o�b�t�@�ۑ�
	SetVertexBuffer( pVtxBuffer );

	return S_OK;
}

//=================================================================================
//	���_���̃Z�b�g
//=================================================================================
void CMeshField::SetVertex( void )
{
	// ���z�������A�N�Z�X�p
	VERTEX_3D *pVtx;
	LPDIRECT3DVERTEXBUFFER9 pVtxBuffer = GetVertexBuffer();
	float width = GetWidth();
	float height = GetHeight();
	int numWidth = GetNumWidth();
	int numHeight = GetNumHeight();

	// ��ݒ�
	D3DXVECTOR3 basePos = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	D3DXVECTOR3 offsetPos = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );

	// �t�B�[���h���S����p�܂ł̊p�x
	float fTheta = atan2f( float(height * height), float(width * width) );

	float allWidth = float(width * numWidth);
	float allHeight = float(height * numHeight);
	float length = sqrtf( allWidth * allWidth + allHeight * allHeight ) * 0.5f;
	basePos.x = -cosf( fTheta ) * length;
	basePos.z = sin( fTheta ) * length;

	// �e�N�X�`�����WUV
	float offsetTexU = 0.0f;
	float offsetTexV = 0.0f;

	// ���[�v�p�Y��
	int index = 0;

	// ���z�������ɏ�������
	pVtxBuffer->Lock( 0, 0, ( void** )&pVtx, 0 );

	// ���W�EUV�l�E�F�̌���
	for( int i = 0; i <= numHeight; i++ )
	{
		// ���[�v�p�Y���X�V
		index = i * (numWidth + 1);

		for( int j = 0; j <= numWidth; j++ )
		{
			pVtx[ j + index ].pos.x = basePos.x + offsetPos.x;
			pVtx[ j + index ].pos.y = m_vHeight[i][j];
			pVtx[ j + index ].pos.z = basePos.z + offsetPos.z;
			pVtx[ j + index ].tex	= D3DXVECTOR2( 0.0f + offsetTexU, 0.0f + offsetTexV );
			pVtx[ j + index ].col	= GetColor();

			// X���V�t�g
			offsetPos.x += width;
			// U�l�V�t�g
			offsetTexU += 1.0f;
		}
		// X���N���A
		offsetPos.x = 0.0f;
		// Z���V�t�g
		offsetPos.z -= height;
		// U�l�N���A
		offsetTexU = 0.0f;
		// V�l�V�t�g
		offsetTexV += 1.0f;
	}

	// �@���̌���
	for( int i = 1; i <= numHeight; i++ )
	{
		// ���[�v�p�Y���X�V
		index = i * (numWidth + 1);

		for( int j = 1; j <= numWidth; j++ )
		{
			// �x�N�g��X���߂�(�I�_����n�_������)
			D3DXVECTOR3 directionX = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
			directionX = pVtx[ j + index + 1 ].pos - pVtx[ j + index - 1 ].pos;
			
			//// �x�N�g��Z���߂�(�I�_����n�_������)
			D3DXVECTOR3 directionZ = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
			directionZ =  pVtx[ j + index - numWidth ].pos - pVtx[ j + index + numWidth ].pos;			

			// ���Ƃ��ɑ΂���@���x�N�g��normalX
			D3DXVECTOR3 normalX = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
			normalX.x = -directionX.y;
			normalX.y = directionX.x;
			normalX.z = 0.0f;

			// ���Ƃ��ɑ΂���@���x�N�g��normalZ
			D3DXVECTOR3 normalZ = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
			normalZ.x = 0.0f;
			normalZ.y = directionZ.z;
			normalZ.z = -directionZ.y;

			// normalX��normalZ�̒��Ԓl
			D3DXVECTOR3 normal = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
			normal = normalX + normalZ;
			D3DXVec3Normalize( &normal, &normal );
			pVtx[ j + index ].nor = normal;
		}
	}

	// ���z���������A�����b�N
	pVtxBuffer->Unlock();

	// �V�F�[�_�[�̒l�Z�b�g
	CCameraManager *pCameraManager = CSceneManager::GetCameraManager();
	CCamera *pCamera = pCameraManager->GetCamera(MAIN_CAMERA);
	CLightManager *pLightManager = CSceneManager::GetLightManager();
	CLight *pLight = pLightManager->GetLight(MAIN_LIGHT);
	CSMeshField *pShader = dynamic_cast<CSMeshField*>(GetShader());
	D3DXVECTOR3 dir = pLight->GetVecDir();
	pShader->SetParam(pCamera->GetViewMtx(), pCamera->GetProjMtx(), 5, 6, dir, pCamera->GetPosition());
	
	return;
}

//=================================================================================
//	���[���h�s��v�Z�E�f�o�C�X�ɃZ�b�g
//=================================================================================
void CMeshField::SetMatrix(void)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// �e�}�g���N�X�̍쐬( ���[���h�E�r���[�E �v���W�F�N�V�����E��]�E���s�E�g�� )
	D3DXMATRIX mtxWorld, mtxRot, mtxPos;

	// �s���P�ʍs��ɂ���(�Ȃ�ł��Ȃ������̍s����쐬����)
	D3DXMatrixIdentity(&mtxWorld);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, D3DXToRadian(GetRotation().y), D3DXToRadian(GetRotation().x), D3DXToRadian(GetRotation().z));
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&mtxPos, GetPosition().x, GetPosition().y, GetPosition().z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxPos);

	// �f�o�C�X�Ƀ��[���h�ϊ��s����w��
	GetShader()->SetMatrix(mtxWorld);
}

//=================================================================================
//	���b�V���t�B�[���h����
//=================================================================================
CMeshField *CMeshField::Create( D3DXVECTOR3 pos, D3DXVECTOR3 rot, float width, float  height,int numWidth, int numHeight, std::vector<std::vector<float>> vHeight)
{
	CMeshField *pMeshField;

	pMeshField = new CMeshField( 0 );

	// ���W�ݒ�
	pMeshField->SetPosition( pos );

	// ��]�p�x�ݒ�
	pMeshField->SetRotation( rot );

	// �|���S���̉��T�C�Y�ݒ�
	pMeshField->SetWidth( width );
	
	// �|���S���̏c�T�C�Y�ݒ�
	pMeshField->SetHeight( height );
	
	// ���u���b�N���ݒ�
	pMeshField->SetNumWidth( numWidth );													
	
	// �c�u���b�N���ݒ�
	pMeshField->SetNumHeight( numHeight );

	// ���ʏ��ݒ�
	pMeshField->m_vHeight = vHeight;

	// �I�u�W�F�N�g�^�C�v�ݒ�
	pMeshField->SetType( SCENE_TYPE_FIELD );

	// �k�ރ|���S���܂߂����|���S����
	pMeshField->SetNumPolygon( (( numWidth + 2 ) * ( numHeight -1 ) + numWidth ) * 2 );

	// �F�ݒ�
	pMeshField->SetColor( D3DXCOLOR( 0.9f, 0.85f, 0.85f, 1.0f ) );
	
	// �e�N�X�`���ǂݍ���
	CTextureManager::Load("data/TEXTURE/sand.jpg");
	CTextureManager::Load("data/TEXTURE/rock_surface.jpg");
	pMeshField->SetTexture("data/TEXTURE/sand.jpg");

	// �V�F�[�_�[�̃Z�b�g
	CShaderManager *pShaderManager = CSceneManager::GetShaderManager();
	CSMeshField *pShader = dynamic_cast<CSMeshField*>(pShaderManager->GetData(SHADER_MESHFIELD));
	pMeshField->SetShader(pShader);

	pMeshField->Init();

	return pMeshField;
}

//=================================================================================
//	P������Ă��鉚�ʂ̍����擾
//=================================================================================
float CMeshField::GetPosHeight( D3DXVECTOR3 position )
{
	VERTEX_3D *pVtx;
	LPDIRECT3DVERTEXBUFFER9 pVtxBuffer = GetVertexBuffer();
	int numWidth = GetNumWidth();
	int numHeight = GetNumHeight();

	// ��s�̒��_��
	int numLineVertex = numWidth + 1;

	// �|���S��3���_�p
	D3DXVECTOR3 p0 = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	D3DXVECTOR3 p1 = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	D3DXVECTOR3 p2 = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );

	// ���_�����_
	D3DXVECTOR3 v01 = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	D3DXVECTOR3 v12 = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	D3DXVECTOR3 v20 = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );

	// ���_��Player
	D3DXVECTOR3 v0p = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	D3DXVECTOR3 v1p = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	D3DXVECTOR3 v2p = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );

	// �@���x�N�g���p
	D3DXVECTOR3 vc0 = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	D3DXVECTOR3 vc1 = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	D3DXVECTOR3 vc2 = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );

	pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	for( int z = 0; z <= numHeight; z++  )
	{
		for( int x = 0; x < numWidth; x++  )
		{
			/*=====================================
						  1�|���S��
			=====================================*/
			// �|���S��3���_�����߂�
			p0 = pVtx[ numLineVertex * ( z + 1 ) + x ].pos;
			p1 = pVtx[ numLineVertex * z + x ].pos;
			p2 = pVtx[ numLineVertex * ( z + 1 ) + ( x + 1 ) ].pos;

			// vc0���߂�
			v01 = p1 - p0;
			v0p = position - p0;
			D3DXVec3Cross( &vc0, &v01, &v0p );
			if( vc0.y >= 0.0f )
			{
				// vc1���߂�
				v12 = p2 - p1;
				v1p = position - p1;
				D3DXVec3Cross( &vc1, &v12, &v1p );
				if( vc1.y >= 0.0f )
				{
					// vc2���߂�
					v20 = p0 - p2;
					v2p = position - p2;
					D3DXVec3Cross( &vc2, &v20, &v2p );
					if( vc2.y >= 0.0f )
					{
						// �����v�Z
						D3DXVECTOR3 N = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
						D3DXVECTOR3 v01 = p1 - p0;
						D3DXVECTOR3 v02 = p2 - p0;
						D3DXVec3Cross( &N, &v01, &v02 );
						D3DXVec3Normalize( &N, &N );
						position.y = p0.y - ( (position.x - p0.x) * N.x + (position.z - p0.z) * N.z) / N.y;
						pVtxBuffer->Unlock();
						return position.y;
					}
				}
			}
			/*=====================================
						  2�|���S��
			=====================================*/
			// �|���S��3���_�����߂�
			p0 = pVtx[ numLineVertex * z + (x + 1) ].pos;
			p1 = pVtx[ numLineVertex * ( z + 1 ) + ( x + 1 ) ].pos;
			p2 = pVtx[ numLineVertex * z + x ].pos;

			// vc0���߂�
			v01 = p1 - p0;
			v0p = position - p0;
			D3DXVec3Cross( &vc0, &v01, &v0p );
			if( vc0.y >= 0.0f )
			{
				// vc1���߂�
				v12 = p2 - p1;
				v1p = position - p1;
				D3DXVec3Cross( &vc1, &v12, &v1p );
				if( vc1.y >= 0.0f )
				{
					// vc2���߂�
					v20 = p0 - p2;
					v2p = position - p2;
					D3DXVec3Cross( &vc2, &v20, &v2p );
					if( vc2.y >= 0.0f )
					{
						// �����v�Z
						D3DXVECTOR3 N = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
						D3DXVECTOR3 v01 = p1 - p0;
						D3DXVECTOR3 v02 = p2 - p0;
						D3DXVec3Cross( &N, &v01, &v02 );
						D3DXVec3Normalize( &N, &N );
						position.y = p0.y - ( (position.x - p0.x) * N.x + (position.z - p0.z) * N.z) / N.y;
						pVtxBuffer->Unlock();
						return position.y;
					}
				}
			}
		}
	}

	pVtxBuffer->Unlock();

	return 0.0f;
}