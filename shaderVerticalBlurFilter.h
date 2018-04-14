#pragma once
//=================================================================================
//	プログラム名	：	
//	処理			：	垂直方向ブラー[shaderVerticalBlurFiltter.h]
//	メイン処理		：	shaderManager.cpp
//	作成者			：	moriya gaku
//=================================================================================

//=================================================================================
// 二重定義防止
//=================================================================================
#ifndef _SHADER_VERTICAL_BLUR_FILTER_H_
#define _SHADER_VERTICAL_BLUR_FILTER_H_

//=================================================================================
// インクルードファイル
//=================================================================================
#include "shader.h"

class CSVerticalBlurFilter : public CShader
{
public:
	CSVerticalBlurFilter() :
		m_pPS(nullptr),
		m_pPConstantTable(nullptr) {}
	~CSVerticalBlurFilter() {}

	HRESULT Init(void);

	void Uninit(void);

	// シェーダプログラム開始
	void Begin(void);

	void SendData(LPDIRECT3DTEXTURE9 pTexture, float mag);

	void Draw(float mag);	// 結果描画

	void End(void);		// シェーダプログラム終了

	void Fillter(LPDIRECT3DSURFACE9 surface, LPDIRECT3DTEXTURE9 pTexture, float mag);

	const LPD3DXCONSTANTTABLE& GetConstantPS(void) { return m_pPConstantTable; }

private:
	LPDIRECT3DPIXELSHADER9 m_pPS;				// ピクセルシェーダ
	LPD3DXCONSTANTTABLE m_pPConstantTable;		// ピクセルシェーダコンスタントテーブル
};


#endif