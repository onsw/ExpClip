#include <windows.h>
#include <CommCtrl.h>
#include "resource.h"
#include "ClipControl.h"

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
LPARAM CALLBACK MainWndProc(HWND hWnd, UINT uMessage,
	WPARAM wParam, LPARAM lParam);
LPARAM CALLBACK MainDlgProc(HWND hWnd, UINT uMessage,
							 WPARAM wParam, LPARAM lParam);
bool RegisterNotify(HWND hWnd);


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

	hTab = GetDlgItem(hWnd, TABC_MAIN);

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
int APIENTRY wWinMain(HINSTANCE hInstCurrent, HINSTANCE hinstPrevious,
					 LPWSTR lpszCmdLine, int nCmdShow)
{
	MSG        msg;
	WNDCLASS   wndclass;
	
	hInst = hInstCurrent;

	//メインウインドウクラスの登録
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
		MessageBox(NULL,TEXT("ウインドウクラスを登録できませんでした"),
				szAppName,MB_APPLMODAL | MB_ICONSTOP);
		return FALSE;
	}
	
	//メインウインドウの生成
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
		MessageBox(NULL, TEXT("メインウインドウを作成できませんでした"),
			szAppName, MB_APPLMODAL | MB_ICONSTOP);
		return FALSE;
	}

	HWND hDlg;
	hDlg = CreateDialog(hInst,
		MAKEINTRESOURCE(DLG_MAIN), hWndMain, (DLGPROC)MainDlgProc);

	// タスクトレイに登録
	RegisterNotify(hWndMain);


	// ClipControlの準備
	ClipControl *cc;
	cc = new ClipControl();

	
	//メインウインドウの表示
	ShowWindow(hWndMain, SW_HIDE);
	ShowWindow(hDlg, nCmdShow);
	UpdateWindow(hWndMain);
	UpdateWindow(hDlg);

	//メッセージループ
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
// 概要: メインウィンドウプロシージャ
// 戻り値:
// 引数:
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
// 概要：タスクトレイへの登録
// 戻り値：
// 引数：
//--------------------------------------------------------------
bool RegisterNotify(HWND hWnd)
{
	NOTIFYICONDATA ni;
	memset(&ni, 0, sizeof(NOTIFYICONDATA));

	ni.cbSize = sizeof(NOTIFYICONDATA);
	ni.hWnd = hWnd;

	ni.uID = 1001;

	ni.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;

	// icon ファイルを読み込む
	ni.hIcon = (HICON)LoadImage(0, "icon.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE);

	// タスクトレイのアイコンにマウスイベントが発生したときにくるアプリケーション定義のメッセージ
	// WARNING
	//   WM_APP 以上の数値でないと message が通知されない
	ni.uCallbackMessage = WM_APP + 1;

	// タスクトレイにアイコンを追加したら自分のウィンドウを隠す。
	Shell_NotifyIcon(NIM_ADD, &ni);

	return 0;
}



//--------------------------------------------------------------
// 概要: メインダイアログプロシージャ
// 戻り値:
// 引数:
//--------------------------------------------------------------
LPARAM CALLBACK MainDlgProc(HWND hWnd, UINT uMessage,
							 WPARAM wParam, LPARAM lParam)
{
	int rc = 0;

	switch(uMessage){
	//ウインドウ作成時の処理
	case WM_CREATE:
	case WM_INITDIALOG:

		MakeTabControl(hWnd, wParam, lParam);
		SetWindowText(hWnd, szAppName);
		break;
		
	//ユーザーメッセージ処理
	case WM_COMMAND:
		switch(LOWORD(wParam)){
		//終了
		case IDCANCEL:
			break;
		}
		break;
		
	//[X]ボタン
	case WM_CLOSE:
		rc = MessageBox(NULL, TEXT("終了しますか？"), TEXT("確認"), MB_YESNO);
		if (rc == IDYES) {
			DestroyWindow(hWnd);
		}
		break;
	
	//プログラム終了時のウインドウ破棄
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	}
	
	return FALSE;
}


