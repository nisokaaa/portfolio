//=================================================================================
//	プログラム名	：	
//	処理			：	プレイヤー処理[player.h]
//	メイン処理		：	main.cpp
//	作成者			：	moriya gaku
//=================================================================================

//=================================================================================
//	二重定義防止
//=================================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

//=================================================================================
//	インクルードファイル
//=================================================================================
#include "main.h"
#include "scene.h"

//=================================================================================
//	前方宣言
//=================================================================================
class MotionActor;
class CPlayerState;

class CPlayer : public CScene
{
public:
	CPlayer( int priority = DEFAULT_PRIORITY) 
		: CScene(priority), 
		m_prevPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
		m_Target(D3DXVECTOR3( 0.0f, 0.0f, 0.0f )),
		m_VecDir(D3DXVECTOR3( 0.0f, 0.0f, 0.0f )),
		m_CameraLookAtVecDir(D3DXVECTOR3( 0.0f, 0.0f, 0.0f )),
		m_LookAt(D3DXVECTOR3( 0.0f, 0.0f, 0.0f )),
		m_Distance(0.0f),
		m_Up(D3DXVECTOR3( 0.0f, 1.0f, 0.0f )),
		m_StartAt(D3DXVECTOR3( 0.0f, 1.0f, 0.0f )),
		m_EndAt(D3DXVECTOR3( 0.0f, 1.0f, 0.0f )),
		m_fYRot(0.0),
		m_fXRot(0.0f),
		m_Angle(0.0f),
		m_Speed(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
		m_bTarget(false),
		m_Hp(0.0f),
		m_Mp(0.0f),
		m_pModel(nullptr),
		m_pState(nullptr)
	{
		D3DXMatrixIdentity( &m_Matrix );
		D3DXMatrixIdentity( &m_MtxRot );
		D3DXMatrixIdentity( &m_MtxPos );
	}

	~CPlayer(){};

	HRESULT Init( string key );
	void Uninit( void );
	void Update( void );
	void Draw( void );

	void SetCameraLookAtVecDir( const D3DXVECTOR3 at ){ m_CameraLookAtVecDir = at; }
	const D3DXVECTOR3& GetCameraLookAtVecDir( void ) const { return m_CameraLookAtVecDir; }

	void SetLookAt( const D3DXVECTOR3 at ){ m_LookAt = at; }
	const D3DXVECTOR3& GetLookAt( void ) const { return m_LookAt; }

	void SetVecDir( const D3DXVECTOR3 at ){ m_VecDir = at; }
	const D3DXVECTOR3& GetVecDir( void ) const { return m_VecDir; }

	void SetSpeed( const float speed ){ m_Speed = D3DXVECTOR3(speed, speed, speed); }
	const float& GetSpeed( void ) const { return m_Speed.x; }

	void SetHp( const float hp ){ m_Hp = hp; }
	const float& GetHp( void ) const { return m_Hp; }

	void SetMp( const float mp ){ m_Mp = mp; }
	const float& GetMp( void ) const { return m_Mp; }

	MotionActor *GetMotion(void) const { return m_pModel; }

	CPlayerState *GetState(void) const { return m_pState; }

	static CPlayer *Create(string key, float hp, float mp, float speed);
	
	// 注視点移動
	void MoveLookAt( void );

	// プレイヤー移動
	void MovePlayer( void );
	
	// プレイヤー回転(モデル)
	void RotatePlayer( void );

	// パラメータ表示
	void ShowParam( void );

	// キャラクターロード
	//void LoadCharacter( const char* str );

private:
	D3DXVECTOR3 m_prevPos;			// 前フレーム座標
	D3DXVECTOR3 m_Target;			// ロックオン
	D3DXVECTOR3 m_VecDir;			// プレイヤーの向き
	D3DXVECTOR3 m_CameraLookAtVecDir;	// プレイヤー→カメラ注視点のベクトル
	D3DXVECTOR3 m_LookAt;			// 注視点
	D3DXVECTOR3 m_Up;				// 上方向
	D3DXVECTOR3 m_StartAt;			// 開始at保存
	D3DXVECTOR3 m_EndAt;			// 終了at保存
	D3DXMATRIX	m_Matrix;			// 姿勢用ワールド行列
	D3DXMATRIX	m_MtxRot;			// 姿勢用回転行列
	D3DXMATRIX	m_MtxPos;			// 姿勢用移動行列

	float		m_fYRot;			// Y軸回転量
	float		m_fXRot;			// X軸回転量

	float		m_Angle;			// At角度
	D3DXVECTOR3	m_Speed;			// スピード
	bool		m_bTarget;			// ターゲットしている
	bool		m_bOnGround;		// 地面についている
	float		m_Distance;			// プレイヤーから注視点までの距離(定数)

	float		m_Hp;				// 体力
	float		m_Mp;				// MP

	MotionActor *m_pModel;			// モデル(モーション付き)
	CPlayerState *m_pState;			// 状態
};
#endif