#pragma once
//=================================================================================
//	�v���O������	�F	
//	����			�F	�J��������[camera.h]
//	���C������		�F	main.cpp
//	�쐬��			�F	moriya gaku
//=================================================================================

//=================================================================================
//	��d��`�h�~
//=================================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

//=================================================================================
//	�C���N���[�h�t�@�C��
//=================================================================================
#include "main.h"

//=================================================================================
//	�}�N����`
//=================================================================================
#define CAMERA_AT_NEAR		( 1.0f )		// �`�扺������
#define CAMERA_AT_FAR		( 5000.0f )		// �`��������

class CCamera
{
public:
	CCamera();										
	~CCamera();										
	
	void Init( D3DXVECTOR3 pos, float distance, float middleAtOffSetY );								
	void Uninit( void );							
	void Update( void );							
	
	void Set( void );

	const D3DXMATRIX &GetViewMtx( void ){ return m_MtxView; }	
	void SetViewMtx( D3DXMATRIX mtxView ){ m_MtxView = mtxView; }

	const D3DXMATRIX& GetProjMtx( void ){ return m_MtxProj; }	
	void SetProjMtx( D3DXMATRIX mtxProj ){ m_MtxProj = mtxProj; }

	const D3DXMATRIX& GetViewPortMtx( void ){ return m_MtxViewPort; }	
	void SetViewPortMtx( D3DXMATRIX mtxViewPort ){ m_MtxViewPort = mtxViewPort; }

	const D3DXVECTOR3& GetPosition( void ) { return m_Position; }					
	void SetPosition( const D3DXVECTOR3 pos ){ m_Position = pos; }					
	
	const D3DXVECTOR3& GetLookAt( void ) const { return m_LookAt; }
	void SetLookAt( const D3DXVECTOR3 at ){ m_LookAt = at; }
	
	const D3DXVECTOR3 GetVecDir( void ) const { return m_VecDir; }								
	
	void SetDistance(float dist) { m_Distance += dist; }

	void SetMiddleOffSet(float middleOffSet) { m_MiddleAtOffSetY += middleOffSet; }

	// ��]�s��쐬
	void CreateRotMatrix( void );

	// �ړ��s��쐬
	void CreatePosMatrix( void );

	// �p�����[�^�\��
	void ShowParam( void );

private:
	D3DXMATRIX		m_MtxView;					// �r���[�s��
	D3DXMATRIX		m_MtxProj;					// �v���W�F�N�V�����s��
	D3DXMATRIX		m_MtxViewPort;				// �r���[�|�[�g�s��
	D3DXMATRIX		m_MtxRot;					// �p���p��]�s��
	D3DXMATRIX		m_MtxPos;					// �p���p�ʒu�s��

	float			m_fYRot;					// Y����]��
	float			m_fXRot;					// X����]��

	float			m_Distance;					// �Ǐ]�Ώۂ���̋���
	float			m_MiddleAtOffSetY;			// �Ǐ]�Ώۓ���܂ł̍���(�����_�̒���)
	float			m_RotTarget;				// �Ȃ��p�ڕW��]��Y
	D3DXVECTOR3		m_LookAt;					// �����_
	D3DXVECTOR3		m_LookAtTarget;				// ��Ԍ�̖ڕW�����_
	D3DXVECTOR3		m_Position;					// ���W
	D3DXVECTOR3		m_VecDir;					// �J�����̌���

	bool			m_InterpolationEnable;		// ��ԉ\�t���O
	bool			m_bInterpolationDuring;		// ��Ԓ��t���O
};

#endif