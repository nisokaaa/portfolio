#include "shaderDeferredGeometry.h"
#include "renderer.h"
#include "camera.h"

HRESULT CShaderDeferredGeometry::Init(void)
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

	if (FAILED(D3DXCompileShaderFromFile("data/HLSL/DeferredGeometry_vs.hlsl", NULL, NULL, "main", "vs_3_0", 0, &code, &err, &m_pVtxConstantTable))) {
		MessageBox(NULL, (LPCSTR)err->GetBufferPointer(), "D3DXCompileShaderFromFile(PV)", MB_OK);
		err->Release();
		return E_FAIL;
	}

	if (FAILED(pDevice->CreateVertexShader((LPDWORD)code->GetBufferPointer(), &m_pVtxShader))) {
		MessageBox(NULL, (LPCSTR)err->GetBufferPointer(), "CreateVtxShader", MB_OK);
		code->Release();
		return E_FAIL;
	}

	if (FAILED(D3DXCompileShaderFromFile("data/HLSL/DeferredGeometry_ps.hlsl", NULL, NULL, "main", "ps_3_0", 0, &code, &err, &m_pPConstantTable))) {
		MessageBox(NULL, (LPCSTR)err->GetBufferPointer(), "D3DXCompileShaderFromFile(PS)", MB_OK);
		err->Release();
		return E_FAIL;
	}

	if (FAILED(pDevice->CreatePixelShader((LPDWORD)code->GetBufferPointer(), &m_pPS))) {
		MessageBox(NULL, (LPCSTR)err->GetBufferPointer(), "CreatePixelShader", MB_OK);
		code->Release();
		return E_FAIL;
	}

	if (FAILED(D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/no_material.png", &m_pNoTexture)))
	{
		MessageBox(NULL, "data/TEXTURE/no_material.png", "LOAD ERROR!", MB_OK);
		return E_FAIL;
	}

	return S_OK;
}


void CShaderDeferredGeometry::Begin(
	const D3DXMATRIX& mtxView,
	const D3DXMATRIX& mtxProj,
	const D3DXVECTOR3& posEye,
	const float* apSplitPos,
	const D3DXMATRIX& shadowMatrix0,
	const D3DXMATRIX& shadowMatrix1,
	const D3DXMATRIX& shadowMatrix2
	)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	
	pDevice->SetPixelShader(NULL);

	pDevice->SetVertexDeclaration(m_pVertexDeclaration);
	pDevice->SetVertexShader(m_pVtxShader);
	pDevice->SetPixelShader(m_pPS);

	m_MtxView = mtxView;
	m_MtxProj = mtxProj;

	m_pVtxConstantTable->SetValue(pDevice, "posEyeW", &posEye, sizeof(posEye));
	
	UINT samplerIndex4 = m_pPConstantTable->GetSamplerIndex("depthSampler");
	
	pDevice->SetTexture(samplerIndex4, CRenderer::GetZBuffTexture());

	D3DXVECTOR3 splitPos = D3DXVECTOR3(apSplitPos[1], apSplitPos[2], apSplitPos[3]);
	m_pVtxConstantTable->SetValue(pDevice, "SplitPos", &splitPos, sizeof(splitPos));

	m_pVtxConstantTable->SetMatrix(pDevice, "Shadow0", &shadowMatrix0);
	m_pVtxConstantTable->SetMatrix(pDevice, "Shadow1", &shadowMatrix1);
	m_pVtxConstantTable->SetMatrix(pDevice, "Shadow2", &shadowMatrix2);

	UINT samplerIndex0 = m_pPConstantTable->GetSamplerIndex("ShadowMap0");
	UINT samplerIndex1 = m_pPConstantTable->GetSamplerIndex("ShadowMap1");
	UINT samplerIndex2 = m_pPConstantTable->GetSamplerIndex("ShadowMap2");

	pDevice->SetTexture(samplerIndex0, CRenderer::GetTestTexture(0));
	pDevice->SetTexture(samplerIndex1, CRenderer::GetTestTexture(1));
	pDevice->SetTexture(samplerIndex2, CRenderer::GetTestTexture(2));

	//float bBrightPassFlg = bBrightPassON ? 1.0f : 0.0f;
	//m_pPConstantTable->SetValue(pDevice, "bBrightPassFlg", &bBrightPassFlg, sizeof(bBrightPassFlg));

	m_pShader = this;

}

void CShaderDeferredGeometry::SetParam(const float& specpower, const bool& bshadowflg)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	m_pPConstantTable->SetValue(pDevice, "SpecPower", &specpower, sizeof(specpower));
	float bShadowFlg = bshadowflg ? 1.0f : 0.0f;
	m_pPConstantTable->SetValue(pDevice, "bShadowFlg", &bShadowFlg, sizeof(bShadowFlg));
}

void CShaderDeferredGeometry::SetMatrix(const D3DXMATRIX& mtxWorld)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	D3DXMATRIX mtxWI, mtxWIT, mtxWVP;
	mtxWVP = mtxWorld * m_MtxView * m_MtxProj;
	D3DXMatrixInverse(&mtxWI, NULL, &mtxWorld);
	D3DXMatrixTranspose(&mtxWIT, &mtxWI);

	m_pVtxConstantTable->SetMatrix(pDevice, "mtxW", &mtxWorld);
	m_pVtxConstantTable->SetMatrix(pDevice, "mtxWVP", &mtxWVP);
	m_pVtxConstantTable->SetMatrix(pDevice, "mtxWIT", &mtxWIT);
}


void CShaderDeferredGeometry::End(void)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	pDevice->SetPixelShader(NULL);
	pDevice->SetVertexShader(NULL);
	pDevice->SetVertexDeclaration(NULL);

	//m_pShader = NULL;
}
