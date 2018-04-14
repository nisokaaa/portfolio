#pragma once
//=================================================================================
//	プログラム名	：	
//	処理			：	シェーダ処理[shader.h]
//	メイン処理		：	main.cpp
//	作成者			：	moriya gaku
//=================================================================================

//=================================================================================
//	二重定義防止
//=================================================================================
#ifndef _SHADER_H_
#define _SHADER_H_

//=================================================================================
//	インクルードファイル
//=================================================================================
#include "main.h"

class CShader
{
public:
	CShader() {}
	~CShader() {}

	virtual HRESULT Init(void) { return S_OK; }
	virtual const LPD3DXCONSTANTTABLE& GetConstantVS(void) { return NULL; }
	virtual const LPD3DXCONSTANTTABLE& GetConstantPS(void) { return NULL; }
	virtual const LPDIRECT3DTEXTURE9& GetNoTexture(void) { return NULL; }
	virtual void Begin(void) {}
	virtual void SendData(void) {}
	virtual void End(void) {}
	virtual void SetMatrix(const D3DXMATRIX& mtxWorld) {}
	virtual void Uninit(void) = 0;
};

#endif