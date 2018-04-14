#pragma once
//=================================================================================
//	�v���O������	�F	
//	����			�F	�e�N�X�`���u�����h�V�F�[�_����[shaderMeshField.h]
//	���C������		�F	main.cpp
//	�쐬��			�F	moriya gaku
//=================================================================================

//=================================================================================
//	��d��`�h�~
//=================================================================================
#ifndef _SHADER_MESHFIELD_H_
#define _SHADER_MESHFIELD_H_

//=================================================================================
//	�C���N���[�h�t�@�C��
//=================================================================================
#include "shader.h"

class CSMeshField : public CShader
{
public:
	CSMeshField() :
		m_pVertexDeclaration(nullptr),
		m_pVtxShader(nullptr),
		m_pVtxConstantTable(nullptr),
		m_pPS(nullptr),
		m_pPConstantTable(nullptr) 
	{
		D3DXMatrixIdentity(&m_MtxView);
		D3DXMatrixIdentity(&m_MtxProj);
	}
	~CSMeshField() {}

	HRESULT Init(void);

	void Uninit(void);

	// �V�F�[�_�v���O�����J�n
	void Begin(void);

	// �V�F�[�_�փf�[�^���M
	void SendData(void);

	// �V�F�[�_�v���O�����I��
	void End(void);

	// �l�ݒ�
	void SetParam(const D3DXMATRIX& mtxView, const D3DXMATRIX& mtxProj,
		const int& nMinHeight, const int& nMaxHeight, const D3DXVECTOR3& lightDirW, const D3DXVECTOR3& eyePosW);

	// �s��ݒ�
	void SetMatrix(const D3DXMATRIX& mtxWorld);
	
	const LPD3DXCONSTANTTABLE& GetConstantVS(void) { return m_pVtxConstantTable; }
	const LPD3DXCONSTANTTABLE& GetConstantPS(void) { return m_pPConstantTable; }

private:
	D3DXMATRIX m_MtxView;
	D3DXMATRIX m_MtxProj;
	int m_nMaxHeight;
	int m_nMinHeight;
	D3DXVECTOR3 m_EyePosW;
	D3DXVECTOR3 m_LightDirW;

	LPDIRECT3DVERTEXDECLARATION9 m_pVertexDeclaration;
	LPDIRECT3DVERTEXSHADER9 m_pVtxShader;		// ���_�V�F�[�_
	LPD3DXCONSTANTTABLE m_pVtxConstantTable;	// ���_�V�F�[�_�R���X�^���g�e�[�u��
	LPDIRECT3DPIXELSHADER9 m_pPS;				// �s�N�Z���V�F�[�_
	LPD3DXCONSTANTTABLE m_pPConstantTable;		// �s�N�Z���V�F�[�_�R���X�^���g�e�[�u��
};

#endif