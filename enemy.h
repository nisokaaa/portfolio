#pragma once
//=================================================================================
//	�v���O������	�F	
//	����			�F	�G����[enemy.h]
//	���C������		�F	main.cpp
//	�쐬��			�F	moriya gaku
//=================================================================================

//=================================================================================
//	��d��`�h�~
//=================================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

//=================================================================================
//	�C���N���[�h�t�@�C��
//=================================================================================
#include "main.h"
#include "scenex.h"
#include "scene2D.h"
#include "gauge.h"

class CEnemy : public CSceneX
{
public:
	typedef enum
	{
		NONE = 0,			// ����
		NEUTRAL,			// �j���[�g����
		ATTACK,				// �U���Ԑ�
		CHASE,				// �ǐՒ�
		DETH,				// ����
	}STATE;

	CEnemy( int priority ) 
	: CSceneX( priority ),
	m_Speed(0.0f),
	m_State(NONE),
	m_VecDir(D3DXVECTOR3( 0.0f, 0.0f, 0.0f )),
	m_Hp(0.0f),
	m_Time(0),
	m_ColLength(0.0f),
	m_pHpGauge(nullptr),
	m_bLockOn(false){}

	~CEnemy(){};

	HRESULT Init( void );
	void Uninit( void );
	void Update( void );
	void Draw( void );

	static void SetCount( const int cnt ){ m_Count = cnt; }
	static const int& GetCount( void ){ return m_Count; }

	void SetSpeed( const float speed ){ m_Speed = speed; }
	const float& GetSpeed( void ) const { return m_Speed; }

	void SetState( const STATE state ){ m_State = state; }
	const STATE& GetState( void ) const { return m_State; }

	void SetVecDir( const D3DXVECTOR3 at ){ m_VecDir = at; }
	const D3DXVECTOR3& GetVecDir( void ) const { return m_VecDir; }

	void SetHp( const float hp ){ m_Hp = hp; }
	const float& GetHp( void ) const { return m_Hp; }

	void SetColLength( const float colLength ){ m_ColLength = colLength; }
	const float& GetColLength( void ) const { return m_ColLength; }

	void SetHpGauge( CGauge* gauge ){ m_pHpGauge = gauge; }
	CGauge* GetHpGauge( void ) const { return m_pHpGauge; }

	void SetLockOnFlg( const bool flg ){ m_bLockOn = flg; }
	const bool& GetLockOnFlg( void ) const { return m_bLockOn; }

private:
	static int	m_Count;		// ����
	STATE		m_State;
	float		m_Speed;
	D3DXVECTOR3 m_VecDir;
	float		m_Hp;
	int			m_Time;			// �G�̐��U�o�ߎ���
	float		m_ColLength;	// �����蔻�苅���a
	CGauge*		m_pHpGauge;		// �g�o�Q�[�W
	D3DXMATRIX	m_MtxWorld;
	bool		m_bLockOn;		// ���`�N���ɕߑ�����Ă�
};

#endif