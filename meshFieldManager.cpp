//=================================================================================
//	プログラム名	：	
//	処理			：	メッシュフィールド情報一括管理[meshFieldManager.cpp]
//	メイン処理		：	sceneManager.cpp
//	作成者			：	moriya gaku
//=================================================================================

//=================================================================================
//	インクルードファイル
//=================================================================================
#include "meshFieldManager.h"
#include "debug.h"
#include "file.h"

//=================================================================================
//  コンストラクタ
//=================================================================================
CMeshFieldManager::CMeshFieldManager()
{
}

//=================================================================================
//  デストラクタ
//=================================================================================
CMeshFieldManager::~CMeshFieldManager()
{
}

//=================================================================================
//	マップ捜索(データ取得)関数
//=================================================================================
CMeshField *CMeshFieldManager::GetData(MESHFIELD_TYPE key)
{
	// オブジェクトを検索
	MESHFIELDMAP::iterator it = m_mpMeshFieldMap.find(key);

	if (it != m_mpMeshFieldMap.end())
	{
		return it->second;
	}

	return NULL;
}

//=================================================================================
//  ファイル読み込み関数
//=================================================================================
CMeshField *CMeshFieldManager::Load(const std::string& filename, MESHFIELD_TYPE key, const int& priority)
{
	if (GetData(key))return NULL;

	//	ファイル読み込み
	FILE  *pFile;
	fopen_s(&pFile, filename.c_str(), "r");

	if (pFile == NULL)
	{
		MessageBox(NULL, "ファイルを読み込めませんでした", "メッシュプレイン生成エラー", MB_OK);
		return NULL;
	}
	//	変数宣言
	D3DXVECTOR3 Pos, Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR2 BlockSize;
	char aTextureName[2][MAX_PATH];
	bool bSetUnEven = false;
	int nBlockX, nBlockY;
	int nTexture = 0;

	char aBuf[256];
	std::vector<std::vector<float>> vHeight;

	while (DivideToken(pFile, " \t\n", aBuf) >= 0)
	{
		//	スクリプト開始
		if (strcmp("SCRIPT", aBuf) == 0)
		{
			while (DivideToken(pFile, " \t\n", aBuf) >= 0)
			{
				//	プレイン情報
				if (strcmp("PLANESET", aBuf) == 0)
				{
					while (DivideToken(pFile, " \t\n", aBuf) >= 0)
					{
						//座標情報
						if (strcmp("POS", aBuf) == 0)
						{
							DivideToken(pFile, " \t\n", aBuf);
							//	x座標
							DivideToken(pFile, " \t\n", aBuf);
							Pos.x = (float)atof(aBuf);
							//	y座標
							DivideToken(pFile, " \t\n", aBuf);
							Pos.y = (float)atof(aBuf);
							//	z座標
							DivideToken(pFile, " \t\n", aBuf);
							Pos.z = (float)atof(aBuf);
						}

						//	プロック情報
						if (strcmp("BLOCK", aBuf) == 0)
						{
							DivideToken(pFile, " \t\n", aBuf);
							//	横ブロック
							DivideToken(pFile, " \t\n", aBuf);
							nBlockX = atoi(aBuf);
							//	縦ブロック
							DivideToken(pFile, " \t\n", aBuf);
							nBlockY = atoi(aBuf);
						}

						//	プロックサイズ情報
						if (strcmp("SIZE", aBuf) == 0)
						{
							DivideToken(pFile, " \t\n", aBuf);
							//	幅
							DivideToken(pFile, " \t\n", aBuf);
							BlockSize.x = (float)atof(aBuf);
							//	高さ
							DivideToken(pFile, " \t\n", aBuf);
							BlockSize.y = (float)atof(aBuf);
						}
						//	テクスチャファイル取得
						if (strcmp("TEXTURE", aBuf) == 0)
						{
							DivideToken(pFile, " \t\n", aBuf);
							DivideToken(pFile, " \t\n", aBuf);
							strcpy_s(aTextureName[nTexture], aBuf);
							nTexture++;
						}
						//	凹凸設定
						if (strcmp("SETUNEVEN", aBuf) == 0)
						{
							DivideToken(pFile, " \t\n", aBuf);

							DivideToken(pFile, " \t\n", aBuf);
							if (strcmp("1", aBuf) == 0)
							{
								bSetUnEven = true;

								vHeight.resize(nBlockY + 1);
								for (int i = 0; i < nBlockY + 1; i++) {
									vHeight[i].resize(nBlockX + 1);
								}
								
								for (int i = 0; i < nBlockY + 1;)
								{
									while (DivideToken(pFile, " \t\n", aBuf) >= 0)
									{
										if (strcmp("HEIGHT", aBuf) == 0)
										{
											DivideToken(pFile, " \t\n", aBuf);
											for (int j = 0; j < nBlockX + 1; j++)
											{
												DivideToken(pFile, " \t\n", aBuf);
												vHeight[i][j] = (float)atof(aBuf);
											}
											i++;
										}
									}
								}
							}

						}
						//	プレイン情報終了
						if (strcmp("END_PLANESET", aBuf) == 0)
						{
							break;
						}
					}
				}
				//	スクリプト終了
				if (strcmp("END_SCRIPT", aBuf) == 0)
				{
					break;
				}
			}
		}
	}

	fclose(pFile);

	//	生成
	CMeshField* pMeshField = CMeshField::Create(Pos, Rot, BlockSize.x, BlockSize.y, nBlockX, nBlockY, vHeight);

	// 追加
	m_mpMeshFieldMap.insert(MESHFIELDMAP::value_type(key, pMeshField));

	return pMeshField;
}


//=================================================================================
//　メッシュフィールド解放
//=================================================================================
void CMeshFieldManager::Unload(MESHFIELD_TYPE key)
{
	// find()でkeyが登録されているか
	// 無い場合の返り値：最後のイテレータ 
	MESHFIELDMAP::const_iterator it = m_mpMeshFieldMap.find(key);

	if (it != m_mpMeshFieldMap.end())
	{
		// LPDIRECTTEXTURE9の解放
		it->second->Uninit();

		// このイテレータを１つ消す
		m_mpMeshFieldMap.erase(it);
	}
}

//=================================================================================
//　メッシュフィールド全解放
//=================================================================================
void CMeshFieldManager::ReleaseAll(void)
{
	// 全イテレータ
	for (MESHFIELDMAP::iterator it = m_mpMeshFieldMap.begin(); it != m_mpMeshFieldMap.end(); )
	{
		// LPDIRECTTEXTURE9の解放
		if (it->second != NULL)
		{
			it->second->Uninit();
			it->second = NULL;

			// このイテレータを１つ消す
			it = m_mpMeshFieldMap.erase(it);
			continue;
		}
		it++;
	}

	m_mpMeshFieldMap.clear();
}