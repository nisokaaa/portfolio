#pragma once
//=================================================================================
//	�v���O������	�F	
//	����			�F	�f�o�b�O�p�I�u�W�F�N�g(���f��)����[debugObj.h]
//	���C������		�F	main.cpp
//	�쐬��			�F	moriya gaku
//=================================================================================

//=================================================================================
//	��d��`�h�~
//=================================================================================
#ifndef _DEBUG_OBJ_MODEL_H_
#define _DEBUG_OBJ_MODEL_H_

//=================================================================================
//	�C���N���[�h�t�@�C��
//=================================================================================
#include "main.h"
#include "sceneX.h"

typedef map < string, CSceneX* > DEBUG_OBJ_MAP;

class CDebugObj
{
public:
	CDebugObj();
	~CDebugObj();

	void Uninit(void);

	CSceneX *Create(const char* filename, string key);
	CSceneX *GetData(string key);						// �}�b�v�{��(�f�[�^�擾)�֐�

	void SetPosition(string name, D3DXVECTOR3 pos);		// ���W�ݒ�֐�
	void SetRotMatrix(string name, D3DXMATRIX mtxRot);	// �p���Z�b�g�֐�
	void SetColor(string name, D3DXCOLOR col);			// �F�ݒ�
	void SetScale(string name, D3DXVECTOR3 scl);		// �傫���ݒ�
		
private:
	DEBUG_OBJ_MAP m_mpModelData;						// ���f�����
};

#endif