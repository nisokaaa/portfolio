//=================================================================================
//	プログラム名	：	
//	処理			：	ライト情報一括管理[lightManager.cpp]
//	メイン処理		：	sceneManager.cpp
//	作成者			：	moriya gaku
//=================================================================================

//=================================================================================
//	インクルードファイル
//=================================================================================
#include "lightManager.h"

//=================================================================================
//	全開放関数
//=================================================================================
void CLightManager::ReleaseAll(void)
{
	// 全イテレータ
	for (LIGHTMAP::iterator it = m_LightMap.begin(); it != m_LightMap.end(); )
	{
		if (it->second != NULL)
		{
			delete it->second;
			it->second = NULL;

			// このイテレータを１つ消す
			it = m_LightMap.erase(it);
			continue;
		}
		it++;
	}
}

//=================================================================================
//	該当シェーダーを返す関数
//=================================================================================
CLight* CLightManager::GetLight(LIGHT_TYPE type)
{
	LIGHTMAP::iterator it = m_LightMap.find(type);

	return it != m_LightMap.end() ? it->second : NULL;
}

//=================================================================================
//	解放関数
//=================================================================================
void CLightManager::Release(LIGHT_TYPE type)
{
	LIGHTMAP::iterator it = m_LightMap.find(type);

	if (it != m_LightMap.end())
	{
		// 解放
		delete it->second;
		it->second = NULL;

		// このイテレータを１つ消す
		m_LightMap.erase(it);
	}
}

//=================================================================================
//	全更新関数
//=================================================================================
void CLightManager::Update(void)
{
	// 全イテレータ
	for (LIGHTMAP::iterator it = m_LightMap.begin(); it != m_LightMap.end(); )
	{
		if (it->second != NULL)
		{
			it->second->Update();
		}
		it++;
	}
}

//=================================================================================
//	生成関数
//=================================================================================
CLight *CLightManager::Create(LIGHT_TYPE type, D3DXVECTOR3 vecDir, D3DXVECTOR3 position, D3DXCOLOR color, D3DXVECTOR3 diffuse, D3DXVECTOR3 ambient)
{
	CLight *p = new CLight;
	p->Init(type, vecDir, position, color, diffuse, ambient);

	m_LightMap.insert(LIGHTMAP::value_type(type, p));

	return p;
}