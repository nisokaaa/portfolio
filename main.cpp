//=================================================================================
//	プログラム名	：	
//	処理			：	メイン処理[main.cpp]
//	メイン処理		：	main.cpp
//	作成者			：	moriya gaku
//=================================================================================

//=================================================================================
//	インクルードファイル
//=================================================================================
#include "main.h"
#include "manager.h"

//=================================================================================
//	マクロ定義
//=================================================================================
#define CLASS_NAME					"AppClass"				// ウインドウのクラス名
#define WINDOW_NAME					"SCOURGE HIVE"			// ウインドウのキャプション名
#define DEBUG_FONT_SIZE	( 20 )								// デバッグメッセージ用フォントサイズ

//=================================================================================
//　プロトタイプ宣言
//=================================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//=================================================================================
//　グローバル変数
//=================================================================================
#ifdef _DEBUG
int						g_nCountFPS;			// FPSカウンタ
#endif

//=================================================================================
//　メイン関数
//=================================================================================
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		hInstance,
		NULL,
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		CLASS_NAME,
		NULL
	};
	RECT rect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
	HWND hWnd;
	MSG msg;
	DWORD dwCurrentTime;
	DWORD dwFrameCount;
	DWORD dwExecLastTime;
	DWORD dwFPSLastTime;
	
	// ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	// 指定したクライアント領域を確保するために必要なウィンドウ座標を計算
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

	// ウィンドウの作成
	hWnd = CreateWindow(CLASS_NAME,
						WINDOW_NAME,
						WS_OVERLAPPEDWINDOW,
						CW_USEDEFAULT,
						CW_USEDEFAULT,
						(rect.right - rect.left),
						(rect.bottom - rect.top),
						NULL,
						NULL,
						hInstance,
						NULL);

	// ウインドウの表示
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// ゲーム開始
	if( FAILED( CManager::Init( hInstance, hWnd, true ) ) )
	{
		return -1;
	}

	// 分解能を設定
	timeBeginPeriod(1);

	// フレームカウント初期化
	dwCurrentTime =
	dwFrameCount = 0;
	dwExecLastTime = 
	dwFPSLastTime = timeGetTime();
	
	// メッセージループ
	while(1)
	{
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
			{// PostQuitMessage()が呼ばれたらループ終了
				break;
			}
			else
			{
				// メッセージの翻訳とディスパッチ
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
        }
		else
		{
			dwCurrentTime = timeGetTime();
			if((dwCurrentTime - dwFPSLastTime) >= 500)	// 0.5秒ごとに実行
			{
#ifdef _DEBUG
				g_nCountFPS = dwFrameCount * 1000 / (dwCurrentTime - dwFPSLastTime);
#endif
				dwFPSLastTime = dwCurrentTime;
				dwFrameCount = 0;
			}

			if((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{
				dwExecLastTime = dwCurrentTime;

				// マネージャーの更新
				CManager::Update();

				// マネージャーの描画
				CManager::Draw();

				dwFrameCount++;
			}
		}
	}

	// ゲーム終了
	CManager::Uninit();
	
	// ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// 分解能を戻す
	timeEndPeriod(1);


	return (int)msg.wParam;
}

//=================================================================================
//　プロシージャ
//=================================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_CREATE:
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_ESCAPE:					// [ESC]キーが押された
			DestroyWindow(hWnd);		// ウィンドウを破棄するよう指示する
			break;
		}
		break;

	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

#ifdef _DEBUG
//=================================================================================
//　FPSの描画処理
//=================================================================================
int GetFPS( void )
{
	return g_nCountFPS;
}
#endif

//=================================================================================
//　ランダム値生成　3.14を渡すと -3.14 ～ 3.14 のランダム値を返す
//=================================================================================
float GetRand( float value )
{
	// 結果用変数
	float resultVal = 0;
	int randVal = 0;

	// 整数へ変換
	randVal = ( (int)value * 2 ) * 100;
	
	// ランダム値生成(０～６２８)
	resultVal = (float)(rand() % randVal);

	// マイナス値を考慮(０～６２８ - ３１４）
	resultVal = resultVal - (value * 100);

	// 小数に変換
	resultVal = resultVal / 100;

	return resultVal;
}

//=================================================================================
//　ランダム値生成　範囲指定
//=================================================================================
float GetRandRnage( float min, float max )
{
	return min + ( (float)rand() * (max - min + 1.0f) / (1.0f + RAND_MAX) );
}

