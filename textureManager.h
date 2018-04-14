#pragma once
//=================================================================================
//	�v���O������	�F	
//	����			�F	�e�N�X�`�����ꊇ�Ǘ�[textureManager.h]
//	���C������		�F	manager.cpp
//	�쐬��			�F	moriya gaku
//=================================================================================

//=================================================================================
//	��d��`�h�~
//=================================================================================
#ifndef _TEXTURE_MANAGER_H_
#define _TEXTURE_MANAGER_H_

//=================================================================================
//	�C���N���[�h�t�@�C��
//=================================================================================
#include "main.h"
#include <iostream>

using namespace std;

class CTextureManager {
public:
	
	// �e�N�X�`���p�̃}�b�v�^��`
	typedef map<string, LPDIRECT3DTEXTURE9> TEXMAP;

    CTextureManager();

    ~CTextureManager();

    // �e�N�X�`����ǂݍ���
    static HRESULT Load( string key );
	
	// �A�����[�h
	static HRESULT Unload( string key );

    // �e�N�X�`�����擾����
    static LPDIRECT3DTEXTURE9 GetResource( string key );

    // �S���
    static HRESULT ReleaseAll( void );

	// ���[�h�ς݃e�N�X�`�����̕\��
	static void Show( void );

private:
    static TEXMAP m_TexMap;	// �e�N�X�`���}�b�v

};

#endif




