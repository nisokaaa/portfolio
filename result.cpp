//=================================================================================
//	プログラム名	：	
//	処理			：	リザルトシーン処理[result.cpp]
//	メイン処理		：	main.cpp
//	作成者			：	moriya gaku
//=================================================================================

//=================================================================================
//	インクルードファイル
//=================================================================================
#include "manager.h"
#include "sceneManager.h"
#include "textureManager.h"
#include "cameraManager.h"
#include "lightManager.h"
#include "input.h"
#include "scene.h"
#include "scene2D.h"
#include "scene3D.h"
#include "sceneX.h"
#include "doom.h"
#include "meshField.h"
#include "fade.h"
#include "doom.h"
#include "sound.h"
#include "score.h"
#include "gauge.h"

//=================================================================================
//	マクロ定義
//=================================================================================
// 大枠エリア
#define WRAP_POS_H					(100)
#define WRAP_POS_W					(100)

// 見出しエリア
#define HEAD_POS_H					(WRAP_POS_H)
#define HEAD_POS_W					(WRAP_POS_W)

// メインコンテンツエリア
#define MAIN_POS_H					(HEAD_POS_H + 100)
#define MAIN_POS_W					(HEAD_POS_W)

// フッタコンテンツエリア
#define FOOTER_POS_H				(MAIN_POS_H + 450)
#define FOOTER_POS_W				(MAIN_POS_W)

// 線
#define THIN_LINE_H					(5)								// 細い 線 高さ
#define THIN_LINE_W					(1500)							// 細い 線 幅
#define MIDDIUM_LINE_H				(30)							// 中くらい 線 高さ
#define MIDDIUM_LINE_W				(1500)							// 中くらい 線 幅

// 段間隔
#define STAGE_INTERVAL				(50)							// 段の間隔							

// 見出し
#define HEADER_FONT_SCALE			(0.25f)							// 見出し 文字サイズ幅
#define HEADER_FONT_SIZE_H			(187 * HEADER_FONT_SCALE)		// 見出し 文字サイズ高さ
#define HEADER_FONT_SIZE_W			(1226 * HEADER_FONT_SCALE)		// 見出し 文字サイズ幅

#define DEF_FONT_SCALE				(0.15f)							// その他 文字スケール
#define DEF_FONT_SIZE_H				(100 * DEF_FONT_SCALE)			// その他 文字サイズ 高さ

// その他文字
#define DIFFICULY_FONT_SIZE_W		(1981 * DEF_FONT_SCALE)			// "難易度" 文字サイズ 幅

#define SCORE_FONT_SIZE_W			(931 * DEF_FONT_SCALE)			// "スコア" 文字サイズ 幅

#define CLEAR_TIME_FONT_SIZE_W		(1641 * DEF_FONT_SCALE)			// "クリア時間" 文字サイズ 幅

#define ENEMY_KILLS_FONT_SIZE_W		(1831 * DEF_FONT_SCALE)			// "撃退数" 文字サイズ 幅

#define CONTINUE_FONT_SIZE_W		(1351 * DEF_FONT_SCALE)			// "コンテニュー回数" 文字サイズ 幅

#define RANK_FONT_SIZE_W			(741 * DEF_FONT_SCALE)			// "RANK" 文字サイズ 幅

#define RANK_ALPHA_FONT_SIZE_W		(378)							// "ランク" 文字サイズ 幅
#define RANK_ALPHA_FONT_SIZE_H		(280)							// "ランク" 文字サイズ 高さ

#define PRESS_BUTTON_FONT_SIZE_W	(4071 * DEF_FONT_SCALE)			// "PRESS START BUTTON " 文字サイズ 幅

// 文字周り
#define TEXT_PADDING				(7)								// 文字パディング

//=================================================================================
//	コンストラクタ
//=================================================================================
CResult::CResult()
{
	m_nTime = 0;
	m_pBackGround = NULL;
	op = -1.0f;
	btnAlpha = 0.0f;
	textAlpha = 0.0f;
}

//=================================================================================
//	デストラクタ
//=================================================================================
CResult::~CResult()
{
}

