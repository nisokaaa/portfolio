#pragma once
//=================================================================================
//	プログラム名	：	
//	処理			：	背景テクスチャ[background.h]
//	メイン処理		：	main.cpp
//	作成者			：	moriya gaku
//=================================================================================

//=================================================================================
//	二重定義防止
//=================================================================================
#ifndef _BACKGROUND_H_
#define _BACKGROUND_H_

//=================================================================================
//	インクルードファイル
//=================================================================================
#include "main.h"
#include "scene2D.h"

class CBackGround
{
public:
	CBackGround() {}

	~CBackGround() {}

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void MakeTexture(void);
	void Create(void);

	// サーフェイス取得
	LPDIRECT3DSURFACE9 GetSurface(void) { return m_pFinalSurface; }

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;			// 頂点バッファ

	LPDIRECT3DSURFACE9 m_pFinalSurface;			// サーフェス
	LPDIRECT3DTEXTURE9 m_pFinalTexture;			// テクスチャ

	LPDIRECT3DSURFACE9 m_p1x1Surface;			// サーフェス
	LPDIRECT3DTEXTURE9 m_p1x1Texture;			// テクスチャ

	LPDIRECT3DSURFACE9 m_p1x4Surface;			// 1/4サイズサーフェス
	LPDIRECT3DTEXTURE9 m_p1x4Texture;			// 1/4サイズテクスチャ

	LPDIRECT3DSURFACE9 m_p1x16Surface;			// 1/16サイズサーフェス
	LPDIRECT3DTEXTURE9 m_p1x16Texture;			// 1/16サイズテクスチャ
};

#endif