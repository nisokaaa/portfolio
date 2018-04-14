#include "shaderDeferredLightingDirect.h"
#include "renderer.h"

//=============================================================================
// ‰Šú‰»ŠÖ”
//=============================================================================
HRESULT CShaderDeferredLightingDirect::Init(void)
{
	LPD3DXBUFFER err = NULL;
	LPD3DXBUFFER code = NULL;

	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	if (FAILED(D3DXCompileShaderFromFile("data/HLSL/DeferredLightingDirect_ps.hlsl", NULL, NULL, "main", "ps_3_0", 0, &code, &err, &m_pPConstantTable))) {
		MessageBox(NULL, (LPCSTR)err->GetBufferPointer(), "D3DXCompileShaderFromFile(PS)", MB_OK);
		err->Release();
		return E_FAIL;
	}

	if (FAILED(pDevice->CreatePixelShader((LPDWORD)code->GetBufferPointer(), &m_pPS))) {
		MessageBox(NULL, (LPCSTR)err->GetBufferPointer(), "CreatePixelShader", MB_OK);
		code->Release();
		return E_FAIL;
	}

	if (code) { code->Release(); }
	if (err) { err->Release(); }

	return S_OK;
}

//=============================================================================
// •`‰æŠJŽnŠÖ”
//=============================================================================
void CShaderDeferredLightingDirect::Begin(
	const D3DXVECTOR3& eyePosW,
	const D3DXVECTOR3& lightDirW)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	pDevice->SetPixelShader(NULL);
	pDevice->SetPixelShader(m_pPS);

	UINT samplerIndex0 = m_pPConstantTable->GetSamplerIndex("diffuseSampler");
	UINT samplerIndex1 = m_pPConstantTable->GetSamplerIndex("normalSampler");
	UINT samplerIndex2 = m_pPConstantTable->GetSamplerIndex("posW_SpecularPowerSampler");
	
	pDevice->SetTexture(samplerIndex0, CRenderer::GetColorTexture());
	pDevice->SetTexture(samplerIndex1, CRenderer::GetNormalTexture());
	pDevice->SetTexture(samplerIndex2, CRenderer::GetPosTexture());

	m_pPConstantTable->SetValue(pDevice, "eyePosW", &eyePosW, sizeof(eyePosW));
	m_pPConstantTable->SetValue(pDevice, "lightDirW", &lightDirW, sizeof(lightDirW));

	m_pShader = this;

}

//=============================================================================
// •`‰æŠÖ”
//=============================================================================
void CShaderDeferredLightingDirect::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// FVFi¡‚©‚çŽg—p‚·‚é’¸“_î•ñj‚ÌÝ’è
	pDevice->SetFVF(FVF_VERTEX_2D);

	// ƒ|ƒŠƒSƒ“‚Ì•`‰æˆ—
	VERTEX_2D vtx[] = {
		{ D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f), 1.0f, D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(0.0f, 0.0f) },
		{ D3DXVECTOR4(SCREEN_WIDTH, 0.0f, 1.0f, 1.0f), 1.0f, D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(1.0f, 0.0f) },
		{ D3DXVECTOR4(0.0f, SCREEN_HEIGHT, 1.0f, 1.0f), 1.0f, D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(0.0f, 1.0f) },
		{ D3DXVECTOR4(SCREEN_WIDTH, SCREEN_HEIGHT, 1.0f, 1.0f), 1.0f, D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(1.0f, 1.0f) },
	};

	pDevice->DrawPrimitiveUP(
		D3DPT_TRIANGLESTRIP,
		2,
		&vtx[0],
		sizeof(VERTEX_2D));
}

//=============================================================================
// •`‰æI—¹ŠÖ”
//=============================================================================
void CShaderDeferredLightingDirect::End(void)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	pDevice->SetPixelShader(NULL);
}
