//=================================================================================
//	�v���O������	�F	
//	����			�F	���f�����ꊇ�Ǘ�[modelManager.cpp]
//	���C������		�F	manager.cpp
//	�쐬��			�F	moriya gaku
//=================================================================================

//=================================================================================
//	�C���N���[�h�t�@�C��
//=================================================================================
#include "modelManager.h"
#include "renderer.h"

//=================================================================================
//	�ÓI�����o�ϐ��錾
//=================================================================================
CModelManager::MODELMAP CModelManager::m_ModelMap;

//=================================================================================
//	�R���X�g���N�^
//=================================================================================
CModelManager::CModelManager()
{
}

//=================================================================================
//	�f�X�g���N�^
//=================================================================================
CModelManager::~CModelManager()
{
}

//=================================================================================
//	���f���ǂݍ��݊֐�
//=================================================================================
HRESULT CModelManager::Load( string key )
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

    // �e�N�X�`��������
    MODELMAP::const_iterator it = m_ModelMap.find( key );
    
	if( it != m_ModelMap.end() )
	{
        // ���f������Ԃ�
        return S_OK;
    }
	else
	{ 
		HRESULT hr;
		CModelData *pModelData = new CModelData;

		hr = D3DXLoadMeshFromX( key.c_str(), D3DXMESH_SYSTEMMEM, pDevice, NULL, &pModelData->pBuffMat, NULL, &pModelData->nNumMat, &pModelData->pMesh );
        
		if( FAILED( hr ) )
		{
            MessageBox(NULL, key.c_str(), "LOAD ERROR!", MB_OK);
            return E_FAIL;
        }

        // �}�b�v�֑}������
        m_ModelMap.insert( MODELMAP::value_type( key, pModelData ) );
	}

	return S_OK;
}

//=================================================================================
//	����֐�
//=================================================================================
HRESULT CModelManager::Unload( string key )
{
	MODELMAP::iterator it = m_ModelMap.find( key );

	if( it != m_ModelMap.end() )
	{
		// �}�e���A���̊J��
		it->second->pBuffMat->Release();
		it->second->pBuffMat = NULL;

		// ���b�V���̊J��
		it->second->pMesh->Release();
		it->second->pMesh = NULL;
		
		it->second->nNumMat	= 0;

		// ���̃C�e���[�^���P����
		m_ModelMap.erase( it );

		return S_OK;
	}
	else
	{
		return E_FAIL;
	}
}

//=================================================================================
//	�擾�֐�
//=================================================================================
CModelData* CModelManager::GetResource( string key )
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

    // ����
    MODELMAP::iterator it = m_ModelMap.find( key );
    
	if( it != m_ModelMap.end() )
	{
        // ���f������Ԃ�
        return it->second;
    }
	else
	{ 
        return NULL;
    }
}

//=================================================================================
//	�S����֐�
//=================================================================================
HRESULT CModelManager::ReleaseAll( void )
{
	// �S�C�e���[�^
	for( MODELMAP::iterator it = m_ModelMap.begin(); it != m_ModelMap.end(); )
	{
		// LPDIRECTTEXTURE9�̉��
		if( it->second != NULL )
		{
			// �}�e���A���̊J��
			it->second->pBuffMat->Release();
			it->second->pBuffMat = NULL;

			// ���b�V���̊J��
			it->second->pMesh->Release();
			it->second->pMesh = NULL;
		
			it->second->nNumMat	= 0;

			// ���̃C�e���[�^���P����
			it = m_ModelMap.erase( it );
			continue;
		}
		it++;
	}

	return S_OK;
}