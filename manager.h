#pragma once
//=================================================================================
//	�v���O������	�F	
//	����			�F	�Q�[���Ǘ�[manager.h]
//	���C������		�F	main.cpp
//	�쐬��			�F	moriya gaku
//=================================================================================

//=================================================================================
//	��d��`�h�~
//=================================================================================
#ifndef _MANAGER_H_
#define _MANAGER_H_

//=================================================================================
//	�C���N���[�h�t�@�C��
//=================================================================================
#include "main.h"

//=================================================================================
//	�O���錾
//=================================================================================
class CInputKeyboard;
class CInputGamePad;
class CInputMouse;

class CManager
{
public:
	CManager();														
	~CManager();

	static HRESULT Init( HINSTANCE hInstance, HWND hWnd, bool bWindow );	
	static void Uninit( void );											
	static void Update( void );											
	static void Draw( void );											
	
	// �Ǌ�����C���^�[�t�F�[�X�̎擾�֐��FCManager�N���X�́Z�Z�N���X�������Ă���O��			
	static CInputKeyboard *GetInputKeyboard( void ){ return m_pInputKeyboard; }
	static CInputGamePad *GetInputGamePad( void ){ return m_pInputGamePad; }
	static CInputMouse *GetInputMouse( void ){ return m_pInputMouse; }

private:
	static CInputKeyboard *m_pInputKeyboard;
	static CInputGamePad *m_pInputGamePad;	
	static CInputMouse *m_pInputMouse;
};


#endif