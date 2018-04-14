#pragma once
//=================================================================================
//	�v���O������	�F	
//	����			�F	�V�[�����ꊇ�Ǘ�[sceneManager.h]
//	���C������		�F	manager.cpp
//	�쐬��			�F	moriya gaku
//=================================================================================

//=================================================================================
//	��d��`�h�~
//=================================================================================
#ifndef _SCENE_MANAGER_H_
#define _SCENE_MANAGER_H_

//=================================================================================
//	�C���N���[�h�t�@�C��
//=================================================================================
#include "main.h"

//=================================================================================
//	�O���錾
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

// ���N���X
class CSceneBase
{
public:
	virtual HRESULT Init( void ) = 0;
	virtual void Uninit( void ) = 0;
	virtual void Update( void ) = 0;
	virtual void Draw( void ) = 0;
};

// �J�ڂ𑀍�
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

		// ���̃V�[�����I������
		if( m_pScene != NULL )
		{
			m_pScene->Uninit();
			delete m_pScene;
			m_pScene = NULL;
		}

		// �V���ȃV�[������������
		m_pScene = pNextScene;
		m_pScene->Init();
	}

	// �J�����}�l�[�W��
	static CCameraManager *GetCameraManager(void){ return m_pCameraManager; }			
	static void SetCamera( CCameraManager *pCameraManager ){ m_pCameraManager = pCameraManager; }	

	// ���C�g
	static CLightManager *GetLightManager( void ){ return m_pLightManager; }
	static void SetLightManager( CLightManager *pLightManager ){ m_pLightManager = pLightManager; }	

	// ���b�V���t�B�[���h
	static CMeshFieldManager *GetMeshFieldManager( void ){ return m_pMeshFieldManager; }
	static void SetMeshFieldManager( CMeshFieldManager *p ){ m_pMeshFieldManager = p; }

	// �f�o�b�O���f��
	static CDebugObj *GetDebugObj(void) { return m_pDebugObj; }
	static void SetDebugObj(CDebugObj *p) { m_pDebugObj = p; }

	// �V�F�[�_�[�}�l�[�W��
	static CShaderManager *GetShaderManager(void) { return m_pShaderManager; }
	static void SetSahderManager(CShaderManager *p) { m_pShaderManager = p; }

	// ���U���g��ʔw�i(game->result)
	static CBackGround *GetBackGround(void) { return m_pBackGround; }
	static void SetBackGround(CBackGround *p) { m_pBackGround = p; }

private:
	static CSceneBase *m_pScene;	// ���̃V�[��	
	static CCameraManager *m_pCameraManager;	
	static CLightManager *m_pLightManager;
	static CMeshFieldManager *m_pMeshFieldManager;
	static CShaderManager *m_pShaderManager;
	static CDebugObj *m_pDebugObj;
	static CBackGround *m_pBackGround;
};

// �^�C�g��
class CTitle : public CSceneBase
{
public:
	CTitle();
	virtual ~CTitle();

	HRESULT Init( void ) override;
	void Uninit( void ) override;
	void Update( void ) override;
	void Draw( void ) override;
	
	// ���j���[
	CMenu *GetMenu(void) const { return m_pMenu; }

	// ����
	int GetTime(void) { return m_nTime; }

private:
	CMenu *m_pMenu;						// ���j���[
	int m_nTime;						// �o�ߎ���(�t���[��)
	CScene2D *m_pBackGround;			// �w�i�͗l
	float op;
	float alpha;
};


// �Q�[��
class CGame : public CSceneBase
{
public:
	CGame();
	virtual ~CGame();

	HRESULT Init( void ) override;
	void Uninit( void ) override;
	void Update( void ) override;
	void Draw( void ) override;
	
	// �v���C���[
	CPlayer *GetPlayer(void) { return m_pPlayer; }
	void SetPlayer(CPlayer *p) { m_pPlayer = p; }

	// ���j���[
	CMenu *GetMenu(void) const { return m_pMenu; }

	// �|�[�Y���[�h�t���O
	void SetPauseFlg(BOOL flg) { m_bPause = flg; }

private:
	CMenu *m_pMenu;				// ���j���[
	int m_Timer;				// �t���[����
	CPlayer *m_pPlayer;			// �v���C���[
	BOOL m_bPause;				// �ꎞ��~��
	CBackGround *m_pBackGround;	// ���ʔw�i
	CScore *m_pScore;			// �X�R�A�l
	CScore *m_pHpScore;			// HP�X�R�A�l
};


// ���U���g
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
	int m_nTime;						// �o�ߎ���(�t���[��)
	CScene2D *m_pBackGround;			// �w�i�͗l
	CScene2D *m_pButton;				// "PRESS START BUTTON"
	std::vector<UI> m_vpBoxBar;			// ���� �����w�i
	std::vector<UI> m_vpBoxText;		// ���� ����
	float op;
	float btnAlpha;
	float textAlpha;
	CScore *m_pScore;
	CGauge *m_pHpGauge;
};
#endif