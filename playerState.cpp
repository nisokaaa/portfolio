//=================================================================================
//	�v���O������	�F	
//	����			�F	�v���C���[��ԏ���[playerState.cpp]
//	���C������		�F	main.cpp
//	�쐬��			�F	moriya gaku
//=================================================================================

//=================================================================================
//	�C���N���[�h�t�@�C��
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
//	CNutoralPlayer �j���[�g�������
//=================================================================================
void CNutoralPlayer::Update(void){

	CPlayer * pPlayer = CState::GetInfo();

	// ���[�V������ԃZ�b�g
	pPlayer->GetMotion()->GetMotionData()->SetMotion(8);
}

//=================================================================================
//	CMovePlayer �ړ����
//=================================================================================
void CMovePlayer::Update(void) {

	CPlayer * pPlayer = CState::GetInfo();

	// ���[�V������ԃZ�b�g
	pPlayer->GetMotion()->GetMotionData()->SetMotion(1);

	// �p�b�h���͂��Ȃ��Ƃ�
	if (fabs((float)CManager::GetInputGamePad()->GetDInput().lX) < 250 && fabs((float)CManager::GetInputGamePad()->GetDInput().lY) < 250)
	{
		// �P�O�̃��[�V�����֖߂�
		pPlayer->GetState()->SetState(new CNutoralPlayer);
	}
}

//=================================================================================
//	CDashPlayer �_�b�V�����
//=================================================================================
void CDashPlayer::Update(void) {

	CPlayer * pPlayer = CState::GetInfo();

	// ���[�V������ԃZ�b�g
	pPlayer->GetMotion()->GetMotionData()->SetMotion(2);

	// ���݂̃��[�V�������I��������A�P�O�̃��[�V�����֖߂�
	bool flg = pPlayer->GetMotion()->GetMotionData()->GetMotionEndFrameFlg();
	if (flg)pPlayer->GetState()->BackQuickState();
}

//=================================================================================
//	CFallPlayer ���~���
//=================================================================================
void CFallPlayer::Update(void) {

	CPlayer * pPlayer = CState::GetInfo();

	// ���[�V������ԃZ�b�g
	pPlayer->GetMotion()->GetMotionData()->SetMotion(5);

	// �p�b�h���͂��Ȃ��Ƃ�
	if (fabs((float)CManager::GetInputGamePad()->GetDInput().lX) < 250 && fabs((float)CManager::GetInputGamePad()->GetDInput().lY) < 250)
	{
		// �P�O�̃��[�V�����֖߂�
		pPlayer->GetState()->SetState(new CNutoralPlayer);
	}
}

//=================================================================================
//	CRisePlayer �㏸���
//=================================================================================
void CRisePlayer::Update(void) {

	CPlayer * pPlayer = CState::GetInfo();

	// ���[�V������ԃZ�b�g
	pPlayer->GetMotion()->GetMotionData()->SetMotion(6);

	// ���݂̃��[�V�������I��������A�P�O�̃��[�V�����֖߂�
	bool flg = pPlayer->GetMotion()->GetMotionData()->GetMotionEndFrameFlg();
	if (flg)pPlayer->GetState()->BackQuickState();
}

//=================================================================================
//	CAttackPlayer �U�����
//=================================================================================
void CAttackPlayer::Update(void) {
	
	CPlayer * pPlayer = CState::GetInfo();

	// ���[�V������ԃZ�b�g
	pPlayer->GetMotion()->GetMotionData()->SetMotion(4);

	// ���݂̃��[�V�������I��������A�P�O�̃��[�V�����֖߂�
	bool flg = pPlayer->GetMotion()->GetMotionData()->GetMotionEndFrameFlg();
	if (flg)pPlayer->GetState()->BackQuickState();
}

//=================================================================================
//	CGardPlayer �K�[�h���
//=================================================================================
void CGardPlayer::Update(void) {

	CPlayer * pPlayer = CState::GetInfo();

	// ���[�V������ԃZ�b�g
	pPlayer->GetMotion()->GetMotionData()->SetMotion(3);

	bool flg = pPlayer->GetMotion()->GetMotionData()->GetMotionEndFrameFlg();
	if (flg)pPlayer->GetState()->BackQuickState();
}

//=================================================================================
//	CPlayerState �X�V�֐�
//=================================================================================
void CPlayerState::Update(void)
{
	m_vpState.back()->Update();
}

//=================================================================================
//	CPlayerState �I���֐�
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