#pragma once
//=================================================================================
//	�v���O������	�F	
//	����			�F	�v���C���[��ԏ���[playerState.h]
//	���C������		�F	main.cpp
//	�쐬��			�F	moriya gaku
//=================================================================================

//=================================================================================
//	��d��`�h�~
//=================================================================================
#ifndef _PLAYER_STATE_H_
#define _PLAYER_STATE_H_

//=================================================================================
//	�C���N���[�h�t�@�C��
//=================================================================================
#include "main.h"

class CState
{
public:
	CState() : m_ClassName("") {}
	virtual ~CState() {}
	virtual void Update(void) = 0;
	CPlayer *GetInfo(void);
	std::string m_ClassName; // �h���N���X���ʗp
};

class CNutoralPlayer : public CState
{
public:
	CNutoralPlayer() {
		const type_info& info = typeid(this);
		m_ClassName = info.name();
	}
	void Update(void);
};

class CMovePlayer : public CState
{
public:
	CMovePlayer() {
		const type_info& info = typeid(this);
		m_ClassName = info.name();
	}
	void Update(void);
};

class CDashPlayer : public CState
{
public:
	CDashPlayer() {
		const type_info& info = typeid(this);
		m_ClassName = info.name();
	}
	void Update(void);
};

class CFallPlayer : public CState
{
public:
	CFallPlayer() {
		const type_info& info = typeid(this);
		m_ClassName = info.name();
	}
	void Update(void);
};

class CRisePlayer : public CState
{
public:
	CRisePlayer() {
		const type_info& info = typeid(this);
		m_ClassName = info.name();
	}
	void Update(void);
};

class CAttackPlayer : public CState
{
public:
	CAttackPlayer() {
		const type_info& info = typeid(this);
		m_ClassName = info.name();
	}
	void Update(void);
};

class CGardPlayer : public CState
{
public:
	CGardPlayer() {
		const type_info& info = typeid(this);
		m_ClassName = info.name();
	}
	void Update(void);
};

class CPlayerState
{
public:
	CPlayerState(){
		m_vpState.push_back(new CNutoralPlayer());
	}

	// �V������Ԃ֑J��
	void SetState(CState *pState) {
		delete m_vpState[0];
		m_vpState[0] = pState;
	}

	// ���[�v�����[�V�����ւ̏�ԑJ��
	void SetQuickState(CState *pState) {
		if (pState->m_ClassName != m_vpState.back()->m_ClassName) {
			if (m_vpState.size() > 1) {
				BackQuickState();
			}
			m_vpState.push_back(pState);
		}
	}

	// ���[�v�����[�V�������璼�O���[�V�����ւ̏�ԑJ��
	void BackQuickState(void) {
		if (m_vpState.size() > 1){
			delete m_vpState.back();
			m_vpState.back() = nullptr;
			m_vpState.pop_back();
		}
	}

	void Update(void);

	void Uninit(void);

private:
	std::vector<CState*> m_vpState;	// ���(�X�^�b�N max2�v�f)
};

#endif