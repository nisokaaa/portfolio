#pragma once
//=================================================================================
//	�v���O������	�F	
//	����			�F	2D�`��I�u�W�F�N�g����[scene2D.h]
//	���C������		�F	main.cpp
//	�쐬��			�F	moriya gaku
//=================================================================================

//=================================================================================
//	��d��`�h�~
//=================================================================================
#ifndef _SCENE2D_H_
#define _SCENE2D_H_

//=================================================================================
//	�C���N���[�h�t�@�C��
//=================================================================================
#include "scene.h"
#include "textureManager.h"

class CScene2D : public CScene
{
public:
	CScene2D( int priority = DEFAULT_PRIORITY, BOOL mode = false)
		: CScene( priority ), m_Key(""), m_pVtxBuff(nullptr),
		m_Length(0.0f), m_Angle(0.0f), m_Width(0.0f),
		m_Height(0.0f), m_TexU(0.0f), m_TexV(0.0f),
		m_TexWidth(0.0f), m_TexHeight(0.0f), m_Scale(1.0f), m_bDrawType(false){}
	
	~CScene2D(){}

	HRESULT Init( void );
	void Uninit( void );
	void Update( void );
	void Draw( void );

	static CScene2D *Create(std::string key, D3DXVECTOR2 pos, float width, float height, float scl, BOOL mode = false, int priority = DEFAULT_PRIORITY);

	virtual VERTEX_2D* SetVertex( VERTEX_2D *pVtx );

	void SetVertexBuffer( const LPDIRECT3DVERTEXBUFFER9 pVtxBuff ){
		m_pVtxBuff = pVtxBuff; }
	const LPDIRECT3DVERTEXBUFFER9& GetVertexBuffer( void ) const { return m_pVtxBuff; }

	void SetWidth( const float width ){ m_Width = width; }
	const float& GetWidth( void ) const { return m_Width; }

	void SetHeight( const float height ){ m_Height = height; }
	const float& GetHeight( void ) const { return m_Height; }

	void SetTexWidth( const float texWidth ){ m_TexWidth = texWidth; }
	const float& GetTexWidth( void ) const { return m_TexWidth; }

	void SetTexHeight( const float texHeight ){ m_TexHeight = texHeight; }
	const float& GetTexHeight( void ) const { return m_TexHeight; }
	
	void SetTexU( const float texU ){ m_TexU = texU; }
	const float& GetTexU( void ) const { return m_TexU; }

	void SetTexV( const float texV ){ m_TexV = texV; }
	const float& GetTexV( void ) const { return m_TexV; }

	void SetTexture( const string key ){ 
		if( FAILED( CTextureManager::Load( key ) ) )return;
		SetKey( key ); }

	void SetKey( const string key ){ m_Key = key; }
	const string& GetKey( void ) const { return m_Key; }

	void SetColor( const D3DXCOLOR color ){ m_Color = color; }
	const D3DXCOLOR& GetColor( void ) const { return m_Color; }

	void SetAlpha(const float a) { m_Color.a = a; }
	const float& GetAlpha(void) const { return m_Color.a; }

	void SetLength( const float length ){ m_Length = length; }
	const float& GetLength( void ) const { return m_Length; }

	void SetAngle( const float angle ){ m_Angle = angle; }
	const float& GetAngle( void ) const { return m_Angle; }

	void SetScale( const float scl ){ m_Scale = scl; }
	const float& GetScale( void ) const { return m_Scale; }

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;				// ���_�o�b�t�@�ւ̃|�C���^
	string m_Key;									// �e�N�X�`���|�C���^�����L�[
	float m_Length;									// �|���S���̑Ίp���̒���
	float m_Angle;									// �|���S���̑Ίp���̊p�x
	float m_Width;									// ��
	float m_Height;									// ����	
	float m_TexU;									// U�l
	float m_TexV;									// V�l
	float m_TexWidth;								// �e�N�X�`����
	float m_TexHeight;								// �e�N�X�`������
	D3DXCOLOR m_Color;								// �F
	float m_Scale;									// �X�P�[��
	BOOL m_bDrawType;								// �`��^�C�v(���S���W�܂��͍�����W����_)
};

#endif


