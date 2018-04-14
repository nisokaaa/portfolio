#pragma once
//=================================================================================
//	プログラム名	：	
//	処理			：	メニュー処理[menu.h]
//	メイン処理		：	main.cpp
//	作成者			：	moriya gaku
//=================================================================================

//=================================================================================
//	二重定義防止
//=================================================================================
#ifndef _MENU_H_
#define _MENU_H_

//=================================================================================
//	インクルードファイル
//=================================================================================
#include "main.h"
#include "scene.h"

//=================================================================================
//	前方宣言
//=================================================================================
class CMenuState;
class CScene2D;

//=================================================================================
//	マクロ定義
//=================================================================================
#define TITLE_MENU_FONT_SIZE	(0.17f)					// メニュー文字サイズ
#define TITLE_MENU_POS_TOP		(700)					// メニュー項目の基点座標
#define TITLE_MENU_OFFSET		(40)					// メニュー項目の間隔
#define TITLE_MENU1_POS			(D3DXVECTOR2(SCREEN_CENTER_X, TITLE_MENU_POS_TOP + TITLE_MENU_OFFSET * 0))
#define TITLE_MENU2_POS			(D3DXVECTOR2(SCREEN_CENTER_X, TITLE_MENU_POS_TOP + TITLE_MENU_OFFSET * 1))
#define TITLE_MENU3_POS			(D3DXVECTOR2(SCREEN_CENTER_X, TITLE_MENU_POS_TOP + TITLE_MENU_OFFSET * 2))

#define GAME_MENU_FONT_SIZE		(0.14f)					// メニュー文字サイズ
#define GAME_MENU_POS_TOP		(SCREEN_CENTER_Y)		// メニュー項目の基点座標
#define GAME_MENU_OFFSET		(65)					// メニュー項目の間隔
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
	CMenuItem *m_pMenu;		// 選択中のメニュー
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