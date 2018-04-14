#pragma once
//=================================================================================
//	プログラム名	：	
//	処理			：	標的処理[reticle.h]
//	メイン処理		：	main.cpp
//	作成者			：	moriya gaku
//=================================================================================

//=================================================================================
//	二重定義防止
//=================================================================================
#ifndef _RETICLE_H_
#define _RETICLE_H_

//=================================================================================
//	インクルードファイル
//=================================================================================
#include "main.h"
#include "scene2D.h"

//=================================================================================
//	マクロ定義
//=================================================================================
#define RETICLE_WIDTH (100.0f)
#define RETICLE_HEIGHT (100.0f)

class CReticle : public CScene2D
{
public:
	CReticle( int priority = DEFAULT_PRIORITY, BOOL mode = false )
	: CScene2D( priority, mode ){}

	~CReticle(){};

	HRESULT Init( void );
	void Uninit( void );
	void Update( void );
	void Draw( void );

	static void SetVecDir( const D3DXVECTOR3 at ){ m_VecDir = at; }
	static const D3DXVECTOR3& GetVecDir( void ){ return m_VecDir; }

	static void SetViewAngle( const float angle ){ m_ViewAngle = angle; }
	static const float& GetViewAngle( void ){ return m_ViewAngle; }
	
	static void Set3DPosition( const float x, const float y, const float z ){ m_3DPosition.x = x; m_3DPosition.y= y; m_3DPosition.z = z; }
	static void Set3DPosition( const D3DXVECTOR3 pos ){ m_3DPosition = pos; }
	static const D3DXVECTOR3& Get3DPosition( void ){ return m_3DPosition; }
	
	static const bool GetLockOnMode( void ){ return m_bLockOn; }

	void ShowParam( void );

private:
	static float		m_ViewAngle;			// 視野角
	static D3DXVECTOR3	m_VecDir;				// ベクトル
	static D3DXVECTOR3	m_3DPosition;			// ３次元座標
	static bool			m_bLockOn;				// ロックオンモード
};

#endif