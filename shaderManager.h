#pragma once
//=================================================================================
//	プログラム名	：	
//	処理			：	シェーダ情報一括管理[shaderManager.h]
//	メイン処理		：	sceneManager.cpp
//	作成者			：	moriya gaku
//=================================================================================
//=================================================================================
//	二重定義防止
//=================================================================================
#ifndef _SHADER_MANAGER_H_
#define _SHADER_MANAGER_H_

//=================================================================================
//	インクルードファイル
//=================================================================================
#include "main.h"
#include "shader.h"
#include "shaderBasic.h"
#include "shaderMeshField.h"
#include "shaderUpPassFilter.h"
#include "shaderDownPassFilter.h"
#include "shaderHorizontalBlurFilter.h"
#include "shaderVerticalBlurFilter.h"

typedef enum
{
	SHADER_BASIC = 0,
	SHADER_UP_PASS_FILTTER,
	SHADER_DOWN_PASS_FILTTER,
	SHADER_HORIZONAL_BLUR_FILTTER,
	SHADER_VERTICAL_BLUR_FILTTER,
	SHADER_MESHFIELD,
	SHADER_BACKGROUND,
}SHADER_TYPE;

class CShaderManager
{
public:
	typedef std::map < SHADER_TYPE, CShader* > SHADERMAP;

	CShaderManager();
	~CShaderManager();

	void Init(void);
	
	void Release(SHADER_TYPE, CShader* pShader);
	
	void ReleaseAll(void);
	
	CShader* GetData(SHADER_TYPE type);

private:
	SHADERMAP m_ShaderMap;
};

#endif