#pragma once
//=================================================================================
//	プログラム名	：	
//	処理			：	弾処理[bullet.h]
//	メイン処理		：	main.cpp
//	作成者			：	moriya gaku
//=================================================================================

//=================================================================================
//	二重定義防止
//=================================================================================
#ifndef _BULLET_H_
#define _BULLET_H_

//=================================================================================
//	インクルードファイル
//=================================================================================
#include "main.h"
#include "billboard.h"

class CEnemy;

class CBullet : public CBillboard
{
public:
	CBullet() 
		: m_VecDir(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
		m_Speed(0.0f),
		m_Attack(0.0f),
		m_Life(0){}									
	
	~CBullet(){};									
	
	HRESULT Init( void );						
	void Uninit( void );						
	void Update( void );						
	void Draw( void );

	// 弾の移動
	virtual void Move( void ){};

	void SetVecDir( const D3DXVECTOR3 at ){ m_VecDir = at; }
	const D3DXVECTOR3& GetVecDir( void ) const { return m_VecDir; }

	void SetSpeed( const float speed ){ m_Speed = speed; }
	const float& GetSpeed( void ) const { return m_Speed; }

	void SetAttack( const float attack ){ m_Attack = attack; }
	const float& GetAttack( void ) const { return m_Attack; }

	void SetLife( const int life ){ m_Life = life; }
	const int& GetLife( void ) const { return m_Life; }

private:
	D3DXVECTOR3 m_VecDir;	// 方向
	float m_Speed;			// 速度
	float m_Attack;			// 攻撃力
	int m_Life;				// 寿命
};

class CNormalBullet : public CBullet
{
public:
	CNormalBullet();
	~CNormalBullet(){};	
	void Move( void );
};

class CHomingBullet : public CBullet
{
public:
	CHomingBullet();
	~CHomingBullet(){};	
	void Move( void );

private:
	D3DXQUATERNION m_Quaternion;
};

#endif