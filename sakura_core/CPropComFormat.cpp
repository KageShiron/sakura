/*!	@file
	共通設定ダイアログボックス、「書式」ページ

	@author Norio Nakatani
*/
/*
	Copyright (C) 1998-2001, Norio Nakatani
	Copyright (C) 2000, jepro
	Copyright (C) 2001, MIK, genta
	Copyright (C) 2002, YAZAKI, MIK, aroka
	Copyright (C) 2003, KEITA
	Copyright (C) 2006, ryoji

	This source code is designed for sakura editor.
	Please contact the copyright holder to use this code for other purpose.
*/

#include "StdAfx.h"
#include "CPropCommon.h"
#include "etc_uty.h"
#include "Debug.h" // 2002/2/10 aroka
#include "sakura.hh"


//@@@ 2001.02.04 Start by MIK: Popup Help
static const DWORD p_helpids[] = {	//10400
	IDC_EDIT_DFORM,						HIDC_EDIT_DFORM,		//日付書式
	IDC_EDIT_TFORM,						HIDC_EDIT_TFORM,		//時刻書式
	IDC_EDIT_DFORM_EX,					HIDC_EDIT_DFORM_EX,		//日付書式（表示例）
	IDC_EDIT_TFORM_EX,					HIDC_EDIT_TFORM_EX,		//時刻書式（表示例）
	IDC_EDIT_MIDASHIKIGOU,				HIDC_EDIT_MIDASHIKIGOU,	//見出し記号
	IDC_EDIT_INYOUKIGOU,				HIDC_EDIT_INYOUKIGOU,	//引用符
	IDC_RADIO_DFORM_0,					HIDC_RADIO_DFORM_0,		//日付書式（標準）
	IDC_RADIO_DFORM_1,					HIDC_RADIO_DFORM_1,		//日付書式（カスタム）
	IDC_RADIO_TFORM_0,					HIDC_RADIO_TFORM_0,		//時刻書式（標準）
	IDC_RADIO_TFORM_1,					HIDC_RADIO_TFORM_1,		//時刻書式（カスタム）
//	IDC_STATIC,							-1,
	0, 0
};
//@@@ 2001.02.04 End

//@@@ 2002.01.12 add start
static const char *p_date_form[] = {
	"yyyy'年'M'月'd'日'",
	"yyyy'年'M'月'd'日('dddd')'",
	"yyyy'年'MM'月'dd'日'",
	"yyyy'年'M'月'd'日' dddd",
	"yyyy'年'MM'月'dd'日' dddd",
	"yyyy/MM/dd",
	"yy/MM/dd",
	"yy/M/d",
	"yyyy/M/d",
	"yy/MM/dd' ('ddd')'",
	"yy/M/d' ('ddd')'",
	"yyyy/MM/dd' ('ddd')'",
	"yyyy/M/d' ('ddd')'",
	"yyyy/M/d' ('ddd')'",
	NULL
};

static const char *p_time_form[] = {
	"hh:mm:ss",
	"tthh'時'mm'分'ss'秒'",
	"H:mm:ss",
	"HH:mm:ss",
	"tt h:mm:ss",
	"tt hh:mm:ss",
	NULL
};
//@@@ 2002.01.12 add end

//	From Here Jun. 2, 2001 genta
/*!
	@param hwndDlg ダイアログボックスのWindow Handle
	@param uMsg メッセージ
	@param wParam パラメータ1
	@param lParam パラメータ2
*/
INT_PTR CALLBACK CPropFormat::DlgProc_page(
	HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	return DlgProc( reinterpret_cast<pDispatchPage>(&CPropFormat::DispatchEvent), hwndDlg, uMsg, wParam, lParam );
}
//	To Here Jun. 2, 2001 genta

