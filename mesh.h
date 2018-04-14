#pragma once
//=================================================================================
//	�v���O������	�F	
//	����			�F	���b�V������[mesh.h]
//	���C������		�F	main.cpp
//	�쐬��			�F	moriya gaku
//=================================================================================

//=================================================================================
//	��d��`�h�~
//=================================================================================
#ifndef _MESH_H_
#define _MESH_H_

//=================================================================================
//	�C���N���[�h�t�@�C��
//=================================================================================
#include "main.h"
#include "scene3D.h"

class CMesh : public CScene3D
{
public:
	CMesh( int priority = DEFAULT_PRIORITY );
	~CMesh(){};

	HRESULT Init( void );												
	void Uninit( void );												
	void Update( void );												
	void Draw( void );													

	void MakeIndexBuffer( void );								// �C���f�b�N�X�o�b�t�@�쐬
	virtual void SetMatrix(void) {};

	void SetIndexBuffer( const LPDIRECT3DINDEXBUFFER9 pIdxBuffer ){ m_pIdxBuffer =  pIdxBuffer; }
	const LPDIRECT3DINDEXBUFFER9& GetIndexBuffer( void ) const { return m_pIdxBuffer; }

	void SetNumWidth( const int numWidth ){ m_NumWidth = numWidth; }
	const int& GetNumWidth( void ) const { return m_NumWidth; }

	void SetNumHeight( const int numHeight ){ m_NumHeight = numHeight; }
	const int& GetNumHeight( void ) const { return m_NumHeight; }

	void SetNumPolygon( const int numPolygon ){ m_NumPolygon = numPolygon; }
	const int& GetNumPolygon( void ) const { return m_NumPolygon; }

	void SetNumVertex( const int numVertex ){ m_NumVertex = numVertex; }
	const int& GetNumVertex( void ) const { return m_NumVertex; }

private:
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuffer;						// �C���f�b�N�X�o�b�t�@
	int m_NumWidth;												// ���u���b�N��
	int m_NumHeight;											// �c�u���b�N��
	int m_NumPolygon;											// ���|���S����
	int m_NumVertex;											// �����_��
};

#endif