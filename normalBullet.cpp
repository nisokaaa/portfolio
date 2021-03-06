///=================================================================================
//	プログラム名	：	
//	処理			：	通常弾処理[normalBullet.cpp]
//	メイン処理		：	main.cpp
//	作成者			：	moriya gaku
//=================================================================================

//=================================================================================
//	インクルードファイル
//=================================================================================
#include "bullet.h"
#include "reticle.h"

//=================================================================================
// 弾の移動処理
//=================================================================================
CNormalBullet::CNormalBullet()
{
	// プレイヤーからターゲットへの方向ベクトル ＝ 弾の進行方向
	SetVecDir( CReticle::GetVecDir() );
}	
	
//=================================================================================
// 弾の移動処理
//=================================================================================
void CNormalBullet::Move( void )
{
	SetPosition( GetPosition() + GetVecDir() * GetSpeed() );
}