//=================================================================================
//	�v���O������	�F	
//	����			�F	���C�g����[light.h]
//	���C������		�F	main.cpp
//	�쐬��			�F	moriya gaku
//=================================================================================

//=================================================================================
//	��d��`�h�~
//=================================================================================
#ifndef _LIGHT_H_
#define _LIGHT_H_

typedef enum {
	MAIN_LIGHT = 0,
}LIGHT_TYPE;

class CLight
{
public:
	CLight();
	~CLight();

	void Init(LIGHT_TYPE type, D3DXVECTOR3 vecDir, D3DXVECTOR3 position, D3DXCOLOR color, D3DXVECTOR3 diffuse, D3DXVECTOR3 ambient);
	void Uninit(void);
	void Update(void);

	const D3DLIGHT9& GetLight(void) const { return m_Light; }

	D3DXVECTOR3 GetPosition(void) { return D3DXVECTOR3(m_Light.Position.x, m_Light.Position.y, m_Light.Position.z); }

	D3DXCOLOR& GetColor(void) { return m_Color; }

	D3DXVECTOR3 GetAmbient(void) { return D3DXVECTOR3(m_Light.Ambient.r, m_Light.Ambient.g, m_Light.Ambient.b); }

	D3DXVECTOR3 GetAttenuation(void) { return D3DXVECTOR3(m_Light.Attenuation0, m_Light.Attenuation1, m_Light.Attenuation2); }

	const D3DXVECTOR3& GetVecDir(void) const { return D3DXVECTOR3(m_Light.Direction); }

	void SetVecDir(const D3DXVECTOR3 dir) { (D3DXVECTOR3)m_Light.Direction = dir; }

private:
	D3DLIGHT9 m_Light;
	D3DXCOLOR m_Color;
	int m_Type;
};

#endif