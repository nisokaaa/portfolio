//=================================================================================
//	プログラム名	：	
//	処理			：	ホーミング弾処理[homingBullet.cpp]
//	メイン処理		：	main.cpp
//	作成者			：	moriya gaku
//=================================================================================

//=================================================================================
//	インクルードファイル
//=================================================================================
#include "bullet.h"
#include "reticle.h"
#include "debug.h"

//=================================================================================
// 弾の移動処理
//=================================================================================
CHomingBullet::CHomingBullet()
{
	D3DXQuaternionIdentity( &m_Quaternion );
}	
	
//=================================================================================
// 弾の移動処理
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

	// 弾→敵ベクトル
	D3DXVECTOR3 vecBulletToEnemy;
	CalcPositionToVector( &vecBulletToEnemy, enemyPos, pos );
	D3DXVec3Normalize( &vecBulletToEnemy, &vecBulletToEnemy );


	// ミサイルのレーダー範囲内に敵
	//if( CheckColSphere( enemyPos, pos, 1.0f, 50.0f ) )
	{
		// 角度を計算
		float rad = CalcVecAngle( vecBulletToEnemy, vecDir );

		// 回転軸を計算
		D3DXVECTOR3 vecRot;
		rad < 0 ? D3DXVec3Cross( &vecRot, &vecBulletToEnemy, &vecDir ) : D3DXVec3Cross( &vecRot, &vecDir, &vecBulletToEnemy );

		// 回転量を計算
		D3DXVECTOR3 vecDistance = enemyPos - pos;
		float weight = fabs(D3DXVec3Length( &vecDistance )) * 0.01f;
		rad = rad * 0.0125f / weight;

		// クオータニオン準備
		D3DXQuaternionRotationAxis( &quaternion, &vecRot, rad );
	
		// 回転行列作成
		D3DXMatrixRotationQuaternion( &mtxRot, &quaternion );

		// 回転
		D3DXVec3TransformCoord( &vecDir, &vecDir, &mtxRot);
		
		// ベクトルセット
		SetVecDir( vecDir );
	}

	// 座標セット
	SetPosition( pos + vecDir * GetSpeed() );
}