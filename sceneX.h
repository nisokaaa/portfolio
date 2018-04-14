#pragma once
//=================================================================================
//	プログラム名	：	
//	処理			：	Xファイル描画オブジェクト処理[sceneX.h]
//	メイン処理		：	main.cpp
//	作成者			：	moriya gaku
//=================================================================================

//=================================================================================
//	二重定義防止
//=================================================================================
#ifndef _SCENE_X_H_
#define _SCENE_X_H_

//=================================================================================
//	インクルードファイル
//=================================================================================
#include "main.h"
#include "scene.h"
#include "modelManager.h"

class CSceneX : public CScene
{
public:
	CSceneX::CSceneX( int priority = DEFAULT_PRIORITY ) 
	: CScene(priority),
	m_Scale(D3DXVECTOR3(0.0f, 0.0f, 0.0f )),
	m_Color(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f)),
	m_Key("")
{
	D3DXMatrixIdentity( &m_Matrix );
	D3DXMatrixIdentity( &m_MtxRot );
	D3DXMatrixIdentity( &m_MtxPos );
}

	~CSceneX(){};

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	const D3DXMATRIX& GetMatrix( void ){ return m_Matrix; }	
	void SetMatrix( D3DXMATRIX mtx ){ m_Matrix = mtx; }

	const D3DXMATRIX& GetRotMatrix( void ){ return m_MtxRot; }	
	void SetRotMatrix( D3DXMATRIX mtx ){ m_MtxRot = mtx; }

	const D3DXMATRIX& GetPosMatrix( void ){ return m_MtxPos; }	
	void SetPosMatrix( D3DXMATRIX mtx ){ m_MtxPos = mtx; }

	void SetScale( const float x, const float y, const float z ){ m_Scale.x = x; m_Scale.y = y; m_Scale.z = z; }
	void SetScale( const D3DXVECTOR3 scl ){ m_Scale = scl; }
	const D3DXVECTOR3& GetScale( void ) const { return m_Scale; }

	void SetColor( const float r, const float g, const float b, const float a ){ m_Color.r = r; m_Color.g = g; m_Color.b = b; m_Color.a = a; }
	void SetColor( const D3DXCOLOR col ){ m_Color = col; }
	const D3DXCOLOR& GetColor( void ) const { return m_Color; }

	void SetModel( const string key ){ 
		if( FAILED( CModelManager::Load( key ) ) )return;
		SetKey( key ); 
	}

	void SetKey( const string key ){ m_Key = key; }
	const string& GetKey( void ) const { return m_Key; }

	void SetMatrixRotate( const D3DXMATRIX mtxRot ){ m_MtxRot = mtxRot; }

	static CSceneX *Create(string key, int priority = DEFAULT_PRIORITY);

	// キャラクタ束縛姿勢行列算出関数
	D3DXMATRIX* CalcLookAtMatrixAxisFix( D3DXMATRIX* pout, D3DXVECTOR3* pPos, D3DXVECTOR3* pLook, D3DXVECTOR3* pUp );

	// 球面線形補間関数
	D3DXVECTOR3* CSceneX::SphereLinear( D3DXVECTOR3* out, D3DXVECTOR3* start, D3DXVECTOR3* end, float t );

	// 球面線形補間による補間姿勢算出関数
	D3DXMATRIX* CalcInterPause( D3DXMATRIX* out, D3DXMATRIX* start, D3DXMATRIX* end, float t );

private:
	D3DXMATRIX m_Matrix;					// ワールド行列
	D3DXMATRIX m_MtxRot;					// 回転行列
	D3DXMATRIX m_MtxPos;					// 移動行列
	D3DXVECTOR3 m_Scale;					// 大きさ	
	D3DXCOLOR m_Color;						// 色
	string m_Key;
	HRESULT LoadTexture( void );
};

#endif
