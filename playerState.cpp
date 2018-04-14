//=================================================================================
//	プログラム名	：	
//	処理			：	プレイヤー状態処理[playerState.cpp]
//	メイン処理		：	main.cpp
//	作成者			：	moriya gaku
//=================================================================================

//=================================================================================
//	インクルードファイル
//=================================================================================
#include "player.h"
#include "playerState.h"
#include "debug.h"
#include "sceneManager.h"
#include "motionActor.h"
#include "motion.h"
#include "manager.h"
#include "input.h"

CPlayer *CState::GetInfo(void) {
	CPlayer *pPlayer = NULL;
	if (CGame *pGame = dynamic_cast<CGame*>(CSceneManager::GetScene())) {
		pPlayer = pGame->GetPlayer();
	}
	return pPlayer;
}

//=================================================================================
//	CNutoralPlayer ニュートラル状態
//=================================================================================
void CNutoralPlayer::Update(void){

	CPlayer * pPlayer = CState::GetInfo();

	// モーション状態セット
	pPlayer->GetMotion()->GetMotionData()->SetMotion(8);
}

//=================================================================================
//	CMovePlayer 移動状態
//=================================================================================
void CMovePlayer::Update(void) {

	CPlayer * pPlayer = CState::GetInfo();

	// モーション状態セット
	pPlayer->GetMotion()->GetMotionData()->SetMotion(1);

	// パッド入力がないとき
	if (fabs((float)CManager::GetInputGamePad()->GetDInput().lX) < 250 && fabs((float)CManager::GetInputGamePad()->GetDInput().lY) < 250)
	{
		// １つ前のモーションへ戻す
		pPlayer->GetState()->SetState(new CNutoralPlayer);
	}
}

//=================================================================================
//	CDashPlayer ダッシュ状態
//=================================================================================
void CDashPlayer::Update(void) {

	CPlayer * pPlayer = CState::GetInfo();

	// モーション状態セット
	pPlayer->GetMotion()->GetMotionData()->SetMotion(2);

	// 現在のモーションが終了したら、１つ前のモーションへ戻す
	bool flg = pPlayer->GetMotion()->GetMotionData()->GetMotionEndFrameFlg();
	if (flg)pPlayer->GetState()->BackQuickState();
}

//=================================================================================
//	CFallPlayer 下降状態
//=================================================================================
void CFallPlayer::Update(void) {

	CPlayer * pPlayer = CState::GetInfo();

	// モーション状態セット
	pPlayer->GetMotion()->GetMotionData()->SetMotion(5);

	// パッド入力がないとき
	if (fabs((float)CManager::GetInputGamePad()->GetDInput().lX) < 250 && fabs((float)CManager::GetInputGamePad()->GetDInput().lY) < 250)
	{
		// １つ前のモーションへ戻す
		pPlayer->GetState()->SetState(new CNutoralPlayer);
	}
}

//=================================================================================
//	CRisePlayer 上昇状態
//=================================================================================
void CRisePlayer::Update(void) {

	CPlayer * pPlayer = CState::GetInfo();

	// モーション状態セット
	pPlayer->GetMotion()->GetMotionData()->SetMotion(6);

	// 現在のモーションが終了したら、１つ前のモーションへ戻す
	bool flg = pPlayer->GetMotion()->GetMotionData()->GetMotionEndFrameFlg();
	if (flg)pPlayer->GetState()->BackQuickState();
}

//=================================================================================
//	CAttackPlayer 攻撃状態
//=================================================================================
void CAttackPlayer::Update(void) {
	
	CPlayer * pPlayer = CState::GetInfo();

	// モーション状態セット
	pPlayer->GetMotion()->GetMotionData()->SetMotion(4);

	// 現在のモーションが終了したら、１つ前のモーションへ戻す
	bool flg = pPlayer->GetMotion()->GetMotionData()->GetMotionEndFrameFlg();
	if (flg)pPlayer->GetState()->BackQuickState();
}

//=================================================================================
//	CGardPlayer ガード状態
//=================================================================================
void CGardPlayer::Update(void) {

	CPlayer * pPlayer = CState::GetInfo();

	// モーション状態セット
	pPlayer->GetMotion()->GetMotionData()->SetMotion(3);

	bool flg = pPlayer->GetMotion()->GetMotionData()->GetMotionEndFrameFlg();
	if (flg)pPlayer->GetState()->BackQuickState();
}

//=================================================================================
//	CPlayerState 更新関数
//=================================================================================
void CPlayerState::Update(void)
{
	m_vpState.back()->Update();
}

//=================================================================================
//	CPlayerState 終了関数
//=================================================================================
void CPlayerState::Uninit(void)
{
	for (int i = 0; i < (int)m_vpState.size(); i++)
	{
		delete m_vpState[i];
		m_vpState[i] = nullptr;
	}
	m_vpState.clear();
}