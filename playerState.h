#pragma once
//=================================================================================
//	プログラム名	：	
//	処理			：	プレイヤー状態処理[playerState.h]
//	メイン処理		：	main.cpp
//	作成者			：	moriya gaku
//=================================================================================

//=================================================================================
//	二重定義防止
//=================================================================================
#ifndef _PLAYER_STATE_H_
#define _PLAYER_STATE_H_

//=================================================================================
//	インクルードファイル
//=================================================================================
#include "main.h"

class CState
{
public:
	CState() : m_ClassName("") {}
	virtual ~CState() {}
	virtual void Update(void) = 0;
	CPlayer *GetInfo(void);
	std::string m_ClassName; // 派生クラス判別用
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

	// 新しい状態へ遷移
	void SetState(CState *pState) {
		delete m_vpState[0];
		m_vpState[0] = pState;
	}

	// ループ無モーションへの状態遷移
	void SetQuickState(CState *pState) {
		if (pState->m_ClassName != m_vpState.back()->m_ClassName) {
			if (m_vpState.size() > 1) {
				BackQuickState();
			}
			m_vpState.push_back(pState);
		}
	}

	// ループ無モーションから直前モーションへの状態遷移
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
	std::vector<CState*> m_vpState;	// 状態(スタック max2要素)
};

#endif