void CPropFormat::ChangeDateExample( HWND hwndDlg )
{
	/* ダイアログデータの取得 Format */
	GetData( hwndDlg );

	/* 日付をフォーマット */
	TCHAR szText[1024];
	SYSTEMTIME systime;
	::GetLocalTime( &systime );
	CShareData::getInstance()->MyGetDateFormat( systime, szText, _countof( szText ) - 1, m_Common.m_sFormat.m_nDateFormatType, m_Common.m_sFormat.m_szDateFormat );
	::SetDlgItemText( hwndDlg, IDC_EDIT_DFORM_EX, szText );
	return;
}
void CPropFormat::ChangeTimeExample( HWND hwndDlg )
{
	/* ダイアログデータの取得 Format */
	GetData( hwndDlg );

	/* 時刻をフォーマット */
	TCHAR szText[1024];
	SYSTEMTIME systime;
	::GetLocalTime( &systime );
	CShareData::getInstance()->MyGetTimeFormat( systime, szText, _countof( szText ) - 1, m_Common.m_sFormat.m_nTimeFormatType, m_Common.m_sFormat.m_szTimeFormat );
	::SetDlgItemText( hwndDlg, IDC_EDIT_TFORM_EX, szText );
	return;
}


/* Format メッセージ処理 */
INT_PTR CPropFormat::DispatchEvent(
	HWND	hwndDlg,	// handle to dialog box
	UINT	uMsg,	// message
	WPARAM	wParam,	// first message parameter
	LPARAM	lParam 	// second message parameter
)
{
	WORD		wNotifyCode;
	WORD		wID;
	HWND		hwndCtl;
	NMHDR*		pNMHDR;
	NM_UPDOWN*	pMNUD;
	int			idCtrl;
//	int			nVal;

	switch( uMsg ){
	case WM_INITDIALOG:
		/* ダイアログデータの設定 Format */
		SetData( hwndDlg );
		// Modified by KEITA for WIN64 2003.9.6
		::SetWindowLongPtr( hwndDlg, DWLP_USER, lParam );

		ChangeDateExample( hwndDlg );
		ChangeTimeExample( hwndDlg );

		/* 見出し記号 */
		::SendMessage( ::GetDlgItem( hwndDlg, IDC_EDIT_MIDASHIKIGOU ), EM_LIMITTEXT, (WPARAM)(sizeof(m_Common.m_sFormat.m_szMidashiKigou) - 1 ), 0 );

		/* 引用符 */
		::SendMessage( ::GetDlgItem( hwndDlg, IDC_EDIT_INYOUKIGOU ), EM_LIMITTEXT, (WPARAM)(sizeof(m_Common.m_sFormat.m_szInyouKigou) - 1 ), 0 );

		/* 日付書式 */
		::SendMessage( ::GetDlgItem( hwndDlg, IDC_EDIT_DFORM ), EM_LIMITTEXT, (WPARAM)(sizeof(m_Common.m_sFormat.m_szDateFormat) - 1 ), 0 );

		/* 時刻書式 */
		::SendMessage( ::GetDlgItem( hwndDlg, IDC_EDIT_TFORM ), EM_LIMITTEXT, (WPARAM)(sizeof(m_Common.m_sFormat.m_szTimeFormat) - 1 ), 0 );



		return TRUE;
	case WM_COMMAND:
		wNotifyCode	= HIWORD(wParam);	/* 通知コード */
		wID			= LOWORD(wParam);	/* 項目ID､ コントロールID､ またはアクセラレータID */
		hwndCtl		= (HWND) lParam;	/* コントロールのハンドル */
		switch( wNotifyCode ){
		case EN_CHANGE:
			if( IDC_EDIT_DFORM == wID ){
				ChangeDateExample( hwndDlg );
				return 0;
			}
			if( IDC_EDIT_TFORM == wID  ){
				ChangeTimeExample( hwndDlg );
				return 0;
			}
			break;

		/* ボタン／チェックボックスがクリックされた */
		case BN_CLICKED:
			switch( wID ){
			case IDC_RADIO_DFORM_0:
			case IDC_RADIO_DFORM_1:
				ChangeDateExample( hwndDlg );
			//	From Here Sept. 10, 2000 JEPRO
			//	日付書式 0=標準 1=カスタム
			//	日付書式をカスタムにするときだけ書式指定文字入力をEnableに設定
				EnableFormatPropInput( hwndDlg );
			//	To Here Sept. 10, 2000
				return 0;
			case IDC_RADIO_TFORM_0:
			case IDC_RADIO_TFORM_1:
				ChangeTimeExample( hwndDlg );
			//	From Here Sept. 10, 2000 JEPRO
			//	時刻書式 0=標準 1=カスタム
			//	時刻書式をカスタムにするときだけ書式指定文字入力をEnableに設定
				EnableFormatPropInput( hwndDlg );
			//	To Here Sept. 10, 2000
				return 0;




			}
			break;	/* BN_CLICKED */
		}
		break;	/* WM_COMMAND */
	case WM_NOTIFY:
		idCtrl = (int)wParam;
		pNMHDR = (NMHDR*)lParam;
		pMNUD  = (NM_UPDOWN*)lParam;
//		switch( idCtrl ){
//		case ???????:
//			return 0L;
//		default:
			switch( pNMHDR->code ){
			case PSN_HELP:
				OnHelp( hwndDlg, IDD_PROP_FORMAT );
				return TRUE;
			case PSN_KILLACTIVE:
//				MYTRACE_A( "Format PSN_KILLACTIVE\n" );
				/* ダイアログデータの取得 Format */
				GetData( hwndDlg );
				return TRUE;
//@@@ 2002.01.03 YAZAKI 最後に表示していたシートを正しく覚えていないバグ修正
			case PSN_SETACTIVE:
				m_nPageNum = ID_PAGENUM_FORMAT;
				return TRUE;
			}
//			break;	/* default */
//		}

//		MYTRACE_A( "pNMHDR->hwndFrom=%xh\n", pNMHDR->hwndFrom );
//		MYTRACE_A( "pNMHDR->idFrom  =%xh\n", pNMHDR->idFrom );
//		MYTRACE_A( "pNMHDR->code    =%xh\n", pNMHDR->code );
//		MYTRACE_A( "pMNUD->iPos    =%d\n", pMNUD->iPos );
//		MYTRACE_A( "pMNUD->iDelta  =%d\n", pMNUD->iDelta );
		break;	/* WM_NOTIFY */

//@@@ 2001.02.04 Start by MIK: Popup Help
	case WM_HELP:
		{
			HELPINFO *p = (HELPINFO *)lParam;
			MyWinHelp( (HWND)p->hItemHandle, m_pszHelpFile, HELP_WM_HELP, (ULONG_PTR)(LPVOID)p_helpids );	// 2006.10.10 ryoji MyWinHelpに変更に変更
		}
		return TRUE;
		/*NOTREACHED*/
		break;
//@@@ 2001.02.04 End

//@@@ 2001.12.22 Start by MIK: Context Menu Help
	//Context Menu
	case WM_CONTEXTMENU:
		MyWinHelp( hwndDlg, m_pszHelpFile, HELP_CONTEXTMENU, (ULONG_PTR)(LPVOID)p_helpids );	// 2006.10.10 ryoji MyWinHelpに変更に変更
		return TRUE;
//@@@ 2001.12.22 End

	}
	return FALSE;
}





