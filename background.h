#pragma once
//=================================================================================
//	�v���O������	�F	
//	����			�F	�w�i�e�N�X�`��[background.h]
//	���C������		�F	main.cpp
//	�쐬��			�F	moriya gaku
//=================================================================================

//=================================================================================
//	��d��`�h�~
//=================================================================================
#ifndef _BACKGROUND_H_
#define _BACKGROUND_H_

//=================================================================================
//	�C���N���[�h�t�@�C��
//=================================================================================
#include "main.h"
#include "scene2D.h"

class CBackGround
{
public:
	CBackGround() {}

	~CBackGround() {}

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void MakeTexture(void);
	void Create(void);

	// �T�[�t�F�C�X�擾
	LPDIRECT3DSURFACE9 GetSurface(void) { return m_pFinalSurface; }

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;			// ���_�o�b�t�@

	LPDIRECT3DSURFACE9 m_pFinalSurface;			// �T�[�t�F�X
	LPDIRECT3DTEXTURE9 m_pFinalTexture;			// �e�N�X�`��

	LPDIRECT3DSURFACE9 m_p1x1Surface;			// �T�[�t�F�X
	LPDIRECT3DTEXTURE9 m_p1x1Texture;			// �e�N�X�`��

	LPDIRECT3DSURFACE9 m_p1x4Surface;			// 1/4�T�C�Y�T�[�t�F�X
	LPDIRECT3DTEXTURE9 m_p1x4Texture;			// 1/4�T�C�Y�e�N�X�`��

	LPDIRECT3DSURFACE9 m_p1x16Surface;			// 1/16�T�C�Y�T�[�t�F�X
	LPDIRECT3DTEXTURE9 m_p1x16Texture;			// 1/16�T�C�Y�e�N�X�`��
};

#endif