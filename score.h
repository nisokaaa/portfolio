#pragma once
//=================================================================================
//	プログラム名	：	
//	処理			：	スコア処理[score.h]
//	メイン処理		：	main.cpp
//	作成者			：	moriya gaku
//=================================================================================

//=================================================================================
//	二重定義防止
//=================================================================================
#ifndef _SCORE_H_
#define _SCORE_H_

//=================================================================================
// インクルードファイル
//=================================================================================
#include "main.h"
#include "scene.h"

class CScore : public CScene
{
public:
	CScore(int priority = DEFAULT_PRIORITY):CScene(priority) {}
	~CScore() {}

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetUVScore(int DigitCount, int value);
	void AddScore(int value);
	void SubScore(int value);
	int  GetScore(void);

	static CScore *Create(std::string key, D3DXVECTOR2 pos, D3DXVECTOR2 size, int digit, float interval = 10.0f, int priority = DEFAULT_PRIORITY);

private:
	void MakeVertexScore(LPDIRECT3DDEVICE9 pDevice);

	int m_nValue;							// スコア値
	std::string m_Key;						// テクスチャキー
	int m_nDigit;							// 桁数
	float m_fInterval;						// 数字の表示間隔
	D3DXVECTOR2 m_fTexFontSize;				// １文字のテクスチャ幅高さ
	std::vector<float> m_vfDigitU;			// 各桁のU値
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffer;	// 頂点バッファ
};

#endif