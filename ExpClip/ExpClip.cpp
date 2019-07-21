#include <windows.h>
#include <CommCtrl.h>
#include "resource.h"

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
LPARAM CALLBACK MainDlgProc(HWND hWnd, UINT uMessage,
							 WPARAM wParam, LPARAM lParam);


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

	hTab = GetDlgItem(hWnd, IDC_TAB1);

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
int APIENTRY WinMain(HINSTANCE hInstCurrent, HINSTANCE hinstPrevious,
					 LPSTR lpszCmdLine, int nCmdShow)
{
	MSG        msg;
	WNDCLASS   wndclass;
	
	hInst = hInstCurrent;

	//���C���E�C���h�E�N���X�̓o�^
	wndclass.style         = 0;
	wndclass.lpfnWndProc   = MainDlgProc;
	wndclass.cbClsExtra    = 0;
	wndclass.cbWndExtra    = DLGWINDOWEXTRA;
	wndclass.hInstance     = hInst;
	wndclass.hIcon         = LoadIcon(NULL,IDI_APPLICATION);
	wndclass.hCursor       = LoadCursor(NULL,IDC_ARROW);
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
	hDlg = CreateDialog(hInst,
		MAKEINTRESOURCE(IDD_DIALOG1), 0, (DLGPROC)MainDlgProc);
	if(hDlg == NULL)
	{
		MessageBox(NULL,TEXT("���C���E�C���h�E���쐬�ł��܂���ł���"),
				szAppName,MB_APPLMODAL | MB_ICONSTOP);
		return FALSE;
	}
	
	//���C���E�C���h�E�̕\��
	ShowWindow(hDlg, nCmdShow);
	UpdateWindow(hDlg);

	//���b�Z�[�W���[�v
	while(GetMessage(&msg, NULL, 0, 0))
	{
		if(IsDialogMessage(hDlg, &msg) == FALSE)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return msg.wParam;
}

//--------------------------------------------------------------
// �T�v: ���C���_�C�A���O�v���V�[�W��
// �߂�l:
// ����:
//--------------------------------------------------------------
LPARAM CALLBACK MainDlgProc(HWND hWnd, UINT uMessage,
							 WPARAM wParam, LPARAM lParam)
{
	switch(uMessage){
	//�E�C���h�E�쐬���̏���
	case WM_INITDIALOG:
		MakeTabControl(hWnd, wParam, lParam);
		SetWindowText(hWnd, szAppName);
		break;
		
	//���[�U�[���b�Z�[�W����
	case WM_COMMAND:
		switch(LOWORD(wParam)){
		//�I��
		case IDCANCEL:
			DestroyWindow(hWnd);
			break;
		
		default:
			return DefWindowProc(hWnd, uMessage, wParam, lParam);
		}
		break;
		
	//[X]�{�^��
	case WM_CLOSE:
		PostMessage(hWnd, WM_COMMAND, IDCANCEL, 0);
		break;
	
	//�v���O�����I�����̃E�C���h�E�j��
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, uMessage, wParam, lParam);
	}
	
	return FALSE;
}


