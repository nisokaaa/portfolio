//=================================================================================
//	�v���O������	�F	
//	����			�F	���C�g����[light.cpp]
//	���C������		�F	main.cpp
//	�쐬��			�F	moriya gaku
//=================================================================================
//=================================================================================
//	�C���N���[�h�t�@�C��
//=================================================================================
#include "main.h"
#include "light.h"
#include "renderer.h"
#include "input.h"
#include "sceneManager.h"
#include "manager.h"
#include "debug.h"

//=================================================================================
//	�R���X�g���N�^
//=================================================================================
CLight::CLight()
{
	ZeroMemory(&m_Light, sizeof(D3DLIGHT9));
}

//=================================================================================
//�@�f�X�g���N�^
//=================================================================================
CLight::~CLight()
{
}

//=================================================================================
//	�������֐�
//=================================================================================
void CLight::Init(LIGHT_TYPE type, D3DXVECTOR3 vecDir, D3DXVECTOR3 position, D3DXCOLOR color, D3DXVECTOR3 diffuse, D3DXVECTOR3 ambient)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	m_Color = color;

	m_Type = type;

	m_Light.Type = D3DLIGHT_DIRECTIONAL;

	// �Ǝ˕���
	D3DXVec3Normalize((D3DXVECTOR3*)&m_Light.Direction, &vecDir);

	// �ʒu
	m_Light.Position.x = position.x;
	m_Light.Position.y = position.y;
	m_Light.Position.z = position.z;

	// �g�U��
	m_Light.Diffuse.r = diffuse.x;
	m_Light.Diffuse.g = diffuse.y;
	m_Light.Diffuse.b = diffuse.z;

	// ����
	m_Light.Ambient.r = ambient.x;
	m_Light.Ambient.g = ambient.y;
	m_Light.Ambient.b = ambient.z;

	// �����_�����O�p�C�v���C���ɐݒ�
	pDevice->SetLight(type, &m_Light);

	// �ݒ�
	pDevice->LightEnable(type, TRUE);

	// ���C�e�B���O���[�h
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

}

//=================================================================================
//	�I���֐�
//=================================================================================
void CLight::Uninit(void)
{
	return;
}

//=================================================================================
//	�X�V�֐�
//=================================================================================
void CLight::Update(void)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	if (CManager::GetInputKeyboard()->GetKeyPress(DIK_Z))
	{
		m_Light.Direction.x -= 0.05f;
	}
	if (CManager::GetInputKeyboard()->GetKeyPress(DIK_X))
	{
		m_Light.Direction.x += 0.05f;
	}

	D3DXVec3Normalize((D3DXVECTOR3*)&m_Light.Direction, (D3DXVECTOR3*)&m_Light.Direction);

	// �����_�����O�p�C�v���C���ɐݒ�
	pDevice->SetLight(m_Type, &m_Light);

	// �ݒ�
	pDevice->LightEnable(m_Type, TRUE);

	// ���C�e�B���O���[�h
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	return;
}