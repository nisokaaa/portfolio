#pragma once
//=================================================================================
//	�v���O������	�F	
//	����			�F	�V�F�[�_����[shader.h]
//	���C������		�F	main.cpp
//	�쐬��			�F	moriya gaku
//=================================================================================

//=================================================================================
//	��d��`�h�~
//=================================================================================
#ifndef _SHADER_H_
#define _SHADER_H_

//=================================================================================
//	�C���N���[�h�t�@�C��
//=================================================================================
#include "main.h"

class CShader
{
public:
	CShader() {}
	~CShader() {}

	virtual HRESULT Init(void) { return S_OK; }
	virtual const LPD3DXCONSTANTTABLE& GetConstantVS(void) { return NULL; }
	virtual const LPD3DXCONSTANTTABLE& GetConstantPS(void) { return NULL; }
	virtual const LPDIRECT3DTEXTURE9& GetNoTexture(void) { return NULL; }
	virtual void Begin(void) {}
	virtual void SendData(void) {}
	virtual void End(void) {}
	virtual void SetMatrix(const D3DXMATRIX& mtxWorld) {}
	virtual void Uninit(void) = 0;
};

#endif