#pragma once
//=================================================================================
//	�v���O������	�F	
//	����			�F	���C������[main.h]
//	���C������		�F	main.cpp
//	�쐬��			�F	moriya gaku
//=================================================================================

//=================================================================================
//	��d��`�h�~
//=================================================================================
#ifndef _MAIN_H_
#define _MAIN_H_

//=================================================================================
//	�C���N���[�h�t�@�C��
//=================================================================================
#include <windows.h>
#include <cstdio>
#include <stdio.h>
#include "d3dx9.h"
#define DIRECTINPUT_VERSION	( 0x0800 )
#include <dinput.h>
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <time.h>
#include <stdlib.h>
#include <typeinfo.h>

//=================================================================================
//	���O��Ԑ錾
//=================================================================================
using namespace std;

//=================================================================================
//	�O���[�o���^��`
//=================================================================================
typedef map < std::string, LPDIRECT3DTEXTURE9 > TEXMAP;

//=================================================================================
//	�O���[�o���}�N����`
//=================================================================================
#define SCREEN_WIDTH		(1760)				// �E�C���h�E�̕�
#define SCREEN_HEIGHT		(990)				// �E�C���h�E�̍���
#define SCREEN_CENTER_X		(SCREEN_WIDTH / 2)	// �E�C���h�E�̒��S�w���W
#define SCREEN_CENTER_Y		(SCREEN_HEIGHT / 2)	// �E�C���h�E�̒��S�x���W
#define SafeRelease(p)      if(p){p->Release();p=NULL;}
#define SafeDelete(p)		if(p){delete p;p=NULL;}
#define SafeUninit(p)		if(p){p->Uninit();delete p;p=NULL;}
#define _CRT_SECURE_NO_WARNINGS
#define NUM_VERTEX			( 4 )				// ���_��
#define NUM_POLYGON			( 2 )				// �|���S����
#define NO_TEXTURE			("data/TEXTURE/no_material.png")

#define FVF_VERTEX_2D (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)
typedef struct
{
	D3DXVECTOR3 pos;	// ���_���W
	float rhw;			// ���W�ϊ��p�W��(1.0f�ŌŒ�)
	D3DCOLOR col;		// ���_�J���[
	D3DXVECTOR2 tex;	// �e�N�X�`�����W
} VERTEX_2D;

#define	FVF_VERTEX_3D		(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)
typedef struct
{
	D3DXVECTOR3 pos;	// ���_���W
	D3DXVECTOR3 nor;	// �@��
	D3DCOLOR col;		// ���_�J���[
	D3DXVECTOR2 tex;	// �e�N�X�`�����W
} VERTEX_3D;

#define COLOR_WHITE		(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f))
#define COLOR_BLACK		(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f))
#define COLOR_GRAY		(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f))
#define COLOR_GRAY0		(D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f))
#define COLOR_GRAY1		(D3DXCOLOR(0.65f, 0.65f, 0.65f, 1.0f))
#define COLOR_GRAY2		(D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f))
#define COLOR_GRAY3		(D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f))
#define COLOR_RED0		(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f))
#define COLOR_RED1		(D3DXCOLOR(1.0f, 0.28f, 0.0f, 1.0f))
#define COLOR_RED2		(D3DXCOLOR(1.0f ,0.62f ,0.019f, 1.0f))
#define COLOR_RED3		(D3DXCOLOR(1.0f ,0.95f ,0.019f, 1.0f))
#define COLOR_GREEN0	(D3DXCOLOR(0.019f, 1.0f, 0.019f, 1.0f))
#define COLOR_GREEN1	(D3DXCOLOR(0.019f, 1.0f, 0.34f, 1.0f))
#define COLOR_GREEN2	(D3DXCOLOR(0.31f, 1.0f, 0.77f, 1.0f))
#define COLOR_GREEN3	(D3DXCOLOR(0.61f, 1.0f, 0.87f, 1.0f))
#define COLOR_BLUE0		(D3DXCOLOR(0.019f, 0.019f, 1.0f, 1.0f))
#define COLOR_BLUE1		(D3DXCOLOR(0.019f, 0.34f, 1.0f, 1.0f))
#define COLOR_BLUE2		(D3DXCOLOR(0.31f, 0.77f, 1.0f, 1.0f))
#define COLOR_BLUE3		(D3DXCOLOR(0.61f, 0.87f, 1.0f, 1.0f))

//=================================================================================
// �Q�[���ݒ�}�N����`
//=================================================================================
#define CREATE_ENEMY					// �G�𔭐�������
//#undef CREATE_ENEMY

//=================================================================================
// ���C�u�����t�@�C��
// [�\���v���p�e�B]->[�����J�[]->[����]->[�ǉ��̈ˑ��t�@�C��]�ɋL�q���Ă��\
//=================================================================================
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "dinput8.lib")	// �L�[�p�b�h�擾�ɕK�v

//=================================================================================
// �v���g�^�C�v�錾
//=================================================================================
int GetFPS( void );

// �����_���l�����@3.14��n���� -3.14 �` 3.14 �̃����_���l��Ԃ�
float GetRand( float value );

// �����_���l����(�͈͎w��)
float GetRandRnage( float min, float max );

// (x, y, z, w)�x�N�g���ƍs��̏�Z
void Vec4Transform(D3DXVECTOR4 *pOut, D3DXVECTOR4 *pV, D3DXMATRIX *pM);

// �Q���W���x�N�g���擾�֐�
D3DXVECTOR3* CalcPositionToVector( D3DXVECTOR3* pout, const D3DXVECTOR3& at, const D3DXVECTOR3& pos );

// 2�x�N�g���Ԃ̂Ȃ��p���v�Z
float CalcVecAngle( D3DXVECTOR3& vec1, D3DXVECTOR3& vec2 );

// ���Ƌ��̓����蔻��
bool CheckColSphere( D3DXVECTOR3& pos1, D3DXVECTOR3& pos2, float length1, float lenfth2 );

// �L�����N�^�p���s��Z�o�֐�
D3DXMATRIX* CalcLookAtMatrix( D3DXMATRIX* pout, D3DXVECTOR3* pPos, D3DXVECTOR3* pLook, D3DXVECTOR3* pUp );

#endif