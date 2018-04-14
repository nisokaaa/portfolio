//=================================================================================
//	�v���O������	�F	
//	����			�F	���͏���[input.cpp]
//	���C������		�F	main.cpp
//	�쐬��			�F	moriya gaku
//=================================================================================

//=================================================================================
//	�C���N���[�h�t�@�C��
//=================================================================================
#include "input.h"

//=================================================================================
//	�}�N����`
//=================================================================================
#define	COUNT_WAIT_REPEAT	(20)				// ���s�[�g�҂�����

//=================================================================================
//	�ÓI�����o�ϐ��̐錾
//=================================================================================
LPDIRECTINPUT8	CInput::m_pDInput = NULL;		// DirectInput�I�u�W�F�N�g

DIMOUSESTATE2 CInputMouse::m_mouseState;		// �}�E�X�̏�Ԃ��󂯂Ƃ郏�[�N
DIMOUSESTATE2 CInputMouse::m_mouseStateTrigger;	// �g���K�[�̃��[�N
POINT CInputMouse::m_posMouseWorld;				// �}�E�X�̃��[���h���擾�ł��郏�[�N

//=================================================================================
// CInput�R���X�g���N�^
//=================================================================================
CInput::CInput()
{
	m_pDIDevice = NULL;
}


//=================================================================================
// CInput�f�X�g���X�^
//=================================================================================
CInput::~CInput()
{
}

//=================================================================================
// CInput����������
//=================================================================================
HRESULT CInput::Init(HINSTANCE hInst, HWND hWnd)
{
	HRESULT hr = S_FALSE;

	// DirectInput�I�u�W�F�N�g�̍쐬
	if(!m_pDInput)
	{
		hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION,
									IID_IDirectInput8, (void**)&m_pDInput, NULL);
	}

	return hr;
}

//=================================================================================
// CInput�X�V����
//=================================================================================
#if 0	// ---> �������z�֐���
HRESULT CInput::Update(void)
{
	return S_OK;
}
#endif

//=================================================================================
// CInput�I������
//=================================================================================
void CInput::Uninit(void)
{
	if(m_pDIDevice)
	{// �f�o�C�X�I�u�W�F�N�g�̊J��
		m_pDIDevice->Unacquire();

		m_pDIDevice->Release();
		m_pDIDevice = NULL;
	}

	if(m_pDInput)
	{// DirectInput�I�u�W�F�N�g�̊J��
		m_pDInput->Release();
		m_pDInput = NULL;
	}
}

//=================================================================================
// CInputKeyboard�R���X�g���X�^
//=================================================================================
CInputKeyboard::CInputKeyboard()
{
	// �e���[�N�̃N���A
	ZeroMemory(m_aKeyState, sizeof m_aKeyState);
	ZeroMemory(m_aKeyStateTrigger, sizeof m_aKeyStateTrigger);
	ZeroMemory(m_aKeyStateRelease, sizeof m_aKeyStateRelease);
	ZeroMemory(m_aKeyStateRepeat, sizeof m_aKeyStateRepeat);
	ZeroMemory(m_aKeyStateRepeatCnt, sizeof m_aKeyStateRepeatCnt);
}

