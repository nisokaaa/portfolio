//=================================================================================
//	プログラム名	：	
//	処理			：	2D描画オブジェクト処理[scene2D.h]
//	メイン処理		：	main.cpp
//	作成者			：	moriya gaku
//=================================================================================

//=================================================================================
//	インクルードファイル
//=================================================================================
#include "scene2D.h"
#include "renderer.h"
#include "textureManager.h"

//=================================================================================
//　初期化処理
//=================================================================================
HRESULT CScene2D::Init( void )
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

	return S_OK;
}

//=================================================================================
//　終了処理
//=================================================================================
void CScene2D::Uninit( void )
{
	// 頂点バッファの開放
	if( m_pVtxBuff != NULL )
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// 親クラスへ削除依頼
	Release();
}

//=================================================================================
//　更新処理
//=================================================================================
void CScene2D::Update( void )
{
	// Zがマイナスの場合は描画しない
	if( GetPosition().z < 0 ) return;

	VERTEX_2D *pVtx;
	
	// 頂点をロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点設定
	pVtx = SetVertex( pVtx );

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

}

//=================================================================================
//　描画処理
//=================================================================================
void CScene2D::Draw( void )
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// テクスチャ設定
	pDevice->SetTexture(0, CTextureManager::GetResource(m_Key));

	// 頂点フォーマット設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// 頂点バッファ設定
    pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
}

//=================================================================================
//　生成関数
//=================================================================================
CScene2D *CScene2D::Create(std::string key, D3DXVECTOR2 pos, float width, float height, float scl, BOOL mode, int priority)
{
	CScene2D *p = new CScene2D(priority);

	p->SetTexture(key);
	p->SetPosition(pos.x, pos.y, 0.0f);
	p->m_Width = width;
	p->m_Height = height;
	p->m_Scale = scl;
	p->m_TexWidth = 1.0f;
	p->m_TexHeight = 1.0f;
	p->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	p->m_bDrawType = mode;

	p->Init();

	return p;
}

//=================================================================================
//　頂点設定
//=================================================================================
VERTEX_2D* CScene2D::SetVertex( VERTEX_2D *pVtx )
{
	D3DXVECTOR3 position = GetPosition();

	// 中心原点で描画
	if(m_bDrawType){
		D3DXVECTOR3 rotation = GetRotation();
		float length = GetLength();
		float angle = GetAngle();
		float width = GetWidth();
		float height = GetHeight();
		float scale = GetScale();

		// ポリゴンの対角線の長さを設定
		length = sqrtf((float)(((width / 2) * (width / 2)) + ((height / 2) * (height / 2))));
		SetLength( length );

		// ポリゴンの対角線の角度を設定
		angle = atan2f((width / 2), (height / 2));
		SetAngle( angle );

		//// 頂点座標の設定
		pVtx[ 0 ].pos.x = ( position.x - sinf(angle - rotation.z) * length * scale);
		pVtx[ 0 ].pos.y = ( position.y - cosf(angle - rotation.z) * length * scale);
		pVtx[ 0 ].pos.z = 0.0f;
		    
		pVtx[ 1 ].pos.x = ( position.x + sinf(angle + rotation.z) * length * scale);
		pVtx[ 1 ].pos.y = ( position.y - cosf(angle + rotation.z) * length * scale);
		pVtx[ 1 ].pos.z = 0.0f;
		    
		pVtx[ 2 ].pos.x = ( position.x - sinf(angle + rotation.z) * length * scale);
		pVtx[ 2 ].pos.y = ( position.y + cosf(angle + rotation.z) * length * scale);
		pVtx[ 2 ].pos.z = 0.0f;
		    
		pVtx[ 3 ].pos.x = ( position.x + sinf(angle - rotation.z) * length * scale);
		pVtx[ 3 ].pos.y = ( position.y + cosf(angle - rotation.z) * length * scale);
		pVtx[ 3 ].pos.z = 0.0f;

		return pVtx;
	}

	// 頂点座標の設定
	pVtx[ 0 ].pos.x = position.x;
	pVtx[ 0 ].pos.y = position.y;
	pVtx[ 0 ].pos.z = 0.0f;
			    
	pVtx[ 1 ].pos.x = position.x + m_Width * m_Scale;
	pVtx[ 1 ].pos.y = position.y;
	pVtx[ 1 ].pos.z = 0.0f;
			    
	pVtx[ 2 ].pos.x = position.x;
	pVtx[ 2 ].pos.y = position.y + m_Height * m_Scale;
	pVtx[ 2 ].pos.z = 0.0f;
			    
	pVtx[ 3 ].pos.x = position.x + m_Width * m_Scale;
	pVtx[ 3 ].pos.y = position.y + m_Height * m_Scale;
	pVtx[ 3 ].pos.z = 0.0f;

	return pVtx;
}