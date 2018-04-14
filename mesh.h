#pragma once
//=================================================================================
//	プログラム名	：	
//	処理			：	メッシュ処理[mesh.h]
//	メイン処理		：	main.cpp
//	作成者			：	moriya gaku
//=================================================================================

//=================================================================================
//	二重定義防止
//=================================================================================
#ifndef _MESH_H_
#define _MESH_H_

//=================================================================================
//	インクルードファイル
//=================================================================================
#include "main.h"
#include "scene3D.h"

class CMesh : public CScene3D
{
public:
	CMesh( int priority = DEFAULT_PRIORITY );
	~CMesh(){};

	HRESULT Init( void );												
	void Uninit( void );												
	void Update( void );												
	void Draw( void );													

	void MakeIndexBuffer( void );								// インデックスバッファ作成
	virtual void SetMatrix(void) {};

	void SetIndexBuffer( const LPDIRECT3DINDEXBUFFER9 pIdxBuffer ){ m_pIdxBuffer =  pIdxBuffer; }
	const LPDIRECT3DINDEXBUFFER9& GetIndexBuffer( void ) const { return m_pIdxBuffer; }

	void SetNumWidth( const int numWidth ){ m_NumWidth = numWidth; }
	const int& GetNumWidth( void ) const { return m_NumWidth; }

	void SetNumHeight( const int numHeight ){ m_NumHeight = numHeight; }
	const int& GetNumHeight( void ) const { return m_NumHeight; }

	void SetNumPolygon( const int numPolygon ){ m_NumPolygon = numPolygon; }
	const int& GetNumPolygon( void ) const { return m_NumPolygon; }

	void SetNumVertex( const int numVertex ){ m_NumVertex = numVertex; }
	const int& GetNumVertex( void ) const { return m_NumVertex; }

private:
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuffer;						// インデックスバッファ
	int m_NumWidth;												// 横ブロック数
	int m_NumHeight;											// 縦ブロック数
	int m_NumPolygon;											// 総ポリゴン数
	int m_NumVertex;											// 総頂点数
};

#endif