//=================================================================================
//	初期化関数
//=================================================================================
HRESULT CResult::Init( void )
{
	// マネージャ取得
	CCameraManager *pCameraManager = CSceneManager::GetCameraManager();
	CLightManager *pLightManager = CSceneManager::GetLightManager();
	CMeshFieldManager *pMeshFieldManager = CSceneManager::GetMeshFieldManager();
	CDebugObj *pDebugManager = CSceneManager::GetDebugObj();

	// カメラ生成
	pCameraManager->Create(MAIN_CAMERA, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 10.0f, 4.0f);

	// ライト生成
	pLightManager->Create(MAIN_LIGHT, D3DXVECTOR3(0.0f, -0.7f, 0.0f), D3DXVECTOR3(0.0f, 30.0f, -30.0f), COLOR_WHITE, D3DXVECTOR3(0.5f, 0.5f, 0.5f), D3DXVECTOR3(0.3f, 0.3f, 0.3f));

	// 背景
	m_pBackGround = CScene2D::Create("data/TEXTURE/result/bg/test_screen.png", D3DXVECTOR2(SCREEN_CENTER_X, SCREEN_CENTER_Y), 1920.0f, 1080.0f, 1.0f, TRUE, 0);

	// RESULTタイトル
	CScene2D::Create("data/TEXTURE/result/text/result.png", D3DXVECTOR2(HEAD_POS_W, HEAD_POS_H), HEADER_FONT_SIZE_W, HEADER_FONT_SIZE_H, 1.0f, FALSE, 0);

	// 難易度表示
	CScene2D::Create("data/TEXTURE/result/text/normal.png", D3DXVECTOR2(HEAD_POS_W + 1150, HEAD_POS_H + (HEADER_FONT_SIZE_H * 0.5f)), DIFFICULY_FONT_SIZE_W, DEF_FONT_SIZE_H, 1.0f, FALSE, 0);

	// RESULT下bar
	CScene2D::Create("data/TEXTURE/result/bg/bar.png", D3DXVECTOR2(HEAD_POS_W, HEAD_POS_H + HEADER_FONT_SIZE_H + 20), THIN_LINE_W, THIN_LINE_H, 1.0f, FALSE, 0);

	// スコア
	m_vpBoxText.push_back(UI(CScene2D::Create("data/TEXTURE/result/text/score.png", D3DXVECTOR2(MAIN_POS_W + 25 + TEXT_PADDING, MAIN_POS_H + STAGE_INTERVAL + TEXT_PADDING), SCORE_FONT_SIZE_W, DEF_FONT_SIZE_H, 1.0f, FALSE, 0)));

	// クリア時間
	m_vpBoxText.push_back(UI(CScene2D::Create("data/TEXTURE/result/text/clear_time.png", D3DXVECTOR2(MAIN_POS_W + 25 + TEXT_PADDING, MAIN_POS_H + STAGE_INTERVAL * 3 + TEXT_PADDING), CLEAR_TIME_FONT_SIZE_W, DEF_FONT_SIZE_H, 1.0f, FALSE, 0)));

	// 撃退数
	m_vpBoxText.push_back(UI(CScene2D::Create("data/TEXTURE/result/text/enemy_kills.png", D3DXVECTOR2(MAIN_POS_W + 25 + TEXT_PADDING, MAIN_POS_H + STAGE_INTERVAL * 5 + TEXT_PADDING), ENEMY_KILLS_FONT_SIZE_W, DEF_FONT_SIZE_H, 1.0f, FALSE, 0)));

	// コンテニュー回数
	m_vpBoxText.push_back(UI(CScene2D::Create("data/TEXTURE/result/text/continue.png", D3DXVECTOR2(MAIN_POS_W + 25 + TEXT_PADDING, MAIN_POS_H + STAGE_INTERVAL * 7 + TEXT_PADDING), CONTINUE_FONT_SIZE_W, DEF_FONT_SIZE_H, 1.0f, FALSE, 0)));

	int cnt = 1;
	for (int i = 0; i < 4; i++)
	{
		// BAR
		m_vpBoxBar.push_back(UI(CScene2D::Create("data/TEXTURE/result/bg/bar.png", D3DXVECTOR2(MAIN_POS_W, MAIN_POS_H + STAGE_INTERVAL * cnt), MIDDIUM_LINE_W, MIDDIUM_LINE_H, 1.0f, FALSE, 0)));
		
		// ランク(RANK)
		m_vpBoxText.push_back(UI(CScene2D::Create("data/TEXTURE/result/text/rank.png", D3DXVECTOR2(MAIN_POS_W + 1280 + TEXT_PADDING, MAIN_POS_H + STAGE_INTERVAL * cnt + TEXT_PADDING), RANK_FONT_SIZE_W, DEF_FONT_SIZE_H, 1.0f, FALSE, 0)));

		// ランク
		m_vpBoxText.push_back(UI(CScene2D::Create("data/TEXTURE/result/text/rank_b.png", D3DXVECTOR2(MAIN_POS_W + 1450 + TEXT_PADDING, MAIN_POS_H + STAGE_INTERVAL * cnt + MIDDIUM_LINE_H * 0.5f), RANK_ALPHA_FONT_SIZE_W, RANK_ALPHA_FONT_SIZE_H, 0.15f, TRUE, 0)));

		cnt += 2;
	}

	// 結果下bar
	CScene2D::Create("data/TEXTURE/result/bg/bar.png", D3DXVECTOR2(FOOTER_POS_W, FOOTER_POS_H), THIN_LINE_W, THIN_LINE_H, 1.0f, FALSE, 0);

	// ランク(RANK)
	m_vpBoxText.push_back(UI(CScene2D::Create("data/TEXTURE/result/text/rank.png", D3DXVECTOR2(FOOTER_POS_W + 1050 + TEXT_PADDING, FOOTER_POS_H + 150), RANK_FONT_SIZE_W, DEF_FONT_SIZE_H, 1.0f, FALSE, 0)));

	// ランク
	m_vpBoxText.push_back(UI(CScene2D::Create("data/TEXTURE/result/text/rank_c.png", D3DXVECTOR2(FOOTER_POS_W + 1350 + TEXT_PADDING, FOOTER_POS_H + 150), RANK_ALPHA_FONT_SIZE_W, RANK_ALPHA_FONT_SIZE_H, 0.4, TRUE, 0)));

	// PRESS START BUTTON
	m_pButton = CScene2D::Create("data/TEXTURE/result/text/press_start_button.png", D3DXVECTOR2(FOOTER_POS_W, FOOTER_POS_H + 250), PRESS_BUTTON_FONT_SIZE_W, DEF_FONT_SIZE_H, 1.0f, FALSE, 0);

	CFade::SetAlpha(1.0f);
	CFade::SetFade(CFade::FADE_IN, new CResult, 0.5f);

	// SE再生
	//PlaySound(SOUND_LABEL_BGM_RESULT);

	// 透明度を0にしておく
	for (int i = 0; i < (int)m_vpBoxBar.size(); i++)
		m_vpBoxBar[i].ui->SetAlpha(m_vpBoxBar[i].alpha);
	m_vpBoxBar[0].flg = TRUE;

	for (int i = 0; i < (int)m_vpBoxText.size(); i++)
		m_vpBoxText[i].ui->SetAlpha(m_vpBoxText[i].alpha);
	m_vpBoxText[0].flg = TRUE;

	// スコア初期化
	//m_pScore = CScore::Create("data/TEXTURE/number.png", D3DXVECTOR2(380.0f, 380.0f), D3DXVECTOR2(1425.0f, 92.0f), 3);

	// ゲージ初期化
	m_pHpGauge = CGauge::Create("data/TEXTURE/gauge2.png", D3DXVECTOR2(104.0f, 3.0f), D3DXVECTOR2(423.0f, 14.0f), 1.2f, 100.0f, 100.0f,
								"data/TEXTURE/gauge_back2.png", D3DXVECTOR2(500.0f, SCREEN_HEIGHT - 600.0f), D3DXVECTOR2(531.0f, 20.0f), 1.2f);

	return S_OK;
}

