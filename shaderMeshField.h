#pragma once
//=================================================================================
//	プログラム名	：	
//	処理			：	テクスチャブレンドシェーダ処理[shaderMeshField.h]
//	メイン処理		：	main.cpp
//	作成者			：	moriya gaku
//=================================================================================

//=================================================================================
//	二重定義防止
//=================================================================================
#ifndef _SHADER_MESHFIELD_H_
#define _SHADER_MESHFIELD_H_

//=================================================================================
//	インクルードファイル
//=================================================================================
#include "shader.h"

class CSMeshField : public CShader
{
public:
	CSMeshField() :
		m_pVertexDeclaration(nullptr),
		m_pVtxShader(nullptr),
		m_pVtxConstantTable(nullptr),
		m_pPS(nullptr),
		m_pPConstantTable(nullptr) 
	{
		D3DXMatrixIdentity(&m_MtxView);
		D3DXMatrixIdentity(&m_MtxProj);
	}
	~CSMeshField() {}

	HRESULT Init(void);

	void Uninit(void);

	// シェーダプログラム開始
	void Begin(void);

	// シェーダへデータ送信
	void SendData(void);

	// シェーダプログラム終了
	void End(void);

	// 値設定
	void SetParam(const D3DXMATRIX& mtxView, const D3DXMATRIX& mtxProj,
		const int& nMinHeight, const int& nMaxHeight, const D3DXVECTOR3& lightDirW, const D3DXVECTOR3& eyePosW);

	// 行列設定
	void SetMatrix(const D3DXMATRIX& mtxWorld);
	
	const LPD3DXCONSTANTTABLE& GetConstantVS(void) { return m_pVtxConstantTable; }
	const LPD3DXCONSTANTTABLE& GetConstantPS(void) { return m_pPConstantTable; }

private:
	D3DXMATRIX m_MtxView;
	D3DXMATRIX m_MtxProj;
	int m_nMaxHeight;
	int m_nMinHeight;
	D3DXVECTOR3 m_EyePosW;
	D3DXVECTOR3 m_LightDirW;

	LPDIRECT3DVERTEXDECLARATION9 m_pVertexDeclaration;
	LPDIRECT3DVERTEXSHADER9 m_pVtxShader;		// 頂点シェーダ
	LPD3DXCONSTANTTABLE m_pVtxConstantTable;	// 頂点シェーダコンスタントテーブル
	LPDIRECT3DPIXELSHADER9 m_pPS;				// ピクセルシェーダ
	LPD3DXCONSTANTTABLE m_pPConstantTable;		// ピクセルシェーダコンスタントテーブル
};

#endif