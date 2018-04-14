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

	// シェーダプログラム開始
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

	void End(void);	// シェーダプログラム終了

	const LPD3DXCONSTANTTABLE& GetConstantVS(void) { return m_pVtxConstantTable; }
	const LPD3DXCONSTANTTABLE& GetConstantPS(void) { return m_pPConstantTable; }
	const LPDIRECT3DTEXTURE9& GetNoTexture(void) { return m_pNoTexture; }

	LPDIRECT3DVERTEXSHADER9 m_pVtxShader;		// 頂点シェーダ

private:
	D3DXMATRIX m_MtxView;
	D3DXMATRIX m_MtxProj;

	LPDIRECT3DTEXTURE9 m_pNoTexture;			// ノーマテリアルテクス

	LPDIRECT3DVERTEXDECLARATION9 m_pVertexDeclaration;

	LPD3DXCONSTANTTABLE m_pVtxConstantTable;	// 頂点シェーダコンスタントテーブル

	LPDIRECT3DPIXELSHADER9 m_pPS;				// ピクセルシェーダ
	LPD3DXCONSTANTTABLE m_pPConstantTable;		// ピクセルシェーダコンスタントテーブル

};



#endif