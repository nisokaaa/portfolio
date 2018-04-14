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

	// シェーダプログラム開始
	void Begin(
		const D3DXVECTOR3& eyePosW,
		const D3DXVECTOR3& lightDirW
	);

	void Draw(void);	// 結果描画

	void End(void);		// シェーダプログラム終了

	const LPD3DXCONSTANTTABLE& GetConstantPS(void) { return m_pPConstantTable; }

private:
	LPDIRECT3DPIXELSHADER9 m_pPS;				// ピクセルシェーダ
	LPD3DXCONSTANTTABLE m_pPConstantTable;		// ピクセルシェーダコンスタントテーブル

};



#endif