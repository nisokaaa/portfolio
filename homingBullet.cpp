//=================================================================================
//	�v���O������	�F	
//	����			�F	�z�[�~���O�e����[homingBullet.cpp]
//	���C������		�F	main.cpp
//	�쐬��			�F	moriya gaku
//=================================================================================

//=================================================================================
//	�C���N���[�h�t�@�C��
//=================================================================================
#include "bullet.h"
#include "reticle.h"
#include "debug.h"

//=================================================================================
// �e�̈ړ�����
//=================================================================================
CHomingBullet::CHomingBullet()
{
	D3DXQuaternionIdentity( &m_Quaternion );
}	
	
//=================================================================================
// �e�̈ړ�����
//=================================================================================
void CHomingBullet::Move( void )
{
	D3DXMATRIX mtxRot;
	D3DXMatrixIdentity( &mtxRot );
	D3DXQUATERNION quaternion;
	D3DXQuaternionIdentity( &quaternion );

	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 enemyPos = CReticle::Get3DPosition();
	D3DXVECTOR3 vecDir = GetVecDir();
	D3DXVec3Normalize( &vecDir, &vecDir );

	// �e���G�x�N�g��
	D3DXVECTOR3 vecBulletToEnemy;
	CalcPositionToVector( &vecBulletToEnemy, enemyPos, pos );
	D3DXVec3Normalize( &vecBulletToEnemy, &vecBulletToEnemy );


	// �~�T�C���̃��[�_�[�͈͓��ɓG
	//if( CheckColSphere( enemyPos, pos, 1.0f, 50.0f ) )
	{
		// �p�x���v�Z
		float rad = CalcVecAngle( vecBulletToEnemy, vecDir );

		// ��]�����v�Z
		D3DXVECTOR3 vecRot;
		rad < 0 ? D3DXVec3Cross( &vecRot, &vecBulletToEnemy, &vecDir ) : D3DXVec3Cross( &vecRot, &vecDir, &vecBulletToEnemy );

		// ��]�ʂ��v�Z
		D3DXVECTOR3 vecDistance = enemyPos - pos;
		float weight = fabs(D3DXVec3Length( &vecDistance )) * 0.01f;
		rad = rad * 0.0125f / weight;

		// �N�I�[�^�j�I������
		D3DXQuaternionRotationAxis( &quaternion, &vecRot, rad );
	
		// ��]�s��쐬
		D3DXMatrixRotationQuaternion( &mtxRot, &quaternion );

		// ��]
		D3DXVec3TransformCoord( &vecDir, &vecDir, &mtxRot);
		
		// �x�N�g���Z�b�g
		SetVecDir( vecDir );
	}

	// ���W�Z�b�g
	SetPosition( pos + vecDir * GetSpeed() );
}