//=================================================================================
//　(x, y, z, w)ベクトルと行列の乗算
//=================================================================================
void Vec4Transform(D3DXVECTOR4 *pOut, D3DXVECTOR4 *pV, D3DXMATRIX *pM)
{
    float vx, vy, vz, vw;
    D3DXVECTOR4 vec;
    float *pF1, *pF2, *pF3, *pF4;

    vx = pV->x;
    vy = pV->y;
    vz = pV->z;
    vw = pV->w;

    pF1 = pM->m[0];
    pF2 = pM->m[1];
    pF3 = pM->m[2];
    pF4 = pM->m[3];
    vec.x = vx * (*pF1) + vy * (*pF2) + vz * (*pF3) + vw * (*pF4);
    pF1++; pF2++; pF3++; pF4++;

    vec.y = vx * (*pF1) + vy * (*pF2) + vz * (*pF3) + vw * (*pF4);
    pF1++; pF2++; pF3++; pF4++;

    vec.z = vx * (*pF1) + vy * (*pF2) + vz * (*pF3) + vw * (*pF4);
    pF1++; pF2++; pF3++; pF4++;

    vec.w = vx * (*pF1) + vy * (*pF2) + vz * (*pF3) + vw * (*pF4);

    *pOut = vec;
}

//=================================================================================
//　２座標→ベクトル取得関数
//=================================================================================
D3DXVECTOR3* CalcPositionToVector( D3DXVECTOR3* pout, const D3DXVECTOR3& at, const D3DXVECTOR3& pos )
{
	D3DXVec3Normalize( pout, pout );
	*pout = at - pos;
	D3DXVec3Normalize( pout, pout );
	return pout;
}

//=================================================================================
//　2ベクトル間のなす角を計算
//=================================================================================
float CalcVecAngle( D3DXVECTOR3& vec1, D3DXVECTOR3& vec2 )
{
	D3DXVec3Normalize( &vec1, &vec1 );		// 長さ１に
	D3DXVec3Normalize( &vec2, &vec2 );

	// 2ベクトル間の角度取得
	float bunsi = D3DXVec3Dot( &vec1, &vec2 );
	float bunbo = D3DXVec3Length( &vec1 ) * D3DXVec3Length( &vec2 );

	float  shou = 0.0f;

	if( bunsi != 0.0f && bunbo != 0.0f )
	{
		shou = bunsi / bunbo;

		// 分子または分母が1のとき商は必ず期待値を返さないのでエラー
		if( shou < -1.0f || shou > 1.0f ) return 0.0f;
	}

	// 外積で符号判断
	D3DXVECTOR3 vecOut;
	D3DXVec3Cross( &vecOut, &vec2, &vec1 ); 

	float rad = 0.0f;

	if( vecOut.y >= 0.0f ){
		rad = acos( shou );
	}else{
		rad = -acos( shou );
	}

	return rad;
}

//=================================================================================
//　球と球の当たり判定
//=================================================================================
bool CheckColSphere( D3DXVECTOR3& pos1, D3DXVECTOR3& pos2, float length1, float length2 )
{
	// 距離を求める
	D3DXVECTOR3 difference = pos2 - pos1;

	float distance = D3DXVec3Length( &difference );

	if( distance < length1+length2 ) return true;

	return false;

}

//=================================================================================
//　キャラクタ姿勢行列算出関数
//=================================================================================
D3DXMATRIX* CalcLookAtMatrix(
   D3DXMATRIX* pout,
   D3DXVECTOR3* pPos,
   D3DXVECTOR3* pLook,
   D3DXVECTOR3* pUp
) {
   D3DXVECTOR3 X, Y, Z;
   Z = *pLook - *pPos;
   D3DXVec3Normalize( &Z, &Z );
   D3DXVec3Cross( &X, D3DXVec3Normalize(&Y, pUp), &Z );
   D3DXVec3Normalize( &X, &X );
   D3DXVec3Normalize( &Y, D3DXVec3Cross( &Y, &Z, &X ));


   pout->_11 = X.x; pout->_12 = X.y; pout->_13 = X.z; pout->_14 = 0;
   pout->_21 = Y.x; pout->_22 = Y.y; pout->_23 = Y.z; pout->_24 = 0;
   pout->_31 = Z.x; pout->_32 = Z.y; pout->_33 = Z.z; pout->_34 = 0;
   pout->_41 = 0.0f; pout->_42 = 0.0f; pout->_43 = 0.0f; pout->_44 = 1.0f;

   return pout;
}