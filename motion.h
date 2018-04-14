#ifndef _MOTION_H_
#define _MOTION_H_

#include "main.h"

struct PART {
	//ここはモデルマネージャーへ(まとめてIDになる)
	LPD3DXMESH Mesh;			//メッシュインターフェースポインタ
	LPD3DXBUFFER MatBuff;		//マテリアル情報
	DWORD nNumMat;				//マテリアル数

	//必須情報
	D3DXVECTOR3 Positon;		//座標
	D3DXVECTOR3 Rotation;		//回転
	D3DXVECTOR3 Scaling;		//拡大率
	D3DXMATRIX Matrix;			//行列

	PART* Parent;				//親
};

struct KEY {
	D3DXVECTOR3 Positon;		//座標
	D3DXVECTOR3 Rotation;		//回転
};

struct KEY_SET {
	int nFrame;		//次の状態(KeySet)へ移行するまでにかける時間
	std::vector<KEY*> vecKey;
};

struct MOTION {
	std::vector<KEY_SET*> vecKeySet;
	bool bLoop;
};

class CMotion {
public:
	CMotion();
	~CMotion();

	bool LoadData(const char* file);
	void UnloadData(void);

	void Update(void);
	void Draw(D3DXMATRIX* mtxWorld);

	std::vector<PART*>* GetParts(void) { return &m_vecParts; }
	std::vector<MOTION*>* GetMotion(void) { return &m_vecMotion; }
	//std::vector<PART*>* GetNowData(void) { return &m_vecNowData; }

	void SetMotion(char No);
	void SetMotionInfo(void);

	float GetTopPosY(void);

	bool GetMotionEndFrameFlg(void) { return m_bMotionEndFrame; }

private:
	bool LoadOffsetData(FILE* fp);
	void UnloadOffsetData(void);

	bool LoadMotionData(FILE* fp);
	void UnloadMotionData(void);

	void SetNowData(void);
	//void ReleaseData(void);

	void Interpolation(void);

	std::vector<PART*> m_vecParts;			//メッシュ情報
	std::vector<MOTION*> m_vecMotion;		//モーションの情報
	//std::vector<PART*> m_vecNowData;			//現在のパーツの情報

	char m_cNowMotion;
	char m_cNowKeyset;
	char m_cNextKeyset;
	int m_nMotionCounter;

	bool m_bMotionEndFrame;

	const char* m_filename;
};

#endif	//_MOTION_H_