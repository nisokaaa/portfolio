//=================================================================================
//	�v���O������	�F	
//	����			�F	�Q�[�W����[gauge.cpp]
//	���C������		�F	main.cpp
//	�쐬��			�F	moriya gaku
//=================================================================================

//=================================================================================
//	�C���N���[�h�t�@�C��
//=================================================================================
#include "gauge.h"
#include "manager.h"
#include "input.h"

//=================================================================================
// ����������
//=================================================================================
HRESULT CGauge::Init(void)
{
	CScene2D::Init();

	return S_OK;
}

//=================================================================================
// �I������
//=================================================================================
void CGauge::Uninit(void)
{
	CScene2D::Uninit();
}

//=================================================================================
// �X�V����
//=================================================================================
void CGauge::Update(void)
{
	// �擾
	float width	= GetWidth();

	m_OffSetWidth = width * (m_Val / m_MaxVal);

	// �ő�l����������
	if( m_Val > m_MaxVal ) 
	{
		m_Val = m_MaxVal;
		m_OffSetWidth = width;
	}

	// �Q�[�W�ŏ��l�����������
	if( m_Val < 0 ) 
	{
		m_Val = 0.0f;
		m_OffSetWidth = 0.0f;
	}

	CScene2D::Update();
}

//=================================================================================
// �`�揈��
//=================================================================================
void CGauge::Draw(void)
{
	CScene2D::Draw();
}

//=================================================================================
// �����֐�
//=================================================================================
CGauge *CGauge::Create(std::string key, D3DXVECTOR2 pos, D3DXVECTOR2 size, float scl, float val, float maxVal,
					   std::string backKey, D3DXVECTOR2 backPos, D3DXVECTOR2 backSize, float backScl, BOOL mode, int priority)
{
	CGauge *p = new CGauge(priority + 1, mode);

	p->SetPosition(D3DXVECTOR3(backPos.x + pos.x, backPos.y + pos.y, 0.0f));
	p->SetTexture(key);
	p->SetWidth(size.x);
	p->SetHeight(size.y);
	p->SetTexWidth(1.0f);
	p->SetTexHeight(1.0f);
	p->SetScale(scl);
	p->SetColor((D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)));
	
	p->m_Val = val;
	p->m_MaxVal = maxVal;
	p->m_OffSetWidth = 0.0f;

	p->m_pBackGround = CScene2D::Create(backKey, D3DXVECTOR2(backPos.x, backPos.y), backSize.x, backSize.y, backScl, mode, priority);

	p->Init();

	return p;
}

//=================================================================================
// ���_�ݒ�
//=================================================================================
VERTEX_2D* CGauge::SetVertex( VERTEX_2D *pVtx )
{
	D3DXVECTOR3 position = GetPosition();
	D3DXVECTOR3 rotation = GetRotation();
	LPDIRECT3DVERTEXBUFFER9 pVtxBuffer = GetVertexBuffer();
	float width = GetWidth();
	float height = GetHeight();
	float scale = GetScale();

	// ���_���W�̐ݒ�
	pVtx[0].pos.x = position.x;
	pVtx[0].pos.y = position.y;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = position.x + m_OffSetWidth * scale;
	pVtx[1].pos.y = position.y;
	pVtx[1].pos.z = 0.0f;
		  
	pVtx[2].pos.x = position.x;
	pVtx[2].pos.y = position.y + height * scale;
	pVtx[2].pos.z = 0.0f;
		  
	pVtx[3].pos.x = position.x + m_OffSetWidth * scale;
	pVtx[3].pos.y = position.y + height * scale;
	pVtx[3].pos.z = 0.0f;

	return pVtx;
}