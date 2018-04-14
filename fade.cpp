//=================================================================================
//	プログラム名	：	
//	処理			：	フェード処理[fade.cpp]
//	メイン処理		：	main.cpp
//	作成者			：	moriya gaku
//=================================================================================

//=================================================================================
//	インクルードファイル
//=================================================================================
#include "fade.h"
#include "sceneManager.h"
#include "textureManager.h"
#include "renderer.h"

//=================================================================================
//　静的メンバ変数宣言
//=================================================================================
LPDIRECT3DVERTEXBUFFER9		CFade::m_pVtxBuff = NULL;					// 頂点バッファへのポインタ
string						CFade::m_Key = "";							// テクスチャへ検索キー

CFade::MODE					CFade::m_FadeMode = FADE_NONE;				// フェード状態
CSceneBase*					CFade::m_pNextScene = NULL;					// 遷移先

float CFade::m_FadeRate		= 0.05f;									// フェードレート
float CFade::m_Width		= SCREEN_WIDTH;								// 幅
float CFade::m_Height		= SCREEN_HEIGHT;							// 高さ	
float CFade::m_TexU			= 0.0f;										// U値
float CFade::m_TexV			= 0.0f;										// V値
float CFade::m_TexWidth		= 1.0f;										// テクスチャ幅
float CFade::m_TexHeight	= 1.0f;										// テクスチャ高さ

// 座標
D3DXVECTOR3 CFade::m_Position = D3DXVECTOR3( SCREEN_CENTER_X, SCREEN_CENTER_Y, 0.0f );

// フェード色
D3DXCOLOR					CFade::m_Color = D3DXCOLOR( 255.0f, 255.0f, 255.0f, 0.0f );	

// ポリゴンの対角線の長さ
float CFade::m_Length		= sqrtf((float)(((m_Width / 2) * (m_Width / 2)) + ((m_Height / 2) * (m_Height / 2))));

// ポリゴンの対角線の角度
float CFade::m_Angle		= atan2f((m_Width / 2), (m_Height / 2));

//===============================================================================
// 初期化処理
//===============================================================================
HRESULT CFade::Init( void )
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// オブジェクトの頂点バッファを生成
    if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
												D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
												FVF_VERTEX_2D,				// 使用する頂点フォーマット
												D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
												&m_pVtxBuff,				// 頂点バッファインターフェースへのポインタ
												NULL)))						// NULLに設定
	{
        return E_FAIL;
	}

	// テクスチャセット
	SetTexture( "data/TEXTURE/fade.png" );

	return S_OK;
}

//===============================================================================
// 終了処理
//===============================================================================
void CFade::Uninit( void )
{
	// 頂点バッファの開放
	if( m_pVtxBuff != NULL )
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

}

//===============================================================================
// 更新処理
//===============================================================================
void CFade::Update( void )
{	
	// 頂点の設定
	VERTEX_2D *pVtx;

	// 頂点をロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[ 0 ].pos.x = m_Position.x - sinf(m_Angle) * m_Length;
	pVtx[ 0 ].pos.y = m_Position.y - cosf(m_Angle) * m_Length;
	pVtx[ 0 ].pos.z = 0.0f;
		    
	pVtx[ 1 ].pos.x = m_Position.x + sinf(m_Angle) * m_Length;
	pVtx[ 1 ].pos.y = m_Position.y - cosf(m_Angle) * m_Length;
	pVtx[ 1 ].pos.z = 0.0f;
		    
	pVtx[ 2 ].pos.x = m_Position.x - sinf(m_Angle) * m_Length;
	pVtx[ 2 ].pos.y = m_Position.y + cosf(m_Angle) * m_Length;
	pVtx[ 2 ].pos.z = 0.0f;
		    
	pVtx[ 3 ].pos.x = m_Position.x + sinf(m_Angle) * m_Length;
	pVtx[ 3 ].pos.y = m_Position.y + cosf(m_Angle) * m_Length;
	pVtx[ 3 ].pos.z = 0.0f;

	pVtx[ 0 ].rhw =
	pVtx[ 1 ].rhw =
	pVtx[ 2 ].rhw =
	pVtx[ 3 ].rhw = 1.0f;

	// 頂点カラー設定
	pVtx[ 0 ].col = m_Color;
	pVtx[ 1 ].col = m_Color;
	pVtx[ 2 ].col = m_Color;
	pVtx[ 3 ].col = m_Color;

	// テクスチャ座標設定
	pVtx[ 0 ].tex = D3DXVECTOR2( m_TexU, m_TexV );
	pVtx[ 1 ].tex = D3DXVECTOR2( m_TexU + m_TexWidth, m_TexV );
	pVtx[ 2 ].tex = D3DXVECTOR2( m_TexU, m_TexV + m_TexHeight );
	pVtx[ 3 ].tex = D3DXVECTOR2( m_TexU + m_TexWidth, m_TexV + m_TexHeight );

	// 頂点をアンロック
	m_pVtxBuff->Unlock();


	// フェード処理
	if( m_FadeMode == FADE_IN )
	{
		// a値を減算して後ろの絵を浮き上がらせる(a値からフェード値を引いていく)
		m_Color.a -= m_FadeRate;

		if( m_Color.a < 0.0f )
		{
			m_Color.a = 0.0f;
			m_FadeMode = FADE_NONE;
		}
	}
	else if( m_FadeMode == FADE_OUT )
	{
		// a値を加算して画面を消していく
		m_Color.a += m_FadeRate;

		if( m_Color.a > 1.0f )
		{
			m_Color.a = 1.0f;
			m_FadeMode = FADE_IN;
			CSceneManager::ChangeScene( m_pNextScene );
		}
	}
}

//===============================================================================
// 描画処理
//===============================================================================
void CFade::Draw( void )
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// 頂点バッファをデータストリームにバインド
    pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, CTextureManager::GetResource( m_Key ));

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
}

//=================================================================================
//	フェードの設定
//=================================================================================
void CFade::SetFade( MODE fadeMode, CSceneBase *pSceneNext, float sec )
{
	m_FadeMode = fadeMode;						// フェードモード設定
	m_pNextScene = pSceneNext;					// 次の遷移先ポインタ
	m_FadeRate = 1.0f / ( 60 * sec );			// フェードレート(速さ)設定
}

//=================================================================================
//	フェード状態取得
//=================================================================================
CFade::MODE CFade::GetFadeMode( void )
{
	return m_FadeMode;
}