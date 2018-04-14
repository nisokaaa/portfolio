#pragma once
//=================================================================================
//	�v���O������	�F	
//	����			�F	�Q�[�W����[gauge.h]
//	���C������		�F	main.cpp
//	�쐬��			�F	moriya gaku
//=================================================================================

//=================================================================================
//	��d��`�h�~
//=================================================================================
#ifndef _GAUGE_H_
#define _GAUGE_H_

//=================================================================================
//	�C���N���[�h�t�@�C��
//=================================================================================
#include "main.h"
#include "scene.h"
#include "scene2D.h"

class CGauge : public CScene2D
{
public:
	CGauge(int priority = DEFAULT_PRIORITY, BOOL mode = FALSE)
		: CScene2D(priority, mode) {}
	~CGauge() {}

	HRESULT Init( void );
	void Uninit( void );	
	void Update( void );
	void Draw( void );

	static CGauge *Create(std::string key, D3DXVECTOR2 pos, D3DXVECTOR2 size, float scl, float val, float maxVal,
						  std::string key2, D3DXVECTOR2 offsetPos, D3DXVECTOR2 size2, float scl2, BOOL mode = FALSE, int priority = DEFAULT_PRIORITY);

	void Add(const float val) { m_Val += val; }
	void Sub(const float val) { m_Val -= val; }
	void SetVal( const float val ){ m_Val = val; }
	const float& GetVal( void ) const { return m_Val; }

	const float& GetMaxVal(void) const { return m_MaxVal; }

	const float& GetOffSetWidth(void) const { return m_OffSetWidth; }
	
	VERTEX_2D* SetVertex(VERTEX_2D *pVtx);

private:
	CScene2D *m_pBackGround;	// �w�i�|���S��
	float m_Val;			// �l
	float m_MaxVal;			// �ő�l
	float m_OffSetWidth;	// �����l
};

#endif