//=================================================================================
//	�v���O������	�F	
//	����			�F	�J�������ꊇ�Ǘ�[cameraManager.cpp]
//	���C������		�F	sceneManager.cpp
//	�쐬��			�F	moriya gaku
//=================================================================================

//=================================================================================
//	�C���N���[�h�t�@�C��
//=================================================================================
#include "cameraManager.h"

//=================================================================================
//	�S�J���֐�
//=================================================================================
void CCameraManager::ReleaseAll(void)
{
	// �S�C�e���[�^
	for (CAMERAMAP::iterator it = m_CameraMap.begin(); it != m_CameraMap.end(); )
	{
		if (it->second != NULL)
		{
			delete it->second;
			it->second = NULL;

			// ���̃C�e���[�^���P����
			it = m_CameraMap.erase(it);
			continue;
		}
		it++;
	}
}

//=================================================================================
//	�Y���V�F�[�_�[��Ԃ��֐�
//=================================================================================
CCamera* CCameraManager::GetCamera(CAMERA_TYPE type)
{
	CAMERAMAP::iterator it = m_CameraMap.find(type);

	return it != m_CameraMap.end() ? it->second : NULL;
}

//=================================================================================
//	����֐�
//=================================================================================
void CCameraManager::Release(CAMERA_TYPE type)
{
	CAMERAMAP::iterator it = m_CameraMap.find(type);

	if (it != m_CameraMap.end())
	{
		// ���
		delete it->second;
		it->second = NULL;

		// ���̃C�e���[�^���P����
		m_CameraMap.erase(it);
	}
}

//=================================================================================
//	�S�X�V�֐�
//=================================================================================
void CCameraManager::Update(void)
{
	// �S�C�e���[�^
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
//	�����֐�
//=================================================================================
CCamera *CCameraManager::Create(CAMERA_TYPE type, D3DXVECTOR3 pos, float distance, float middleAtOffSetY)
{
	CCamera *p = new CCamera;
	p->Init(pos, distance, middleAtOffSetY);
	m_CameraMap.insert(CAMERAMAP::value_type(type, p));

	return p;
}