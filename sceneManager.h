#pragma once
//=================================================================================
//	プログラム名	：	
//	処理			：	シーン情報一括管理[sceneManager.h]
//	メイン処理		：	manager.cpp
//	作成者			：	moriya gaku
//=================================================================================

//=================================================================================
//	二重定義防止
//=================================================================================
#ifndef _SCENE_MANAGER_H_
#define _SCENE_MANAGER_H_

//=================================================================================
//	インクルードファイル
//=================================================================================
#include "main.h"

//=================================================================================
//	前方宣言
//=================================================================================
class CMeshFieldManager;
class CShaderManager;
class CCameraManager;
class CLightManager;
class CPlayer;
class CDebugObj;
class CMenu;
class CScene2D;
class CBackGround;
class CGauge;
class CScore;

// 基底クラス
class CSceneBase
{
public:
	virtual HRESULT Init( void ) = 0;
	virtual void Uninit( void ) = 0;
	virtual void Update( void ) = 0;
	virtual void Draw( void ) = 0;
};

// 遷移を操作
class CSceneManager
{
public:
	CSceneManager();
	virtual ~CSceneManager();

	static HRESULT Init( void );
	static void Uninit( void );
	static void Update( void );
	static void Draw( void );

	static CSceneBase *GetScene( void ){ return m_pScene; }

	static void ChangeScene( CSceneBase *pNextScene ){

		// 今のシーンを終了処理
		if( m_pScene != NULL )
		{
			m_pScene->Uninit();
			delete m_pScene;
			m_pScene = NULL;
		}

		// 新たなシーン初期化処理
		m_pScene = pNextScene;
		m_pScene->Init();
	}

	// カメラマネージャ
	static CCameraManager *GetCameraManager(void){ return m_pCameraManager; }			
	static void SetCamera( CCameraManager *pCameraManager ){ m_pCameraManager = pCameraManager; }	

	// ライト
	static CLightManager *GetLightManager( void ){ return m_pLightManager; }
	static void SetLightManager( CLightManager *pLightManager ){ m_pLightManager = pLightManager; }	

	// メッシュフィールド
	static CMeshFieldManager *GetMeshFieldManager( void ){ return m_pMeshFieldManager; }
	static void SetMeshFieldManager( CMeshFieldManager *p ){ m_pMeshFieldManager = p; }

	// デバッグモデル
	static CDebugObj *GetDebugObj(void) { return m_pDebugObj; }
	static void SetDebugObj(CDebugObj *p) { m_pDebugObj = p; }

	// シェーダーマネージャ
	static CShaderManager *GetShaderManager(void) { return m_pShaderManager; }
	static void SetSahderManager(CShaderManager *p) { m_pShaderManager = p; }

	// リザルト画面背景(game->result)
	static CBackGround *GetBackGround(void) { return m_pBackGround; }
	static void SetBackGround(CBackGround *p) { m_pBackGround = p; }

private:
	static CSceneBase *m_pScene;	// 今のシーン	
	static CCameraManager *m_pCameraManager;	
	static CLightManager *m_pLightManager;
	static CMeshFieldManager *m_pMeshFieldManager;
	static CShaderManager *m_pShaderManager;
	static CDebugObj *m_pDebugObj;
	static CBackGround *m_pBackGround;
};

// タイトル
class CTitle : public CSceneBase
{
public:
	CTitle();
	virtual ~CTitle();

	HRESULT Init( void ) override;
	void Uninit( void ) override;
	void Update( void ) override;
	void Draw( void ) override;
	
	// メニュー
	CMenu *GetMenu(void) const { return m_pMenu; }

	// 時間
	int GetTime(void) { return m_nTime; }

private:
	CMenu *m_pMenu;						// メニュー
	int m_nTime;						// 経過時間(フレーム)
	CScene2D *m_pBackGround;			// 背景模様
	float op;
	float alpha;
};


// ゲーム
class CGame : public CSceneBase
{
public:
	CGame();
	virtual ~CGame();

	HRESULT Init( void ) override;
	void Uninit( void ) override;
	void Update( void ) override;
	void Draw( void ) override;
	
	// プレイヤー
	CPlayer *GetPlayer(void) { return m_pPlayer; }
	void SetPlayer(CPlayer *p) { m_pPlayer = p; }

	// メニュー
	CMenu *GetMenu(void) const { return m_pMenu; }

	// ポーズモードフラグ
	void SetPauseFlg(BOOL flg) { m_bPause = flg; }

private:
	CMenu *m_pMenu;				// メニュー
	int m_Timer;				// フレーム数
	CPlayer *m_pPlayer;			// プレイヤー
	BOOL m_bPause;				// 一時停止中
	CBackGround *m_pBackGround;	// 効果背景
	CScore *m_pScore;			// スコア値
	CScore *m_pHpScore;			// HPスコア値
};


// リザルト
class CResult : public CSceneBase
{
public:
	struct UI{
		UI() {}
		UI(CScene2D *p) : ui(nullptr), alpha(0.0f), flg(FALSE){
			ui = p;
		}
		CScene2D *ui;
		float alpha;
		BOOL flg;
	};

	CResult();
	virtual ~CResult();

	HRESULT Init( void ) override;
	void Uninit( void ) override;
	void Update( void ) override;
	void Draw( void ) override;

private:
	int m_nTime;						// 経過時間(フレーム)
	CScene2D *m_pBackGround;			// 背景模様
	CScene2D *m_pButton;				// "PRESS START BUTTON"
	std::vector<UI> m_vpBoxBar;			// 結果 文字背景
	std::vector<UI> m_vpBoxText;		// 結果 文字
	float op;
	float btnAlpha;
	float textAlpha;
	CScore *m_pScore;
	CGauge *m_pHpGauge;
};
#endif