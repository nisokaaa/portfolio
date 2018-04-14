#pragma once
//=================================================================================
//	プログラム名	：	
//	処理			：	メッシュフィールド処理[meshField.h]
//	メイン処理		：	main.cpp
//	作成者			：	moriya gaku
//=================================================================================

//=================================================================================
//	二重定義防止
//=================================================================================
#ifndef _MESH_FIELD_H_
#define _MESH_FIELD_H_

//=================================================================================
//	インクルードファイル
//=================================================================================
#include "main.h"
#include "mesh.h"

//=================================================================================
//	タイプ定義
//=================================================================================
typedef enum
{
	GAME_MAIN_STAGE = 0,
}MESHFIELD_TYPE;

class CMeshField : public CMesh
{
public:
	CMeshField(int priority = DEFAULT_PRIORITY) : CMesh( priority ){};
	~CMeshField(){};											
	
	void Uninit(void);

	HRESULT MakeVertexBuffer( void );			// 頂点バッファ作成
	void SetVertex( void );						// 頂点情報セット
	void SetMatrix(void);						// ワールド行列計算・デバイスにセット

	float GetPosHeight( D3DXVECTOR3 position );	// Pが乗っている凹凸の高さ取得

	static CMeshField *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float width, float  height, int numWidth, int numHeight, std::vector<std::vector<float>> vHeight);

private:
	std::vector<std::vector<float>> m_vHeight;
};

#endif