/* ダイアログデータの設定 Format */
void CPropFormat::SetData( HWND hwndDlg )
{

	/* 見出し記号 */
	::SetDlgItemText( hwndDlg, IDC_EDIT_MIDASHIKIGOU, m_Common.m_sFormat.m_szMidashiKigou );

	/* 引用符 */
	::SetDlgItemText( hwndDlg, IDC_EDIT_INYOUKIGOU, m_Common.m_sFormat.m_szInyouKigou );


	//日付書式のタイプ
	if( 0 == m_Common.m_sFormat.m_nDateFormatType ){
		::CheckDlgButton( hwndDlg, IDC_RADIO_DFORM_0, BST_CHECKED );
	}else{
		::CheckDlgButton( hwndDlg, IDC_RADIO_DFORM_1, BST_CHECKED );
	}
	//日付書式
	::SetDlgItemText( hwndDlg, IDC_EDIT_DFORM, m_Common.m_sFormat.m_szDateFormat );

	//時刻書式のタイプ
	if( 0 == m_Common.m_sFormat.m_nTimeFormatType ){
		::CheckDlgButton( hwndDlg, IDC_RADIO_TFORM_0, BST_CHECKED );
	}else{
		::CheckDlgButton( hwndDlg, IDC_RADIO_TFORM_1, BST_CHECKED );
	}
	//時刻書式
	::SetDlgItemText( hwndDlg, IDC_EDIT_TFORM, m_Common.m_sFormat.m_szTimeFormat );

	//	From Here Sept. 10, 2000 JEPRO
	//	日付/時刻書式 0=標準 1=カスタム
	//	日付/時刻書式をカスタムにするときだけ書式指定文字入力をEnableに設定
	EnableFormatPropInput( hwndDlg );
	//	To Here Sept. 10, 2000

	return;
}




