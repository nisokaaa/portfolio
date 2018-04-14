#pragma once
//=================================================================================
//	プログラム名	：	
//	処理			：	2D描画オブジェクト処理[scene2D.h]
//	メイン処理		：	main.cpp
//	作成者			：	moriya gaku
//=================================================================================

//=================================================================================
//	二重定義防止
//=================================================================================
#ifndef _SCENE2D_H_
#define _SCENE2D_H_

//=================================================================================
//	インクルードファイル
//=================================================================================
#include "scene.h"
#include "textureManager.h"

class CScene2D : public CScene
{
public:
	CScene2D( int priority = DEFAULT_PRIORITY, BOOL mode = false)
		: CScene( priority ), m_Key(""), m_pVtxBuff(nullptr),
		m_Length(0.0f), m_Angle(0.0f), m_Width(0.0f),
		m_Height(0.0f), m_TexU(0.0f), m_TexV(0.0f),
		m_TexWidth(0.0f), m_TexHeight(0.0f), m_Scale(1.0f), m_bDrawType(false){}
	
	~CScene2D(){}

	HRESULT Init( void );
	void Uninit( void );
	void Update( void );
	void Draw( void );

	static CScene2D *Create(std::string key, D3DXVECTOR2 pos, float width, float height, float scl, BOOL mode = false, int priority = DEFAULT_PRIORITY);

	virtual VERTEX_2D* SetVertex( VERTEX_2D *pVtx );

	void SetVertexBuffer( const LPDIRECT3DVERTEXBUFFER9 pVtxBuff ){
		m_pVtxBuff = pVtxBuff; }
	const LPDIRECT3DVERTEXBUFFER9& GetVertexBuffer( void ) const { return m_pVtxBuff; }

	void SetWidth( const float width ){ m_Width = width; }
	const float& GetWidth( void ) const { return m_Width; }

	void SetHeight( const float height ){ m_Height = height; }
	const float& GetHeight( void ) const { return m_Height; }

	void SetTexWidth( const float texWidth ){ m_TexWidth = texWidth; }
	const float& GetTexWidth( void ) const { return m_TexWidth; }

	void SetTexHeight( const float texHeight ){ m_TexHeight = texHeight; }
	const float& GetTexHeight( void ) const { return m_TexHeight; }
	
	void SetTexU( const float texU ){ m_TexU = texU; }
	const float& GetTexU( void ) const { return m_TexU; }

	void SetTexV( const float texV ){ m_TexV = texV; }
	const float& GetTexV( void ) const { return m_TexV; }

	void SetTexture( const string key ){ 
		if( FAILED( CTextureManager::Load( key ) ) )return;
		SetKey( key ); }

	void SetKey( const string key ){ m_Key = key; }
	const string& GetKey( void ) const { return m_Key; }

	void SetColor( const D3DXCOLOR color ){ m_Color = color; }
	const D3DXCOLOR& GetColor( void ) const { return m_Color; }

	void SetAlpha(const float a) { m_Color.a = a; }
	const float& GetAlpha(void) const { return m_Color.a; }

	void SetLength( const float length ){ m_Length = length; }
	const float& GetLength( void ) const { return m_Length; }

	void SetAngle( const float angle ){ m_Angle = angle; }
	const float& GetAngle( void ) const { return m_Angle; }

	void SetScale( const float scl ){ m_Scale = scl; }
	const float& GetScale( void ) const { return m_Scale; }

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;				// 頂点バッファへのポインタ
	string m_Key;									// テクスチャポインタ検索キー
	float m_Length;									// ポリゴンの対角線の長さ
	float m_Angle;									// ポリゴンの対角線の角度
	float m_Width;									// 幅
	float m_Height;									// 高さ	
	float m_TexU;									// U値
	float m_TexV;									// V値
	float m_TexWidth;								// テクスチャ幅
	float m_TexHeight;								// テクスチャ高さ
	D3DXCOLOR m_Color;								// 色
	float m_Scale;									// スケール
	BOOL m_bDrawType;								// 描画タイプ(中心座標または左上座標を基点)
};

#endif


