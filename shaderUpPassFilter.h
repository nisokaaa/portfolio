#pragma once
//=================================================================================
//	�v���O������	�F	
//	����			�F	1/4�o�b�t�@�ɃT�C�Y�A�b�v[shaderUpPassFiltter.h]
//	���C������		�F	shaderManager.cpp
//	�쐬��			�F	moriya gaku
//=================================================================================

//=================================================================================
//	��d��`�h�~
//=================================================================================
#ifndef _SHADER_UP_PASS_FILTER_H_
#define _SHADER_UP_PASS_FILTER_H_

//=================================================================================
//	�C���N���[�h�t�@�C��
//=================================================================================
#include "shader.h"

class CSUpPassFilter : public CShader
{
public:
	CSUpPassFilter() :
		m_pPS(nullptr),
		m_pPConstantTable(nullptr) {}
	~CSUpPassFilter() {}

	HRESULT Init(void);

	void Uninit(void);

	// �V�F�[�_�v���O�����J�n
	void Begin(void);

	void SendData(LPDIRECT3DTEXTURE9 pTexture);

	void Draw(float mag);	// ���ʕ`��

	void End(void);		// �V�F�[�_�v���O�����I��

	void Fillter(LPDIRECT3DSURFACE9 surface, LPDIRECT3DTEXTURE9 pTexture, float mag);

	const LPD3DXCONSTANTTABLE& GetConstantPS(void) { return m_pPConstantTable; }

private:
	LPDIRECT3DPIXELSHADER9 m_pPS;				// �s�N�Z���V�F�[�_
	LPD3DXCONSTANTTABLE m_pPConstantTable;		// �s�N�Z���V�F�[�_�R���X�^���g�e�[�u��
};

#endif