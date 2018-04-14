#pragma once
//=================================================================================
//	�v���O������	�F	
//	����			�F	���C�g���ꊇ�Ǘ�[lightManager.h]
//	���C������		�F	sceneManager.cpp
//	�쐬��			�F	moriya gaku
//=================================================================================
//=================================================================================
//	��d��`�h�~
//=================================================================================
#ifndef _LIGHT_MANAGER_H_
#define _LIGHT_MANAGER_H_

//=================================================================================
//	�C���N���[�h�t�@�C��
//=================================================================================
#include "main.h"
#include "light.h"

class CLightManager
{
public:
	typedef std::map < LIGHT_TYPE, CLight* > LIGHTMAP;

	CLightManager() {}
	~CLightManager() {}

	void ReleaseAll(void);

	void Update(void);

	void Release(LIGHT_TYPE type);

	CLight *GetLight(LIGHT_TYPE type);

	CLight *Create(LIGHT_TYPE type, D3DXVECTOR3 vecDir, D3DXVECTOR3 position, D3DXCOLOR color, D3DXVECTOR3 diffuse, D3DXVECTOR3 ambient);

private:
	LIGHTMAP m_LightMap;
};

#endif