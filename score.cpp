//=================================================================================
//	プログラム名	：	
//	処理			：	スコア処理[score.cpp]
//	メイン処理		：	main.cpp
//	作成者			：	moriya gaku
//=================================================================================

//=================================================================================
// iインクルードファイル
//=================================================================================
#include "score.h"
#include "renderer.h"
#include "textureManager.h"

//=================================================================================
// 初期化処理
//=================================================================================
HRESULT CScore::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	//頂点の作成
	MakeVertexScore(pDevice);

	for (int i = 0; i < m_nDigit; i++)
		m_vfDigitU.push_back(0);

	return S_OK;
}

//=================================================================================
// 終了処理
//=================================================================================
void CScore::Uninit(void)
{
	if(m_pVtxBuffer != NULL)
	{
		m_pVtxBuffer->Release();	//テクスタインターフェイス解放
		m_pVtxBuffer = NULL;		//中身の掃除
	}
}

//=================================================================================
// 更新処理
//=================================================================================
void CScore::Update(void)
{
	// 座標取得
	D3DXVECTOR3 pos = GetPosition();

	//仮想アドレス
	VERTEX_2D* pVtx = NULL;

	//頂点バッファをロックして仮想アドレスを取得する
	m_pVtxBuffer -> Lock( 0 , 0 , ( void** ) &pVtx , 0 );

	for(int i = 0 ; i < m_nDigit ; i++)
	{
		//UV座標の設定
		pVtx[4 * i + 0].tex = D3DXVECTOR2(m_vfDigitU[i], 0);
		pVtx[4 * i + 1].tex = D3DXVECTOR2(m_vfDigitU[i] + 0.1f, 0);
		pVtx[4 * i + 2].tex = D3DXVECTOR2(m_vfDigitU[i], 1);
		pVtx[4 * i + 3].tex = D3DXVECTOR2(m_vfDigitU[i] + 0.1f, 1);

		//頂点座標の設定(2D座標・右回りで呼ぶこと！)   ( x , y , z )
		pVtx[4 * i + 0].pos = D3DXVECTOR3(pos.x + (m_fTexFontSize.x + m_fInterval) * i, pos.y ,0.0f);//左上
		pVtx[4 * i + 1].pos = D3DXVECTOR3(pos.x + m_fTexFontSize.x + (m_fTexFontSize.x + m_fInterval) * i, pos.y, 0.0f);//右上
		pVtx[4 * i + 2].pos = D3DXVECTOR3(pos.x + (m_fTexFontSize.x + m_fInterval) * i, pos.y + m_fTexFontSize.y , 0.0f);	//左下
		pVtx[4 * i + 3].pos = D3DXVECTOR3(pos.x + m_fTexFontSize.x + (m_fTexFontSize.x + m_fInterval) * i, pos.y + m_fTexFontSize.y, 0.0f);//右下
	}
	//ロックを解除する
	m_pVtxBuffer -> Unlock();

	for(int i = 0 ; i < m_nDigit ; i++)
	{
		SetUVScore(i, m_nValue);
	}
}

//=================================================================================
// 描画処理
//=================================================================================
void CScore::Draw(void)
{	
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	pDevice->SetStreamSource(0, m_pVtxBuffer, 0, sizeof(VERTEX_2D));
	
	pDevice->SetFVF(FVF_VERTEX_2D);
	
	pDevice->SetTexture(0 ,CTextureManager::GetResource(m_Key));
	
	for(int nIndex = 0; nIndex < m_nDigit; nIndex++)
	{
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nIndex, NUM_POLYGON);
	}
}

//=================================================================================
// 生成関数
//=================================================================================
CScore *CScore::Create(std::string key, D3DXVECTOR2 pos, D3DXVECTOR2 size, int digit, float interval , int priority)
{
	CScore *p = NULL;
	p = new CScore(priority);
	
	CTextureManager::Load(key);
	p->m_Key = key;
	p->SetPosition(D3DXVECTOR3(pos.x, pos.y, 0.0f));
	p->m_nDigit = digit;
	p->m_fInterval = interval;
	p->m_fTexFontSize = D3DXVECTOR2(size.x * 0.1f, size.y);

	p->Init();

	return p;
}

//=================================================================================
// 頂点の作成
//=================================================================================
void CScore::MakeVertexScore(LPDIRECT3DDEVICE9 pDevice)
{	
	// 座標取得
	D3DXVECTOR3 pos = GetPosition();

	//頂点バッファ―の作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * m_nDigit, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuffer, NULL);
	
	VERTEX_2D* pVtx;

	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);
	
	for(int i = 0; i < m_nDigit; i++)
	{
		//頂点座標の設定(2D座標・右回りで呼ぶこと！)   ( x , y , z )
		pVtx[4 * i + 0].pos = D3DXVECTOR3(pos.x + m_fInterval * i, pos.y, 0.0f);//左上
		pVtx[4 * i + 1].pos = D3DXVECTOR3(pos.x + m_fTexFontSize.x * i + m_fInterval * i, pos.y, 0.0f);//右上
		pVtx[4 * i + 2].pos = D3DXVECTOR3(pos.x + m_fInterval * i, pos.y + m_fTexFontSize.y, 0.0f);	//左下
		pVtx[4 * i + 3].pos = D3DXVECTOR3(pos.x + m_fTexFontSize.x * i + m_fInterval * i, pos.y + m_fTexFontSize.y, 0.0f);//右下
		
		//rhwの設定(必ず1.0が入る)
		pVtx[4 * i + 0].rhw = 1.0;
		pVtx[4 * i + 1].rhw = 1.0;
		pVtx[4 * i + 2].rhw = 1.0;
		pVtx[4 * i + 3].rhw = 1.0;
		//頂点カラー
		pVtx[4 * i + 0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[4 * i + 1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[4 * i + 2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[4 * i + 3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		//テクスチャのUV値
		pVtx[4 * i + 0].tex = D3DXVECTOR2(0.0, 0.0);
		pVtx[4 * i + 1].tex = D3DXVECTOR2(1.0, 0.0);
		pVtx[4 * i + 2].tex = D3DXVECTOR2(0.0, 1.0);
		pVtx[4 * i + 3].tex = D3DXVECTOR2(1.0, 1.0);
	}

	//ロック解除
	m_pVtxBuffer -> Unlock();
}

//=================================================================================
// UVの指定
//=================================================================================
void CScore::SetUVScore(int DigitCount,int value)
{
	int DigitNum;

	for(int nCount = 0; nCount < DigitCount; nCount++)
	{
		value /= 10;
	}
	DigitNum = value % 10;
	m_vfDigitU[m_nDigit -1  -DigitCount] = 0.1 * DigitNum;
}

//=================================================================================
// スコア加算
//=================================================================================
void CScore::AddScore(int value)
{
	// 桁数限界値計算
	std::string sVal = "";
	for (int i = 0; i < m_nDigit; i++)
		sVal += "9";
	int maxVal = std::atoi(sVal.c_str());

	if (m_nValue < maxVal) m_nValue += value;
}

//=================================================================================
// スコア減算
//=================================================================================
void CScore::SubScore(int value)
{
	if(m_nValue > 0) m_nValue -= value;
}

//=================================================================================
// スコア取得
//=================================================================================
int CScore::GetScore(void){
	return m_nValue;
}
