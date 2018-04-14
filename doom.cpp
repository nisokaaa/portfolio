//=================================================================================
//	�v���O������	�F	
//	����			�F	���b�V���h�[������[doom.cpp]
//	���C������		�F	main.cpp
//	�쐬��			�F	moriya gaku
//=================================================================================

//=================================================================================
//	�C���N���[�h�t�@�C��
//=================================================================================
#include "main.h"
#include "doom.h"
#include "manager.h"
#include "renderer.h"
#include "textureManager.h"
#include "shaderManager.h"
#include "sceneManager.h"
#include "cameraManager.h"
#include "lightManager.h"

//=================================================================================
//	�R���X�g���N�^
//=================================================================================
CDoom::CDoom()
{
	m_AngleXY = 0.0f;
	m_AngleXZ = 0.0f;
}

//=================================================================================
//	���_�o�b�t�@�쐬
//=================================================================================
HRESULT CDoom::MakeVertexBuffer( void )
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
			sizeof( VERTEX_3D ) * ( numHeight + 1 ) * ( numWidth + 1 ), 
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
			pDevice -> CreateVertexBuffer( sizeof( VERTEX_3D ) * ( numHeight + 1 ) * ( numWidth + 1 ), D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &pVtxBuffer, NULL );	
		}

	}

	// ���_�o�b�t�@�ۑ�
	SetVertexBuffer( pVtxBuffer );

	return S_OK;
}

//=================================================================================
//	���_���̃Z�b�g
//=================================================================================
void CDoom::SetVertex( void )
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// ���z�������A�N�Z�X�p
	VERTEX_3D *pVtx;
	LPDIRECT3DVERTEXBUFFER9 pVtxBuffer = GetVertexBuffer();
	float width = GetWidth();
	float height = GetHeight();
	int numWidth = GetNumWidth();
	int numHeight = GetNumHeight();

	// �e�N�X�`�����WUV
	float fOffSetTexU = 1.0f / numWidth;
	float fTexU = 0.0f;
	float fOffSetTexV = 1.0f / numHeight;
	float fTexV = 0.0f;
	static float MoveU = 0.0f;

	// ���[�v�p�Y��
	int nIndex = 0;
	
	// ���z�������ɏ�������
	pVtxBuffer->Lock( 0, 0, ( void** )&pVtx, 0 );
	
	// �������猩���ړ��p
	float fAngleValXZ = m_AngleXZ / numWidth;
	float fAngleXZ = 0.0f;
	
	// �悱���猩���ړ��p
	float fAngleValXY = m_AngleXY / numHeight;
	float fAngleXY = 90.0f;

	// ���̔��a
	float fr = (width * numWidth) * 0.5f;
	
	// �ړ���X
	float fLength = 0.0f;

	// ���W�ۑ��p(�O)
	D3DXVECTOR3 fPos = D3DXVECTOR3( 0.0f, fr, 0.0f );

	// �t�B�[���h�̏c�~���|���S���������J��Ԃ�
	for( int nCntDoomY = 0; nCntDoomY <= numHeight; nCntDoomY++ )
	{
		// ���[�v�p�Y���X�V
		nIndex = nCntDoomY * (numWidth + 1);

		for( int nCntDoomX = 0; nCntDoomX <= numWidth; nCntDoomX++ )
		{	
			// XZ���W��]
			fPos.x = fLength * cosf( AngleToRadian(fAngleXZ) );
			fPos.z = fLength * sinf( AngleToRadian(fAngleXZ) );

			// ���_�o�b�t�@�֊i�[
			pVtx[ nCntDoomX + nIndex ].pos	= fPos;
			pVtx[ nCntDoomX + nIndex ].tex	= D3DXVECTOR2( fTexU + MoveU, fTexV );
			pVtx[ nCntDoomX + nIndex ].nor	= D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
			pVtx[ nCntDoomX + nIndex ].col	= D3DCOLOR_RGBA( 255, 255, 255, 255 );
	
			//MyOutputDebugString( ("pVtx[ %d ] %f, %f, %f )\n"),nCntDoomX + nIndex, fPos.x, fPos.y, fPos.z );
			
			// U�l�V�t�g
			fTexU += fOffSetTexU;

			// ���v���Ɉړ�
			fAngleXZ += fAngleValXZ;
		}

		// U�l�N���A
		fTexU = 0.0f;

		// V�l�V�t�g
		fTexV += fOffSetTexV;

		// �p�x��������ړ�
		fAngleXY -= fAngleValXY;
		
		// Y�ړ�
		float fPrevPosY = fPos.y;
		float diffY = fPrevPosY - (fr * sinf( AngleToRadian(fAngleXY) ));
		fPos.y -= diffY;
		
		// X�ړ�
		fLength = fr * cosf( AngleToRadian(fAngleXY) );

		// XZ������
		fAngleXZ = 0.0f;
	}

	MoveU += 0.00008f;

	// ���z���������A�����b�N
	pVtxBuffer->Unlock();

	// �V�F�[�_�[�̒l�Z�b�g
	CCameraManager *pCameraManager = CSceneManager::GetCameraManager();
	CCamera *pCamera = pCameraManager->GetCamera(MAIN_CAMERA);
	CLightManager *pLightManager = CSceneManager::GetLightManager();
	CLight *pLight = pLightManager->GetLight(MAIN_LIGHT);
	CSBasic *pShader = dynamic_cast<CSBasic*>(GetShader());
	D3DXVECTOR3 dir = pLight->GetVecDir();
	pShader->SetParam(pCamera->GetViewMtx(), pCamera->GetProjMtx(), dir, pCamera->GetPosition(), D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f), "data/TEXTURE/skydome/309.JPG");

	return;
}

