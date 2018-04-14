#pragma once
//=================================================================================
//	プログラム名	：	
//	処理			：	モデル情報一括管理[modelManager.h]
//	メイン処理		：	manager.cpp
//	作成者			：	moriya gaku
//=================================================================================

//=================================================================================
//	二重定義防止
//=================================================================================
#ifndef _MODEL_MANAGER_H_
#define _MODEL_MANAGER_H_

//=================================================================================
//	インクルードファイル
//=================================================================================
#include "main.h"
#include "model.h"

class CModelManager {
public:
	
	// モデルマップ型定義
	typedef map<string, CModelData*> MODELMAP;

    CModelManager();

    ~CModelManager();

    // モデル読み込む
    static HRESULT Load( string key );

	// アンロード
	static HRESULT Unload( string key );

    // モデル情報を取得する
    static CModelData* GetResource( string key );

    // 全解放
    static HRESULT ReleaseAll( void );

private:
    static MODELMAP m_ModelMap;	// モデルデータマップ

};

#endif




