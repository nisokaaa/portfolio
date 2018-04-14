//=================================================================================
//	�v���O������	�F	
//	����			�F	�`��ݒ菈��[renderer.cpp]
//	���C������		�F	main.cpp
//	�쐬��			�F	moriya gaku
//=================================================================================

//=================================================================================
//	�C���N���[�h�t�@�C��
//=================================================================================
#include "main.h"
#include "renderer.h"
#include "debug.h"

//=================================================================================
//�@�ÓI�����o�ϐ���錾
//=================================================================================
LPDIRECT3D9 CRenderer::m_pD3D = NULL;
LPDIRECT3DDEVICE9 CRenderer::m_pD3DDevice = NULL;
LPDIRECT3DSURFACE9 CRenderer::m_pDeviceSurf = NULL;
LPDIRECT3DSURFACE9 CRenderer::m_pDeviceDepthStencil = NULL;

//=================================================================================
//	�R���X�g���N�^
//=================================================================================
CRenderer::CRenderer()
{
	m_pD3D = NULL;
	m_pD3DDevice = NULL;
}

//=================================================================================
//	�f�X�g���N�^
//=================================================================================
CRenderer::~CRenderer()
{
}

//=================================================================================
//	����������
//=================================================================================
HRESULT CRenderer::Init( HWND hWnd, bool bWindow )
{
	// D3D�̐ݒ���\����
	D3DPRESENT_PARAMETERS d3dpp;

	// �f�B�X�v���C�̏�ԍ\����
    D3DDISPLAYMODE d3ddm;

	// Direct3D�I�u�W�F�N�g�̍쐬
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if(m_pD3D == NULL)
	{
		return E_FAIL;
	}

	// ���݂̃f�B�X�v���C���[�h���擾
    if(FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// �f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp));								// ���[�N���[���N���A
	d3dpp.BackBufferCount			 = 1;							// �o�b�N�o�b�t�@�̐�
	d3dpp.BackBufferWidth			 = SCREEN_WIDTH;				// �Q�[����ʃT�C�Y(��)
	d3dpp.BackBufferHeight			 = SCREEN_HEIGHT;				// �Q�[����ʃT�C�Y(����)
	d3dpp.BackBufferFormat			 = d3ddm.Format;				// �J���[���[�h�̎w��
	d3dpp.SwapEffect				 = D3DSWAPEFFECT_DISCARD;		// �f���M���ɓ������ăt���b�v����
	d3dpp.EnableAutoDepthStencil	 = TRUE;						// �f�v�X�o�b�t�@�i�y�o�b�t�@�j�ƃX�e���V���o�b�t�@���쐬
	d3dpp.AutoDepthStencilFormat	 = D3DFMT_D16;					// �f�v�X�o�b�t�@�Ƃ���16bit���g��
	d3dpp.Windowed					 = bWindow;						// �E�B���h�E���[�h
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		// ���t���b�V�����[�g
	d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_DEFAULT;	// �C���^�[�o��

	// �f�o�C�X�̐���
	// �f�B�X�v���C�A�_�v�^��\�����߂̃f�o�C�X���쐬
	// �`��ƒ��_�������n�[�h�E�F�A�ōs�Ȃ�
	if(FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, 
									D3DDEVTYPE_HAL, 
									hWnd, 
									D3DCREATE_HARDWARE_VERTEXPROCESSING, 
									&d3dpp, &m_pD3DDevice)))
	{
		// ��L�̐ݒ肪���s������
		// �`����n�[�h�E�F�A�ōs���A���_������CPU�ōs�Ȃ�
		if(FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, 
										D3DDEVTYPE_HAL, 
										hWnd, 
										D3DCREATE_SOFTWARE_VERTEXPROCESSING, 
										&d3dpp, &m_pD3DDevice)))
		{
			// ��L�̐ݒ肪���s������
			// �`��ƒ��_������CPU�ōs�Ȃ�
			if(FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, 
											D3DDEVTYPE_REF, hWnd, 
											D3DCREATE_SOFTWARE_VERTEXPROCESSING, 
											&d3dpp, &m_pD3DDevice)))
			{
				// �������s
				return E_FAIL;
			}
		}
	}

	// �����_�[�X�e�[�g�̐ݒ�
    m_pD3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE /*D3DCULL_CCW*/ );		// �J�����O�ݒ�
	m_pD3DDevice->SetRenderState( D3DRS_ZENABLE, TRUE );								// Z�o�b�t�@���g�p����
	m_pD3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );						// ���u�����h���s��
	m_pD3DDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );					// ���\�[�X�J���[�̎w��
	m_pD3DDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );				// ���f�X�e�B�l�[�V�����J���[�̎w��


	//m_pD3DDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );				// ���C���[�t���[���I��


	// �T���v���[�X�e�[�g�̐ݒ�
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);				// �e�N�X�`���t�l�̌J��Ԃ��ݒ�
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);				// �e�N�X�`���u�l�̌J��Ԃ��ݒ�
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);				// �e�N�X�`���g�厞�̕�Ԑݒ�
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);				// �e�N�X�`���k�����̕�Ԑݒ�

	// �e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);			// �A���t�@�u�����f�B���O����(�����l��D3DTOP_SELECTARG1)
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);				// �ŏ��̃A���t�@����(�����l��D3DTA_TEXTURE�A�e�N�X�`�����Ȃ��ꍇ��D3DTA_DIFFUSE)
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);				// �Q�Ԗڂ̃A���t�@����(�����l��D3DTA_CURRENT)

	m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);  
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	// �f�o�C�X�̃o�b�N�o�b�t�@��ۑ�
	m_pD3DDevice->GetRenderTarget(0, &m_pDeviceSurf);				//�o�b�N�o�b�t�@�̃T�[�t�F�C�X���擾
	m_pD3DDevice->GetDepthStencilSurface(&m_pDeviceDepthStencil);	//�o�b�N�o�b�t�@�̐[�x�o�b�t�@���擾

	// �f�o�b�O
	CDebug::Init( m_pD3DDevice );

	return S_OK;
}

