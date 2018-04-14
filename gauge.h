#pragma once
//=================================================================================
//	プログラム名	：	
//	処理			：	ゲージ処理[gauge.h]
//	メイン処理		：	main.cpp
//	作成者			：	moriya gaku
//=================================================================================

//=================================================================================
//	二重定義防止
//=================================================================================
#ifndef _GAUGE_H_
#define _GAUGE_H_

//=================================================================================
//	インクルードファイル
//=================================================================================
#include "main.h"
#include "scene.h"
#include "scene2D.h"

class CGauge : public CScene2D
{
public:
	CGauge(int priority = DEFAULT_PRIORITY, BOOL mode = FALSE)
		: CScene2D(priority, mode) {}
	~CGauge() {}

	HRESULT Init( void );
	void Uninit( void );	
	void Update( void );
	void Draw( void );

	static CGauge *Create(std::string key, D3DXVECTOR2 pos, D3DXVECTOR2 size, float scl, float val, float maxVal,
						  std::string key2, D3DXVECTOR2 offsetPos, D3DXVECTOR2 size2, float scl2, BOOL mode = FALSE, int priority = DEFAULT_PRIORITY);

	void Add(const float val) { m_Val += val; }
	void Sub(const float val) { m_Val -= val; }
	void SetVal( const float val ){ m_Val = val; }
	const float& GetVal( void ) const { return m_Val; }

	const float& GetMaxVal(void) const { return m_MaxVal; }

	const float& GetOffSetWidth(void) const { return m_OffSetWidth; }
	
	VERTEX_2D* SetVertex(VERTEX_2D *pVtx);

private:
	CScene2D *m_pBackGround;	// 背景ポリゴン
	float m_Val;			// 値
	float m_MaxVal;			// 最大値
	float m_OffSetWidth;	// 減少値
};

#endif