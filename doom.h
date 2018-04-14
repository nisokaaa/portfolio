#pragma once
//=================================================================================
//	�v���O������	�F	
//	����			�F	�h�[������[doom.h]
//	���C������		�F	main.cpp
//	�쐬��			�F	moriya gaku
//=================================================================================

//=================================================================================
//	��d��`�h�~
//=================================================================================
#ifndef _DOOM_H_
#define _DOOM_H_

//=================================================================================
//	�C���N���[�h�t�@�C��
//=================================================================================
#include "main.h"
#include "mesh.h"

class CDoom : public CMesh
{
public:
	CDoom();
	~CDoom(){};

	HRESULT MakeVertexBuffer( void );	// ���_�o�b�t�@�쐬
	void SetVertex( void );				// ���_���Z�b�g
	void SetMatrix(void);				// ���[���h�s��v�Z�E�f�o�C�X�ɃZ�b�g

	static CDoom *Create( D3DXVECTOR3 pos, D3DXVECTOR3 rot, float width, float height,int numWidth, int numHeight, float angleXY, float angleXZ, const char* texPath );
	float AngleToRadian( float angle );	// �p�x�����W�A��

	void SetAngleXY( const float angleXY ){ m_AngleXY = angleXY; }
	const float& GetAngleXY( void ) const { return m_AngleXY; }

	void SetAngleXZ( const float angleXZ ){ m_AngleXZ = angleXZ; }
	const float& GetAngleXZ( void ) const { return m_AngleXZ; }


private:
	float m_AngleXY;					// XY���Ԃ̊p�x(90�Ŕ���)
	float m_AngleXZ;					// XZ���Ԃ̊p�x(361�Ŕ���)

};

#endif