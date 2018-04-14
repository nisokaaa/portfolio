//=================================================================================
//	�v���O������	�F	
//	����			�F	���������u���[[shaderVerticalBlurFiltter.cpp]
//	���C������		�F	shaderManager.cpp
//	�쐬��			�F	moriya gaku
//=================================================================================

//=================================================================================
// �C���N���[�h�t�@�C��
//=================================================================================
#include "shaderVerticalBlurFilter.h"
#include "renderer.h"

//=================================================================================
// �������֐�
//=================================================================================
HRESULT CSVerticalBlurFilter::Init(void)
{
	LPD3DXBUFFER err = NULL;
	LPD3DXBUFFER code = NULL;

	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	if (FAILED(D3DXCompileShaderFromFile("data/HLSL/VerticalBlurFilter_ps.hlsl", NULL, NULL, "main", "ps_3_0", 0, &code, &err, &m_pPConstantTable))) {
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
// �I���֐�
//=================================================================================
void CSVerticalBlurFilter::Uninit(void)
{
}

//=================================================================================
// �`��J�n�֐�
//=================================================================================
void CSVerticalBlurFilter::Begin(void)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_MIRROR); // �e�N�X�`��V�l�̌J��Ԃ��ݒ�(�ύX)

	pDevice->SetPixelShader(m_pPS);

	//pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP); // �e�N�X�`���t�l�̌J��Ԃ��ݒ�(����)
}

//=================================================================================
// �l�ݒ�֐�
//=================================================================================
void CSVerticalBlurFilter::SendData(LPDIRECT3DTEXTURE9 pTexture, float mag)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	float height = (float)SCREEN_HEIGHT * mag;
	m_pPConstantTable->SetValue(pDevice, "SCREEN_HEIGHT", &height, sizeof(height));
	UINT samplerIndex0 = m_pPConstantTable->GetSamplerIndex("sampler0");
	pDevice->SetTexture(samplerIndex0, pTexture);
}

//=================================================================================
// �`��֐�
//=================================================================================
void CSVerticalBlurFilter::Draw(float mag)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// FVF�i������g�p���钸�_���j�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �|���S���̕`�揈��
	VERTEX_2D vtx[] = {
		{ D3DXVECTOR3(0.0f - 0.5f, 0.0f - 0.5f, 0.0f),									1.0f, D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(0.0f, 0.0f) },
		{ D3DXVECTOR3((SCREEN_WIDTH * mag) - 0.5f, 0.0f - 0.5f, 0.0f),					1.0f, D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(1.0f, 0.0f) },
		{ D3DXVECTOR3(0.0f - 0.5f, (SCREEN_HEIGHT * mag) - 0.5f, 0.0f),					1.0f, D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(0.0f, 1.0f) },
		{ D3DXVECTOR3((SCREEN_WIDTH * mag) - 0.5f, (SCREEN_HEIGHT * mag) - 0.5f, 0.0f), 1.0f, D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(1.0f, 1.0f) },
	};

	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, &vtx[0], sizeof(VERTEX_2D));
}

//=================================================================================
// �`��I���֐�
//=================================================================================
void CSVerticalBlurFilter::End(void)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	pDevice->SetPixelShader(NULL);
}

//=================================================================================
// �e�N�X�`���ɏ�������
//=================================================================================
void CSVerticalBlurFilter::Fillter(LPDIRECT3DSURFACE9 surface, LPDIRECT3DTEXTURE9 pTexture, float mag)
{
	Begin();
	CRenderer::GetDevice()->SetRenderTarget(0, surface);
	SendData(pTexture, mag);
	Draw(mag);
	End();
}