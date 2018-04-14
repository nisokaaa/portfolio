#pragma once
//=================================================================================
//	�v���O������	�F	
//	����			�F	�S�`��I�u�W�F�N�g����[scene.h]
//	���C������		�F	main.cpp
//	�쐬��			�F	moriya gaku
//=================================================================================

//=================================================================================
//	��d��`�h�~
//=================================================================================
#ifndef _SCENE_H_
#define _SCENE_H_

//=================================================================================
//	�C���N���[�h�t�@�C��
//=================================================================================
#include "main.h"

//=================================================================================
//	�}�N����`
//=================================================================================
#define		PRIORITY_MAX		( 8 )						// �D��x���̍ő�l
#define		DEFAULT_PRIORITY	( 3 )						// �f�t�H���g�̗D��x��

//=================================================================================
//	�O���錾
//=================================================================================
class CShader;

class CScene
{
public:
	// �I�u�W�F�N�g�̃^�C�v
	typedef enum
	{
		SCENE_TYPE_NONE = 0,
		SCENE_TYPE_PLAYER,
		SCENE_TYPE_ENEMY,
		SCENE_TYPE_FIELD,
		SCENE_TYPE_HP_GAUGE,
		SCENE_TYPE_MP_GAUGE,
		SCENE_TYPE_GAUGE_BACK,
		SCENE_TYPE_RETICLE,
		SCENE_TYPE_MAX,
	}SCENE_TYPE;

	CScene( int priority = DEFAULT_PRIORITY );								
	virtual ~CScene(){};									

	virtual HRESULT Init( void ){return S_OK;};								
	virtual void Uninit( void ) = 0;										
	virtual void Update( void ) = 0;										
	virtual void Draw( void ) = 0;											

	static void UpdateAll(int nOnlyUpdatePriority = -1);
	static void DrawAll(int nOnlyDrawPriority = -1, int nExcludePriority = -1);
	static void ReleaseAll( void );

	void Release( void );									// �h���N���X���������g��scene���

	void SetPosition( const float x, const float y, const float z ){ m_Position.x = x; m_Position.y= y; m_Position.z = z; }
	void SetPosition( const D3DXVECTOR3 pos ){ m_Position = pos; }
	const D3DXVECTOR3& GetPosition( void ) const { return m_Position; }

	void SetRotation( const float x, const float y, const float z ){ m_Rotation.x = x; m_Rotation.y= y; m_Rotation.z = z; }
	void SetRotation( const D3DXVECTOR3 rot ){ m_Rotation = rot; }
	const D3DXVECTOR3& GetRotation( void ) const { return m_Rotation; }

	void SetType( const SCENE_TYPE type ){ m_Type = type; }													
	const SCENE_TYPE& GetType( void ) const { return m_Type; }

	void SetShader(CShader *pShader) { m_pShader = pShader; }
	CShader* GetShader(void) const { return m_pShader; }

	static CScene *GetList( int priority ){ return m_Top[ priority ]; }
	CScene *GetNextScene( void ){ return m_Next; }

private:
	D3DXVECTOR3 m_Position;									// �ʒu���W
	D3DXVECTOR3 m_Rotation;									// ��]
	
	static CScene *m_Top[ PRIORITY_MAX ];					// ���X�g�g�b�v�|�C���^
	CScene *m_Next;											// �����X�g�ւ̃|�C���^
	SCENE_TYPE m_Type;										// �I�u�W�F�N�g�̃^�C�v
	bool m_Delete;											// ���t���[���ł̍폜�\��t���O
	CShader *m_pShader;										// �g�p����V�F�[�_
};

#endif