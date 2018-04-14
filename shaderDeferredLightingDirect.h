#pragma once

#ifndef _SHADERDEFERRED_SHADER_LIGHTING_DIRECT_H_
#define _SHADERDEFERRED_SHADER_LIGHTING_DIRECT_H_

#include "shader.h"

class CShaderDeferredLightingDirect : public CShader
{
public:
	CShaderDeferredLightingDirect() :
		m_pPS(nullptr),
		m_pPConstantTable(nullptr) {}
	~CShaderDeferredLightingDirect() {}

	HRESULT Init(void);

	// �V�F�[�_�v���O�����J�n
	void Begin(
		const D3DXVECTOR3& eyePosW,
		const D3DXVECTOR3& lightDirW
	);

	void Draw(void);	// ���ʕ`��

	void End(void);		// �V�F�[�_�v���O�����I��

	const LPD3DXCONSTANTTABLE& GetConstantPS(void) { return m_pPConstantTable; }

private:
	LPDIRECT3DPIXELSHADER9 m_pPS;				// �s�N�Z���V�F�[�_
	LPD3DXCONSTANTTABLE m_pPConstantTable;		// �s�N�Z���V�F�[�_�R���X�^���g�e�[�u��

};



#endif