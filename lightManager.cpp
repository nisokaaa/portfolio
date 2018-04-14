//=================================================================================
//	�v���O������	�F	
//	����			�F	���C�g���ꊇ�Ǘ�[lightManager.cpp]
//	���C������		�F	sceneManager.cpp
//	�쐬��			�F	moriya gaku
//=================================================================================

//=================================================================================
//	�C���N���[�h�t�@�C��
//=================================================================================
#include "lightManager.h"

//=================================================================================
//	�S�J���֐�
//=================================================================================
void CLightManager::ReleaseAll(void)
{
	// �S�C�e���[�^
	for (LIGHTMAP::iterator it = m_LightMap.begin(); it != m_LightMap.end(); )
	{
		if (it->second != NULL)
		{
			delete it->second;
			it->second = NULL;

			// ���̃C�e���[�^���P����
			it = m_LightMap.erase(it);
			continue;
		}
		it++;
	}
}

//=================================================================================
//	�Y���V�F�[�_�[��Ԃ��֐�
//=================================================================================
CLight* CLightManager::GetLight(LIGHT_TYPE type)
{
	LIGHTMAP::iterator it = m_LightMap.find(type);

	return it != m_LightMap.end() ? it->second : NULL;
}

//=================================================================================
//	����֐�
//=================================================================================
void CLightManager::Release(LIGHT_TYPE type)
{
	LIGHTMAP::iterator it = m_LightMap.find(type);

	if (it != m_LightMap.end())
	{
		// ���
		delete it->second;
		it->second = NULL;

		// ���̃C�e���[�^���P����
		m_LightMap.erase(it);
	}
}

//=================================================================================
//	�S�X�V�֐�
//=================================================================================
void CLightManager::Update(void)
{
	// �S�C�e���[�^
	for (LIGHTMAP::iterator it = m_LightMap.begin(); it != m_LightMap.end(); )
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
CLight *CLightManager::Create(LIGHT_TYPE type, D3DXVECTOR3 vecDir, D3DXVECTOR3 position, D3DXCOLOR color, D3DXVECTOR3 diffuse, D3DXVECTOR3 ambient)
{
	CLight *p = new CLight;
	p->Init(type, vecDir, position, color, diffuse, ambient);

	m_LightMap.insert(LIGHTMAP::value_type(type, p));

	return p;
}