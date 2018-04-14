//=================================================================================
//	�v���O������	�F	
//	����			�F	�f�o�b�O����[debug.cpp]
//	���C������		�F	main.cpp
//	�쐬��			�F	moriya gaku
//=================================================================================

//=================================================================================
//	�C���N���[�h�t�@�C��
//=================================================================================
#include "debug.h"
#include "renderer.h"

char CDebug::m_Text[DEBUG_CHAR_MAX] = { "" };
LPD3DXFONT CDebug::m_pFont = NULL;

//=================================================================================
// �`��֐�
//=================================================================================
void CDebug::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// �����`��
	RECT rect = { SCREEN_WIDTH - 500, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	m_pFont->DrawText(NULL, m_Text, -1, &rect, NULL, D3DCOLOR_RGBA(0, 255, 255, 255));
}

//=================================================================================
// �o�b�t�@�N���A�֐�
//=================================================================================
void CDebug::Clear(void) {
	ClearText();
}

//=================================================================================
// �����o�b�t�@�N���A�֐�
//=================================================================================
void CDebug::ClearText(void) {
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// ��������
	strcpy_s(m_Text, _countof(m_Text), "");
}

//=================================================================================
// �`��o�b�t�@�ɕ�����ǉ�����
//=================================================================================
void CDebug::AddText(const char *text) {
	strcat_s(m_Text, _countof(m_Text), text);
}