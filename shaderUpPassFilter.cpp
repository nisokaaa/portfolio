//=================================================================================
//	プログラム名	：	
//	処理			：	1/4バッファにサイズアップ[shaderUpPassFiltter.cpp]
//	メイン処理		：	shaderManager.cpp
//	作成者			：	moriya gaku
//=================================================================================

//=================================================================================
// インクルードファイル
//=================================================================================
#include "shaderUpPassFilter.h"
#include "renderer.h"

//=================================================================================
// 初期化関数
//=================================================================================
HRESULT CSUpPassFilter::Init(void)
{
	LPD3DXBUFFER err = NULL;
	LPD3DXBUFFER code = NULL;

	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	if (FAILED(D3DXCompileShaderFromFile("data/HLSL/UpPassFilter_ps.hlsl", NULL, NULL, "main", "ps_3_0", 0, &code, &err, &m_pPConstantTable))) {
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

//=================================================================================
// 終了関数
//=================================================================================
void CSUpPassFilter::Uninit(void)
{
}

//=================================================================================
// 描画開始関数
//=================================================================================
void CSUpPassFilter::Begin(void)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	pDevice->SetPixelShader(m_pPS);
}

//=================================================================================
// 値設定関数
//=================================================================================
void CSUpPassFilter::SendData(LPDIRECT3DTEXTURE9 pTexture)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	UINT samplerIndex0 = m_pPConstantTable->GetSamplerIndex("Sampler0");
	pDevice->SetTexture(samplerIndex0, pTexture);
}

//=================================================================================
// 描画関数
//=================================================================================
void CSUpPassFilter::Draw(float mag)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// FVF（今から使用する頂点情報）の設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// ポリゴンの描画処理
	VERTEX_2D vtx[] = {
		{ D3DXVECTOR3(0.0f - 0.5f, 0.0f - 0.5f, 0.0f),									1.0f, D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(0.0f, 0.0f) },
		{ D3DXVECTOR3((SCREEN_WIDTH * mag) - 0.5f, 0.0f - 0.5f, 0.0f),					1.0f, D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(1.0f, 0.0f) },
		{ D3DXVECTOR3(0.0f - 0.5f, (SCREEN_HEIGHT * mag) - 0.5f, 0.0f),					1.0f, D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(0.0f, 1.0f) },
		{ D3DXVECTOR3((SCREEN_WIDTH * mag) - 0.5f, (SCREEN_HEIGHT * mag) - 0.5f, 0.0f), 1.0f, D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(1.0f, 1.0f) },
	};

	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, &vtx[0], sizeof(VERTEX_2D));
}

//=================================================================================
// 描画終了関数
//=================================================================================
void CSUpPassFilter::End(void)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	pDevice->SetPixelShader(NULL);
}

//=================================================================================
// テクスチャに書き込む
//=================================================================================
void CSUpPassFilter::Fillter(LPDIRECT3DSURFACE9 surface, LPDIRECT3DTEXTURE9 pTexture, float mag)
{
	Begin();
	SendData(pTexture);
	CRenderer::GetDevice()->SetRenderTarget(0, surface);
	Draw(mag);
	End();
}