//=================================================================================
// CInputKeyboard�f�X�g���X�^
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

	// ���͏����̏�����
	CInput::Init(hInst, hWnd);

	// �f�o�C�X�I�u�W�F�N�g���쐬
	hr = m_pDInput->CreateDevice(GUID_SysKeyboard, &m_pDIDevice, NULL);
	if(FAILED(hr) || m_pDIDevice == NULL)
	{
		MessageBox(hWnd, "�L�[�{�[�h���˂��I", "�x���I", MB_ICONWARNING);
		return hr;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	hr = m_pDIDevice->SetDataFormat(&c_dfDIKeyboard);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "�L�[�{�[�h�̃f�[�^�t�H�[�}�b�g��ݒ�ł��܂���ł����B", "�x���I", MB_ICONWARNING);
		return hr;
	}

	// �������[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
	hr = m_pDIDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if(FAILED(hr))
	{
		MessageBox(hWnd, "�L�[�{�[�h�̋������[�h��ݒ�ł��܂���ł����B", "�x���I", MB_ICONWARNING);
		return hr;
	}

	// m_pDIDevice�A�N�Z�X�����l��(���͐���J�n)
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

	// �f�o�C�X����f�[�^���擾
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
// �}�E�X�̏���������
//=================================================================================
HRESULT CInputMouse::Init( HINSTANCE hInst, HWND hWnd )
{
	HRESULT hr;

	// ���͏����̏�����
	CInput::Init( hInst, hWnd );

	// �f�o�C�X�I�u�W�F�N�g���쐬
	hr = m_pDInput->CreateDevice( GUID_SysMouse, &m_pDIDevice, NULL );
	if( FAILED( hr )/*|| m_pDIDevice == NULL*/ )
	{
		MessageBox( hWnd, "�}�E�X���Ȃ�", "�x��", MB_ICONWARNING );
		return hr;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	hr = m_pDIDevice->SetDataFormat( &c_dfDIMouse2 );
	if( FAILED( hr ) )
	{
		MessageBox( hWnd, "�}�E�X�̃f�[�^�t�H�[�}�b�g", "�x��", MB_ICONWARNING );
		return hr;
	}

	// �������[�h��ݒ�( �t�H�A�O���E���h����r���I )
	hr = m_pDIDevice->SetCooperativeLevel( hWnd, ( DISCL_FOREGROUND | DISCL_NONEXCLUSIVE ) );
	if( FAILED( hr ) )
	{
		MessageBox( hWnd, "�}�E�X�̋������[�h��ݒ�", "�x��", MB_ICONWARNING );
		return hr;
	}

	// �f�o�C�X�̐ݒ�
	DIPROPDWORD dipdw;

	dipdw.diph.dwSize = sizeof( dipdw );
	dipdw.diph.dwHeaderSize = sizeof( dipdw.diph );
	dipdw.diph.dwObj = 0;
	dipdw.diph.dwHow = DIPH_DEVICE;
	dipdw.dwData = DIPROPAXISMODE_REL;

	hr = m_pDIDevice->SetProperty( DIPROP_AXISMODE, &dipdw.diph );

	// �}�E�X�ւ̃A�N�Z�X�����l��( ���͐���J�n )
	m_pDIDevice->Acquire();

	return hr;
}

//=================================================================================
// �}�E�X�̏I������
//=================================================================================
void CInputMouse::Uninit( void )
{
	// ���͏����̉��
	CInput::Uninit();
}

//=================================================================================
// �}�E�X�̍X�V����
//=================================================================================
HRESULT CInputMouse::Update( void )
{
	HRESULT hr;
	DIMOUSESTATE2 mouseState;

	if( !m_pDIDevice )
	{
		return S_FALSE;
	}

	// �f�o�C�X����f�[�^���擾
	hr = m_pDIDevice->GetDeviceState( sizeof( mouseState ), &mouseState );

	if( SUCCEEDED( hr ) )
	{
		m_mouseStateTrigger.lX = ( ( m_mouseState.lX ^ mouseState.lX ) & mouseState.lX );
		m_mouseStateTrigger.lY = ( ( m_mouseState.lY ^ mouseState.lY ) & mouseState.lY );
		m_mouseStateTrigger.lZ = ( ( m_mouseState.lZ ^ mouseState.lZ ) & mouseState.lZ );

		// �����[�X
		//m_mouseStateTrigger.lX = ( ( m_mouseState.lX ^ mouseState.lX ) & m_mouseState.lX );

		for( int CntKey = 0; CntKey < 8; CntKey++ )
		{
			m_mouseStateTrigger.rgbButtons[CntKey] = ( ( m_mouseState.rgbButtons[CntKey] ^ mouseState.rgbButtons[CntKey] ) & mouseState.rgbButtons[CntKey] );
		}

		m_mouseState = mouseState;

		// �X�N���[�����W���擾
		GetCursorPos( &m_posMouseWorld );
	}
	else
	{
		m_pDIDevice->Acquire();
	}

	return hr;
}

//=================================================================================
// �}�E�X�f�[�^�擾( ���v���X )
//=================================================================================
BOOL CInputMouse::GetLeftPress( void )
{
	return ( m_mouseState.rgbButtons[0] & 0x80 ? TRUE : FALSE );
}

//=================================================================================
// �}�E�X�f�[�^�擾( ���g���K�[ )
//=================================================================================
BOOL CInputMouse::GetLeftTrigger( void )
{
	return ( m_mouseStateTrigger.rgbButtons[0] & 0x80 ? TRUE : FALSE );
}

//=================================================================================
// �}�E�X�f�[�^�擾( �E�v���X )
//=================================================================================
BOOL CInputMouse::GetRightPress( void )
{
	return ( m_mouseState.rgbButtons[1] & 0x80 ? TRUE : FALSE );
}

//=================================================================================
// �}�E�X�f�[�^�擾( �E�g���K�[ )
//=================================================================================
BOOL CInputMouse::GetRightTrigger( void )
{
	return ( m_mouseStateTrigger.rgbButtons[1] & 0x80 ? TRUE : FALSE );
}

//=================================================================================
// �}�E�X�f�[�^�擾( �����v���X )
//=================================================================================
BOOL CInputMouse::GetCenterPress( void )
{
	return ( m_mouseState.rgbButtons[2] & 0x80 ? TRUE : FALSE );
}

//=================================================================================
// �}�E�X�f�[�^�擾( �����g���K�[ )
//=================================================================================
BOOL CInputMouse::GetCenterTrigger( void )
{
	return ( m_mouseStateTrigger.rgbButtons[2] & 0x80 ? TRUE : FALSE );
}

//=================================================================================
// �}�E�X�f�[�^�擾( X���ړ� )
//=================================================================================
LONG CInputMouse::GetAxisX( void )
{
	return m_mouseState.lX;
}

//=================================================================================
// �}�E�X�f�[�^�擾( Y���ړ� )
//=================================================================================
LONG CInputMouse::GetAxisY( void )
{
	return m_mouseState.lY;
}

//=================================================================================
// �}�E�X�f�[�^�擾( Z���ړ� )
//=================================================================================
LONG CInputMouse::GetAxisZ( void )
{
	return m_mouseState.lZ;
}

//=================================================================================
// CInputGamePad �R���X�g���N�^
//=================================================================================
CInputGamePad::CInputGamePad(){
	// �e���[�N�̃N���A
	ZeroMemory(&m_js, sizeof m_js);
	ZeroMemory(m_aPadState, sizeof m_aPadState);
	ZeroMemory(m_aPadStateTrigger, sizeof m_aPadStateTrigger);
	m_bStickState = false;
	m_bStickStateTrigger = false;
}

//=================================================================================
// CInputGamePad �f�X�g���N�^
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
	
	// �f�o�C�X�������ł��Ȃ�������return E_FAILE
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

	// �����[�h��ݒ�i��Βl���[�h�ɐݒ�B�f�t�H���g�Ȃ̂ŕK�������ݒ�͕K�v�Ȃ��j
	DIPROPDWORD diprop;
	diprop.diph.dwSize	= sizeof(diprop); 
	diprop.diph.dwHeaderSize	= sizeof(diprop.diph); 
	diprop.diph.dwObj	= 0;
	diprop.diph.dwHow	= DIPH_DEVICE;
	diprop.dwData		= DIPROPAXISMODE_ABS;
	diprop.dwData		= DIPROPAXISMODE_REL;	// ���Βl���[�h�̏ꍇ
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
	// DirectInput�̃f�o�C�X�����
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
				//�L�[�g���K�[�@�����[�X���̍쐬
				//�g���K�[���O��̃t���[���͉����Ă��Ȃ��č���͉����Ă���
				m_aPadStateTrigger[ nCntPad ] = (m_aPadState[ nCntPad ] ^ m_js.rgbButtons[ nCntPad ]) & m_js.rgbButtons[ nCntPad ];
				
				// ����g�����ߍ��̏�Ԃ����Ă���
				m_aPadState[ nCntPad ] = m_js.rgbButtons[nCntPad];
			}

			// �O��̃t���[���͉����Ă��Ȃ��č���͉����Ă���
			if (!m_bStickState) {
				m_bStickStateTrigger = CheckInputL();
			}
			else {
				m_bStickStateTrigger = false;
			}
			// ����g�����ߍ��̏�Ԃ����Ă���
			m_bStickState = CheckInputL();

		}
		else
		{
			// �A�N�Z�X�����擾
			m_pDIDevice->Acquire();
		}
	}

	return hr;
}

