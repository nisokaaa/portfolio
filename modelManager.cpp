//=================================================================================
//	プログラム名	：	
//	処理			：	モデル情報一括管理[modelManager.cpp]
//	メイン処理		：	manager.cpp
//	作成者			：	moriya gaku
//=================================================================================

//=================================================================================
//	インクルードファイル
//=================================================================================
#include "modelManager.h"
#include "renderer.h"

//=================================================================================
//	静的メンバ変数宣言
//=================================================================================
CModelManager::MODELMAP CModelManager::m_ModelMap;

//=================================================================================
//	コンストラクタ
//=================================================================================
CModelManager::CModelManager()
{
}

//=================================================================================
//	デストラクタ
//=================================================================================
CModelManager::~CModelManager()
{
}

//=================================================================================
//	モデル読み込み関数
//=================================================================================
HRESULT CModelManager::Load( string key )
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

    // テクスチャを検索
    MODELMAP::const_iterator it = m_ModelMap.find( key );
    
	if( it != m_ModelMap.end() )
	{
        // モデル情報を返す
        return S_OK;
    }
	else
	{ 
		HRESULT hr;
		CModelData *pModelData = new CModelData;

		hr = D3DXLoadMeshFromX( key.c_str(), D3DXMESH_SYSTEMMEM, pDevice, NULL, &pModelData->pBuffMat, NULL, &pModelData->nNumMat, &pModelData->pMesh );
        
		if( FAILED( hr ) )
		{
            MessageBox(NULL, key.c_str(), "LOAD ERROR!", MB_OK);
            return E_FAIL;
        }

        // マップへ挿入する
        m_ModelMap.insert( MODELMAP::value_type( key, pModelData ) );
	}

	return S_OK;
}

//=================================================================================
//	解放関数
//=================================================================================
HRESULT CModelManager::Unload( string key )
{
	MODELMAP::iterator it = m_ModelMap.find( key );

	if( it != m_ModelMap.end() )
	{
		// マテリアルの開放
		it->second->pBuffMat->Release();
		it->second->pBuffMat = NULL;

		// メッシュの開放
		it->second->pMesh->Release();
		it->second->pMesh = NULL;
		
		it->second->nNumMat	= 0;

		// このイテレータを１つ消す
		m_ModelMap.erase( it );

		return S_OK;
	}
	else
	{
		return E_FAIL;
	}
}

//=================================================================================
//	取得関数
//=================================================================================
CModelData* CModelManager::GetResource( string key )
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

    // 検索
    MODELMAP::iterator it = m_ModelMap.find( key );
    
	if( it != m_ModelMap.end() )
	{
        // モデル情報を返す
        return it->second;
    }
	else
	{ 
        return NULL;
    }
}

//=================================================================================
//	全解放関数
//=================================================================================
HRESULT CModelManager::ReleaseAll( void )
{
	// 全イテレータ
	for( MODELMAP::iterator it = m_ModelMap.begin(); it != m_ModelMap.end(); )
	{
		// LPDIRECTTEXTURE9の解放
		if( it->second != NULL )
		{
			// マテリアルの開放
			it->second->pBuffMat->Release();
			it->second->pBuffMat = NULL;

			// メッシュの開放
			it->second->pMesh->Release();
			it->second->pMesh = NULL;
		
			it->second->nNumMat	= 0;

			// このイテレータを１つ消す
			it = m_ModelMap.erase( it );
			continue;
		}
		it++;
	}

	return S_OK;
}