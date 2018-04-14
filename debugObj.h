#pragma once
//=================================================================================
//	プログラム名	：	
//	処理			：	デバッグ用オブジェクト(モデル)処理[debugObj.h]
//	メイン処理		：	main.cpp
//	作成者			：	moriya gaku
//=================================================================================

//=================================================================================
//	二重定義防止
//=================================================================================
#ifndef _DEBUG_OBJ_MODEL_H_
#define _DEBUG_OBJ_MODEL_H_

//=================================================================================
//	インクルードファイル
//=================================================================================
#include "main.h"
#include "sceneX.h"

typedef map < string, CSceneX* > DEBUG_OBJ_MAP;

class CDebugObj
{
public:
	CDebugObj();
	~CDebugObj();

	void Uninit(void);

	CSceneX *Create(const char* filename, string key);
	CSceneX *GetData(string key);						// マップ捜索(データ取得)関数

	void SetPosition(string name, D3DXVECTOR3 pos);		// 座標設定関数
	void SetRotMatrix(string name, D3DXMATRIX mtxRot);	// 姿勢セット関数
	void SetColor(string name, D3DXCOLOR col);			// 色設定
	void SetScale(string name, D3DXVECTOR3 scl);		// 大きさ設定
		
private:
	DEBUG_OBJ_MAP m_mpModelData;						// モデル情報
};

#endif