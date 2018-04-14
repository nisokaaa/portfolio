//=================================================================================
//	�v���O������	�F	
//	����			�F	�X�R�A����[score.cpp]
//	���C������		�F	main.cpp
//	�쐬��			�F	moriya gaku
//=================================================================================

//=================================================================================
// i�C���N���[�h�t�@�C��
//=================================================================================
#include "score.h"
#include "renderer.h"
#include "textureManager.h"

//=================================================================================
// ����������
//=================================================================================
HRESULT CScore::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	//���_�̍쐬
	MakeVertexScore(pDevice);

	for (int i = 0; i < m_nDigit; i++)
		m_vfDigitU.push_back(0);

	return S_OK;
}

//=================================================================================
// �I������
//=================================================================================
void CScore::Uninit(void)
{
	if(m_pVtxBuffer != NULL)
	{
		m_pVtxBuffer->Release();	//�e�N�X�^�C���^�[�t�F�C�X���
		m_pVtxBuffer = NULL;		//���g�̑|��
	}
}

//=================================================================================
// �X�V����
//=================================================================================
void CScore::Update(void)
{
	// ���W�擾
	D3DXVECTOR3 pos = GetPosition();

	//���z�A�h���X
	VERTEX_2D* pVtx = NULL;

	//���_�o�b�t�@�����b�N���ĉ��z�A�h���X���擾����
	m_pVtxBuffer -> Lock( 0 , 0 , ( void** ) &pVtx , 0 );

	for(int i = 0 ; i < m_nDigit ; i++)
	{
		//UV���W�̐ݒ�
		pVtx[4 * i + 0].tex = D3DXVECTOR2(m_vfDigitU[i], 0);
		pVtx[4 * i + 1].tex = D3DXVECTOR2(m_vfDigitU[i] + 0.1f, 0);
		pVtx[4 * i + 2].tex = D3DXVECTOR2(m_vfDigitU[i], 1);
		pVtx[4 * i + 3].tex = D3DXVECTOR2(m_vfDigitU[i] + 0.1f, 1);

		//���_���W�̐ݒ�(2D���W�E�E���ŌĂԂ��ƁI)   ( x , y , z )
		pVtx[4 * i + 0].pos = D3DXVECTOR3(pos.x + (m_fTexFontSize.x + m_fInterval) * i, pos.y ,0.0f);//����
		pVtx[4 * i + 1].pos = D3DXVECTOR3(pos.x + m_fTexFontSize.x + (m_fTexFontSize.x + m_fInterval) * i, pos.y, 0.0f);//�E��
		pVtx[4 * i + 2].pos = D3DXVECTOR3(pos.x + (m_fTexFontSize.x + m_fInterval) * i, pos.y + m_fTexFontSize.y , 0.0f);	//����
		pVtx[4 * i + 3].pos = D3DXVECTOR3(pos.x + m_fTexFontSize.x + (m_fTexFontSize.x + m_fInterval) * i, pos.y + m_fTexFontSize.y, 0.0f);//�E��
	}
	//���b�N����������
	m_pVtxBuffer -> Unlock();

	for(int i = 0 ; i < m_nDigit ; i++)
	{
		SetUVScore(i, m_nValue);
	}
}

//=================================================================================
// �`�揈��
//=================================================================================
void CScore::Draw(void)
{	
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	pDevice->SetStreamSource(0, m_pVtxBuffer, 0, sizeof(VERTEX_2D));
	
	pDevice->SetFVF(FVF_VERTEX_2D);
	
	pDevice->SetTexture(0 ,CTextureManager::GetResource(m_Key));
	
	for(int nIndex = 0; nIndex < m_nDigit; nIndex++)
	{
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nIndex, NUM_POLYGON);
	}
}

//=================================================================================
// �����֐�
//=================================================================================
CScore *CScore::Create(std::string key, D3DXVECTOR2 pos, D3DXVECTOR2 size, int digit, float interval , int priority)
{
	CScore *p = NULL;
	p = new CScore(priority);
	
	CTextureManager::Load(key);
	p->m_Key = key;
	p->SetPosition(D3DXVECTOR3(pos.x, pos.y, 0.0f));
	p->m_nDigit = digit;
	p->m_fInterval = interval;
	p->m_fTexFontSize = D3DXVECTOR2(size.x * 0.1f, size.y);

	p->Init();

	return p;
}

//=================================================================================
// ���_�̍쐬
//=================================================================================
void CScore::MakeVertexScore(LPDIRECT3DDEVICE9 pDevice)
{	
	// ���W�擾
	D3DXVECTOR3 pos = GetPosition();

	//���_�o�b�t�@�\�̍쐬
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * m_nDigit, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuffer, NULL);
	
	VERTEX_2D* pVtx;

	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);
	
	for(int i = 0; i < m_nDigit; i++)
	{
		//���_���W�̐ݒ�(2D���W�E�E���ŌĂԂ��ƁI)   ( x , y , z )
		pVtx[4 * i + 0].pos = D3DXVECTOR3(pos.x + m_fInterval * i, pos.y, 0.0f);//����
		pVtx[4 * i + 1].pos = D3DXVECTOR3(pos.x + m_fTexFontSize.x * i + m_fInterval * i, pos.y, 0.0f);//�E��
		pVtx[4 * i + 2].pos = D3DXVECTOR3(pos.x + m_fInterval * i, pos.y + m_fTexFontSize.y, 0.0f);	//����
		pVtx[4 * i + 3].pos = D3DXVECTOR3(pos.x + m_fTexFontSize.x * i + m_fInterval * i, pos.y + m_fTexFontSize.y, 0.0f);//�E��
		
		//rhw�̐ݒ�(�K��1.0������)
		pVtx[4 * i + 0].rhw = 1.0;
		pVtx[4 * i + 1].rhw = 1.0;
		pVtx[4 * i + 2].rhw = 1.0;
		pVtx[4 * i + 3].rhw = 1.0;
		//���_�J���[
		pVtx[4 * i + 0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[4 * i + 1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[4 * i + 2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[4 * i + 3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		//�e�N�X�`����UV�l
		pVtx[4 * i + 0].tex = D3DXVECTOR2(0.0, 0.0);
		pVtx[4 * i + 1].tex = D3DXVECTOR2(1.0, 0.0);
		pVtx[4 * i + 2].tex = D3DXVECTOR2(0.0, 1.0);
		pVtx[4 * i + 3].tex = D3DXVECTOR2(1.0, 1.0);
	}

	//���b�N����
	m_pVtxBuffer -> Unlock();
}

//=================================================================================
// UV�̎w��
//=================================================================================
void CScore::SetUVScore(int DigitCount,int value)
{
	int DigitNum;

	for(int nCount = 0; nCount < DigitCount; nCount++)
	{
		value /= 10;
	}
	DigitNum = value % 10;
	m_vfDigitU[m_nDigit -1  -DigitCount] = 0.1 * DigitNum;
}

//=================================================================================
// �X�R�A���Z
//=================================================================================
void CScore::AddScore(int value)
{
	// �������E�l�v�Z
	std::string sVal = "";
	for (int i = 0; i < m_nDigit; i++)
		sVal += "9";
	int maxVal = std::atoi(sVal.c_str());

	if (m_nValue < maxVal) m_nValue += value;
}

//=================================================================================
// �X�R�A���Z
//=================================================================================
void CScore::SubScore(int value)
{
	if(m_nValue > 0) m_nValue -= value;
}

//=================================================================================
// �X�R�A�擾
//=================================================================================
int CScore::GetScore(void){
	return m_nValue;
}
