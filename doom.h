#pragma once
//=================================================================================
//	プログラム名	：	
//	処理			：	ドーム処理[doom.h]
//	メイン処理		：	main.cpp
//	作成者			：	moriya gaku
//=================================================================================

//=================================================================================
//	二重定義防止
//=================================================================================
#ifndef _DOOM_H_
#define _DOOM_H_

//=================================================================================
//	インクルードファイル
//=================================================================================
#include "main.h"
#include "mesh.h"

class CDoom : public CMesh
{
public:
	CDoom();
	~CDoom(){};

	HRESULT MakeVertexBuffer( void );	// 頂点バッファ作成
	void SetVertex( void );				// 頂点情報セット
	void SetMatrix(void);				// ワールド行列計算・デバイスにセット

	static CDoom *Create( D3DXVECTOR3 pos, D3DXVECTOR3 rot, float width, float height,int numWidth, int numHeight, float angleXY, float angleXZ, const char* texPath );
	float AngleToRadian( float angle );	// 角度→ラジアン

	void SetAngleXY( const float angleXY ){ m_AngleXY = angleXY; }
	const float& GetAngleXY( void ) const { return m_AngleXY; }

	void SetAngleXZ( const float angleXZ ){ m_AngleXZ = angleXZ; }
	const float& GetAngleXZ( void ) const { return m_AngleXZ; }


private:
	float m_AngleXY;					// XY軸間の角度(90で半球)
	float m_AngleXZ;					// XZ軸間の角度(361で半球)

};

#endif