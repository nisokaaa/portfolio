//=================================================================================
//	�v���O������	�F	
//	����			�F	�V�F�[�_�[���ꊇ�Ǘ�[shaderManager.cpp]
//	���C������		�F	sceneManager.cpp
//	�쐬��			�F	moriya gaku
//=================================================================================

//=================================================================================
//	�C���N���[�h�t�@�C��
//=================================================================================
#include "shaderManager.h"

//=================================================================================
//	�R���X�g���N�^
//=================================================================================
CShaderManager::CShaderManager()
{
}

//=================================================================================
//	�f�X�g���N�^
//=================================================================================
CShaderManager::~CShaderManager()
{
}

//=================================================================================
//	�������֐�
//=================================================================================
void CShaderManager::Init(void)
{
	CShader* p = NULL;

	p = new CSBasic();
	p->Init();
	m_ShaderMap.insert(SHADERMAP::value_type(SHADER_BASIC, p));

	p = new CSMeshField();
	p->Init();
	m_ShaderMap.insert(SHADERMAP::value_type(SHADER_MESHFIELD, p));

	p = new CSUpPassFilter();
	p->Init();
	m_ShaderMap.insert(SHADERMAP::value_type(SHADER_UP_PASS_FILTTER, p));

	p = new CSDownPassFilter();
	p->Init();
	m_ShaderMap.insert(SHADERMAP::value_type(SHADER_DOWN_PASS_FILTTER, p));

	p = new CSHorizontalBlurFilter();
	p->Init();
	m_ShaderMap.insert(SHADERMAP::value_type(SHADER_HORIZONAL_BLUR_FILTTER, p));

	p = new CSVerticalBlurFilter();
	p->Init();
	m_ShaderMap.insert(SHADERMAP::value_type(SHADER_VERTICAL_BLUR_FILTTER, p));
}

//=================================================================================
//	�Y���V�F�[�_�[��Ԃ��֐�
//=================================================================================
CShader* CShaderManager::GetData(SHADER_TYPE type)
{
	SHADERMAP::iterator it = m_ShaderMap.find(type);

	return it != m_ShaderMap.end() ? it->second : NULL;
}

//=================================================================================
//	����֐�
//=================================================================================
void CShaderManager::Release(SHADER_TYPE type, CShader* pShader)
{
	SHADERMAP::iterator it = m_ShaderMap.find(type);

	if (it != m_ShaderMap.end())
	{
		// ���
		delete it->second;
		it->second = NULL;

		// ���̃C�e���[�^���P����
		m_ShaderMap.erase(it);
	}
}

//=================================================================================
//	�S�J���֐�
//=================================================================================
void CShaderManager::ReleaseAll(void)
{
	// �S�C�e���[�^
	for (SHADERMAP::iterator it = m_ShaderMap.begin(); it != m_ShaderMap.end(); )
	{
		if (it->second != NULL)
		{
			it->second->Uninit();
			delete it->second;
			it->second = NULL;

			// ���̃C�e���[�^���P����
			it = m_ShaderMap.erase(it);
			continue;
		}
		it++;
	}
}