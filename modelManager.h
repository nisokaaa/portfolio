#pragma once
//=================================================================================
//	�v���O������	�F	
//	����			�F	���f�����ꊇ�Ǘ�[modelManager.h]
//	���C������		�F	manager.cpp
//	�쐬��			�F	moriya gaku
//=================================================================================

//=================================================================================
//	��d��`�h�~
//=================================================================================
#ifndef _MODEL_MANAGER_H_
#define _MODEL_MANAGER_H_

//=================================================================================
//	�C���N���[�h�t�@�C��
//=================================================================================
#include "main.h"
#include "model.h"

class CModelManager {
public:
	
	// ���f���}�b�v�^��`
	typedef map<string, CModelData*> MODELMAP;

    CModelManager();

    ~CModelManager();

    // ���f���ǂݍ���
    static HRESULT Load( string key );

	// �A�����[�h
	static HRESULT Unload( string key );

    // ���f�������擾����
    static CModelData* GetResource( string key );

    // �S���
    static HRESULT ReleaseAll( void );

private:
    static MODELMAP m_ModelMap;	// ���f���f�[�^�}�b�v

};

#endif




