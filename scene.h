#pragma once
//=================================================================================
//	プログラム名	：	
//	処理			：	全描画オブジェクト処理[scene.h]
//	メイン処理		：	main.cpp
//	作成者			：	moriya gaku
//=================================================================================

//=================================================================================
//	二重定義防止
//=================================================================================
#ifndef _SCENE_H_
#define _SCENE_H_

//=================================================================================
//	インクルードファイル
//=================================================================================
#include "main.h"

//=================================================================================
//	マクロ定義
//=================================================================================
#define		PRIORITY_MAX		( 8 )						// 優先度数の最大値
#define		DEFAULT_PRIORITY	( 3 )						// デフォルトの優先度数

//=================================================================================
//	前方宣言
//=================================================================================
class CShader;

class CScene
{
public:
	// オブジェクトのタイプ
	typedef enum
	{
		SCENE_TYPE_NONE = 0,
		SCENE_TYPE_PLAYER,
		SCENE_TYPE_ENEMY,
		SCENE_TYPE_FIELD,
		SCENE_TYPE_HP_GAUGE,
		SCENE_TYPE_MP_GAUGE,
		SCENE_TYPE_GAUGE_BACK,
		SCENE_TYPE_RETICLE,
		SCENE_TYPE_MAX,
	}SCENE_TYPE;

	CScene( int priority = DEFAULT_PRIORITY );								
	virtual ~CScene(){};									

	virtual HRESULT Init( void ){return S_OK;};								
	virtual void Uninit( void ) = 0;										
	virtual void Update( void ) = 0;										
	virtual void Draw( void ) = 0;											

	static void UpdateAll(int nOnlyUpdatePriority = -1);
	static void DrawAll(int nOnlyDrawPriority = -1, int nExcludePriority = -1);
	static void ReleaseAll( void );

	void Release( void );									// 派生クラスが自分自身のscene解放

	void SetPosition( const float x, const float y, const float z ){ m_Position.x = x; m_Position.y= y; m_Position.z = z; }
	void SetPosition( const D3DXVECTOR3 pos ){ m_Position = pos; }
	const D3DXVECTOR3& GetPosition( void ) const { return m_Position; }

	void SetRotation( const float x, const float y, const float z ){ m_Rotation.x = x; m_Rotation.y= y; m_Rotation.z = z; }
	void SetRotation( const D3DXVECTOR3 rot ){ m_Rotation = rot; }
	const D3DXVECTOR3& GetRotation( void ) const { return m_Rotation; }

	void SetType( const SCENE_TYPE type ){ m_Type = type; }													
	const SCENE_TYPE& GetType( void ) const { return m_Type; }

	void SetShader(CShader *pShader) { m_pShader = pShader; }
	CShader* GetShader(void) const { return m_pShader; }

	static CScene *GetList( int priority ){ return m_Top[ priority ]; }
	CScene *GetNextScene( void ){ return m_Next; }

private:
	D3DXVECTOR3 m_Position;									// 位置座標
	D3DXVECTOR3 m_Rotation;									// 回転
	
	static CScene *m_Top[ PRIORITY_MAX ];					// リストトップポインタ
	CScene *m_Next;											// 次リストへのポインタ
	SCENE_TYPE m_Type;										// オブジェクトのタイプ
	bool m_Delete;											// 次フレームでの削除予約フラグ
	CShader *m_pShader;										// 使用するシェーダ
};

#endif