//=================================================================================
//	�v���O������	�F	
//	����			�F	�e�N�X�`���u�����h�V�F�[�_����[shaderMeshField.cpp]
//	���C������		�F	main.cpp
//	�쐬��			�F	moriya gaku
//=================================================================================

//=================================================================================
//	�C���N���[�h�t�@�C��
//=================================================================================
#include "shaderMeshField.h"
#include "renderer.h"
#include "sceneManager.h"
#include "textureManager.h"

//=================================================================================
//�@�������֐�
//=================================================================================
HRESULT CSMeshField::Init(void)
{
	LPD3DXBUFFER err = NULL;
	LPD3DXBUFFER code = NULL;

	D3DVERTEXELEMENT9 elements[] = {
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
		{ 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		D3DDECL_END()
	};

	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	pDevice->CreateVertexDeclaration(elements, &m_pVertexDeclaration);

	if (FAILED(D3DXCompileShaderFromFile("data/HLSL/MeshField_vs.hlsl", NULL, NULL, "main", "vs_3_0", 0, &code, &err, &m_pVtxConstantTable))) {
		MessageBox(NULL, (LPCSTR)err->GetBufferPointer(), "D3DXCompileShaderFromFile(PV)", MB_OK);
		err->Release();
		return E_FAIL;
	}

	if (FAILED(pDevice->CreateVertexShader((LPDWORD)code->GetBufferPointer(), &m_pVtxShader))) {
		MessageBox(NULL, (LPCSTR)err->GetBufferPointer(), "CreateVtxShader", MB_OK);
		code->Release();
		return E_FAIL;
	}

	if (FAILED(D3DXCompileShaderFromFile("data/HLSL/MeshField_ps.hlsl", NULL, NULL, "main", "ps_3_0", 0, &code, &err, &m_pPConstantTable))) {
		MessageBox(NULL, (LPCSTR)err->GetBufferPointer(), "D3DXCompileShaderFromFile(PS)", MB_OK);
		err->Release();
		return E_FAIL;
	}

	if (FAILED(pDevice->CreatePixelShader((LPDWORD)code->GetBufferPointer(), &m_pPS))) {
		MessageBox(NULL, (LPCSTR)err->GetBufferPointer(), "CreatePixelShader", MB_OK);
		code->Release();
		return E_FAIL;
	}

	return S_OK;
}

//=================================================================================
// �I���֐�
//=================================================================================
void CSMeshField::Uninit(void)
{
}

//=================================================================================
//�@�V�F�[�_�[�J�n�֐�
//=================================================================================
void CSMeshField::Begin(void)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// �J�n
	pDevice->SetVertexDeclaration(m_pVertexDeclaration);
	pDevice->SetVertexShader(m_pVtxShader);
	pDevice->SetPixelShader(m_pPS);
}

//=================================================================================
//�@�l�ݒ�֐�
//=================================================================================
void CSMeshField::SendData(void)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	UINT samplerIndex0 = m_pPConstantTable->GetSamplerIndex("texture1");
	UINT samplerIndex1 = m_pPConstantTable->GetSamplerIndex("texture2");

	pDevice->SetTexture(samplerIndex0, CTextureManager::GetResource("data/TEXTURE/rock_surface.jpg"));
	pDevice->SetTexture(samplerIndex1, CTextureManager::GetResource("data/TEXTURE/sand.jpg"));

	m_pPConstantTable->SetValue(pDevice, "nMaxHeight", &m_nMaxHeight, sizeof(m_nMaxHeight));
	m_pPConstantTable->SetValue(pDevice, "nMinHeight", &m_nMinHeight, sizeof(m_nMinHeight));
	m_pPConstantTable->SetValue(pDevice, "eyePosW", &m_EyePosW, sizeof(m_EyePosW));
	m_pPConstantTable->SetValue(pDevice, "lightDirW", &m_LightDirW, sizeof(m_LightDirW));
}

//=================================================================================
//�@�V�F�[�_�[�I���֐�
//=================================================================================
void CSMeshField::End(void)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// �I��
	pDevice->SetPixelShader(NULL);
	pDevice->SetVertexShader(NULL);
	pDevice->SetVertexDeclaration(NULL);
}

//=================================================================================
//�@�l�ݒ�֐�
//=================================================================================
void CSMeshField::SetParam(const D3DXMATRIX& mtxView, const D3DXMATRIX& mtxProj,
	const int& nMinHeight, const int& nMaxHeight, const D3DXVECTOR3& lightDirW, const D3DXVECTOR3& eyePosW)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	m_MtxView		= mtxView;
	m_MtxProj		= mtxProj;
	m_nMaxHeight	= nMaxHeight;
	m_nMinHeight	= nMinHeight;
	m_EyePosW		= eyePosW;
	m_LightDirW		= lightDirW;
}

//=================================================================================
//�@�s��ݒ�֐�
//=================================================================================
void CSMeshField::SetMatrix(const D3DXMATRIX& mtxWorld)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	D3DXMATRIX mtxWI, mtxWIT, mtxWVP;
	mtxWVP = mtxWorld * m_MtxView * m_MtxProj;
	D3DXMatrixInverse(&mtxWI, NULL, &mtxWorld);
	D3DXMatrixTranspose(&mtxWIT, &mtxWI);

	m_pVtxConstantTable->SetMatrix(pDevice, "mtxWorld", &mtxWorld);
	m_pVtxConstantTable->SetMatrix(pDevice, "mtxWVP", &mtxWVP);
	m_pVtxConstantTable->SetMatrix(pDevice, "mtxWIT", &mtxWIT);
}