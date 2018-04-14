//=================================================================================
//	プログラム名	：	
//	処理			：	カメラ情報一括管理[cameraManager.cpp]
//	メイン処理		：	sceneManager.cpp
//	作成者			：	moriya gaku
//=================================================================================

//=================================================================================
//	インクルードファイル
//=================================================================================
#include "cameraManager.h"

//=================================================================================
//	全開放関数
//=================================================================================
void CCameraManager::ReleaseAll(void)
{
	// 全イテレータ
	for (CAMERAMAP::iterator it = m_CameraMap.begin(); it != m_CameraMap.end(); )
	{
		if (it->second != NULL)
		{
			delete it->second;
			it->second = NULL;

			// このイテレータを１つ消す
			it = m_CameraMap.erase(it);
			continue;
		}
		it++;
	}
}

//=================================================================================
//	該当シェーダーを返す関数
//=================================================================================
CCamera* CCameraManager::GetCamera(CAMERA_TYPE type)
{
	CAMERAMAP::iterator it = m_CameraMap.find(type);

	return it != m_CameraMap.end() ? it->second : NULL;
}

//=================================================================================
//	解放関数
//=================================================================================
void CCameraManager::Release(CAMERA_TYPE type)
{
	CAMERAMAP::iterator it = m_CameraMap.find(type);

	if (it != m_CameraMap.end())
	{
		// 解放
		delete it->second;
		it->second = NULL;

		// このイテレータを１つ消す
		m_CameraMap.erase(it);
	}
}

//=================================================================================
//	全更新関数
//=================================================================================
void CCameraManager::Update(void)
{
	// 全イテレータ
	for (CAMERAMAP::iterator it = m_CameraMap.begin(); it != m_CameraMap.end(); )
	{
		if (it->second != NULL)
		{
			it->second->Update();
		}
		it++;
	}
}

//=================================================================================
//	生成関数
//=================================================================================
CCamera *CCameraManager::Create(CAMERA_TYPE type, D3DXVECTOR3 pos, float distance, float middleAtOffSetY)
{
	CCamera *p = new CCamera;
	p->Init(pos, distance, middleAtOffSetY);
	m_CameraMap.insert(CAMERAMAP::value_type(type, p));

	return p;
}