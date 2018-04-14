#pragma once
//=================================================================================
//	�v���O������	�F	
//	����			�F	���͏���[input.h]
//	���C������		�F	main.cpp
//	�쐬��			�F	moriya gaku
//=================================================================================

//=================================================================================
//	��d��`�h�~
//=================================================================================
#ifndef _INPUT_H_
#define _INPUT_H_

//=================================================================================
//	�C���N���[�h�t�@�C��
//=================================================================================
#include "main.h"

//=================================================================================
//	�}�N����`
//=================================================================================
#define SENSITIVITY			( 250.0f )		// �X�e�b�N���x
#define	NUM_KEY_MAX			( 256 )			// �L�[�ő吔
#define DIDEVICE_BUFFERSIZE	( 100 )			// �f�o�C�X�ɐݒ肷��o�b�t�@�E�T�C�Y
#define NUM_PAD_BUTTON_MAX	( 9 )			// �Q�[���p�b�h�{�^���̍ő吔

//=================================================================================
//	���̓N���X
//=================================================================================
class CInput
{
public:
	CInput();
	virtual ~CInput();

	virtual HRESULT Init(HINSTANCE hInst, HWND hWnd);
	virtual void Uninit(void);
	virtual HRESULT Update(void) = 0;		// ---> �������z�֐���
	LPDIRECTINPUTDEVICE8 GetDIDevice( void ){ return m_pDIDevice; }
	void SetDIDevice( LPDIRECTINPUTDEVICE8 pDIDevice ){ m_pDIDevice = pDIDevice; }

protected:
	static LPDIRECTINPUT8 m_pDInput;	// DirectInput�I�u�W�F�N�g
	IDirectInputDevice8* m_pDIDevice;	// Device�I�u�W�F�N�g(���͂ɕK�v)
};

//=================================================================================
//	�L�[�{�[�h���̓N���X
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
	BYTE	m_aKeyState[NUM_KEY_MAX];			// �L�[�{�[�h�̏�Ԃ��󂯎�郏�[�N
	BYTE	m_aKeyStateTrigger[NUM_KEY_MAX];	// �g���K�[���[�N
	BYTE	m_aKeyStateRelease[NUM_KEY_MAX];	// �����[�X���[�N
	BYTE	m_aKeyStateRepeat[NUM_KEY_MAX];		// ���s�[�g���[�N
	int		m_aKeyStateRepeatCnt[NUM_KEY_MAX];	// ���s�[�g�J�E���^
};

//=================================================================================
//	�}�E�X���̓N���X
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
	static DIMOUSESTATE2 m_mouseState;			// �}�E�X�̏�Ԃ��󂯂Ƃ郏�[�N
	static DIMOUSESTATE2 m_mouseStateTrigger;	// �g���K�[�̃��[�N
	static POINT m_posMouseWorld;				// �}�E�X�̃��[���h���擾�ł��郏�[�N
};

//=================================================================================
//	�Q�[���p�b�h���̓N���X
//=================================================================================
class CInputGamePad : public CInput
{
public:
	CInputGamePad();
	~CInputGamePad();

	HRESULT Init( HINSTANCE hInstance, HWND hWnd );
	void Uninit( void );
	HRESULT Update( void );
	
	DIJOYSTATE2 GetDInput( void );						// ��Ԏ擾
	BOOL GetPress( BYTE nButton );						// ������Ԃ̎擾( �v���X )
	BOOL GetTrigger( BYTE nButton );					// ������Ԃ̎擾( �g���K�[ )
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
	DIDEVCAPS				m_diDevCaps;									// �\��
	DIJOYSTATE2				m_js;											// ���
	BYTE					m_aPadState[ NUM_PAD_BUTTON_MAX ];				// ���͏�񃏁[�N
	BYTE					m_aPadStateTrigger[ NUM_PAD_BUTTON_MAX ];		// �g���K�[��񃏁[�N
	BOOL					m_bStickState;									// �p�b�h���͏�񃏁[�N
	BOOL					m_bStickStateTrigger;							// �p�b�h�g���K�[��񃏁[�N
};

#endif