//=================================================================================
//	プログラム名	：	
//	処理			：	メッシュ情報処理[mesh.cpp]
//	メイン処理		：	main.cpp
//	作成者			：	moriya gaku
//=================================================================================

//=================================================================================
//	インクルードファイル
//=================================================================================
#include "mesh.h"
#include "scene3D.h"
#include "renderer.h"
#include "manager.h"
#include "textureManager.h"
#include "shaderManager.h"

//=================================================================================
//	コンストラクタ
//=================================================================================
CMesh::CMesh( int priority ) : CScene3D( priority )
{
	m_pIdxBuffer = NULL;	
	m_NumWidth = 0;			
	m_NumHeight = 0;		
	m_NumPolygon = 0;
	m_NumVertex = 0;
}

//=================================================================================
//	初期化関数
//=================================================================================
HRESULT CMesh::Init( void )
{
	CScene3D::Init();

	// 頂点インデックス作成
	MakeIndexBuffer();

	return S_OK;
}

//=================================================================================
//	終了関数
//=================================================================================
void CMesh::Uninit( void )
{	
	CScene3D::Uninit();

	if( m_pIdxBuffer != NULL )
	{
		m_pIdxBuffer->Release();		// インデックスバッファインターフェースの解放
		m_pIdxBuffer = NULL;
	}

	return;
}

//=================================================================================
//	更新関数
//=================================================================================
void CMesh::Update( void )
{
	CScene3D::Update();

	return;
}

//=================================================================================
//	描画関数
//=================================================================================
void CMesh::Draw( void )
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	LPDIRECT3DVERTEXBUFFER9 pVtxBuffer = GetVertexBuffer();
	int numWidth = GetNumWidth();
	int numHeight = GetNumHeight();
	int numPolygon = GetNumPolygon();

	if( pDevice != NULL )
	{
		SetMatrix();

		// VRAMとGPU間で伝送する設定の処理
		pDevice->SetStreamSource( 0, pVtxBuffer, 0, sizeof( VERTEX_3D ) );

		// フォーマット設定
		pDevice -> SetFVF( FVF_VERTEX_3D );

		// デバイスへインデックスバッファを設定
		pDevice -> SetIndices( m_pIdxBuffer );

		// ポリゴン描画( プリミティブ数＋縮退ポリゴン４枚分 )
		pDevice -> DrawIndexedPrimitive( D3DPT_TRIANGLESTRIP, 0, 0, ( numWidth + 1 ) * ( numHeight + 1 ), 0, numPolygon );
	}

	return;
}

//=================================================================================
//	頂点インデックス作成
//=================================================================================
void CMesh::MakeIndexBuffer( void )
{
	// 16ビット添字
	WORD *pIdx;
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// 縮退ポリゴン含めた総頂点数
	m_NumVertex = (( m_NumWidth + 2 ) * ( m_NumHeight -1 ) + m_NumWidth ) * 2 + 2;

	/*------------------------------------------------------------------------*/
	/*	インデックスバッファの作成
	/*------------------------------------------------------------------------*/
	if( FAILED( pDevice->CreateIndexBuffer(
		sizeof( WORD ) * m_NumVertex,			// バッファ量
		D3DUSAGE_WRITEONLY,						// 使用方法フラグ
		D3DFMT_INDEX16,							// インデックスフォーマット
		D3DPOOL_MANAGED,						// メモリ管理方法
		&m_pIdxBuffer,							// インデックスバッファインタフェースへのポインタのアドレス
		NULL
		) ) )
	{
		MessageBox( NULL, "インデックスバッファの作成に失敗しました。", "INDEX_BUFFER_ERROR", MB_OK );
	}
	else
	{
		pDevice->CreateIndexBuffer( sizeof( WORD ) * m_NumVertex, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIdxBuffer, NULL );
	}

	/*------------------------------------------------------------------------*/
	/*	メッシュフィールド頂点情報セット(1回度のみ)
	/*------------------------------------------------------------------------*/
	// 仮想メモリに書き込み
	m_pIdxBuffer->Lock( 0, 0, (void**)&pIdx, 0 );

	int nLoop = 0;

	for( int i = 0; i < m_NumHeight; i++ )
	{
		for( int j = 0; j < m_NumWidth + 1; j++ )
		{
			pIdx[ nLoop ] = ( m_NumWidth + 1 ) * ( i + 1 ) + j;
			nLoop++;
			pIdx[ nLoop ] = ( m_NumWidth + 1 ) * i + j;
			nLoop++;
		}
		if( i != m_NumHeight -1 )
		{
			pIdx[ nLoop ] = pIdx[ nLoop -1 ];
			nLoop++;
			pIdx[ nLoop ] = ( m_NumWidth + 1 ) * ( i + 2 );
			nLoop++;
		}
	}
	// 仮想メモリをアンロック
	m_pIdxBuffer->Unlock();

	return;
}