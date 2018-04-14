#pragma once
//=================================================================================
//	プログラム名	：	
//	処理			：	ライト情報一括管理[lightManager.h]
//	メイン処理		：	sceneManager.cpp
//	作成者			：	moriya gaku
//=================================================================================
//=================================================================================
//	二重定義防止
//=================================================================================
#ifndef _LIGHT_MANAGER_H_
#define _LIGHT_MANAGER_H_

//=================================================================================
//	インクルードファイル
//=================================================================================
#include "main.h"
#include "light.h"

class CLightManager
{
public:
	typedef std::map < LIGHT_TYPE, CLight* > LIGHTMAP;

	CLightManager() {}
	~CLightManager() {}

	void ReleaseAll(void);

	void Update(void);

	void Release(LIGHT_TYPE type);

	CLight *GetLight(LIGHT_TYPE type);

	CLight *Create(LIGHT_TYPE type, D3DXVECTOR3 vecDir, D3DXVECTOR3 position, D3DXCOLOR color, D3DXVECTOR3 diffuse, D3DXVECTOR3 ambient);

private:
	LIGHTMAP m_LightMap;
};

#endif