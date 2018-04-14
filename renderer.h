#pragma once
//=================================================================================
//	�v���O������	�F	
//	����			�F	�`��ݒ菈��[renderer.h]
//	���C������		�F	main.cpp
//	�쐬��			�F	moriya gaku
//=================================================================================

//=================================================================================
//	��d��`�h�~
//=================================================================================
#ifndef _RENDERER_H_
#define _RENDERER_H_

//=================================================================================
//	�C���N���[�h�t�@�C��
//=================================================================================
#include "main.h"

class CRenderer
{
public:
	CRenderer();								
	~CRenderer();		

	static HRESULT Init( HWND hWnd, bool bWindow );	
	static void Uninit( void );

	static HRESULT DrawBegin( void );
	static void DrawEnd( void );
	
	static void DrawFPS( void );						// FPS�`��
	static void DrawMenu( void );						// ������@�\��

	static LPDIRECT3DDEVICE9 GetDevice( void );		

	static LPDIRECT3DSURFACE9 GetDeviceSurf(void) { return m_pDeviceSurf; }
	static LPDIRECT3DSURFACE9 GetDeviceDepthStencil(void) { return m_pDeviceDepthStencil; }
	
private:
	static LPDIRECT3D9	m_pD3D;							// Direct3D�I�u�W�F�N�g
	static LPDIRECT3DDEVICE9 m_pD3DDevice;				// �f�o�C�X�C���^�t�F�[�X

	static LPDIRECT3DSURFACE9 m_pDeviceSurf;			// �o�b�N�o�b�t�@�T�[�t�F�X�APresent()�Ŏw�肷��ׂɕێ�
	static LPDIRECT3DSURFACE9 m_pDeviceDepthStencil;	// �f�v�X�X�e���V���T�[�t�F�C�X
};

#endif