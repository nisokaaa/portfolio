#pragma once
//=================================================================================
//	プログラム名	：	
//	処理			：	サウンド処理[sound.h]
//	メイン処理		：	main.cpp
//	作成者			：	moriya gaku
//=================================================================================

//=================================================================================
//	二重定義防止
//=================================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

//=================================================================================
//	インクルードファイル
//=================================================================================
#include "main.h"

//=================================================================================
//	サウンドファイル
//=================================================================================
typedef enum
{
	SOUND_LABEL_BGM_TITLE = 0,
	SOUND_LABEL_BGM_GAME,
	SOUND_LABEL_BGM_RESULT,
	SOUNC_LABEL_SE_TITLE_ENTER,
	SOUNC_LABEL_SE_TITLE_SELECT,
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//=================================================================================
//	プロトタイプ宣言
//=================================================================================
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
void UpdateSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif