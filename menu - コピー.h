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

typedef enum
{
	MENU1 = 0,
	MENU2,
	MENU3,
};
class CMenuItem
{
public:
	CMenuItem():pos(D3DXVECTOR2(0.0f, 0.0f)){}
	virtual ~CMenuItem() {}
	virtual void Init(void) = 0;
	virtual void Update(void);

protected:
	D3DXVECTOR2 pos;
};

class CMenu1 : public CMenuItem
{
public:
	CMenu1() {}
	~CMenu1() {}
	void Init(void) override;
	void Update(void) override;
};

class CMenu2 : public CMenuItem
{
public:
	CMenu2() {}
	~CMenu2() {}
	void Init(void) override;
	void Update(void) override;
};

class CMenu3 : public CMenuItem
{
public:
	CMenu3() {}
	~CMenu3() {}
	void Init(void) override;
	void Update(void) override;
};

class CMenu
{
public:
	CMenu():m_pMenu(new CMenu1()), m_pNextMenu(nullptr){
		m_pSelectUI = nullptr;
		op = -1;
	}
	~CMenu() {}

	void Init(void);
	void Uninit(void);
	void Update(void);

	void CreateMenuItem(std::string key, D3DXVECTOR2 pos, float width, float height, float scl, BOOL mode = false, int priority = DEFAULT_PRIORITY);
	
	CScene2D *GetSelectUI(void) { return m_pSelectUI; }

	CMenuItem* GetMenuItem(void) { return m_pMenu; }

	void SetMenu(void) {
		if (!m_pNextMenu)return;
		m_pMenu = m_pNextMenu;
	}

	void SetNextMenu(CMenuItem *pMenu) {
		m_pNextMenu = pMenu;
		m_pNextMenu->Init();
	}

private:
	CMenuItem *m_pMenu; // 選択中のメニュー
	CMenuItem *m_pNextMenu;
	std::vector<CScene2D*> m_vpItemUI;
	CScene2D *m_pSelectUI;
	int op;
};

#endif