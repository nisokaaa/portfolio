//=================================================================================
//	プログラム名	：	
//	処理			：	入力処理[input.cpp]
//	メイン処理		：	main.cpp
//	作成者			：	moriya gaku
//=================================================================================

//=================================================================================
//	インクルードファイル
//=================================================================================
#include "input.h"

//=================================================================================
//	マクロ定義
//=================================================================================
#define	COUNT_WAIT_REPEAT	(20)				// リピート待ち時間

//=================================================================================
//	静的メンバ変数の宣言
//=================================================================================
LPDIRECTINPUT8	CInput::m_pDInput = NULL;		// DirectInputオブジェクト

DIMOUSESTATE2 CInputMouse::m_mouseState;		// マウスの状態を受けとるワーク
DIMOUSESTATE2 CInputMouse::m_mouseStateTrigger;	// トリガーのワーク
POINT CInputMouse::m_posMouseWorld;				// マウスのワールドを取得できるワーク

//=================================================================================
// CInputコンストラクタ
//=================================================================================
CInput::CInput()
{
	m_pDIDevice = NULL;
}


//=================================================================================
// CInputデストラスタ
//=================================================================================
CInput::~CInput()
{
}

//=================================================================================
// CInput初期化処理
//=================================================================================
HRESULT CInput::Init(HINSTANCE hInst, HWND hWnd)
{
	HRESULT hr = S_FALSE;

	// DirectInputオブジェクトの作成
	if(!m_pDInput)
	{
		hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION,
									IID_IDirectInput8, (void**)&m_pDInput, NULL);
	}

	return hr;
}

//=================================================================================
// CInput更新処理
//=================================================================================
#if 0	// ---> 純粋仮想関数化
HRESULT CInput::Update(void)
{
	return S_OK;
}
#endif

//=================================================================================
// CInput終了処理
//=================================================================================
void CInput::Uninit(void)
{
	if(m_pDIDevice)
	{// デバイスオブジェクトの開放
		m_pDIDevice->Unacquire();

		m_pDIDevice->Release();
		m_pDIDevice = NULL;
	}

	if(m_pDInput)
	{// DirectInputオブジェクトの開放
		m_pDInput->Release();
		m_pDInput = NULL;
	}
}

//=================================================================================
// CInputKeyboardコンストラスタ
//=================================================================================
CInputKeyboard::CInputKeyboard()
{
	// 各ワークのクリア
	ZeroMemory(m_aKeyState, sizeof m_aKeyState);
	ZeroMemory(m_aKeyStateTrigger, sizeof m_aKeyStateTrigger);
	ZeroMemory(m_aKeyStateRelease, sizeof m_aKeyStateRelease);
	ZeroMemory(m_aKeyStateRepeat, sizeof m_aKeyStateRepeat);
	ZeroMemory(m_aKeyStateRepeatCnt, sizeof m_aKeyStateRepeatCnt);
}

//=================================================================================
// CInputKeyboardデストラスタ
//=================================================================================
CInputKeyboard::~CInputKeyboard()
{
}

//=================================================================================
// CInputKeyborad Init
//=================================================================================
HRESULT CInputKeyboard::Init(HINSTANCE hInst, HWND hWnd)
{
	HRESULT hr;

	// 入力処理の初期化
	CInput::Init(hInst, hWnd);

	// デバイスオブジェクトを作成
	hr = m_pDInput->CreateDevice(GUID_SysKeyboard, &m_pDIDevice, NULL);
	if(FAILED(hr) || m_pDIDevice == NULL)
	{
		MessageBox(hWnd, "キーボードがねぇ！", "警告！", MB_ICONWARNING);
		return hr;
	}

	// データフォーマットを設定
	hr = m_pDIDevice->SetDataFormat(&c_dfDIKeyboard);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "キーボードのデータフォーマットを設定できませんでした。", "警告！", MB_ICONWARNING);
		return hr;
	}

	// 協調モードを設定（フォアグラウンド＆非排他モード）
	hr = m_pDIDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if(FAILED(hr))
	{
		MessageBox(hWnd, "キーボードの協調モードを設定できませんでした。", "警告！", MB_ICONWARNING);
		return hr;
	}

	// m_pDIDeviceアクセス権を獲得(入力制御開始)
	m_pDIDevice->Acquire();

	return hr;
}

