#ifndef _MOTION_H_
#define _MOTION_H_

#include "main.h"

struct PART {
	//�����̓��f���}�l�[�W���[��(�܂Ƃ߂�ID�ɂȂ�)
	LPD3DXMESH Mesh;			//���b�V���C���^�[�t�F�[�X�|�C���^
	LPD3DXBUFFER MatBuff;		//�}�e���A�����
	DWORD nNumMat;				//�}�e���A����

	//�K�{���
	D3DXVECTOR3 Positon;		//���W
	D3DXVECTOR3 Rotation;		//��]
	D3DXVECTOR3 Scaling;		//�g�嗦
	D3DXMATRIX Matrix;			//�s��

	PART* Parent;				//�e
};

struct KEY {
	D3DXVECTOR3 Positon;		//���W
	D3DXVECTOR3 Rotation;		//��]
};

struct KEY_SET {
	int nFrame;		//���̏��(KeySet)�ֈڍs����܂łɂ����鎞��
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

	std::vector<PART*> m_vecParts;			//���b�V�����
	std::vector<MOTION*> m_vecMotion;		//���[�V�����̏��
	//std::vector<PART*> m_vecNowData;			//���݂̃p�[�c�̏��

	char m_cNowMotion;
	char m_cNowKeyset;
	char m_cNextKeyset;
	int m_nMotionCounter;

	bool m_bMotionEndFrame;

	const char* m_filename;
};

#endif	//_MOTION_H_