/* ダイアログデータの取得 Format */
int CPropFormat::GetData( HWND hwndDlg )
{
//@@@ 2002.01.03 YAZAKI 最後に表示していたシートを正しく覚えていないバグ修正
//	m_nPageNum = ID_PAGENUM_FORMAT;

	/* 見出し記号 */
	::GetDlgItemText( hwndDlg, IDC_EDIT_MIDASHIKIGOU, m_Common.m_sFormat.m_szMidashiKigou, sizeof(m_Common.m_sFormat.m_szMidashiKigou) );

//	/* 外部ヘルプ１ */
//	::GetDlgItemText( hwndDlg, IDC_EDIT_EXTHELP1, m_Common.m_szExtHelp1, MAX_PATH - 1 );
//
//	/* 外部HTMLヘルプ */
//	::GetDlgItemText( hwndDlg, IDC_EDIT_EXTHTMLHELP, m_Common.m_szExtHtmlHelp, MAX_PATH - 1 );

	/* 引用符 */
	::GetDlgItemText( hwndDlg, IDC_EDIT_INYOUKIGOU, m_Common.m_sFormat.m_szInyouKigou, sizeof(m_Common.m_sFormat.m_szInyouKigou) );


	//日付書式のタイプ
	if( BST_CHECKED == ::IsDlgButtonChecked( hwndDlg, IDC_RADIO_DFORM_0 ) ){
		m_Common.m_sFormat.m_nDateFormatType = 0;
	}else{
		m_Common.m_sFormat.m_nDateFormatType = 1;
	}
	//日付書式
	::GetDlgItemText( hwndDlg, IDC_EDIT_DFORM, m_Common.m_sFormat.m_szDateFormat, sizeof( m_Common.m_sFormat.m_szDateFormat ));

	//時刻書式のタイプ
	if( BST_CHECKED == ::IsDlgButtonChecked( hwndDlg, IDC_RADIO_TFORM_0 ) ){
		m_Common.m_sFormat.m_nTimeFormatType = 0;
	}else{
		m_Common.m_sFormat.m_nTimeFormatType = 1;
	}

	//時刻書式
	::GetDlgItemText( hwndDlg, IDC_EDIT_TFORM, m_Common.m_sFormat.m_szTimeFormat, sizeof( m_Common.m_sFormat.m_szTimeFormat ));

	return TRUE;
}





//	From Here Sept. 10, 2000 JEPRO
//	チェック状態に応じてダイアログボックス要素のEnable/Disableを
//	適切に設定する
void CPropFormat::EnableFormatPropInput( HWND hwndDlg )
{
	//	日付書式をカスタムにするかどうか
	if( ::IsDlgButtonChecked( hwndDlg, IDC_RADIO_DFORM_1 ) ){
		::EnableWindow( ::GetDlgItem( hwndDlg, IDC_LABEL_DFORM ), TRUE );
		::EnableWindow( ::GetDlgItem( hwndDlg, IDC_EDIT_DFORM ), TRUE );
	}else{
		::EnableWindow( ::GetDlgItem( hwndDlg, IDC_LABEL_DFORM ), FALSE );
		::EnableWindow( ::GetDlgItem( hwndDlg, IDC_EDIT_DFORM ), FALSE );
	}

	//	時刻書式をカスタムにするかどうか
	if( ::IsDlgButtonChecked( hwndDlg, IDC_RADIO_TFORM_1 ) ){
		::EnableWindow( ::GetDlgItem( hwndDlg, IDC_LABEL_TFORM ), TRUE );
		::EnableWindow( ::GetDlgItem( hwndDlg, IDC_EDIT_TFORM ), TRUE );
	}else{
		::EnableWindow( ::GetDlgItem( hwndDlg, IDC_LABEL_TFORM ), FALSE );
		::EnableWindow( ::GetDlgItem( hwndDlg, IDC_EDIT_TFORM ), FALSE );
	}
}
//	To Here Sept. 10, 2000


/*[EOF]*/
