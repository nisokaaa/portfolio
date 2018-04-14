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
#include "sound.h"

//=================================================================================
//　CMenu1メニュー項目の関数
//=================================================================================
void CTitleMenu1::Init(void)
{
	pos = TITLE_MENU1_POS;
}
void CTitleMenu1::Update(void)
{
	CTitle *pTitle = dynamic_cast<CTitle*>(CSceneManager::GetScene());
	CMenu *pMenu = pTitle->GetMenu();

	CInputGamePad *pPad = CManager::GetInputGamePad();
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();

	if (pPad->GetStickTrigger() && pPad->CheckInputLStickUp())
		pMenu->SetNextMenu(new CTitleMenu3);
	if (pPad->GetStickTrigger() && pPad->CheckInputLStickDown())
		pMenu->SetNextMenu(new CTitleMenu2);

	if (pKeyboard->GetKeyTrigger(DIK_RETURN) || pPad->GetPress(DIJOFS_BUTTON0) || pPad->GetPress(DIJOFS_BUTTON1) || pPad->GetPress(DIJOFS_BUTTON2) || pPad->GetPress(DIJOFS_BUTTON3) )
	{
		PlaySound(SOUNC_LABEL_SE_TITLE_ENTER);
		CFade::SetFade( CFade::FADE_OUT, new CGame, 1.5f );
	}

	// 選択時に明るくする
	pMenu->m_vpItemUI[0]->SetAlpha(1.0f);
}

//=================================================================================
//　CMenu2 メニュー項目の関数
//=================================================================================
void CTitleMenu2::Init(void)
{ 
	pos = TITLE_MENU2_POS;
}
void CTitleMenu2::Update(void)
{
	CTitle *pTitle = dynamic_cast<CTitle*>(CSceneManager::GetScene());
	CMenu *pMenu = pTitle->GetMenu();

	CInputGamePad *pPad = CManager::GetInputGamePad();
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();

	if (pPad->GetStickTrigger() && pPad->CheckInputLStickUp())
		pMenu->SetNextMenu(new CTitleMenu1);
	if (pPad->GetStickTrigger() && pPad->CheckInputLStickDown())
		pMenu->SetNextMenu(new CTitleMenu3);

	if (pKeyboard->GetKeyTrigger(DIK_RETURN) || pPad->GetPress(DIJOFS_BUTTON0) || pPad->GetPress(DIJOFS_BUTTON1) || pPad->GetPress(DIJOFS_BUTTON2) || pPad->GetPress(DIJOFS_BUTTON3))
	{
	}

	// 選択時に明るくする
	pMenu->m_vpItemUI[1]->SetAlpha(1.0f);
}

//=================================================================================
//　CMenu3 メニュー項目の関数
//=================================================================================
void CTitleMenu3::Init(void)
{
	pos = TITLE_MENU3_POS;
}
void CTitleMenu3::Update(void)
{
	CTitle *pTitle = dynamic_cast<CTitle*>(CSceneManager::GetScene());
	CMenu *pMenu = pTitle->GetMenu();

	CInputGamePad *pPad = CManager::GetInputGamePad();
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();

	if (pPad->GetStickTrigger() && pPad->CheckInputLStickUp())
		pMenu->SetNextMenu(new CTitleMenu2);
	if (pPad->GetStickTrigger() && pPad->CheckInputLStickDown())
		pMenu->SetNextMenu(new CTitleMenu1);

	if (pKeyboard->GetKeyTrigger(DIK_RETURN) || pPad->GetPress(DIJOFS_BUTTON0) || pPad->GetPress(DIJOFS_BUTTON1) || pPad->GetPress(DIJOFS_BUTTON2) || pPad->GetPress(DIJOFS_BUTTON3))
	{
	}

	// 選択時に明るくする
	pMenu->m_vpItemUI[2]->SetAlpha(1.0f);
}

//=================================================================================
//　CMenu1メニュー項目の関数
//=================================================================================
void CGameMenu1::Init(void)
{
	pos = GAME_MENU1_POS;
}
void CGameMenu1::Update(void)
{
	CGame *pGame = dynamic_cast<CGame*>(CSceneManager::GetScene());
	CMenu *pMenu = pGame->GetMenu();

	CInputGamePad *pPad = CManager::GetInputGamePad();
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();

	if (pPad->GetStickTrigger() && pPad->CheckInputLStickUp())
		pMenu->SetNextMenu(new CGameMenu3);
	if (pPad->GetStickTrigger() && pPad->CheckInputLStickDown())
		pMenu->SetNextMenu(new CGameMenu2);

	if (pKeyboard->GetKeyTrigger(DIK_RETURN) || pPad->GetPress(DIJOFS_BUTTON0) || pPad->GetPress(DIJOFS_BUTTON1) || pPad->GetPress(DIJOFS_BUTTON2) || pPad->GetPress(DIJOFS_BUTTON3))
	{
		// ポーズモードから抜ける
		pGame->SetPauseFlg(FALSE);
	}

	// 選択時に明るくする
	pMenu->m_vpItemUI[0]->SetAlpha(1.0f);
}

