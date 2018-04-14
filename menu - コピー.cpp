//=================================================================================
//	プログラム名	：	
//	処理			：	メニュー処理[menu.cpp]
//	メイン処理		：	main.cpp
//	作成者			：	moriya gaku
//=================================================================================

//=================================================================================
//	インクルードファイル
//=================================================================================
#include "menu.h"
#include "scene.h"
#include "scene2D.h"
#include "input.h"
#include "manager.h"
#include "sceneManager.h"
#include "debug.h"
#include "fade.h"

//=================================================================================
//　CMenuItem
//=================================================================================
void CMenuItem::Update(void)
{
	CTitle *pTitle = dynamic_cast<CTitle*>(CSceneManager::GetScene());
	CMenu *pMenu = pTitle->GetMenu();
	pMenu->GetSelectUI()->SetPosition(D3DXVECTOR3(pos.x, pos.y, 0.0f));
}

//=================================================================================
//　CMenu1メニュー項目の関数
//=================================================================================
void CMenu1::Init(void)
{
	pos = D3DXVECTOR2(SCREEN_CENTER_X, SCREEN_HEIGHT - 400.0f);
}
void CMenu1::Update(void)
{
	CMenuItem::Update();

	CTitle *pTitle = dynamic_cast<CTitle*>(CSceneManager::GetScene());
	CMenu *pMenu = pTitle->GetMenu();

	CInputGamePad *pPad = CManager::GetInputGamePad();
	if (pPad->GetStickTrigger() && pPad->CheckInputLStickUp())
		pMenu->SetNextMenu(new CMenu3);
	if (pPad->GetStickTrigger() && pPad->CheckInputLStickDown())
		pMenu->SetNextMenu(new CMenu2);

	if (pPad->GetPress(DIJOFS_BUTTON0) || pPad->GetPress(DIJOFS_BUTTON1) || pPad->GetPress(DIJOFS_BUTTON2) || pPad->GetPress(DIJOFS_BUTTON3) )
	{
		CFade::SetFade( CFade::FADE_OUT, new CGame, 0.5f );
	}
}

//=================================================================================
//　CMenu2 メニュー項目の関数
//=================================================================================
void CMenu2::Init(void)
{ 
	pos = D3DXVECTOR2(SCREEN_CENTER_X, SCREEN_HEIGHT - 350.0f);
}
void CMenu2::Update(void)
{
	CMenuItem::Update();

	CTitle *pTitle = dynamic_cast<CTitle*>(CSceneManager::GetScene());
	CMenu *pMenu = pTitle->GetMenu();

	CInputGamePad *pPad = CManager::GetInputGamePad();
	if (pPad->GetStickTrigger() && pPad->CheckInputLStickUp())
		pMenu->SetNextMenu(new CMenu1);
	if (pPad->GetStickTrigger() && pPad->CheckInputLStickDown())
		pMenu->SetNextMenu(new CMenu3);
}

//=================================================================================
//　CMenu3 メニュー項目の関数
//=================================================================================
void CMenu3::Init(void)
{
	pos = D3DXVECTOR2(SCREEN_CENTER_X, SCREEN_HEIGHT - 300.0f);
}
void CMenu3::Update(void)
{
	CMenuItem::Update();

	CTitle *pTitle = dynamic_cast<CTitle*>(CSceneManager::GetScene());
	CMenu *pMenu = pTitle->GetMenu();

	CInputGamePad *pPad = CManager::GetInputGamePad();
	if (pPad->GetStickTrigger() && pPad->CheckInputLStickUp())
		pMenu->SetNextMenu(new CMenu2);
	if (pPad->GetStickTrigger() && pPad->CheckInputLStickDown())
		pMenu->SetNextMenu(new CMenu1);
}

//=================================================================================
//　初期化関数
//=================================================================================
void CMenu::Init(void)
{
	m_pMenu->Init();

	m_pSelectUI = CScene2D::Create("data/TEXTURE/no_material.png", D3DXVECTOR2(SCREEN_CENTER_X, SCREEN_CENTER_Y), 1565.0f, 93.0f, 0.35f, TRUE);
	m_pSelectUI->SetColor(D3DXCOLOR(0.8f, 0.8f, 0.8f, 0.0f));
}

//=================================================================================
//　終了関数
//=================================================================================
void CMenu::Uninit(void)
{
	for (int i = 0; i < (int)m_vpItemUI.size(); i++)
		m_vpItemUI[i]->Uninit();
}

//=================================================================================
//　更新関数
//=================================================================================
void CMenu::Update(void)
{
	CTitle *pTitle = dynamic_cast<CTitle*>(CSceneManager::GetScene());

	// メニュー項目
	m_pMenu->Update();

	// 次の状態へ切り替える
	SetMenu();

	float time = pTitle->GetTime();
	if ((pTitle->GetTime() % 100) == 0) op *= -1;
	D3DXCOLOR color = m_pSelectUI->GetColor();
	color.a += op * 0.01f;
	if (color.a > 1.0f)color.a = 1.0f;
	if (color.a < 0.0f)color.a = 0.0f;
	m_pSelectUI->SetColor(color);

	for (int i = 0; i < (int)m_vpItemUI.size(); i++) {
		D3DXCOLOR color = m_vpItemUI[i]->GetColor();
		color.a = 0.5f;
		m_vpItemUI[i]->SetColor(color);
	}
	if (dynamic_cast<CMenu1*>(m_pMenu))
	{
		D3DXCOLOR color = m_vpItemUI[0]->GetColor();
		color.a = 1.0f;
		m_vpItemUI[0]->SetColor(color);
	}
	if (dynamic_cast<CMenu2*>(m_pMenu))
	{
		D3DXCOLOR color = m_vpItemUI[1]->GetColor();
		color.a = 1.0f;
		m_vpItemUI[1]->SetColor(color);
	}
	if (dynamic_cast<CMenu3*>(m_pMenu))
	{
		D3DXCOLOR color = m_vpItemUI[2]->GetColor();
		color.a = 1.0f;
		m_vpItemUI[2]->SetColor(color);
	}
}

//=================================================================================
//　メニュー項目生成関数
//=================================================================================
void CMenu::CreateMenuItem(std::string key, D3DXVECTOR2 pos, float width, float height, float scl, BOOL mode, int priority)
{
	m_vpItemUI.push_back(CScene2D::Create(key, pos, width, height, scl, mode, priority));
}