#pragma once
//=================================================================================
//	�v���O������	�F	
//	����			�F	3D�`��I�u�W�F�N�g����[scene3D.h]
//	���C������		�F	main.cpp
//	�쐬��			�F	moriya gaku
//=================================================================================

//=================================================================================
//	��d��`�h�~
//=================================================================================
#ifndef _SCENE3D_H_
#define _SCENE3D_H_

//=================================================================================
//	�C���N���[�h�t�@�C��
//=================================================================================
#include "main.h"
#include "scene.h"
#include "textureManager.h"

class CScene3D : public CScene
{
public:
	CScene3D( int priority = DEFAULT_PRIORITY );
	~CScene3D();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetScale( const D3DXVECTOR3 scale ){ m_Scale = scale; }
	const D3DXVECTOR3& GetScale( void ) const { return m_Scale; }

	void SetColor( const D3DXCOLOR color ){ m_Color = color; }
	const D3DXCOLOR& GetColor( void ) const { return m_Color; }

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

	void SetVertexBuffer( const LPDIRECT3DVERTEXBUFFER9 pVtxBuffer ){
		m_pVtxBuffer = pVtxBuffer; }
	const LPDIRECT3DVERTEXBUFFER9& GetVertexBuffer( void ) const { return m_pVtxBuffer; }

	void SetTexture( const string key ){ 
		if( FAILED( CTextureManager::Load( key ) ) )return;
		SetKey( key ); }

	void SetKey( const string key ){ m_Key = key; }
	const string& GetKey( void ) const { return m_Key; }

private:
	D3DXMATRIX m_mtxWorld;
	D3DXVECTOR3 m_Scale;					// �傫��	
	D3DXCOLOR m_Color;						// �F
	float m_Width;							// ��
	float m_Height;							// ����

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffer;	// ���_�o�b�t�@�ւ̃|�C���^
	string m_Key;							// �e�N�X�`���|�C���^�����L�[(�Œ�V�F�[�_�[�p)
	float m_Length;							// �|���S���̑Ίp���̒���
	float m_Angle;							// �|���S���̑Ίp���̊p�x	
	float m_TexU;							// U�l
	float m_TexV;							// V�l
	float m_TexWidth;						// �e�N�X�`����
	float m_TexHeight;						// �e�N�X�`������

	virtual HRESULT MakeVertexBuffer( void );
	virtual void SetVertex( void );
};

#endif
