#pragma once
//=================================================================================
//	�v���O������	�F	
//	����			�F	���j���[����[menu.h]
//	���C������		�F	main.cpp
//	�쐬��			�F	moriya gaku
//=================================================================================

//=================================================================================
//	��d��`�h�~
//=================================================================================
#ifndef _MENU_H_
#define _MENU_H_

//=================================================================================
//	�C���N���[�h�t�@�C��
//=================================================================================
#include "main.h"
#include "scene.h"

//=================================================================================
//	�O���錾
//=================================================================================
class CMenuState;
class CScene2D;

//=================================================================================
//	�}�N����`
//=================================================================================
#define TITLE_MENU_FONT_SIZE	(0.17f)					// ���j���[�����T�C�Y
#define TITLE_MENU_POS_TOP		(700)					// ���j���[���ڂ̊�_���W
#define TITLE_MENU_OFFSET		(40)					// ���j���[���ڂ̊Ԋu
#define TITLE_MENU1_POS			(D3DXVECTOR2(SCREEN_CENTER_X, TITLE_MENU_POS_TOP + TITLE_MENU_OFFSET * 0))
#define TITLE_MENU2_POS			(D3DXVECTOR2(SCREEN_CENTER_X, TITLE_MENU_POS_TOP + TITLE_MENU_OFFSET * 1))
#define TITLE_MENU3_POS			(D3DXVECTOR2(SCREEN_CENTER_X, TITLE_MENU_POS_TOP + TITLE_MENU_OFFSET * 2))

#define GAME_MENU_FONT_SIZE		(0.14f)					// ���j���[�����T�C�Y
#define GAME_MENU_POS_TOP		(SCREEN_CENTER_Y)		// ���j���[���ڂ̊�_���W
#define GAME_MENU_OFFSET		(65)					// ���j���[���ڂ̊Ԋu
#define GAME_MENU1_POS			(D3DXVECTOR2(SCREEN_CENTER_X, GAME_MENU_POS_TOP + GAME_MENU_OFFSET * 0))
#define GAME_MENU2_POS			(D3DXVECTOR2(SCREEN_CENTER_X, GAME_MENU_POS_TOP + GAME_MENU_OFFSET * 1))
#define GAME_MENU3_POS			(D3DXVECTOR2(SCREEN_CENTER_X, GAME_MENU_POS_TOP + GAME_MENU_OFFSET * 2))

class CMenuItem
{
public:
	CMenuItem():pos(D3DXVECTOR2(0.0f, 0.0f)){}
	virtual ~CMenuItem() {}
	virtual void Init(void) = 0;
	virtual void Update(void) = 0;

	D3DXVECTOR2 pos;
};

class CMenu
{
public:
	CMenu(CMenuItem *p):m_pMenu(p), m_pNextMenu(nullptr){
		m_pPrevMenu = m_pMenu;
		m_pSelectUI = nullptr;
		op = -1;
		move = 0.0f;
		offset = 0.0f;
		interval = 0.0f;
		time = 0;
	}
	~CMenu() {}

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void CreateMenuItem(std::string key, D3DXVECTOR2 pos, float width, float height, float scl, BOOL mode = false, int priority = DEFAULT_PRIORITY);
	
	void SetMenu(void) {
		if (!m_pNextMenu)return;
		m_pMenu = m_pNextMenu;
	}

	void SetNextMenu(CMenuItem *pMenu);

	std::vector<CScene2D*> m_vpItemUI;
	CScene2D *m_pSelectUI;

	int op;
	float move;
	float offset;
	float interval;
	int time;

private:
	CMenuItem *m_pMenu;		// �I�𒆂̃��j���[
	CMenuItem *m_pNextMenu;
	CMenuItem *m_pPrevMenu;
};

class CTitleMenu1 : public CMenuItem
{
public:
	CTitleMenu1() {}
	~CTitleMenu1() {}
	void Init(void) override;
	void Update(void) override;
};

class CTitleMenu2 : public CMenuItem
{
public:
	CTitleMenu2() {}
	~CTitleMenu2() {}
	void Init(void) override;
	void Update(void) override;
};

class CTitleMenu3 : public CMenuItem
{
public:
	CTitleMenu3() {}
	~CTitleMenu3() {}
	void Init(void) override;
	void Update(void) override;
};

class CGameMenu1 : public CMenuItem
{
public:
	CGameMenu1() {}
	~CGameMenu1() {}
	void Init(void) override;
	void Update(void) override;
};

class CGameMenu2 : public CMenuItem
{
public:
	CGameMenu2() {}
	~CGameMenu2() {}
	void Init(void) override;
	void Update(void) override;
};

class CGameMenu3 : public CMenuItem
{
public:
	CGameMenu3() {}
	~CGameMenu3() {}
	void Init(void) override;
	void Update(void) override;
};

#endif