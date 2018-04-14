#pragma once
//=================================================================================
//	プログラム名	：	
//	処理			：	ゲーム管理[manager.h]
//	メイン処理		：	main.cpp
//	作成者			：	moriya gaku
//=================================================================================

//=================================================================================
//	二重定義防止
//=================================================================================
#ifndef _MANAGER_H_
#define _MANAGER_H_

//=================================================================================
//	インクルードファイル
//=================================================================================
#include "main.h"

//=================================================================================
//	前方宣言
//=================================================================================
class CInputKeyboard;
class CInputGamePad;
class CInputMouse;

class CManager
{
public:
	CManager();														
	~CManager();

	static HRESULT Init( HINSTANCE hInstance, HWND hWnd, bool bWindow );	
	static void Uninit( void );											
	static void Update( void );											
	static void Draw( void );											
	
	// 管轄するインターフェースの取得関数：CManagerクラスは〇〇クラスを持っている前提			
	static CInputKeyboard *GetInputKeyboard( void ){ return m_pInputKeyboard; }
	static CInputGamePad *GetInputGamePad( void ){ return m_pInputGamePad; }
	static CInputMouse *GetInputMouse( void ){ return m_pInputMouse; }

private:
	static CInputKeyboard *m_pInputKeyboard;
	static CInputGamePad *m_pInputGamePad;	
	static CInputMouse *m_pInputMouse;
};


#endif