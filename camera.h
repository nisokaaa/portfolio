#pragma once
//=================================================================================
//	プログラム名	：	
//	処理			：	カメラ処理[camera.h]
//	メイン処理		：	main.cpp
//	作成者			：	moriya gaku
//=================================================================================

//=================================================================================
//	二重定義防止
//=================================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

//=================================================================================
//	インクルードファイル
//=================================================================================
#include "main.h"

//=================================================================================
//	マクロ定義
//=================================================================================
#define CAMERA_AT_NEAR		( 1.0f )		// 描画下限距離
#define CAMERA_AT_FAR		( 5000.0f )		// 描画上限距離

class CCamera
{
public:
	CCamera();										
	~CCamera();										
	
	void Init( D3DXVECTOR3 pos, float distance, float middleAtOffSetY );								
	void Uninit( void );							
	void Update( void );							
	
	void Set( void );

	const D3DXMATRIX &GetViewMtx( void ){ return m_MtxView; }	
	void SetViewMtx( D3DXMATRIX mtxView ){ m_MtxView = mtxView; }

	const D3DXMATRIX& GetProjMtx( void ){ return m_MtxProj; }	
	void SetProjMtx( D3DXMATRIX mtxProj ){ m_MtxProj = mtxProj; }

	const D3DXMATRIX& GetViewPortMtx( void ){ return m_MtxViewPort; }	
	void SetViewPortMtx( D3DXMATRIX mtxViewPort ){ m_MtxViewPort = mtxViewPort; }

	const D3DXVECTOR3& GetPosition( void ) { return m_Position; }					
	void SetPosition( const D3DXVECTOR3 pos ){ m_Position = pos; }					
	
	const D3DXVECTOR3& GetLookAt( void ) const { return m_LookAt; }
	void SetLookAt( const D3DXVECTOR3 at ){ m_LookAt = at; }
	
	const D3DXVECTOR3 GetVecDir( void ) const { return m_VecDir; }								
	
	void SetDistance(float dist) { m_Distance += dist; }

	void SetMiddleOffSet(float middleOffSet) { m_MiddleAtOffSetY += middleOffSet; }

	// 回転行列作成
	void CreateRotMatrix( void );

	// 移動行列作成
	void CreatePosMatrix( void );

	// パラメータ表示
	void ShowParam( void );

private:
	D3DXMATRIX		m_MtxView;					// ビュー行列
	D3DXMATRIX		m_MtxProj;					// プロジェクション行列
	D3DXMATRIX		m_MtxViewPort;				// ビューポート行列
	D3DXMATRIX		m_MtxRot;					// 姿勢用回転行列
	D3DXMATRIX		m_MtxPos;					// 姿勢用位置行列

	float			m_fYRot;					// Y軸回転量
	float			m_fXRot;					// X軸回転量

	float			m_Distance;					// 追従対象からの距離
	float			m_MiddleAtOffSetY;			// 追従対象頭上までの高さ(注視点の中間)
	float			m_RotTarget;				// なす角目標回転量Y
	D3DXVECTOR3		m_LookAt;					// 注視点
	D3DXVECTOR3		m_LookAtTarget;				// 補間後の目標注視点
	D3DXVECTOR3		m_Position;					// 座標
	D3DXVECTOR3		m_VecDir;					// カメラの向き

	bool			m_InterpolationEnable;		// 補間可能フラグ
	bool			m_bInterpolationDuring;		// 補間中フラグ
};

#endif