//=================================================================================
//	�I������
//=================================================================================
void CRenderer::Uninit( void )
{
	// �f�o�b�O�N���X
	CDebug::Uninit();

	// �f�o�C�X�̔j��
	if(m_pD3DDevice != NULL)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = NULL;
	}

	// Direct3D�I�u�W�F�N�g�̔j��
	if( m_pD3D != NULL)
	{
		m_pD3D->Release();
		m_pD3D = NULL;
	}

	return;
}

//===============================================================================
// �`��J�n����
//===============================================================================
HRESULT CRenderer::DrawBegin( void )
{
	// �f�t�H���g�̃����_�[�^�[�Q�b�g�֐؂�ւ�
	m_pD3DDevice->SetRenderTarget(0, m_pDeviceSurf);
	m_pD3DDevice->SetDepthStencilSurface(m_pDeviceDepthStencil);

	// ��ʃN���A
	m_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), COLOR_GRAY2, 1.0f, 0);

	// Direct3D�ɂ��`��̊J�n
	if(FAILED(m_pD3DDevice->BeginScene()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//===============================================================================
// �`��I������
//===============================================================================
void CRenderer::DrawEnd( void )
{
	// Direct3D�ɂ��`��̏I��
	m_pD3DDevice->EndScene();

	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);

}

//===============================================================================
// �f�o�C�X�̎擾����
//===============================================================================
LPDIRECT3DDEVICE9 CRenderer::GetDevice( void )
{
	if( m_pD3DDevice != NULL )
	{
		return m_pD3DDevice;
	}
	
	return NULL;
}

//===============================================================================
// �f�o�C�X�̎擾����
//===============================================================================
#ifdef _DEBUG
void CRenderer::DrawFPS( void )
{
	RECT rect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
	char str[256];
	int nCountFPS;

	// FPS�擾
	nCountFPS = GetFPS();
	wsprintf(str, "FPS:%d\n", nCountFPS);

	// �e�L�X�g�`��
	CDebug::AddText( str );

	return;
}
#endif