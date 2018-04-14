#pragma once
//=================================================================================
//	プログラム名	：	
//	処理			：	フェード処理[fade.h]
//	メイン処理		：	main.cpp
//	作成者			：	moriya gaku
//=================================================================================

//=================================================================================
//	二重定義防止
//=================================================================================
#ifndef _FADE_H_
#define _FADE_H_

//=================================================================================
//	インクルードファイル
//=================================================================================
#include "main.h"
#include "sceneManager.h"
#include "textureManager.h"

using namespace std;


class CFade
{

public:
	typedef enum
	{
		FADE_NONE = 0,
		FADE_IN,
		FADE_OUT,
	}MODE;

	CFade(){};
	~CFade(){};

	static HRESULT Init( void );
	static void Uninit( void );
	static void Update( void );
	static void Draw( void );

	static void SetFade( MODE fadeMode, CSceneBase *pSceneNext, float sec );
	static MODE GetFadeMode( void );

	static void SetTexture( const string key ){ 
		if( FAILED( CTextureManager::Load( key ) ) )return;
		SetKey( key ); }

	static void SetKey( const string key ){ m_Key = key; }
	static const string& GetKey( void ){ return m_Key; }

	static void SetAlpha(const float a) { m_Color.a = a; };

private:
	static LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;				// 頂点バッファへのポインタ
	static string m_Key;									// テクスチャポインタ検索キー
	
	static D3DXVECTOR3 m_Position;							// 座標
	static float m_Length;									// ポリゴンの対角線の長さ
	static float m_Angle;									// ポリゴンの対角線の角度
	static float m_Width;									// 幅
	static float m_Height;									// 高さ	
	static float m_TexU;									// U値
	static float m_TexV;									// V値
	static float m_TexWidth;								// テクスチャ幅
	static float m_TexHeight;								// テクスチャ高さ

	static D3DXCOLOR	m_Color;							// フェード色
	static CFade::MODE	m_FadeMode;							// フェード状態
	static CSceneBase*	m_pNextScene;						// 遷移先
	static float		m_FadeRate;							// フェードレート

};

#endif

//LPDIRECT3DTEXTURE9			g_pTextureFade = NULL;		// テクスチャインターフェース
//LPDIRECT3DVERTEXBUFFER9		g_pVtxBufferFade = NULL;	// 頂点バッファインタフェースへのポインタ					
//
//D3DXCOLOR					g_colorFade;				// フェード色
//FADE						g_fade;						// フェード状態
//MODE						g_modeNext = MODE_TITLE;	// モード
//float						g_fadeRate;					// フェードレート