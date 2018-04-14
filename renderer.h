#pragma once
//=================================================================================
//	プログラム名	：	
//	処理			：	描画設定処理[renderer.h]
//	メイン処理		：	main.cpp
//	作成者			：	moriya gaku
//=================================================================================

//=================================================================================
//	二重定義防止
//=================================================================================
#ifndef _RENDERER_H_
#define _RENDERER_H_

//=================================================================================
//	インクルードファイル
//=================================================================================
#include "main.h"

class CRenderer
{
public:
	CRenderer();								
	~CRenderer();		

	static HRESULT Init( HWND hWnd, bool bWindow );	
	static void Uninit( void );

	static HRESULT DrawBegin( void );
	static void DrawEnd( void );
	
	static void DrawFPS( void );						// FPS描画
	static void DrawMenu( void );						// 操作方法表示

	static LPDIRECT3DDEVICE9 GetDevice( void );		

	static LPDIRECT3DSURFACE9 GetDeviceSurf(void) { return m_pDeviceSurf; }
	static LPDIRECT3DSURFACE9 GetDeviceDepthStencil(void) { return m_pDeviceDepthStencil; }
	
private:
	static LPDIRECT3D9	m_pD3D;							// Direct3Dオブジェクト
	static LPDIRECT3DDEVICE9 m_pD3DDevice;				// デバイスインタフェース

	static LPDIRECT3DSURFACE9 m_pDeviceSurf;			// バックバッファサーフェス、Present()で指定する為に保持
	static LPDIRECT3DSURFACE9 m_pDeviceDepthStencil;	// デプスステンシルサーフェイス
};

#endif