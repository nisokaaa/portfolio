#pragma once
//=================================================================================
//	�v���O������	�F	
//	����			�F	�X�R�A����[score.h]
//	���C������		�F	main.cpp
//	�쐬��			�F	moriya gaku
//=================================================================================

//=================================================================================
//	��d��`�h�~
//=================================================================================
#ifndef _SCORE_H_
#define _SCORE_H_

//=================================================================================
// �C���N���[�h�t�@�C��
//=================================================================================
#include "main.h"
#include "scene.h"

class CScore : public CScene
{
public:
	CScore(int priority = DEFAULT_PRIORITY):CScene(priority) {}
	~CScore() {}

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetUVScore(int DigitCount, int value);
	void AddScore(int value);
	void SubScore(int value);
	int  GetScore(void);

	static CScore *Create(std::string key, D3DXVECTOR2 pos, D3DXVECTOR2 size, int digit, float interval = 10.0f, int priority = DEFAULT_PRIORITY);

private:
	void MakeVertexScore(LPDIRECT3DDEVICE9 pDevice);

	int m_nValue;							// �X�R�A�l
	std::string m_Key;						// �e�N�X�`���L�[
	int m_nDigit;							// ����
	float m_fInterval;						// �����̕\���Ԋu
	D3DXVECTOR2 m_fTexFontSize;				// �P�����̃e�N�X�`��������
	std::vector<float> m_vfDigitU;			// �e����U�l
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffer;	// ���_�o�b�t�@
};

#endif