//=================================================================================
// CInputGamePad �p�b�h��Ԏ擾
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
		if (m_aPadStateTrigger[0] & 0x80) return true; // A�{�^��
		break;
	case DIJOFS_BUTTON1:
		if (m_aPadStateTrigger[1] & 0x80) return true; // B�{�^��
		break;
	case DIJOFS_BUTTON2:
		if (m_aPadStateTrigger[2] & 0x80) return true; // X�{�^��
		break;
	case DIJOFS_BUTTON3:
		if (m_aPadStateTrigger[3] & 0x80) return true; // Y�{�^��
		break;
	case DIJOFS_BUTTON4:
		if (m_aPadStateTrigger[4] & 0x80) return true; // L1�{�^��
		break;
	case DIJOFS_BUTTON5:
		if (m_aPadStateTrigger[5] & 0x80) return true; // R1�{�^��
		break;
	case DIJOFS_BUTTON6:
		if (m_aPadStateTrigger[6] & 0x80) return true; // BACK�{�^��
		break;
	case DIJOFS_BUTTON7:
		if (m_aPadStateTrigger[7] & 0x80) return true; // START�{�^��
		break;
	case DIJOFS_BUTTON8:
		if (m_aPadStateTrigger[8] & 0x80) return true; // ���X�e�b�N��������
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
	
	// �f�o�C�X�ۑ�
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