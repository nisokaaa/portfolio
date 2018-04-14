#pragma once
//=================================================================================
//	�v���O������	�F	
//	����			�F	���b�V���t�B�[���h����[meshField.h]
//	���C������		�F	main.cpp
//	�쐬��			�F	moriya gaku
//=================================================================================

//=================================================================================
//	��d��`�h�~
//=================================================================================
#ifndef _MESH_FIELD_H_
#define _MESH_FIELD_H_

//=================================================================================
//	�C���N���[�h�t�@�C��
//=================================================================================
#include "main.h"
#include "mesh.h"

//=================================================================================
//	�^�C�v��`
//=================================================================================
typedef enum
{
	GAME_MAIN_STAGE = 0,
}MESHFIELD_TYPE;

class CMeshField : public CMesh
{
public:
	CMeshField(int priority = DEFAULT_PRIORITY) : CMesh( priority ){};
	~CMeshField(){};											
	
	void Uninit(void);

	HRESULT MakeVertexBuffer( void );			// ���_�o�b�t�@�쐬
	void SetVertex( void );						// ���_���Z�b�g
	void SetMatrix(void);						// ���[���h�s��v�Z�E�f�o�C�X�ɃZ�b�g

	float GetPosHeight( D3DXVECTOR3 position );	// P������Ă��鉚�ʂ̍����擾

	static CMeshField *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float width, float  height, int numWidth, int numHeight, std::vector<std::vector<float>> vHeight);

private:
	std::vector<std::vector<float>> m_vHeight;
};

#endif