#pragma once
//=================================================================================
//	�v���O������	�F	
//	����			�F	�f�o�b�O����[debug.h]
//	���C������		�F	main.cpp
//	�쐬��			�F	moriya gaku
//=================================================================================

//=================================================================================
//	��d��`�h�~
//=================================================================================
#ifndef _DEBUG_H_
#define _DEBUG_H_

//=================================================================================
//	�C���N���[�h�t�@�C��
//=================================================================================
#include <stdlib.h>
#include "main.h"

//=================================================================================
//	�}�N����`
//=================================================================================
#define DEBUG_CHAR_MAX ( 4096 )

class CDebug
{
public:
	CDebug(){}
	~CDebug(){}

	static void Init(LPDIRECT3DDEVICE9 pD3DDevice) {
		CDebug::AddText("\n_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/\n\n");
		D3DXCreateFont(pD3DDevice, 20, 10, 0, 0, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS,
			DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &m_pFont);
	}

	static void Uninit( void ){
		m_pFont->Release();
		m_pFont = NULL;
	}

	static void Clear(void);

	static void ClearText(void);

	static void AddText(const char *text);

	static void Draw(void);

private:
	static char m_Text[ DEBUG_CHAR_MAX ];
	static LPD3DXFONT m_pFont;
};


#endif