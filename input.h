#pragma once
//=================================================================================
//	プログラム名	：	
//	処理			：	入力処理[input.h]
//	メイン処理		：	main.cpp
//	作成者			：	moriya gaku
//=================================================================================

//=================================================================================
//	二重定義防止
//=================================================================================
#ifndef _INPUT_H_
#define _INPUT_H_

//=================================================================================
//	インクルードファイル
//=================================================================================
#include "main.h"

//=================================================================================
//	マクロ定義
//=================================================================================
#define SENSITIVITY			( 250.0f )		// ステック感度
#define	NUM_KEY_MAX			( 256 )			// キー最大数
#define DIDEVICE_BUFFERSIZE	( 100 )			// デバイスに設定するバッファ・サイズ
#define NUM_PAD_BUTTON_MAX	( 9 )			// ゲームパッドボタンの最大数

//=================================================================================
//	入力クラス
//=================================================================================
class CInput
{
public:
	CInput();
	virtual ~CInput();

	virtual HRESULT Init(HINSTANCE hInst, HWND hWnd);
	virtual void Uninit(void);
	virtual HRESULT Update(void) = 0;		// ---> 純粋仮想関数化
	LPDIRECTINPUTDEVICE8 GetDIDevice( void ){ return m_pDIDevice; }
	void SetDIDevice( LPDIRECTINPUTDEVICE8 pDIDevice ){ m_pDIDevice = pDIDevice; }

protected:
	static LPDIRECTINPUT8 m_pDInput;	// DirectInputオブジェクト
	IDirectInputDevice8* m_pDIDevice;	// Deviceオブジェクト(入力に必要)
};

//=================================================================================
//	キーボード入力クラス
//=================================================================================
class CInputKeyboard : public CInput
{
public:
	CInputKeyboard();
	~CInputKeyboard();

	HRESULT Init(HINSTANCE hInst, HWND hWnd);
	void Uninit(void);
	HRESULT Update(void);

	BOOL GetKeyPress(int nKey);
	BOOL GetKeyTrigger(int nKey);
	BOOL GetKeyRelease(int nKey);
	BOOL GetKeyRepeat(int nKey);
	void FlushKeyTrigger(int nKey);

private:
	BYTE	m_aKeyState[NUM_KEY_MAX];			// キーボードの状態を受け取るワーク
	BYTE	m_aKeyStateTrigger[NUM_KEY_MAX];	// トリガーワーク
	BYTE	m_aKeyStateRelease[NUM_KEY_MAX];	// リリースワーク
	BYTE	m_aKeyStateRepeat[NUM_KEY_MAX];		// リピートワーク
	int		m_aKeyStateRepeatCnt[NUM_KEY_MAX];	// リピートカウンタ
};

//=================================================================================
//	マウス入力クラス
//=================================================================================
class CInputMouse : public CInput
{
public:
	CInputMouse();
	~CInputMouse();

	HRESULT Init( HINSTANCE hInst, HWND hWnd );
	void Uninit( void );
	HRESULT Update( void );

	BOOL GetLeftPress( void );
	BOOL GetLeftTrigger( void );
	BOOL GetRightPress( void );
	BOOL GetRightTrigger( void );
	BOOL GetCenterPress( void );
	BOOL GetCenterTrigger( void );
	LONG GetAxisX( void );
	LONG GetAxisY( void );
	LONG GetAxisZ( void );
	POINT *GetPosWorld( void ){ return &m_posMouseWorld; }

private:
	static DIMOUSESTATE2 m_mouseState;			// マウスの状態を受けとるワーク
	static DIMOUSESTATE2 m_mouseStateTrigger;	// トリガーのワーク
	static POINT m_posMouseWorld;				// マウスのワールドを取得できるワーク
};

//=================================================================================
//	ゲームパッド入力クラス
//=================================================================================
class CInputGamePad : public CInput
{
public:
	CInputGamePad();
	~CInputGamePad();

	HRESULT Init( HINSTANCE hInstance, HWND hWnd );
	void Uninit( void );
	HRESULT Update( void );
	
	DIJOYSTATE2 GetDInput( void );						// 状態取得
	BOOL GetPress( BYTE nButton );						// 押下状態の取得( プレス )
	BOOL GetTrigger( BYTE nButton );					// 押下状態の取得( トリガー )
	BOOL CheckInputL(void)			{ return fabs((float)m_js.lX) > SENSITIVITY || fabs((float)m_js.lY) > SENSITIVITY ? true : false; }
	BOOL CheckInputR(void)			{ return fabs((float)m_js.lRx) > SENSITIVITY || fabs((float)m_js.lRy) > SENSITIVITY ? true : false; }
	BOOL CheckInputLStickUp(void)	{ return (float)m_js.lY < SENSITIVITY ? true : false; }
	BOOL CheckInputLStickDown(void) { return (float)m_js.lY > SENSITIVITY ? true : false; }
	BOOL CheckInputLStickRight(void){ return (float)m_js.lX > SENSITIVITY ? true : false; }
	BOOL CheckInputLStickLeft(void) { return (float)m_js.lX < SENSITIVITY ? true : false; }
	BOOL GetStickTrigger(void)		{ return m_bStickStateTrigger; }

	static BOOL CALLBACK EnumJoysticksCallback( const DIDEVICEINSTANCE *pdidInstance , VOID *pContext );
	static BOOL CALLBACK EnumAxesCallback( const DIDEVICEOBJECTINSTANCE *pdidoi , VOID *pContext );

private:
	DIDEVCAPS				m_diDevCaps;									// 能力
	DIJOYSTATE2				m_js;											// 状態
	BYTE					m_aPadState[ NUM_PAD_BUTTON_MAX ];				// 入力情報ワーク
	BYTE					m_aPadStateTrigger[ NUM_PAD_BUTTON_MAX ];		// トリガー情報ワーク
	BOOL					m_bStickState;									// パッド入力情報ワーク
	BOOL					m_bStickStateTrigger;							// パッドトリガー情報ワーク
};

#endif