//=================================================================================
// CInputKeyborad Uninit
//=================================================================================
void CInputKeyboard::Uninit(void)
{
	CInput::Uninit();
}

//=================================================================================
// CInputKeyborad Update
//=================================================================================
HRESULT CInputKeyboard::Update(void)
{
	HRESULT hr;
	BYTE aKeyState[NUM_KEY_MAX];

	if(!m_pDIDevice)
	{
		return E_FAIL;
	}

	// デバイスからデータを取得
	hr = m_pDIDevice->GetDeviceState(sizeof(aKeyState), aKeyState);
	if(SUCCEEDED(hr))
	{
		for(int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			m_aKeyStateTrigger[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];
			m_aKeyStateRelease[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & ~aKeyState[nCntKey];
			m_aKeyStateRepeat[nCntKey] = m_aKeyStateTrigger[nCntKey];

			if(aKeyState[nCntKey])
			{
				m_aKeyStateRepeatCnt[nCntKey]++;
				if(m_aKeyStateRepeatCnt[nCntKey] >= COUNT_WAIT_REPEAT)
				{
					m_aKeyStateRepeat[nCntKey] = aKeyState[nCntKey];
				}
			}
			else
			{
				m_aKeyStateRepeatCnt[nCntKey] = 0;
				m_aKeyStateRepeat[nCntKey] = 0;
			}

			m_aKeyState[nCntKey] = aKeyState[nCntKey];
		}
	}
	else
	{
		m_pDIDevice->Acquire();
	}

	return hr;
}

//=================================================================================
// CInputKeyborad Press
//=================================================================================
BOOL CInputKeyboard::GetKeyPress(int nKey)
{
	return (m_aKeyState[nKey] & 0x80) ? TRUE: FALSE;
}

//=================================================================================
// CInputKeyborad Trigger
//=================================================================================
BOOL CInputKeyboard::GetKeyTrigger(int nKey)
{
	return (m_aKeyStateTrigger[nKey] & 0x80) ? TRUE: FALSE;
}

//=================================================================================
// CInputKeyborad Release
//=================================================================================
BOOL CInputKeyboard::GetKeyRelease(int nKey)
{
	return (m_aKeyStateRelease[nKey] & 0x80) ? TRUE: FALSE;
}

//=================================================================================
// CInputKeyborad Repeat
//=================================================================================
BOOL CInputKeyboard::GetKeyRepeat(int nKey)
{
	return (m_aKeyStateRepeat[nKey] & 0x80) ? TRUE: FALSE;
}

//=================================================================================
// CInputKeyborad Finish
//=================================================================================
void CInputKeyboard::FlushKeyTrigger(int nKey)
{
	m_aKeyStateTrigger[nKey] = 0;
}


CInputMouse::CInputMouse()
{
	memset( &m_mouseState, NULL, sizeof( m_mouseState ) );
	memset ( &m_mouseStateTrigger, NULL, sizeof( m_mouseStateTrigger ) );
	memset ( &m_posMouseWorld, NULL, sizeof( m_posMouseWorld ) );
}

CInputMouse::~CInputMouse()
{

}
//=================================================================================
// マウスの初期化処理
//=================================================================================
HRESULT CInputMouse::Init( HINSTANCE hInst, HWND hWnd )
{
	HRESULT hr;

	// 入力処理の初期化
	CInput::Init( hInst, hWnd );

	// デバイスオブジェクトを作成
	hr = m_pDInput->CreateDevice( GUID_SysMouse, &m_pDIDevice, NULL );
	if( FAILED( hr )/*|| m_pDIDevice == NULL*/ )
	{
		MessageBox( hWnd, "マウスがない", "警告", MB_ICONWARNING );
		return hr;
	}

	// データフォーマットを設定
	hr = m_pDIDevice->SetDataFormat( &c_dfDIMouse2 );
	if( FAILED( hr ) )
	{
		MessageBox( hWnd, "マウスのデータフォーマット", "警告", MB_ICONWARNING );
		return hr;
	}

	// 協調モードを設定( フォアグラウンド＆非排他的 )
	hr = m_pDIDevice->SetCooperativeLevel( hWnd, ( DISCL_FOREGROUND | DISCL_NONEXCLUSIVE ) );
	if( FAILED( hr ) )
	{
		MessageBox( hWnd, "マウスの協調モードを設定", "警告", MB_ICONWARNING );
		return hr;
	}

	// デバイスの設定
	DIPROPDWORD dipdw;

	dipdw.diph.dwSize = sizeof( dipdw );
	dipdw.diph.dwHeaderSize = sizeof( dipdw.diph );
	dipdw.diph.dwObj = 0;
	dipdw.diph.dwHow = DIPH_DEVICE;
	dipdw.dwData = DIPROPAXISMODE_REL;

	hr = m_pDIDevice->SetProperty( DIPROP_AXISMODE, &dipdw.diph );

	// マウスへのアクセス権を獲得( 入力制御開始 )
	m_pDIDevice->Acquire();

	return hr;
}

//=================================================================================
// マウスの終了処理
//=================================================================================
void CInputMouse::Uninit( void )
{
	// 入力処理の解放
	CInput::Uninit();
}

//=================================================================================
// マウスの更新処理
//=================================================================================
HRESULT CInputMouse::Update( void )
{
	HRESULT hr;
	DIMOUSESTATE2 mouseState;

	if( !m_pDIDevice )
	{
		return S_FALSE;
	}

	// デバイスからデータを取得
	hr = m_pDIDevice->GetDeviceState( sizeof( mouseState ), &mouseState );

	if( SUCCEEDED( hr ) )
	{
		m_mouseStateTrigger.lX = ( ( m_mouseState.lX ^ mouseState.lX ) & mouseState.lX );
		m_mouseStateTrigger.lY = ( ( m_mouseState.lY ^ mouseState.lY ) & mouseState.lY );
		m_mouseStateTrigger.lZ = ( ( m_mouseState.lZ ^ mouseState.lZ ) & mouseState.lZ );

		// リリース
		//m_mouseStateTrigger.lX = ( ( m_mouseState.lX ^ mouseState.lX ) & m_mouseState.lX );

		for( int CntKey = 0; CntKey < 8; CntKey++ )
		{
			m_mouseStateTrigger.rgbButtons[CntKey] = ( ( m_mouseState.rgbButtons[CntKey] ^ mouseState.rgbButtons[CntKey] ) & mouseState.rgbButtons[CntKey] );
		}

		m_mouseState = mouseState;

		// スクリーン座標を取得
		GetCursorPos( &m_posMouseWorld );
	}
	else
	{
		m_pDIDevice->Acquire();
	}

	return hr;
}

//=================================================================================
// マウスデータ取得( 左プレス )
//=================================================================================
BOOL CInputMouse::GetLeftPress( void )
{
	return ( m_mouseState.rgbButtons[0] & 0x80 ? TRUE : FALSE );
}

//=================================================================================
// マウスデータ取得( 左トリガー )
//=================================================================================
BOOL CInputMouse::GetLeftTrigger( void )
{
	return ( m_mouseStateTrigger.rgbButtons[0] & 0x80 ? TRUE : FALSE );
}

//=================================================================================
// マウスデータ取得( 右プレス )
//=================================================================================
BOOL CInputMouse::GetRightPress( void )
{
	return ( m_mouseState.rgbButtons[1] & 0x80 ? TRUE : FALSE );
}

//=================================================================================
// マウスデータ取得( 右トリガー )
//=================================================================================
BOOL CInputMouse::GetRightTrigger( void )
{
	return ( m_mouseStateTrigger.rgbButtons[1] & 0x80 ? TRUE : FALSE );
}

//=================================================================================
// マウスデータ取得( 中央プレス )
//=================================================================================
BOOL CInputMouse::GetCenterPress( void )
{
	return ( m_mouseState.rgbButtons[2] & 0x80 ? TRUE : FALSE );
}

//=================================================================================
// マウスデータ取得( 中央トリガー )
//=================================================================================
BOOL CInputMouse::GetCenterTrigger( void )
{
	return ( m_mouseStateTrigger.rgbButtons[2] & 0x80 ? TRUE : FALSE );
}

//=================================================================================
// マウスデータ取得( X軸移動 )
//=================================================================================
LONG CInputMouse::GetAxisX( void )
{
	return m_mouseState.lX;
}

//=================================================================================
// マウスデータ取得( Y軸移動 )
//=================================================================================
LONG CInputMouse::GetAxisY( void )
{
	return m_mouseState.lY;
}

//=================================================================================
// マウスデータ取得( Z軸移動 )
//=================================================================================
LONG CInputMouse::GetAxisZ( void )
{
	return m_mouseState.lZ;
}

//=================================================================================
// CInputGamePad コンストラクタ
//=================================================================================
CInputGamePad::CInputGamePad(){
	// 各ワークのクリア
	ZeroMemory(&m_js, sizeof m_js);
	ZeroMemory(m_aPadState, sizeof m_aPadState);
	ZeroMemory(m_aPadStateTrigger, sizeof m_aPadStateTrigger);
	m_bStickState = false;
	m_bStickStateTrigger = false;
}

//=================================================================================
// CInputGamePad デストラクタ
//=================================================================================
CInputGamePad::~CInputGamePad(){
}

//=================================================================================
// CInputGamePad Init
////===============================================================================
BOOL CALLBACK EnumJoysticksCallback( const DIDEVICEINSTANCE *pdidInstance , VOID *pContext );
BOOL CALLBACK EnumAxesCallback( const DIDEVICEOBJECTINSTANCE *pdidoi , VOID *pContext );
HRESULT CInputGamePad::Init( HINSTANCE hInstance, HWND hWnd )
{
	HRESULT hr;
	
	hr = DirectInput8Create( hInstance , DIRECTINPUT_VERSION , IID_IDirectInput8 ,
	                         (void**)&m_pDInput , NULL );
	if ( FAILED( hr ) ){
		MessageBox( hWnd , "Can't create DirectInput object." , "Error" , MB_OK );
		return E_FAIL;
	}
	
	hr = m_pDInput->EnumDevices( DI8DEVCLASS_GAMECTRL , EnumJoysticksCallback ,
	                        this , DIEDFL_ATTACHEDONLY );
	if ( FAILED( hr ) ){
		MessageBox( hWnd , "Can't create Device." , "Error" , MB_OK );
		return E_FAIL;
	}
	
	// デバイスが生成できなかったらreturn E_FAILE
	if( !m_pDIDevice ) return E_FAIL;

	hr = m_pDIDevice->SetDataFormat( &c_dfDIJoystick2 );
	if ( FAILED( hr ) ){
		MessageBox( hWnd , "Can't set data format." , "Error" , MB_OK );
		return E_FAIL;
	}
	
	hr = m_pDIDevice->SetCooperativeLevel( hWnd , DISCL_EXCLUSIVE | DISCL_FOREGROUND );
	if ( FAILED( hr ) ){
		MessageBox( hWnd , "Can't set cooperative level." , "Error" , MB_OK );
		return E_FAIL;
	}
	
	m_diDevCaps.dwSize = sizeof( DIDEVCAPS );
	hr = m_pDIDevice->GetCapabilities( &m_diDevCaps );
	if ( FAILED( hr ) ){
		MessageBox( hWnd , "Can't create device capabilities." , "Error" , MB_OK );
		return E_FAIL;
	}
	
	hr = m_pDIDevice->EnumObjects( EnumAxesCallback , this , DIDFT_AXIS );
	if ( FAILED( hr ) ){
		MessageBox( hWnd , "Can't set property." , "Error" , MB_OK );
		return E_FAIL;
	}

	// 軸モードを設定（絶対値モードに設定。デフォルトなので必ずしも設定は必要ない）
	DIPROPDWORD diprop;
	diprop.diph.dwSize	= sizeof(diprop); 
	diprop.diph.dwHeaderSize	= sizeof(diprop.diph); 
	diprop.diph.dwObj	= 0;
	diprop.diph.dwHow	= DIPH_DEVICE;
	diprop.dwData		= DIPROPAXISMODE_ABS;
	diprop.dwData		= DIPROPAXISMODE_REL;	// 相対値モードの場合
	//hr = m_pDIDevice->SetProperty(DIPROP_AXISMODE, &diprop.diph);
	if (FAILED(hr))
	{
		return E_FAIL;
	}

	hr = m_pDIDevice->Poll();
	if ( FAILED( hr ) ){
		hr = m_pDIDevice->Acquire();
		while( hr == DIERR_INPUTLOST ){
			hr = m_pDIDevice->Acquire();
		}
	}
	

	return S_OK;
}

//=================================================================================
// CInputGamePad Uninit
//=================================================================================
void CInputGamePad::Uninit( void )
{
	// DirectInputのデバイスを解放
	if( m_pDIDevice )
	{
		m_pDIDevice->Unacquire();
	}
	if( m_pDIDevice != NULL )
	{
		m_pDIDevice->Release();
	}
}

//=================================================================================
// CInputGamePad Update
//=================================================================================
HRESULT CInputGamePad::Update( void )
{	
	HRESULT hr = S_OK;

	for(int nCntPad = 0; nCntPad < NUM_PAD_BUTTON_MAX; nCntPad++)
	{		
		m_js.rgbButtons[ nCntPad ] = 0;
	}

	if ( m_pDIDevice != NULL )
	{
		m_pDIDevice->Poll();
		hr = m_pDIDevice->GetDeviceState(sizeof(DIJOYSTATE2), &m_js);
		if(SUCCEEDED(hr))
		{
			for(int nCntPad = 0; nCntPad < NUM_PAD_BUTTON_MAX; nCntPad++)
			{
				//キートリガー　リリース情報の作成
				//トリガー＝前回のフレームは押していなくて今回は押している
				m_aPadStateTrigger[ nCntPad ] = (m_aPadState[ nCntPad ] ^ m_js.rgbButtons[ nCntPad ]) & m_js.rgbButtons[ nCntPad ];
				
				// 次回使うため今の状態を入れておく
				m_aPadState[ nCntPad ] = m_js.rgbButtons[nCntPad];
			}

			// 前回のフレームは押していなくて今回は押している
			if (!m_bStickState) {
				m_bStickStateTrigger = CheckInputL();
			}
			else {
				m_bStickStateTrigger = false;
			}
			// 次回使うため今の状態を入れておく
			m_bStickState = CheckInputL();

		}
		else
		{
			// アクセス権を取得
			m_pDIDevice->Acquire();
		}
	}

	return hr;
}

//=================================================================================
// CInputGamePad パッド状態取得
//=================================================================================
DIJOYSTATE2 CInputGamePad::GetDInput( void )
{
	return m_js;
}

//=================================================================================
// CInputGamePad Press
//=================================================================================
BOOL CInputGamePad::GetPress( BYTE nButton )
{
	switch( nButton )
	{
	case DIJOFS_BUTTON0:
		if( m_aPadState[ 0 ] & 0x80 ) return true;
		break;
	case DIJOFS_BUTTON1:
		if( m_aPadState[ 1 ] & 0x80 ) return true;
		break;
	case DIJOFS_BUTTON2:
		if( m_aPadState[ 2 ] & 0x80 ) return true;
		break;
	case DIJOFS_BUTTON3:
		if( m_aPadState[ 3 ] & 0x80 ) return true;
		break;
	case DIJOFS_BUTTON4:
		if( m_aPadState[ 4 ] & 0x80 ) return true;
		break;
	case DIJOFS_BUTTON5:
		if( m_aPadState[ 5 ] & 0x80 ) return true;
		break;
	default:
		break;
	}

	return false;
}

//=================================================================================
// CInputGamePad Trigger
//=================================================================================
BOOL CInputGamePad::GetTrigger( BYTE nButton )
{
	switch( nButton )
	{
	case DIJOFS_BUTTON0:
		if (m_aPadStateTrigger[0] & 0x80) return true; // Aボタン
		break;
	case DIJOFS_BUTTON1:
		if (m_aPadStateTrigger[1] & 0x80) return true; // Bボタン
		break;
	case DIJOFS_BUTTON2:
		if (m_aPadStateTrigger[2] & 0x80) return true; // Xボタン
		break;
	case DIJOFS_BUTTON3:
		if (m_aPadStateTrigger[3] & 0x80) return true; // Yボタン
		break;
	case DIJOFS_BUTTON4:
		if (m_aPadStateTrigger[4] & 0x80) return true; // L1ボタン
		break;
	case DIJOFS_BUTTON5:
		if (m_aPadStateTrigger[5] & 0x80) return true; // R1ボタン
		break;
	case DIJOFS_BUTTON6:
		if (m_aPadStateTrigger[6] & 0x80) return true; // BACKボタン
		break;
	case DIJOFS_BUTTON7:
		if (m_aPadStateTrigger[7] & 0x80) return true; // STARTボタン
		break;
	case DIJOFS_BUTTON8:
		if (m_aPadStateTrigger[8] & 0x80) return true; // 左ステック押し込み
		break;
	default:
		break;
	}

	return false;
}

BOOL CInputGamePad::EnumJoysticksCallback( const DIDEVICEINSTANCE *pdidInstance , VOID *pContext )
{
	HRESULT hr;
	CInput *p = (CInput*)pContext;
	IDirectInputDevice8* pDIDevice = p->GetDIDevice();

	hr = p->m_pDInput->CreateDevice( pdidInstance->guidInstance , &pDIDevice, NULL );
	
	// デバイス保存
	p->SetDIDevice( pDIDevice );

	if ( FAILED( hr ) ) return DIENUM_CONTINUE;
	
	return DIENUM_STOP;
}

BOOL CInputGamePad::EnumAxesCallback( const DIDEVICEOBJECTINSTANCE *pdidoi , VOID *pContext )
{
	HRESULT     hr;
	DIPROPRANGE diprg;
	CInput *p = (CInput*)pContext;
	IDirectInputDevice8* pDIDevice = p->GetDIDevice();

	diprg.diph.dwSize       = sizeof( DIPROPRANGE );
	diprg.diph.dwHeaderSize = sizeof( DIPROPHEADER );
	diprg.diph.dwHow        = DIPH_BYID;
	diprg.diph.dwObj        = pdidoi->dwType;
	diprg.lMin              = 0 - 1000;
	diprg.lMax              = 0 + 1000;
	hr = pDIDevice->SetProperty( DIPROP_RANGE , &diprg.diph );

	if ( FAILED( hr ) ) return DIENUM_STOP;
	
	return DIENUM_CONTINUE;
}