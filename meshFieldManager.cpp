//=================================================================================
//	�v���O������	�F	
//	����			�F	���b�V���t�B�[���h���ꊇ�Ǘ�[meshFieldManager.cpp]
//	���C������		�F	sceneManager.cpp
//	�쐬��			�F	moriya gaku
//=================================================================================

//=================================================================================
//	�C���N���[�h�t�@�C��
//=================================================================================
#include "meshFieldManager.h"
#include "debug.h"
#include "file.h"

//=================================================================================
//  �R���X�g���N�^
//=================================================================================
CMeshFieldManager::CMeshFieldManager()
{
}

//=================================================================================
//  �f�X�g���N�^
//=================================================================================
CMeshFieldManager::~CMeshFieldManager()
{
}

//=================================================================================
//	�}�b�v�{��(�f�[�^�擾)�֐�
//=================================================================================
CMeshField *CMeshFieldManager::GetData(MESHFIELD_TYPE key)
{
	// �I�u�W�F�N�g������
	MESHFIELDMAP::iterator it = m_mpMeshFieldMap.find(key);

	if (it != m_mpMeshFieldMap.end())
	{
		return it->second;
	}

	return NULL;
}

//=================================================================================
//  �t�@�C���ǂݍ��݊֐�
//=================================================================================
CMeshField *CMeshFieldManager::Load(const std::string& filename, MESHFIELD_TYPE key, const int& priority)
{
	if (GetData(key))return NULL;

	//	�t�@�C���ǂݍ���
	FILE  *pFile;
	fopen_s(&pFile, filename.c_str(), "r");

	if (pFile == NULL)
	{
		MessageBox(NULL, "�t�@�C����ǂݍ��߂܂���ł���", "���b�V���v���C�������G���[", MB_OK);
		return NULL;
	}
	//	�ϐ��錾
	D3DXVECTOR3 Pos, Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR2 BlockSize;
	char aTextureName[2][MAX_PATH];
	bool bSetUnEven = false;
	int nBlockX, nBlockY;
	int nTexture = 0;

	char aBuf[256];
	std::vector<std::vector<float>> vHeight;

	while (DivideToken(pFile, " \t\n", aBuf) >= 0)
	{
		//	�X�N���v�g�J�n
		if (strcmp("SCRIPT", aBuf) == 0)
		{
			while (DivideToken(pFile, " \t\n", aBuf) >= 0)
			{
				//	�v���C�����
				if (strcmp("PLANESET", aBuf) == 0)
				{
					while (DivideToken(pFile, " \t\n", aBuf) >= 0)
					{
						//���W���
						if (strcmp("POS", aBuf) == 0)
						{
							DivideToken(pFile, " \t\n", aBuf);
							//	x���W
							DivideToken(pFile, " \t\n", aBuf);
							Pos.x = (float)atof(aBuf);
							//	y���W
							DivideToken(pFile, " \t\n", aBuf);
							Pos.y = (float)atof(aBuf);
							//	z���W
							DivideToken(pFile, " \t\n", aBuf);
							Pos.z = (float)atof(aBuf);
						}

						//	�v���b�N���
						if (strcmp("BLOCK", aBuf) == 0)
						{
							DivideToken(pFile, " \t\n", aBuf);
							//	���u���b�N
							DivideToken(pFile, " \t\n", aBuf);
							nBlockX = atoi(aBuf);
							//	�c�u���b�N
							DivideToken(pFile, " \t\n", aBuf);
							nBlockY = atoi(aBuf);
						}

						//	�v���b�N�T�C�Y���
						if (strcmp("SIZE", aBuf) == 0)
						{
							DivideToken(pFile, " \t\n", aBuf);
							//	��
							DivideToken(pFile, " \t\n", aBuf);
							BlockSize.x = (float)atof(aBuf);
							//	����
							DivideToken(pFile, " \t\n", aBuf);
							BlockSize.y = (float)atof(aBuf);
						}
						//	�e�N�X�`���t�@�C���擾
						if (strcmp("TEXTURE", aBuf) == 0)
						{
							DivideToken(pFile, " \t\n", aBuf);
							DivideToken(pFile, " \t\n", aBuf);
							strcpy_s(aTextureName[nTexture], aBuf);
							nTexture++;
						}
						//	���ʐݒ�
						if (strcmp("SETUNEVEN", aBuf) == 0)
						{
							DivideToken(pFile, " \t\n", aBuf);

							DivideToken(pFile, " \t\n", aBuf);
							if (strcmp("1", aBuf) == 0)
							{
								bSetUnEven = true;

								vHeight.resize(nBlockY + 1);
								for (int i = 0; i < nBlockY + 1; i++) {
									vHeight[i].resize(nBlockX + 1);
								}
								
								for (int i = 0; i < nBlockY + 1;)
								{
									while (DivideToken(pFile, " \t\n", aBuf) >= 0)
									{
										if (strcmp("HEIGHT", aBuf) == 0)
										{
											DivideToken(pFile, " \t\n", aBuf);
											for (int j = 0; j < nBlockX + 1; j++)
											{
												DivideToken(pFile, " \t\n", aBuf);
												vHeight[i][j] = (float)atof(aBuf);
											}
											i++;
										}
									}
								}
							}

						}
						//	�v���C�����I��
						if (strcmp("END_PLANESET", aBuf) == 0)
						{
							break;
						}
					}
				}
				//	�X�N���v�g�I��
				if (strcmp("END_SCRIPT", aBuf) == 0)
				{
					break;
				}
			}
		}
	}

	fclose(pFile);

	//	����
	CMeshField* pMeshField = CMeshField::Create(Pos, Rot, BlockSize.x, BlockSize.y, nBlockX, nBlockY, vHeight);

	// �ǉ�
	m_mpMeshFieldMap.insert(MESHFIELDMAP::value_type(key, pMeshField));

	return pMeshField;
}


//=================================================================================
//�@���b�V���t�B�[���h���
//=================================================================================
void CMeshFieldManager::Unload(MESHFIELD_TYPE key)
{
	// find()��key���o�^����Ă��邩
	// �����ꍇ�̕Ԃ�l�F�Ō�̃C�e���[�^ 
	MESHFIELDMAP::const_iterator it = m_mpMeshFieldMap.find(key);

	if (it != m_mpMeshFieldMap.end())
	{
		// LPDIRECTTEXTURE9�̉��
		it->second->Uninit();

		// ���̃C�e���[�^���P����
		m_mpMeshFieldMap.erase(it);
	}
}

//=================================================================================
//�@���b�V���t�B�[���h�S���
//=================================================================================
void CMeshFieldManager::ReleaseAll(void)
{
	// �S�C�e���[�^
	for (MESHFIELDMAP::iterator it = m_mpMeshFieldMap.begin(); it != m_mpMeshFieldMap.end(); )
	{
		// LPDIRECTTEXTURE9�̉��
		if (it->second != NULL)
		{
			it->second->Uninit();
			it->second = NULL;

			// ���̃C�e���[�^���P����
			it = m_mpMeshFieldMap.erase(it);
			continue;
		}
		it++;
	}

	m_mpMeshFieldMap.clear();
}