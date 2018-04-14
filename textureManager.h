#pragma once
//=================================================================================
//	プログラム名	：	
//	処理			：	テクスチャ情報一括管理[textureManager.h]
//	メイン処理		：	manager.cpp
//	作成者			：	moriya gaku
//=================================================================================

//=================================================================================
//	二重定義防止
//=================================================================================
#ifndef _TEXTURE_MANAGER_H_
#define _TEXTURE_MANAGER_H_

//=================================================================================
//	インクルードファイル
//=================================================================================
#include "main.h"
#include <iostream>

using namespace std;

class CTextureManager {
public:
	
	// テクスチャ用のマップ型定義
	typedef map<string, LPDIRECT3DTEXTURE9> TEXMAP;

    CTextureManager();

    ~CTextureManager();

    // テクスチャを読み込む
    static HRESULT Load( string key );
	
	// アンロード
	static HRESULT Unload( string key );

    // テクスチャを取得する
    static LPDIRECT3DTEXTURE9 GetResource( string key );

    // 全解放
    static HRESULT ReleaseAll( void );

	// ロード済みテクスチャ情報の表示
	static void Show( void );

private:
    static TEXMAP m_TexMap;	// テクスチャマップ

};

#endif