//=================================================================================
//　CMenu2 メニュー項目の関数
//=================================================================================
void CGameMenu2::Init(void)
{
	pos = GAME_MENU2_POS;
}
void CGameMenu2::Update(void)
{
	CGame *pGame = dynamic_cast<CGame*>(CSceneManager::GetScene());
	CMenu *pMenu = pGame->GetMenu();

	CInputGamePad *pPad = CManager::GetInputGamePad();
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();

	if (pPad->GetStickTrigger() && pPad->CheckInputLStickUp())
		pMenu->SetNextMenu(new CGameMenu1);
	if (pPad->GetStickTrigger() && pPad->CheckInputLStickDown())
		pMenu->SetNextMenu(new CGameMenu3);

	if (pKeyboard->GetKeyTrigger(DIK_RETURN) || pPad->GetPress(DIJOFS_BUTTON0) || pPad->GetPress(DIJOFS_BUTTON1) || pPad->GetPress(DIJOFS_BUTTON2) || pPad->GetPress(DIJOFS_BUTTON3))
	{
		CFade::SetFade(CFade::FADE_OUT, new CTitle, 1.0f);
	}

	// 選択時に明るくする
	pMenu->m_vpItemUI[1]->SetAlpha(1.0f);
}

//=================================================================================
//　CMenu3 メニュー項目の関数
//=================================================================================
void CGameMenu3::Init(void)
{
	pos = GAME_MENU3_POS;
}
void CGameMenu3::Update(void)
{
	CGame *pGame = dynamic_cast<CGame*>(CSceneManager::GetScene());
	CMenu *pMenu = pGame->GetMenu();

	CInputGamePad *pPad = CManager::GetInputGamePad();
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();

	if (pPad->GetStickTrigger() && pPad->CheckInputLStickUp())
		pMenu->SetNextMenu(new CGameMenu2);
	if (pPad->GetStickTrigger() && pPad->CheckInputLStickDown())
		pMenu->SetNextMenu(new CGameMenu1);

	if (pKeyboard->GetKeyTrigger(DIK_RETURN) || pPad->GetPress(DIJOFS_BUTTON0) || pPad->GetPress(DIJOFS_BUTTON1) || pPad->GetPress(DIJOFS_BUTTON2) || pPad->GetPress(DIJOFS_BUTTON3))
	{
		DestroyWindow(GetActiveWindow());
	}

	// 選択時に明るくする
	pMenu->m_vpItemUI[2]->SetAlpha(1.0f);
}

//=================================================================================
//　初期化関数
//=================================================================================
HRESULT CMenu::Init(void)
{
	m_pMenu->Init();

	m_pSelectUI = CScene2D::Create("data/TEXTURE/select.png", D3DXVECTOR2(0.0f, 0.0f), 1500.0f, 25.0f, 0.55f, TRUE, 7);
	m_pSelectUI->SetColor(D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f));

	return S_OK;
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
	// 点滅
	if ((time % 100) == 0) op *= -1;
	D3DXCOLOR color = m_pSelectUI->GetColor();
	color.a += (float)op * 0.01f;
	if (color.a > 1.0f)color.a = 1.0f;
	if (color.a < 0.0f)color.a = 0.0f;
	m_pSelectUI->SetColor(color);

	// 非選択項目を暗く
	for (int i = 0; i < (int)m_vpItemUI.size(); i++)
		m_vpItemUI[i]->SetAlpha(0.5f);

	// メニュー項目
	m_pMenu->Update();

	// 次の状態へ切り替える
	SetMenu();

	// 移動
	if (interval > 0.0f && interval - offset >= move) move += offset;
	if (interval < 0.0f && interval - offset <= move) move += offset;

	m_pSelectUI->SetPosition(D3DXVECTOR3(m_pMenu->pos.x, m_pPrevMenu->pos.y + move, 0.0f));

	time += 1;
}

//=================================================================================
//　描画関数
//=================================================================================
void CMenu::Draw(void)
{

}

//=================================================================================
//　メニュー項目生成関数
//=================================================================================
void CMenu::CreateMenuItem(std::string key, D3DXVECTOR2 pos, float width, float height, float scl, BOOL mode, int priority)
{
	m_vpItemUI.push_back(CScene2D::Create(key, pos, width, height, scl, mode, priority));
}

//=================================================================================
//　次メニュー項目セット関数
//=================================================================================
void CMenu::SetNextMenu(CMenuItem *pMenu)
{
	m_pNextMenu = pMenu;
	m_pPrevMenu = m_pMenu;
	m_pNextMenu->Init();

	// 選択UIの補間移動量を計算
	move = 0;										// 移動量リセット
	interval = m_pNextMenu->pos.y - m_pMenu->pos.y;	// 総移動量を計算
	offset = interval / (float)15;					// Xフレームかけて移動

	PlaySound(SOUNC_LABEL_SE_TITLE_SELECT);
}