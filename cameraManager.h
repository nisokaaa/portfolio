#pragma once
//=================================================================================
//	�v���O������	�F	
//	����			�F	�J�������ꊇ�Ǘ�[cameraManager.h]
//	���C������		�F	sceneManager.cpp
//	�쐬��			�F	moriya gaku
//=================================================================================
//=================================================================================
//	��d��`�h�~
//=================================================================================
#ifndef _CAMERA_MANAGER_H_
#define _CAMERA_MANAGER_H_

//=================================================================================
//	�C���N���[�h�t�@�C��
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