//=================================================================================
//	���[���h�s��v�Z�E�f�o�C�X�ɃZ�b�g
//=================================================================================
void CDoom::SetMatrix(void)
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

	//// �f�o�C�X�Ƀ��[���h�ϊ��s����w��
	//pDevice -> SetTransform( D3DTS_WORLD, &mtxWorld );

	//pDevice->SetTexture(0, CTextureManager::GetResource(GetKey()));

	// �f�o�C�X�Ƀ��[���h�ϊ��s����w��
	GetShader()->SetMatrix(mtxWorld);
}

//=================================================================================
//	���b�V���t�B�[���h����
//=================================================================================
CDoom *CDoom::Create( D3DXVECTOR3 pos, D3DXVECTOR3 rot, float width, float  height,int numWidth, int numHeight, float angleXY, float angleXZ, const char* texPath )
{
	CDoom *pDoom = NULL;

	pDoom = new CDoom;

	// ���W�ݒ�
	pDoom->SetPosition( pos );

	// ��]�p�x�ݒ�
	pDoom->SetRotation( rot );

	// �|���S���̉��T�C�Y�ݒ�
	pDoom->SetWidth( width );
	
	// �|���S���̏c�T�C�Y�ݒ�
	pDoom->SetHeight( height );
	
	// ���u���b�N���ݒ�
	pDoom->SetNumWidth( numWidth );													
	
	// �c�u���b�N���ݒ�
	pDoom->SetNumHeight( numHeight );
	
	// XY���Ԃ̊p�x(90�Ŕ���)
	pDoom->SetAngleXY( angleXY );
	
	// XZ���Ԃ̊p�x(361�Ŕ���)
	pDoom->SetAngleXZ( angleXZ );
	
	// �k�ރ|���S���܂߂����|���S����
	pDoom->SetNumPolygon( (( numWidth + 2 ) * ( numHeight -1 ) + numWidth ) * 2 );

	// �e�N�X�`���ǂݍ���
	pDoom->SetTexture( texPath );

	// �V�F�[�_�[�̃Z�b�g
	CShaderManager *pShaderManager = CSceneManager::GetShaderManager();
	CSBasic *pShader = dynamic_cast<CSBasic*>(pShaderManager->GetData(SHADER_BASIC));
	pDoom->SetShader(pShader);

	pDoom->Init();

	return pDoom;
}

float CDoom::AngleToRadian( float angle )
{
	return (angle * 3.14f) / 180.0f;
};
