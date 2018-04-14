#pragma once
//=================================================================================
//	プログラム名	：	
//	処理			：	メイン処理[main.h]
//	メイン処理		：	main.cpp
//	作成者			：	moriya gaku
//=================================================================================

//=================================================================================
//	二重定義防止
//=================================================================================
#ifndef _MAIN_H_
#define _MAIN_H_

//=================================================================================
//	インクルードファイル
//=================================================================================
#include <windows.h>
#include <cstdio>
#include <stdio.h>
#include "d3dx9.h"
#define DIRECTINPUT_VERSION	( 0x0800 )
#include <dinput.h>
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <time.h>
#include <stdlib.h>
#include <typeinfo.h>

//=================================================================================
//	名前空間宣言
//=================================================================================
using namespace std;

//=================================================================================
//	グローバル型定義
//=================================================================================
typedef map < std::string, LPDIRECT3DTEXTURE9 > TEXMAP;

//=================================================================================
//	グローバルマクロ定義
//=================================================================================
#define SCREEN_WIDTH		(1760)				// ウインドウの幅
#define SCREEN_HEIGHT		(990)				// ウインドウの高さ
#define SCREEN_CENTER_X		(SCREEN_WIDTH / 2)	// ウインドウの中心Ｘ座標
#define SCREEN_CENTER_Y		(SCREEN_HEIGHT / 2)	// ウインドウの中心Ｙ座標
#define SafeRelease(p)      if(p){p->Release();p=NULL;}
#define SafeDelete(p)		if(p){delete p;p=NULL;}
#define SafeUninit(p)		if(p){p->Uninit();delete p;p=NULL;}
#define _CRT_SECURE_NO_WARNINGS
#define NUM_VERTEX			( 4 )				// 頂点数
#define NUM_POLYGON			( 2 )				// ポリゴン数
#define NO_TEXTURE			("data/TEXTURE/no_material.png")

#define FVF_VERTEX_2D (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)
typedef struct
{
	D3DXVECTOR3 pos;	// 頂点座標
	float rhw;			// 座標変換用係数(1.0fで固定)
	D3DCOLOR col;		// 頂点カラー
	D3DXVECTOR2 tex;	// テクスチャ座標
} VERTEX_2D;

#define	FVF_VERTEX_3D		(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)
typedef struct
{
	D3DXVECTOR3 pos;	// 頂点座標
	D3DXVECTOR3 nor;	// 法線
	D3DCOLOR col;		// 頂点カラー
	D3DXVECTOR2 tex;	// テクスチャ座標
} VERTEX_3D;

#define COLOR_WHITE		(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f))
#define COLOR_BLACK		(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f))
#define COLOR_GRAY		(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f))
#define COLOR_GRAY0		(D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f))
#define COLOR_GRAY1		(D3DXCOLOR(0.65f, 0.65f, 0.65f, 1.0f))
#define COLOR_GRAY2		(D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f))
#define COLOR_GRAY3		(D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f))
#define COLOR_RED0		(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f))
#define COLOR_RED1		(D3DXCOLOR(1.0f, 0.28f, 0.0f, 1.0f))
#define COLOR_RED2		(D3DXCOLOR(1.0f ,0.62f ,0.019f, 1.0f))
#define COLOR_RED3		(D3DXCOLOR(1.0f ,0.95f ,0.019f, 1.0f))
#define COLOR_GREEN0	(D3DXCOLOR(0.019f, 1.0f, 0.019f, 1.0f))
#define COLOR_GREEN1	(D3DXCOLOR(0.019f, 1.0f, 0.34f, 1.0f))
#define COLOR_GREEN2	(D3DXCOLOR(0.31f, 1.0f, 0.77f, 1.0f))
#define COLOR_GREEN3	(D3DXCOLOR(0.61f, 1.0f, 0.87f, 1.0f))
#define COLOR_BLUE0		(D3DXCOLOR(0.019f, 0.019f, 1.0f, 1.0f))
#define COLOR_BLUE1		(D3DXCOLOR(0.019f, 0.34f, 1.0f, 1.0f))
#define COLOR_BLUE2		(D3DXCOLOR(0.31f, 0.77f, 1.0f, 1.0f))
#define COLOR_BLUE3		(D3DXCOLOR(0.61f, 0.87f, 1.0f, 1.0f))

//=================================================================================
// ゲーム設定マクロ定義
//=================================================================================
#define CREATE_ENEMY					// 敵を発生させる
//#undef CREATE_ENEMY

//=================================================================================
// ライブラリファイル
// [構成プロパティ]->[リンカー]->[入力]->[追加の依存ファイル]に記述しても可能
//=================================================================================
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "dinput8.lib")	// キーパッド取得に必要

//=================================================================================
// プロトタイプ宣言
//=================================================================================
int GetFPS( void );

// ランダム値生成　3.14を渡すと -3.14 ～ 3.14 のランダム値を返す
float GetRand( float value );

// ランダム値生成(範囲指定)
float GetRandRnage( float min, float max );

// (x, y, z, w)ベクトルと行列の乗算
void Vec4Transform(D3DXVECTOR4 *pOut, D3DXVECTOR4 *pV, D3DXMATRIX *pM);

// ２座標→ベクトル取得関数
D3DXVECTOR3* CalcPositionToVector( D3DXVECTOR3* pout, const D3DXVECTOR3& at, const D3DXVECTOR3& pos );

// 2ベクトル間のなす角を計算
float CalcVecAngle( D3DXVECTOR3& vec1, D3DXVECTOR3& vec2 );

// 球と球の当たり判定
bool CheckColSphere( D3DXVECTOR3& pos1, D3DXVECTOR3& pos2, float length1, float lenfth2 );

// キャラクタ姿勢行列算出関数
D3DXMATRIX* CalcLookAtMatrix( D3DXMATRIX* pout, D3DXVECTOR3* pPos, D3DXVECTOR3* pLook, D3DXVECTOR3* pUp );

#endif