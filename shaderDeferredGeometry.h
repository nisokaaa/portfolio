#pragma once

#ifndef _SHADERDEFERRED_SHADER_GEOMETRY_H_
#define _SHADERDEFERRED_SHADER_GEOMETRY_H_

#include "shader.h"

class CShaderDeferredGeometry : public CShader
{
public:
	CShaderDeferredGeometry() :
		m_pNoTexture(nullptr),
		m_pVertexDeclaration(nullptr),
		m_pVtxShader(nullptr),
		m_pVtxConstantTable(nullptr),
		m_pPS(nullptr),
		m_pPConstantTable(nullptr){}
	~CShaderDeferredGeometry() 
	{
		D3DXMatrixIdentity(&m_MtxView);
		D3DXMatrixIdentity(&m_MtxProj);
	}

	HRESULT Init(void);

	// �V�F�[�_�v���O�����J�n
	void Begin(
		const D3DXMATRIX& mtxView,
		const D3DXMATRIX& mtxProj,
		const D3DXVECTOR3& posEye,
		const float* apSplitPos,
		const D3DXMATRIX& shadowMatrix0,
		const D3DXMATRIX& shadowMatrix1,
		const D3DXMATRIX& shadowMatrix2
	);
	void SetParam(const float& specpower, const bool& bshadowflg);

	void SetMatrix(const D3DXMATRIX& mtxWorld);

	void End(void);	// �V�F�[�_�v���O�����I��

	const LPD3DXCONSTANTTABLE& GetConstantVS(void) { return m_pVtxConstantTable; }
	const LPD3DXCONSTANTTABLE& GetConstantPS(void) { return m_pPConstantTable; }
	const LPDIRECT3DTEXTURE9& GetNoTexture(void) { return m_pNoTexture; }

	LPDIRECT3DVERTEXSHADER9 m_pVtxShader;		// ���_�V�F�[�_

private:
	D3DXMATRIX m_MtxView;
	D3DXMATRIX m_MtxProj;

	LPDIRECT3DTEXTURE9 m_pNoTexture;			// �m�[�}�e���A���e�N�X

	LPDIRECT3DVERTEXDECLARATION9 m_pVertexDeclaration;

	LPD3DXCONSTANTTABLE m_pVtxConstantTable;	// ���_�V�F�[�_�R���X�^���g�e�[�u��

	LPDIRECT3DPIXELSHADER9 m_pPS;				// �s�N�Z���V�F�[�_
	LPD3DXCONSTANTTABLE m_pPConstantTable;		// �s�N�Z���V�F�[�_�R���X�^���g�e�[�u��

};



#endif