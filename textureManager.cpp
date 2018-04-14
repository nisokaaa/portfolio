//=================================================================================
//	�v���O������	�F	
//	����			�F	�e�N�X�`�����ꊇ�Ǘ�[textureManager.cpp]
//	���C������		�F	manager.cpp
//	�쐬��			�F	moriya gaku
//=================================================================================

//=================================================================================
//	�C���N���[�h�t�@�C��
//=================================================================================
#include "textureManager.h"
#include "renderer.h"

//=================================================================================
//�@�ÓI�����o�ϐ��錾
//=================================================================================
CTextureManager::TEXMAP CTextureManager::m_TexMap;

//=================================================================================
//�@�e�N�X�`�����[�h
//=================================================================================
HRESULT CTextureManager::Load( string key )
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

    // �e�N�X�`��������
    TEXMAP::const_iterator it = m_TexMap.find( key );
    
	if( it != m_TexMap.end() )
	{
		// ���łɓǂݍ��ݍς�
        return S_OK;  
    }
	else
	{ 
        // �V�����}�b�v�ɒǉ�����
        HRESULT hr;
        LPDIRECT3DTEXTURE9 texture;
        hr = D3DXCreateTextureFromFile( pDevice, key.c_str(), &texture );
        
		if( FAILED( hr ) )
		{
            MessageBox(NULL, key.c_str(), "LOAD ERROR!", MB_OK);
            return E_FAIL;
        }

        // �}�b�v�֑}������
        m_TexMap.insert( TEXMAP::value_type( key, texture ) );
    }

    return S_OK;
}

//=================================================================================
//�@�e�N�X�`�����
//=================================================================================
HRESULT CTextureManager::Unload( string key )
{
	// find()��key���o�^����Ă��邩
	// �����ꍇ�̕Ԃ�l�F�Ō�̃C�e���[�^ 
	TEXMAP::const_iterator it = m_TexMap.find( key );

	if( it != m_TexMap.end() )
	{
		// LPDIRECTTEXTURE9�̉��
		it->second->Release();

		// ���̃C�e���[�^���P����
		m_TexMap.erase( it );

		return S_OK;
	}
	else
	{
		return E_FAIL;
	}

}

//=================================================================================
//�@�e�N�X�`���擾
//=================================================================================
LPDIRECT3DTEXTURE9 CTextureManager::GetResource( string key )
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

    // �e�N�X�`��������
    TEXMAP::iterator it = m_TexMap.find( key );
    
	if( it != m_TexMap.end() )
	{
        // �e�N�X�`����Ԃ�
        return it->second;  
    }
	else
	{ 
        return NULL;
    }
}

//=================================================================================
//�@�e�N�X�`���S���
//=================================================================================
HRESULT CTextureManager::ReleaseAll( void )
{
	// �S�C�e���[�^
	for( TEXMAP::iterator it = m_TexMap.begin(); it != m_TexMap.end(); )
	{
		// LPDIRECTTEXTURE9�̉��
		if( it->second != NULL )
		{
			it->second->Release();
			it->second = NULL;

			// ���̃C�e���[�^���P����
			it = m_TexMap.erase( it );
			continue;
		}
		it++;
	}

	return S_OK;

}

//=================================================================================
//�@���[�h�ς݃e�N�X�`�����̕\��
//=================================================================================
void CTextureManager::Show( void )
{
	char buff[ CHAR_MAX ];

	OutputDebugString( "\n_/_/_/_/_/_/_/_/_/_/_/ Loaded Texture _/_/_/_/_/_/_/_/_/_/_/_/\n\n" );

	for( TEXMAP::const_iterator it = m_TexMap.begin(); it != m_TexMap.end(); it++ )
	{
		sprintf_s( buff, "%s\n", it->first.c_str() );
		OutputDebugString( buff );
	}

	OutputDebugString( "\n\n_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/\n\n" );

	return;

}