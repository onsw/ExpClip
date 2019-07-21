#include <windows.h>
#include <CommCtrl.h>
#include "resource.h"

//==============================================================
// 定数
//==============================================================
static TCHAR  szClassName[]=TEXT("ExpClipClass");
static TCHAR  szAppName[]=TEXT("ExpClip");
static TCHAR  szAppVer[]=TEXT("1.0");

//==============================================================
// グローバル変数
//==============================================================
HWND		hDlg;         // メインダイアログハンドル
HINSTANCE   hInst;		  // インスタンスハンドル


//==============================================================
// 関数プロトタイプ宣言
//==============================================================
LPARAM CALLBACK MainDlgProc(HWND hWnd, UINT uMessage,
							 WPARAM wParam, LPARAM lParam);


//--------------------------------------------------------------
// 概要: タブコントロールの作成
// 戻り値:
// 引数:
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
// 概要: メイン関数
// 戻り値:
// 引数:
//--------------------------------------------------------------
int APIENTRY WinMain(HINSTANCE hInstCurrent, HINSTANCE hinstPrevious,
					 LPSTR lpszCmdLine, int nCmdShow)
{
	MSG        msg;
	WNDCLASS   wndclass;
	
	hInst = hInstCurrent;

	//メインウインドウクラスの登録
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
		MessageBox(NULL,TEXT("ウインドウクラスを登録できませんでした"),
				szAppName,MB_APPLMODAL | MB_ICONSTOP);
		return FALSE;
	}
	
	//メインウインドウの生成
	hDlg = CreateDialog(hInst,
		MAKEINTRESOURCE(IDD_DIALOG1), 0, (DLGPROC)MainDlgProc);
	if(hDlg == NULL)
	{
		MessageBox(NULL,TEXT("メインウインドウを作成できませんでした"),
				szAppName,MB_APPLMODAL | MB_ICONSTOP);
		return FALSE;
	}
	
	//メインウインドウの表示
	ShowWindow(hDlg, nCmdShow);
	UpdateWindow(hDlg);

	//メッセージループ
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
// 概要: メインダイアログプロシージャ
// 戻り値:
// 引数:
//--------------------------------------------------------------
LPARAM CALLBACK MainDlgProc(HWND hWnd, UINT uMessage,
							 WPARAM wParam, LPARAM lParam)
{
	switch(uMessage){
	//ウインドウ作成時の処理
	case WM_INITDIALOG:
		MakeTabControl(hWnd, wParam, lParam);
		SetWindowText(hWnd, szAppName);
		break;
		
	//ユーザーメッセージ処理
	case WM_COMMAND:
		switch(LOWORD(wParam)){
		//終了
		case IDCANCEL:
			DestroyWindow(hWnd);
			break;
		
		default:
			return DefWindowProc(hWnd, uMessage, wParam, lParam);
		}
		break;
		
	//[X]ボタン
	case WM_CLOSE:
		PostMessage(hWnd, WM_COMMAND, IDCANCEL, 0);
		break;
	
	//プログラム終了時のウインドウ破棄
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, uMessage, wParam, lParam);
	}
	
	return FALSE;
}


