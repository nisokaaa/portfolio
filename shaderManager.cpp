//=================================================================================
//	プログラム名	：	
//	処理			：	シェーダー情報一括管理[shaderManager.cpp]
//	メイン処理		：	sceneManager.cpp
//	作成者			：	moriya gaku
//=================================================================================

//=================================================================================
//	インクルードファイル
//=================================================================================
#include "shaderManager.h"

//=================================================================================
//	コンストラクタ
//=================================================================================
CShaderManager::CShaderManager()
{
}

//=================================================================================
//	デストラクタ
//=================================================================================
CShaderManager::~CShaderManager()
{
}

//=================================================================================
//	初期化関数
//=================================================================================
void CShaderManager::Init(void)
{
	CShader* p = NULL;

	p = new CSBasic();
	p->Init();
	m_ShaderMap.insert(SHADERMAP::value_type(SHADER_BASIC, p));

	p = new CSMeshField();
	p->Init();
	m_ShaderMap.insert(SHADERMAP::value_type(SHADER_MESHFIELD, p));

	p = new CSUpPassFilter();
	p->Init();
	m_ShaderMap.insert(SHADERMAP::value_type(SHADER_UP_PASS_FILTTER, p));

	p = new CSDownPassFilter();
	p->Init();
	m_ShaderMap.insert(SHADERMAP::value_type(SHADER_DOWN_PASS_FILTTER, p));

	p = new CSHorizontalBlurFilter();
	p->Init();
	m_ShaderMap.insert(SHADERMAP::value_type(SHADER_HORIZONAL_BLUR_FILTTER, p));

	p = new CSVerticalBlurFilter();
	p->Init();
	m_ShaderMap.insert(SHADERMAP::value_type(SHADER_VERTICAL_BLUR_FILTTER, p));
}

//=================================================================================
//	該当シェーダーを返す関数
//=================================================================================
CShader* CShaderManager::GetData(SHADER_TYPE type)
{
	SHADERMAP::iterator it = m_ShaderMap.find(type);

	return it != m_ShaderMap.end() ? it->second : NULL;
}

//=================================================================================
//	解放関数
//=================================================================================
void CShaderManager::Release(SHADER_TYPE type, CShader* pShader)
{
	SHADERMAP::iterator it = m_ShaderMap.find(type);

	if (it != m_ShaderMap.end())
	{
		// 解放
		delete it->second;
		it->second = NULL;

		// このイテレータを１つ消す
		m_ShaderMap.erase(it);
	}
}

//=================================================================================
//	全開放関数
//=================================================================================
void CShaderManager::ReleaseAll(void)
{
	// 全イテレータ
	for (SHADERMAP::iterator it = m_ShaderMap.begin(); it != m_ShaderMap.end(); )
	{
		if (it->second != NULL)
		{
			it->second->Uninit();
			delete it->second;
			it->second = NULL;

			// このイテレータを１つ消す
			it = m_ShaderMap.erase(it);
			continue;
		}
		it++;
	}
}