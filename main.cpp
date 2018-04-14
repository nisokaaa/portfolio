//=================================================================================
//	�v���O������	�F	
//	����			�F	���C������[main.cpp]
//	���C������		�F	main.cpp
//	�쐬��			�F	moriya gaku
//=================================================================================

//=================================================================================
//	�C���N���[�h�t�@�C��
//=================================================================================
#include "main.h"
#include "manager.h"

//=================================================================================
//	�}�N����`
//=================================================================================
#define CLASS_NAME					"AppClass"				// �E�C���h�E�̃N���X��
#define WINDOW_NAME					"SCOURGE HIVE"			// �E�C���h�E�̃L���v�V������
#define DEBUG_FONT_SIZE	( 20 )								// �f�o�b�O���b�Z�[�W�p�t�H���g�T�C�Y

//=================================================================================
//�@�v���g�^�C�v�錾
//=================================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//=================================================================================
//�@�O���[�o���ϐ�
//=================================================================================
#ifdef _DEBUG
int						g_nCountFPS;			// FPS�J�E���^
#endif

//=================================================================================
//�@���C���֐�
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
	
	// �E�B���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	// �w�肵���N���C�A���g�̈���m�ۂ��邽�߂ɕK�v�ȃE�B���h�E���W���v�Z
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

	// �E�B���h�E�̍쐬
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

	// �E�C���h�E�̕\��
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// �Q�[���J�n
	if( FAILED( CManager::Init( hInstance, hWnd, true ) ) )
	{
		return -1;
	}

	// ����\��ݒ�
	timeBeginPeriod(1);

	// �t���[���J�E���g������
	dwCurrentTime =
	dwFrameCount = 0;
	dwExecLastTime = 
	dwFPSLastTime = timeGetTime();
	
	// ���b�Z�[�W���[�v
	while(1)
	{
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
			{// PostQuitMessage()���Ă΂ꂽ�烋�[�v�I��
				break;
			}
			else
			{
				// ���b�Z�[�W�̖|��ƃf�B�X�p�b�`
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
        }
		else
		{
			dwCurrentTime = timeGetTime();
			if((dwCurrentTime - dwFPSLastTime) >= 500)	// 0.5�b���ƂɎ��s
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

				// �}�l�[�W���[�̍X�V
				CManager::Update();

				// �}�l�[�W���[�̕`��
				CManager::Draw();

				dwFrameCount++;
			}
		}
	}

	// �Q�[���I��
	CManager::Uninit();
	
	// �E�B���h�E�N���X�̓o�^������
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// ����\��߂�
	timeEndPeriod(1);


	return (int)msg.wParam;
}

//=================================================================================
//�@�v���V�[�W��
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
		case VK_ESCAPE:					// [ESC]�L�[�������ꂽ
			DestroyWindow(hWnd);		// �E�B���h�E��j������悤�w������
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
//�@FPS�̕`�揈��
//=================================================================================
int GetFPS( void )
{
	return g_nCountFPS;
}
#endif

//=================================================================================
//�@�����_���l�����@3.14��n���� -3.14 �` 3.14 �̃����_���l��Ԃ�
//=================================================================================
float GetRand( float value )
{
	// ���ʗp�ϐ�
	float resultVal = 0;
	int randVal = 0;

	// �����֕ϊ�
	randVal = ( (int)value * 2 ) * 100;
	
	// �����_���l����(�O�`�U�Q�W)
	resultVal = (float)(rand() % randVal);

	// �}�C�i�X�l���l��(�O�`�U�Q�W - �R�P�S�j
	resultVal = resultVal - (value * 100);

	// �����ɕϊ�
	resultVal = resultVal / 100;

	return resultVal;
}

//=================================================================================
//�@�����_���l�����@�͈͎w��
//=================================================================================
float GetRandRnage( float min, float max )
{
	return min + ( (float)rand() * (max - min + 1.0f) / (1.0f + RAND_MAX) );
}

//=================================================================================
//�@(x, y, z, w)�x�N�g���ƍs��̏�Z
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
//�@�Q���W���x�N�g���擾�֐�
//=================================================================================
D3DXVECTOR3* CalcPositionToVector( D3DXVECTOR3* pout, const D3DXVECTOR3& at, const D3DXVECTOR3& pos )
{
	D3DXVec3Normalize( pout, pout );
	*pout = at - pos;
	D3DXVec3Normalize( pout, pout );
	return pout;
}

//=================================================================================
//�@2�x�N�g���Ԃ̂Ȃ��p���v�Z
//=================================================================================
float CalcVecAngle( D3DXVECTOR3& vec1, D3DXVECTOR3& vec2 )
{
	D3DXVec3Normalize( &vec1, &vec1 );		// �����P��
	D3DXVec3Normalize( &vec2, &vec2 );

	// 2�x�N�g���Ԃ̊p�x�擾
	float bunsi = D3DXVec3Dot( &vec1, &vec2 );
	float bunbo = D3DXVec3Length( &vec1 ) * D3DXVec3Length( &vec2 );

	float  shou = 0.0f;

	if( bunsi != 0.0f && bunbo != 0.0f )
	{
		shou = bunsi / bunbo;

		// ���q�܂��͕��ꂪ1�̂Ƃ����͕K�����Ғl��Ԃ��Ȃ��̂ŃG���[
		if( shou < -1.0f || shou > 1.0f ) return 0.0f;
	}

	// �O�ςŕ������f
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
//�@���Ƌ��̓����蔻��
//=================================================================================
bool CheckColSphere( D3DXVECTOR3& pos1, D3DXVECTOR3& pos2, float length1, float length2 )
{
	// ���������߂�
	D3DXVECTOR3 difference = pos2 - pos1;

	float distance = D3DXVec3Length( &difference );

	if( distance < length1+length2 ) return true;

	return false;

}

//=================================================================================
//�@�L�����N�^�p���s��Z�o�֐�
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