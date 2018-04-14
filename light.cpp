//=================================================================================
//	プログラム名	：	
//	処理			：	ライト処理[light.cpp]
//	メイン処理		：	main.cpp
//	作成者			：	moriya gaku
//=================================================================================
//=================================================================================
//	インクルードファイル
//=================================================================================
#include "main.h"
#include "light.h"
#include "renderer.h"
#include "input.h"
#include "sceneManager.h"
#include "manager.h"
#include "debug.h"

//=================================================================================
//	コンストラクタ
//=================================================================================
CLight::CLight()
{
	ZeroMemory(&m_Light, sizeof(D3DLIGHT9));
}

//=================================================================================
//　デストラクタ
//=================================================================================
CLight::~CLight()
{
}

//=================================================================================
//	初期化関数
//=================================================================================
void CLight::Init(LIGHT_TYPE type, D3DXVECTOR3 vecDir, D3DXVECTOR3 position, D3DXCOLOR color, D3DXVECTOR3 diffuse, D3DXVECTOR3 ambient)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	m_Color = color;

	m_Type = type;

	m_Light.Type = D3DLIGHT_DIRECTIONAL;

	// 照射方向
	D3DXVec3Normalize((D3DXVECTOR3*)&m_Light.Direction, &vecDir);

	// 位置
	m_Light.Position.x = position.x;
	m_Light.Position.y = position.y;
	m_Light.Position.z = position.z;

	// 拡散光
	m_Light.Diffuse.r = diffuse.x;
	m_Light.Diffuse.g = diffuse.y;
	m_Light.Diffuse.b = diffuse.z;

	// 環境光
	m_Light.Ambient.r = ambient.x;
	m_Light.Ambient.g = ambient.y;
	m_Light.Ambient.b = ambient.z;

	// レンダリングパイプラインに設定
	pDevice->SetLight(type, &m_Light);

	// 設定
	pDevice->LightEnable(type, TRUE);

	// ライティングモード
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

}

//=================================================================================
//	終了関数
//=================================================================================
void CLight::Uninit(void)
{
	return;
}

//=================================================================================
//	更新関数
//=================================================================================
void CLight::Update(void)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	if (CManager::GetInputKeyboard()->GetKeyPress(DIK_Z))
	{
		m_Light.Direction.x -= 0.05f;
	}
	if (CManager::GetInputKeyboard()->GetKeyPress(DIK_X))
	{
		m_Light.Direction.x += 0.05f;
	}

	D3DXVec3Normalize((D3DXVECTOR3*)&m_Light.Direction, (D3DXVECTOR3*)&m_Light.Direction);

	// レンダリングパイプラインに設定
	pDevice->SetLight(m_Type, &m_Light);

	// 設定
	pDevice->LightEnable(m_Type, TRUE);

	// ライティングモード
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	return;
}