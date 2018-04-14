#pragma once
//=================================================================================
//	�v���O������	�F	
//	����			�F	���b�V���t�B�[���h���ꊇ�Ǘ�[meshFieldManager.h]
//	���C������		�F	sceneManager.cpp
//	�쐬��			�F	moriya gaku
//=================================================================================

//=================================================================================
//	��d��`�h�~
//=================================================================================
#ifndef _MESH_FIELD_MANAGER_
#define _MESH_FIELD_MANAGER_

//=================================================================================
//	�C���N���[�h�t�@�C��
//=================================================================================
#include "main.h"
#include "meshField.h"

typedef map<MESHFIELD_TYPE, CMeshField*> MESHFIELDMAP;

class CMeshFieldManager
{
public:
	CMeshFieldManager();
	~CMeshFieldManager();

	CMeshField *GetData(MESHFIELD_TYPE key);				// �}�b�v�{��(�f�[�^�擾)�֐�

	CMeshField *Load(const std::string& filename, MESHFIELD_TYPE key, const int& priority = DEFAULT_PRIORITY);
	
	void Unload(MESHFIELD_TYPE key);
	
	void ReleaseAll(void);

private:
	MESHFIELDMAP m_mpMeshFieldMap;							// ���f���f�[�^�}�b�v
};

#endif