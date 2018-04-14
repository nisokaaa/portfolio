#pragma once
//=================================================================================
//	プログラム名	：	
//	処理			：	カメラ情報一括管理[cameraManager.h]
//	メイン処理		：	sceneManager.cpp
//	作成者			：	moriya gaku
//=================================================================================
//=================================================================================
//	二重定義防止
//=================================================================================
#ifndef _CAMERA_MANAGER_H_
#define _CAMERA_MANAGER_H_

//=================================================================================
//	インクルードファイル
//=================================================================================
#include "main.h"
#include "camera.h"

#define CAMERA_MAX (5)

typedef enum
{
	MAIN_CAMERA = 0,
	SUB_CAMERA,
}CAMERA_TYPE;

class CCameraManager
{
public:
	typedef std::map < CAMERA_TYPE, CCamera* > CAMERAMAP;

	CCameraManager() {}
	~CCameraManager() {}

	void ReleaseAll(void);

	void Update(void);

	void Release(CAMERA_TYPE type);

	CCamera *GetCamera(CAMERA_TYPE type);

	CCamera *Create(CAMERA_TYPE type, D3DXVECTOR3 pos, float distance, float middleAtOffSetY);

private:
	CAMERAMAP m_CameraMap;

};

#endif