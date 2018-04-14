//=================================================================================
//	プログラム名	：	
//	処理			：	テクスチャ情報一括管理[textureManager.cpp]
//	メイン処理		：	manager.cpp
//	作成者			：	moriya gaku
//=================================================================================

//=================================================================================
//	インクルードファイル
//=================================================================================
#include "textureManager.h"
#include "renderer.h"

//=================================================================================
//　静的メンバ変数宣言
//=================================================================================
CTextureManager::TEXMAP CTextureManager::m_TexMap;

//=================================================================================
//　テクスチャロード
//=================================================================================
HRESULT CTextureManager::Load( string key )
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

    // テクスチャを検索
    TEXMAP::const_iterator it = m_TexMap.find( key );
    
	if( it != m_TexMap.end() )
	{
		// すでに読み込み済み
        return S_OK;  
    }
	else
	{ 
        // 新しくマップに追加する
        HRESULT hr;
        LPDIRECT3DTEXTURE9 texture;
        hr = D3DXCreateTextureFromFile( pDevice, key.c_str(), &texture );
        
		if( FAILED( hr ) )
		{
            MessageBox(NULL, key.c_str(), "LOAD ERROR!", MB_OK);
            return E_FAIL;
        }

        // マップへ挿入する
        m_TexMap.insert( TEXMAP::value_type( key, texture ) );
    }

    return S_OK;
}

//=================================================================================
//　テクスチャ解放
//=================================================================================
HRESULT CTextureManager::Unload( string key )
{
	// find()でkeyが登録されているか
	// 無い場合の返り値：最後のイテレータ 
	TEXMAP::const_iterator it = m_TexMap.find( key );

	if( it != m_TexMap.end() )
	{
		// LPDIRECTTEXTURE9の解放
		it->second->Release();

		// このイテレータを１つ消す
		m_TexMap.erase( it );

		return S_OK;
	}
	else
	{
		return E_FAIL;
	}

}

//=================================================================================
//　テクスチャ取得
//=================================================================================
LPDIRECT3DTEXTURE9 CTextureManager::GetResource( string key )
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

    // テクスチャを検索
    TEXMAP::iterator it = m_TexMap.find( key );
    
	if( it != m_TexMap.end() )
	{
        // テクスチャを返す
        return it->second;  
    }
	else
	{ 
        return NULL;
    }
}

//=================================================================================
//　テクスチャ全解放
//=================================================================================
HRESULT CTextureManager::ReleaseAll( void )
{
	// 全イテレータ
	for( TEXMAP::iterator it = m_TexMap.begin(); it != m_TexMap.end(); )
	{
		// LPDIRECTTEXTURE9の解放
		if( it->second != NULL )
		{
			it->second->Release();
			it->second = NULL;

			// このイテレータを１つ消す
			it = m_TexMap.erase( it );
			continue;
		}
		it++;
	}

	return S_OK;

}

//=================================================================================
//　ロード済みテクスチャ情報の表示
//=================================================================================
void CTextureManager::Show( void )
{
	char buff[ CHAR_MAX ];

	OutputDebugString( "\n_/_/_/_/_/_/_/_/_/_/_/ Loaded Texture _/_/_/_/_/_/_/_/_/_/_/_/\n\n" );

	for( TEXMAP::const_iterator it = m_TexMap.begin(); it != m_TexMap.end(); it++ )
	{
		sprintf_s( buff, "%s\n", it->first.c_str() );
		OutputDebugString( buff );
	}

	OutputDebugString( "\n\n_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/\n\n" );

	return;

}