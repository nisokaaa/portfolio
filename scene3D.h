#pragma once
//=================================================================================
//	プログラム名	：	
//	処理			：	3D描画オブジェクト処理[scene3D.h]
//	メイン処理		：	main.cpp
//	作成者			：	moriya gaku
//=================================================================================

//=================================================================================
//	二重定義防止
//=================================================================================
#ifndef _SCENE3D_H_
#define _SCENE3D_H_

//=================================================================================
//	インクルードファイル
//=================================================================================
#include "main.h"
#include "scene.h"
#include "textureManager.h"

class CScene3D : public CScene
{
public:
	CScene3D( int priority = DEFAULT_PRIORITY );
	~CScene3D();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetScale( const D3DXVECTOR3 scale ){ m_Scale = scale; }
	const D3DXVECTOR3& GetScale( void ) const { return m_Scale; }

	void SetColor( const D3DXCOLOR color ){ m_Color = color; }
	const D3DXCOLOR& GetColor( void ) const { return m_Color; }

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

	void SetVertexBuffer( const LPDIRECT3DVERTEXBUFFER9 pVtxBuffer ){
		m_pVtxBuffer = pVtxBuffer; }
	const LPDIRECT3DVERTEXBUFFER9& GetVertexBuffer( void ) const { return m_pVtxBuffer; }

	void SetTexture( const string key ){ 
		if( FAILED( CTextureManager::Load( key ) ) )return;
		SetKey( key ); }

	void SetKey( const string key ){ m_Key = key; }
	const string& GetKey( void ) const { return m_Key; }

private:
	D3DXMATRIX m_mtxWorld;
	D3DXVECTOR3 m_Scale;					// 大きさ	
	D3DXCOLOR m_Color;						// 色
	float m_Width;							// 幅
	float m_Height;							// 高さ

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffer;	// 頂点バッファへのポインタ
	string m_Key;							// テクスチャポインタ検索キー(固定シェーダー用)
	float m_Length;							// ポリゴンの対角線の長さ
	float m_Angle;							// ポリゴンの対角線の角度	
	float m_TexU;							// U値
	float m_TexV;							// V値
	float m_TexWidth;						// テクスチャ幅
	float m_TexHeight;						// テクスチャ高さ

	virtual HRESULT MakeVertexBuffer( void );
	virtual void SetVertex( void );
};

#endif