//=================================================================================
//	終了関数
//=================================================================================
void CResult::Uninit( void )
{
	CSceneManager::GetCameraManager()->Release(MAIN_CAMERA);

	CSceneManager::GetLightManager()->Release(MAIN_LIGHT);

	// オブジェクトの破棄
	CScene::ReleaseAll();

	//StopSound(SOUND_LABEL_BGM_RESULT);
}

//=================================================================================
//	描画関数
//=================================================================================
void CResult::Draw( void )
{
	// カメラセット(すべてのオブジェクトを描画する前にカメラ置く)
	CCameraManager *pCameraManager = CSceneManager::GetCameraManager();
	CCamera *pCamera = pCameraManager->GetCamera(MAIN_CAMERA);
	if (pCamera != NULL) pCamera->Set();

	// オブジェクトの描画
	CScene::DrawAll();
}

//=================================================================================
//	更新関数
//=================================================================================
void CResult::Update( void )
{
	// オブジェクトの更新
	CScene::UpdateAll();

	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();
	if (pKeyboard->GetKeyTrigger(DIK_RETURN))
	{
		CFade::SetFade( CFade::FADE_OUT, new CTitle, 1.0f );
	}

	if (pKeyboard->GetKeyPress(DIK_U))
	{
		m_pHpGauge->Add(2.0f);
	}
	if (pKeyboard->GetKeyPress(DIK_D))
	{
		m_pHpGauge->Sub(2.0f);
	}

	// 点滅
	if (m_nTime % 65 == 0) op *= -1;
	btnAlpha += op * 0.01f;
	if (btnAlpha > 1.0f) btnAlpha = 1.0f;
	if (btnAlpha < 0.0f) btnAlpha = 0.0f;
	m_pButton->SetAlpha(btnAlpha);

	// 隠しておいた要素を徐々に表示
	for (int i = 0; i < (int)m_vpBoxBar.size(); i++)
	{
		if (m_vpBoxBar[i].flg)
		{
			m_vpBoxBar[i].ui->SetAlpha(m_vpBoxBar[i].alpha);

			m_vpBoxBar[i].alpha += 0.04f;

			if (m_vpBoxBar[i].alpha > 1.0f)
			{
				m_vpBoxBar[i].flg = FALSE;
				
				if(i + 1 < m_vpBoxBar.size())
					m_vpBoxBar[i + 1].flg = TRUE;
			}
		}
	}

	// 隠しておいた要素を徐々に表示
	for (int i = 0; i < (int)m_vpBoxText.size(); i++)
	{
		if (m_vpBoxText[i].flg)
		{
			m_vpBoxText[i].ui->SetAlpha(m_vpBoxText[i].alpha);

			m_vpBoxText[i].alpha += 0.04f;

			if (m_vpBoxText[i].alpha > 1.0f)
			{
				m_vpBoxText[i].flg = FALSE;

				if (i + 1 < m_vpBoxText.size())
					m_vpBoxText[i + 1].flg = TRUE;
			}
		}
	}

	// タイマーカウントアップ
	m_nTime += 1;
}