//=================================================================================
//	�v���O������	�F	
//	����			�F	�w�i�e�N�X�`��[background.cpp]
//	���C������		�F	main.cpp
//	�쐬��			�F	moriya gaku
//=================================================================================

//=================================================================================
//	�C���N���[�h�t�@�C��
//=================================================================================
#include "background.h"
#include "shaderManager.h"
#include "sceneManager.h"
#include "renderer.h"

//=================================================================================
// ����������
//=================================================================================
HRESULT CBackGround::Init(void)
{
	return S_OK;
}

//=================================================================================
// �I������
//=================================================================================
void CBackGround::Uninit(void)
{
}

//=================================================================================
// �X�V����
//=================================================================================
void CBackGround::Update(void)
{
}

//=================================================================================
// �`�揈��
//=================================================================================
void CBackGround::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	pDevice->SetTexture(0, m_pFinalTexture);

	CRenderer::GetDevice()->SetRenderTarget(0, CRenderer::GetDeviceSurf());
	
	// �`��
	pDevice->SetFVF(FVF_VERTEX_2D);

	//pDevice->SetTexture(0, m_pTexture);

	VERTEX_2D vtx[] = {
		{ D3DXVECTOR3(0.0f, 0.0f, 0.0f),					1.0f, D3DCOLOR_RGBA(30, 30, 30, 255), D3DXVECTOR2(0.0f, 0.0f) },
		{ D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f),			1.0f, D3DCOLOR_RGBA(30, 30, 30, 255), D3DXVECTOR2(1.0f, 0.0f) },
		{ D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f),			1.0f, D3DCOLOR_RGBA(30, 30, 30, 255), D3DXVECTOR2(0.0f, 1.0f) },
		{ D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f),	1.0f, D3DCOLOR_RGBA(30, 30, 30, 255), D3DXVECTOR2(1.0f, 1.0f) },
	};

	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, &vtx[0], sizeof(VERTEX_2D));
}

//=================================================================================
// �e�N�X�`�����H����
//=================================================================================
void CBackGround::MakeTexture(void)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// �e�N�X�`�����H
	CShaderManager *pShaderManager = CSceneManager::GetShaderManager();
	CSUpPassFilter *pSUpPassFillter = dynamic_cast<CSUpPassFilter*>(pShaderManager->GetData(SHADER_UP_PASS_FILTTER));
	CSDownPassFilter *pSDownPassFillter = dynamic_cast<CSDownPassFilter*>(pShaderManager->GetData(SHADER_DOWN_PASS_FILTTER));
	CSHorizontalBlurFilter *pSHBlurFillter = dynamic_cast<CSHorizontalBlurFilter*>(pShaderManager->GetData(SHADER_HORIZONAL_BLUR_FILTTER));
	CSVerticalBlurFilter *pSVBlurFillter = dynamic_cast<CSVerticalBlurFilter*>(pShaderManager->GetData(SHADER_VERTICAL_BLUR_FILTTER));

	pSDownPassFillter->Begin();
	CRenderer::GetDevice()->SetRenderTarget(0, m_p1x4Surface);
	pSDownPassFillter->SendData(m_pFinalTexture);
	pSDownPassFillter->Draw(0.25f);
	pSDownPassFillter->End();

	pSVBlurFillter->Begin();
	CRenderer::GetDevice()->SetRenderTarget(0, m_p1x4Surface);
	pSVBlurFillter->SendData(m_p1x4Texture, 0.25f);
	pSVBlurFillter->Draw(0.25f);
	pSVBlurFillter->End();

	pSHBlurFillter->Begin();
	CRenderer::GetDevice()->SetRenderTarget(0, m_p1x4Surface);
	pSHBlurFillter->SendData(m_p1x4Texture, 0.25f);
	pSHBlurFillter->Draw(0.25f);
	pSHBlurFillter->End();

	pSUpPassFillter->Begin();
	CRenderer::GetDevice()->SetRenderTarget(0, m_pFinalSurface);
	pSUpPassFillter->SendData(m_p1x4Texture);
	pSUpPassFillter->Draw(1.00f);
	pSUpPassFillter->End();
}

//=================================================================================
// ��������
//=================================================================================
void CBackGround::Create(void)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// �e�N�X�`���ƃT�[�t�F�C�X�쐬
	UINT uiWidth = SCREEN_WIDTH;
	UINT uiHeight = SCREEN_HEIGHT;
	D3DXCreateTexture(pDevice, uiWidth, uiHeight, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8B8G8R8, D3DPOOL_DEFAULT, &m_pFinalTexture);
	D3DXCreateTexture(pDevice, uiWidth, uiHeight, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8B8G8R8, D3DPOOL_DEFAULT, &m_p1x1Texture);
	D3DXCreateTexture(pDevice, uiWidth * 0.25f, uiHeight * 0.25f, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8B8G8R8, D3DPOOL_DEFAULT, &m_p1x4Texture);
	D3DXCreateTexture(pDevice, uiWidth * 0.0625f, uiHeight * 0.0625f, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8B8G8R8, D3DPOOL_DEFAULT, &m_p1x16Texture);
	m_pFinalTexture->GetSurfaceLevel(0, &m_pFinalSurface);
	m_p1x1Texture->GetSurfaceLevel(0, &m_p1x1Surface);
	m_p1x4Texture->GetSurfaceLevel(0, &m_p1x4Surface);
	m_p1x16Texture->GetSurfaceLevel(0, &m_p1x16Surface);
}