//=================================================================================
//	プログラム名	：	
//	処理			：	3D描画オブジェクト処理[scene3D.cpp]
//	メイン処理		：	main.cpp
//	作成者			：	moriya gaku
//=================================================================================

//=================================================================================
//	インクルードファイル
//=================================================================================
#include "scene3D.h"
#include "renderer.h"
#include "textureManager.h"

//=================================================================================
//	コンストラクタ
//=================================================================================
CScene3D::CScene3D( int priority ) : CScene( priority )
{
	m_pVtxBuffer = NULL;
	m_Key = "";
	m_Scale = D3DXVECTOR3( 1.0f, 1.0f, 1.0f );
	m_Color = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );
	m_Length = 0.0f;								// ポリゴンの対角線の長さ
	m_Angle = 0.0f;									// ポリゴンの対角線の角度
	m_Width = 0.0f;									// 幅
	m_Height = 0.0f;								// 高さ	
	m_TexU = 0.0f;									// U値
	m_TexV = 0.0f;									// V値
	m_TexWidth = 0.0f;								// テクスチャ幅
	m_TexHeight = 0.0f;								// テクスチャ高さ
	D3DXMatrixIdentity(&m_mtxWorld);
}

//=================================================================================
//	デストラクタ
//=================================================================================
CScene3D::~CScene3D()
{
}

//=================================================================================
//　初期化処理
//=================================================================================
HRESULT CScene3D::Init(void)
{
	// 頂点バッファ生成
	MakeVertexBuffer();

	// 頂点情報セット
	SetVertex();

	return S_OK;
}

//=================================================================================
//　終了処理
//=================================================================================
void CScene3D::Uninit(void)
{
	// 頂点バッファの開放
	if(m_pVtxBuffer)
	{
		m_pVtxBuffer->Release();
		m_pVtxBuffer = NULL;
	}

	// 親クラスへ削除依頼
	Release();
}

//=================================================================================
//　更新処理
//=================================================================================
void CScene3D::Update(void)
{
	// 頂点情報セット
	SetVertex();
}

//=================================================================================
//　描画処理
//=================================================================================
void CScene3D::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	D3DXMATRIX mtxScale, mtxRot, mtxTranslate;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// スケールを反映
	D3DXMatrixScaling(&mtxScale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, D3DXToRadian(GetRotation().y), D3DXToRadian(GetRotation().x), D3DXToRadian(GetRotation().z));
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTranslate, GetPosition().x, GetPosition().y, GetPosition().z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTranslate);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 頂点バッファをレンダリングパイプラインに設定
	pDevice->SetStreamSource(0, m_pVtxBuffer, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, CTextureManager::GetResource( m_Key ));

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
}

//=================================================================================
//　頂点バッファ作成
//=================================================================================
HRESULT CScene3D::MakeVertexBuffer( void )
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

    if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
												D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
												FVF_VERTEX_3D,				// 使用する頂点フォーマット
												D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
												&m_pVtxBuffer,				// 頂点バッファインターフェースへのポインタ
												NULL)))						// NULLに設定
	{
        return E_FAIL;
	}

	return S_OK;
}

//=================================================================================
//　頂点情報をセット
//=================================================================================
void CScene3D::SetVertex( void )
{
	//頂点バッファの中身を埋める
	VERTEX_3D *pVtx;

	// ポリゴンの対角線の長さを設定
	m_Length = sqrtf((float)(((m_Width / 2) * (m_Width / 2)) + ((m_Height / 2) * (m_Height / 2))));

	// ポリゴンの対角線の角度を設定
	m_Angle = atan2f((m_Width / 2), (m_Height / 2));

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos.x = -sinf(m_Angle) * m_Length;
	pVtx[0].pos.y = cosf(m_Angle) * m_Length;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = sinf(m_Angle) * m_Length;
	pVtx[1].pos.y = cosf(m_Angle) * m_Length;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = -sinf(m_Angle) * m_Length;
	pVtx[2].pos.y = -cosf(m_Angle) * m_Length;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = sinf(m_Angle) * m_Length;
	pVtx[3].pos.y = -cosf(m_Angle) * m_Length;
	pVtx[3].pos.z = 0.0f;

	// 法線の設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	// 頂点カラーの設定
	pVtx[0].col = m_Color;
	pVtx[1].col = m_Color;
	pVtx[2].col = m_Color;
	pVtx[3].col = m_Color;

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点データをアンロックする
	m_pVtxBuffer->Unlock();

}