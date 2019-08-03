#include <windows.h>
#include <CommCtrl.h>
#include "resource.h"
#include "ClipControl.h"

//==============================================================
// �萔
//==============================================================
static TCHAR  szClassName[]=TEXT("ExpClipClass");
static TCHAR  szAppName[]=TEXT("ExpClip");
static TCHAR  szAppVer[]=TEXT("1.0");

//==============================================================
// �O���[�o���ϐ�
//==============================================================
HWND		hDlg;         // ���C���_�C�A���O�n���h��
HINSTANCE   hInst;		  // �C���X�^���X�n���h��


//==============================================================
// �֐��v���g�^�C�v�錾
//==============================================================
LPARAM CALLBACK MainWndProc(HWND hWnd, UINT uMessage,
	WPARAM wParam, LPARAM lParam);
LPARAM CALLBACK MainDlgProc(HWND hWnd, UINT uMessage,
							 WPARAM wParam, LPARAM lParam);
bool RegisterNotify(HWND hWnd);


//--------------------------------------------------------------
// �T�v: �^�u�R���g���[���̍쐬
// �߂�l:
// ����:
//--------------------------------------------------------------
HWND MakeTabControl(HWND hWnd, WPARAM wp, LPARAM lp)
{
	HWND hTab;
	HINSTANCE hInst;
	TCITEM tcItem;
	TCHAR sTab1_name[] = TEXT("aaa");
	TCHAR sTab2_name[] = TEXT("Tokyo mew mew");

	hTab = GetDlgItem(hWnd, TABC_MAIN);

	tcItem.mask = TCIF_TEXT;
	tcItem.pszText = sTab1_name;
	TabCtrl_InsertItem(hTab , 0 , &tcItem);

	tcItem.pszText = sTab2_name;
	TabCtrl_InsertItem(hTab , 1 , &tcItem);

	return hTab;
}

//--------------------------------------------------------------
// �T�v: ���C���֐�
// �߂�l:
// ����:
//--------------------------------------------------------------
int APIENTRY wWinMain(HINSTANCE hInstCurrent, HINSTANCE hinstPrevious,
					 LPWSTR lpszCmdLine, int nCmdShow)
{
	MSG        msg;
	WNDCLASS   wndclass;
	
	hInst = hInstCurrent;

	//���C���E�C���h�E�N���X�̓o�^
	wndclass.style         = 0;
	wndclass.lpfnWndProc   = MainWndProc;
	wndclass.cbClsExtra    = 0;
	wndclass.cbWndExtra    = DLGWINDOWEXTRA;
	wndclass.hInstance     = hInst;
	wndclass.hIcon         = LoadIcon(hInst, TEXT("ICON_MAIN"));
	wndclass.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)(COLOR_BTNFACE+1);
	wndclass.lpszMenuName  = NULL;
	wndclass.lpszClassName = szClassName;
	if(RegisterClass(&wndclass) == 0)
	{
		MessageBox(NULL,TEXT("�E�C���h�E�N���X��o�^�ł��܂���ł���"),
				szAppName,MB_APPLMODAL | MB_ICONSTOP);
		return FALSE;
	}
	
	//���C���E�C���h�E�̐���
	HWND hWndMain;
	hWndMain = CreateWindowEx(
		0, //WS_EX_TRANSPARENT,
		szClassName,
		szAppName,
		0, //WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		100, 100,
		NULL,
		NULL,
		hInst,
		NULL
	);
	
	if (hWndMain == NULL)
	{
		MessageBox(NULL, TEXT("���C���E�C���h�E���쐬�ł��܂���ł���"),
			szAppName, MB_APPLMODAL | MB_ICONSTOP);
		return FALSE;
	}

	HWND hDlg;
	hDlg = CreateDialog(hInst,
		MAKEINTRESOURCE(DLG_MAIN), hWndMain, (DLGPROC)MainDlgProc);

	// �^�X�N�g���C�ɓo�^
	RegisterNotify(hWndMain);


	// ClipControl�̏���
	ClipControl *cc;
	cc = new ClipControl();

	
	//���C���E�C���h�E�̕\��
	ShowWindow(hWndMain, SW_HIDE);
	ShowWindow(hDlg, nCmdShow);
	UpdateWindow(hWndMain);
	UpdateWindow(hDlg);

	//���b�Z�[�W���[�v
	while(GetMessage(&msg, NULL, 0, 0))
	{
		//if(IsDialogMessage(hDlg, &msg) == FALSE)
		//{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		//}
	}

	return msg.wParam;
}

//--------------------------------------------------------------
// �T�v: ���C���E�B���h�E�v���V�[�W��
// �߂�l:
// ����:
//--------------------------------------------------------------
LPARAM CALLBACK MainWndProc(HWND hWnd, UINT uMessage,
	WPARAM wParam, LPARAM lParam)
{
	int rc;

	switch (uMessage) {

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, uMessage, wParam, lParam);
	}

	return FALSE;
}

//--------------------------------------------------------------
// �T�v�F�^�X�N�g���C�ւ̓o�^
// �߂�l�F
// �����F
//--------------------------------------------------------------
bool RegisterNotify(HWND hWnd)
{
	NOTIFYICONDATA ni;
	memset(&ni, 0, sizeof(NOTIFYICONDATA));

	ni.cbSize = sizeof(NOTIFYICONDATA);
	ni.hWnd = hWnd;

	ni.uID = 1001;

	ni.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;

	// icon �t�@�C����ǂݍ���
	ni.hIcon = (HICON)LoadImage(0, "icon.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE);

	// �^�X�N�g���C�̃A�C�R���Ƀ}�E�X�C�x���g�����������Ƃ��ɂ���A�v���P�[�V������`�̃��b�Z�[�W
	// WARNING
	//   WM_APP �ȏ�̐��l�łȂ��� message ���ʒm����Ȃ�
	ni.uCallbackMessage = WM_APP + 1;

	// �^�X�N�g���C�ɃA�C�R����ǉ������玩���̃E�B���h�E���B���B
	Shell_NotifyIcon(NIM_ADD, &ni);

	return 0;
}



//--------------------------------------------------------------
// �T�v: ���C���_�C�A���O�v���V�[�W��
// �߂�l:
// ����:
//--------------------------------------------------------------
LPARAM CALLBACK MainDlgProc(HWND hWnd, UINT uMessage,
							 WPARAM wParam, LPARAM lParam)
{
	int rc = 0;

	switch(uMessage){
	//�E�C���h�E�쐬���̏���
	case WM_CREATE:
	case WM_INITDIALOG:

		MakeTabControl(hWnd, wParam, lParam);
		SetWindowText(hWnd, szAppName);
		break;
		
	//���[�U�[���b�Z�[�W����
	case WM_COMMAND:
		switch(LOWORD(wParam)){
		//�I��
		case IDCANCEL:
			break;
		}
		break;
		
	//[X]�{�^��
	case WM_CLOSE:
		rc = MessageBox(NULL, TEXT("�I�����܂����H"), TEXT("�m�F"), MB_YESNO);
		if (rc == IDYES) {
			DestroyWindow(hWnd);
		}
		break;
	
	//�v���O�����I�����̃E�C���h�E�j��
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	}
	
	return FALSE;
}


