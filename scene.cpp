//=================================================================================
//	�v���O������	�F	
//	����			�F	�S�`��I�u�W�F�N�g����[scene.cpp]
//	���C������		�F	main.cpp
//	�쐬��			�F	moriya gaku
//=================================================================================

//=================================================================================
//	�C���N���[�h�t�@�C��
//=================================================================================
#include "scene.h"
#include "manager.h"
#include "renderer.h"
#include "shader.h"

//=================================================================================
//�@�ÓI�����o�ϐ���錾
//=================================================================================
CScene *CScene::m_Top[ PRIORITY_MAX ] = {NULL};

//=================================================================================
//	�R���X�g���N�^
//=================================================================================
CScene::CScene( int priority )
{
	// ���X�g�\���̏�����
	if( m_Top[ priority ] == NULL )
	{
		m_Top[ priority ] = this;
	}
	else
	{
		CScene *scene = m_Top[ priority ];
		while( scene->m_Next != NULL )
		{
			scene = scene->m_Next;
		}
		scene->m_Next = this;
	}

	m_Next = NULL;
	m_Position = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	m_Rotation = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	m_Type = SCENE_TYPE_NONE;
	m_Delete = false;
	m_pShader = nullptr;
}

//=================================================================================
// �S�`��I�u�W�F�N�g�̍X�V����
//=================================================================================
void CScene::UpdateAll(int nOnlyUpdatePriority)
{
	for( int i = 0; i < PRIORITY_MAX; i++ )
	{
		// �w�肳�ꂽ�D��x�̂ݍX�V�������Ƃ�
		if (i != nOnlyUpdatePriority && nOnlyUpdatePriority != -1)continue;

		CScene *scene = m_Top[ i ];
		CScene *scenePrev = m_Top[ i ];
		CScene *sceneNext = NULL;

		while( scene != NULL )
		{
			// �폜�˗�����ĂȂ����m�͍X�V
			if(!scene->m_Delete)scene->Update();
			scene = scene->m_Next;
		}

		scene = m_Top[ i ];
		while( scene != NULL )
		{
			sceneNext = scene->m_Next;

			if( scene->m_Delete == true )
			{
				if( scene == m_Top[ i ] )
				{
					// �擪��j�����鏀��
					m_Top[ i ] = scene->m_Next;
				}
				else
				{
					// ���X�g�\���̂Ȃ��ς�
					scenePrev->m_Next = scene->m_Next;
				}

				// �j��
				delete scene;
				scene = NULL;
	
			}
			else
			{
				scenePrev = scene;
			}

			// �i�߂�
			scene = sceneNext;
		}
	}
}

//=================================================================================
// �S�`��I�u�W�F�N�g�̕`�揈��
//=================================================================================
void CScene::DrawAll(int nOnlyDrawPriority, int nExcludePriority)
{
	for( int i = 0; i < PRIORITY_MAX; i++ )
	{
		// �w�肳�ꂽ�D��x�̂ݕ`�悳�������Ƃ�
		if (i != nOnlyDrawPriority && nOnlyDrawPriority != -1)continue;

		CScene *scene = m_Top[ i ];

		// �w�肳�ꂽ�D��x�͕`�悳�������Ȃ��Ƃ�
		if (i == nExcludePriority && nExcludePriority != -1)scene = NULL;
		
		while( scene != NULL )
		{
			CShader *pShader = scene->GetShader();
			if(pShader)
			{
				scene->GetShader()->Begin();	// �V�F�[�_�[�J�n

				scene->GetShader()->SendData();	// �f�[�^���M

				scene->Draw();					// �`��

				scene->GetShader()->End();		// �V�F�[�_�[�I��
			}
			else
			{
				scene->Draw();
			}
			
			scene = scene->m_Next;			// ���|�C���^��
		}
	}
}

//=================================================================================
// �h���N���X���������g��scene���
//=================================================================================
void CScene::Release( void )
{
	// �폜�t���O�I��
	m_Delete = true;

}

//=================================================================================
// �S�Ă̕`��I�u�W�F�N�g�̉��
//=================================================================================
void CScene::ReleaseAll( void )
{
	for( int i = 0; i < PRIORITY_MAX; i++ )
	{
		CScene *scene = m_Top[ i ];
		CScene *bufScene = NULL;

		while( scene != NULL )
		{
			bufScene = scene->m_Next;
			scene->Uninit();
			delete scene;
			scene = bufScene;
		}

		m_Top[ i ] = NULL;
	}
}