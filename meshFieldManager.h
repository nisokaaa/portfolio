#pragma once
//=================================================================================
//	プログラム名	：	
//	処理			：	メッシュフィールド情報一括管理[meshFieldManager.h]
//	メイン処理		：	sceneManager.cpp
//	作成者			：	moriya gaku
//=================================================================================

//=================================================================================
//	二重定義防止
//=================================================================================
#ifndef _MESH_FIELD_MANAGER_
#define _MESH_FIELD_MANAGER_

//=================================================================================
//	インクルードファイル
//=================================================================================
#include "main.h"
#include "meshField.h"

typedef map<MESHFIELD_TYPE, CMeshField*> MESHFIELDMAP;

class CMeshFieldManager
{
public:
	CMeshFieldManager();
	~CMeshFieldManager();

	CMeshField *GetData(MESHFIELD_TYPE key);				// マップ捜索(データ取得)関数

	CMeshField *Load(const std::string& filename, MESHFIELD_TYPE key, const int& priority = DEFAULT_PRIORITY);
	
	void Unload(MESHFIELD_TYPE key);
	
	void ReleaseAll(void);

private:
	MESHFIELDMAP m_mpMeshFieldMap;							// モデルデータマップ
};

#endif