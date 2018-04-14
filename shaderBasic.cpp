//=================================================================================
//	プログラム名	：	
//	処理			：	通常シェーダ処理[shaderBasic.cpp]
//	メイン処理		：	main.cpp
//	作成者			：	moriya gaku
//=================================================================================

//=================================================================================
//	インクルードファイル
//=================================================================================
#include "shaderBasic.h"
#include "renderer.h"
#include "sceneManager.h"
#include "textureManager.h"

//=================================================================================
//　初期化関数
//=================================================================================
HRESULT CSBasic::Init(void)
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

	if (FAILED(D3DXCompileShaderFromFile("data/HLSL/Basic_vs.hlsl", NULL, NULL, "main", "vs_3_0", 0, &code, &err, &m_pVtxConstantTable))) {
		MessageBox(NULL, (LPCSTR)err->GetBufferPointer(), "D3DXCompileShaderFromFile(PV)", MB_OK);
		err->Release();
		return E_FAIL;
	}

	if (FAILED(pDevice->CreateVertexShader((LPDWORD)code->GetBufferPointer(), &m_pVtxShader))) {
		MessageBox(NULL, (LPCSTR)err->GetBufferPointer(), "CreateVtxShader", MB_OK);
		code->Release();
		return E_FAIL;
	}

	if (FAILED(D3DXCompileShaderFromFile("data/HLSL/Basic_ps.hlsl", NULL, NULL, "main", "ps_3_0", 0, &code, &err, &m_pPConstantTable))) {
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
// 終了関数
//=================================================================================
void CSBasic::Uninit(void)
{
}

//=================================================================================
//　シェーダー開始関数
//=================================================================================
void CSBasic::Begin(void)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// 開始
	pDevice->SetVertexDeclaration(m_pVertexDeclaration);
	pDevice->SetVertexShader(m_pVtxShader);
	pDevice->SetPixelShader(m_pPS);

	/*pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);*/
}

//=================================================================================
//　シェーダへデータ送信
//=================================================================================
void CSBasic::SendData(void)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	UINT samplerIndex0 = m_pPConstantTable->GetSamplerIndex("texture1");

	pDevice->SetTexture(samplerIndex0, CTextureManager::GetResource("data/TEXTURE/skydome/309.JPG"));

	m_pPConstantTable->SetValue(pDevice, "Material", &m_Mat, sizeof(m_Mat));
	m_pPConstantTable->SetValue(pDevice, "eyePosW", &m_EyePosW, sizeof(m_EyePosW));
	m_pPConstantTable->SetValue(pDevice, "lightDirW", &m_LightDirW, sizeof(m_LightDirW));
}


//=================================================================================
//　シェーダー終了関数
//=================================================================================
void CSBasic::End(void)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// 終了
	pDevice->SetPixelShader(NULL);
	pDevice->SetVertexShader(NULL);
	pDevice->SetVertexDeclaration(NULL);
}

//=================================================================================
//　値設定関数
//=================================================================================
void CSBasic::SetParam(const D3DXMATRIX& mtxView, const D3DXMATRIX& mtxProj, const D3DXVECTOR3& lightDirW, const D3DXVECTOR3& eyePosW, const D3DXVECTOR4& mat, std::string key)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	m_MtxView	= mtxView;
	m_MtxProj	= mtxProj;
	m_Mat		= mat;
	m_LightDirW = lightDirW;
	m_EyePosW	= eyePosW;
}

//=================================================================================
//　行列設定関数
//=================================================================================
void CSBasic::SetMatrix(const D3DXMATRIX& mtxWorld)
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