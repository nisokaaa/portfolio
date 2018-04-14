//=================================================================================
//	�v���O������	�F	
//	����			�F	�v���C���[����[player.h]
//	���C������		�F	main.cpp
//	�쐬��			�F	moriya gaku
//=================================================================================

//=================================================================================
//	��d��`�h�~
//=================================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

//=================================================================================
//	�C���N���[�h�t�@�C��
//=================================================================================
#include "main.h"
#include "scene.h"

//=================================================================================
//	�O���錾
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
	
	// �����_�ړ�
	void MoveLookAt( void );

	// �v���C���[�ړ�
	void MovePlayer( void );
	
	// �v���C���[��](���f��)
	void RotatePlayer( void );

	// �p�����[�^�\��
	void ShowParam( void );

	// �L�����N�^�[���[�h
	//void LoadCharacter( const char* str );

private:
	D3DXVECTOR3 m_prevPos;			// �O�t���[�����W
	D3DXVECTOR3 m_Target;			// ���b�N�I��
	D3DXVECTOR3 m_VecDir;			// �v���C���[�̌���
	D3DXVECTOR3 m_CameraLookAtVecDir;	// �v���C���[���J���������_�̃x�N�g��
	D3DXVECTOR3 m_LookAt;			// �����_
	D3DXVECTOR3 m_Up;				// �����
	D3DXVECTOR3 m_StartAt;			// �J�nat�ۑ�
	D3DXVECTOR3 m_EndAt;			// �I��at�ۑ�
	D3DXMATRIX	m_Matrix;			// �p���p���[���h�s��
	D3DXMATRIX	m_MtxRot;			// �p���p��]�s��
	D3DXMATRIX	m_MtxPos;			// �p���p�ړ��s��

	float		m_fYRot;			// Y����]��
	float		m_fXRot;			// X����]��

	float		m_Angle;			// At�p�x
	D3DXVECTOR3	m_Speed;			// �X�s�[�h
	bool		m_bTarget;			// �^�[�Q�b�g���Ă���
	bool		m_bOnGround;		// �n�ʂɂ��Ă���
	float		m_Distance;			// �v���C���[���璍���_�܂ł̋���(�萔)

	float		m_Hp;				// �̗�
	float		m_Mp;				// MP

	MotionActor *m_pModel;			// ���f��(���[�V�����t��)
	CPlayerState *m_pState;			// ���
};
#endif