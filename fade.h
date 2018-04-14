#pragma once
//=================================================================================
//	�v���O������	�F	
//	����			�F	�t�F�[�h����[fade.h]
//	���C������		�F	main.cpp
//	�쐬��			�F	moriya gaku
//=================================================================================

//=================================================================================
//	��d��`�h�~
//=================================================================================
#ifndef _FADE_H_
#define _FADE_H_

//=================================================================================
//	�C���N���[�h�t�@�C��
//=================================================================================
#include "main.h"
#include "sceneManager.h"
#include "textureManager.h"

using namespace std;


class CFade
{

public:
	typedef enum
	{
		FADE_NONE = 0,
		FADE_IN,
		FADE_OUT,
	}MODE;

	CFade(){};
	~CFade(){};

	static HRESULT Init( void );
	static void Uninit( void );
	static void Update( void );
	static void Draw( void );

	static void SetFade( MODE fadeMode, CSceneBase *pSceneNext, float sec );
	static MODE GetFadeMode( void );

	static void SetTexture( const string key ){ 
		if( FAILED( CTextureManager::Load( key ) ) )return;
		SetKey( key ); }

	static void SetKey( const string key ){ m_Key = key; }
	static const string& GetKey( void ){ return m_Key; }

	static void SetAlpha(const float a) { m_Color.a = a; };

private:
	static LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;				// ���_�o�b�t�@�ւ̃|�C���^
	static string m_Key;									// �e�N�X�`���|�C���^�����L�[
	
	static D3DXVECTOR3 m_Position;							// ���W
	static float m_Length;									// �|���S���̑Ίp���̒���
	static float m_Angle;									// �|���S���̑Ίp���̊p�x
	static float m_Width;									// ��
	static float m_Height;									// ����	
	static float m_TexU;									// U�l
	static float m_TexV;									// V�l
	static float m_TexWidth;								// �e�N�X�`����
	static float m_TexHeight;								// �e�N�X�`������

	static D3DXCOLOR	m_Color;							// �t�F�[�h�F
	static CFade::MODE	m_FadeMode;							// �t�F�[�h���
	static CSceneBase*	m_pNextScene;						// �J�ڐ�
	static float		m_FadeRate;							// �t�F�[�h���[�g

};

#endif

//LPDIRECT3DTEXTURE9			g_pTextureFade = NULL;		// �e�N�X�`���C���^�[�t�F�[�X
//LPDIRECT3DVERTEXBUFFER9		g_pVtxBufferFade = NULL;	// ���_�o�b�t�@�C���^�t�F�[�X�ւ̃|�C���^					
//
//D3DXCOLOR					g_colorFade;				// �t�F�[�h�F
//FADE						g_fade;						// �t�F�[�h���
//MODE						g_modeNext = MODE_TITLE;	// ���[�h
//float						g_fadeRate;					// �t�F�[�h���[�g