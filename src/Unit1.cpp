//----------------------------------------------------------------------//
// CapStick																//
//	メインフォーム														//
//																		//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
#include <memory>
#include <algorithm>
#include <math.h>
#include <mmsystem.h>
#include <htmlhelp.h>
#include <Winapi.ShellAPI.hpp>
#include <Vcl.Clipbrd.hpp>
#include <Vcl.Imaging.jpeg.hpp>
#include <Vcl.HtmlHelpViewer.hpp>
#include "usr_wic.h"
#include "UserFunc.h"
#include "Unit1.h"
#include "Unit2.h"

#pragma link "Vcl.HTMLHelpViewer"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TCapStickForm *CapStickForm;

HINSTANCE	  hHHctrl	  = NULL;
FUNC_HtmlHelp lpfHtmlHelp = NULL;

//--------------------------------------------------------------------------
//標準マウスカーソルリスト
TCursor CsrTable[MAX_STD_CSR] = {
	crArrow,
	crCross,
	crIBeam,
	crUpArrow,
	crSizeNESW,
	crSizeNS,
	crSizeNWSE,
	crSizeWE,
	crSizeAll,
	crNo,
	crHourGlass,
	crDrag,
	crNoDrop,
	crHSplit,
	crVSplit,
	crMultiDrag,
	crSQLWait,
	crAppStart,
	crHelp,
	crHandPoint
	};

//--------------------------------------------------------------------------
__fastcall TCapStickForm::TCapStickForm(TComponent* Owner)
	: TForm(Owner)
{
}

//---------------------------------------------------------------------------
void __fastcall TCapStickForm::FormCreate(TObject *Sender)
{
	VersionNo = get_VersionNo();

	ImgBuff  = new Graphics::TBitmap();
	UndoList = new TStringList();

	//ヘルプファイル初期化
	Application->HelpFile = ChangeFileExt(Application->ExeName, ".CHM");
	hHHctrl = ::LoadLibrary(_T("hhctrl.ocx"));
	if (hHHctrl) lpfHtmlHelp = (FUNC_HtmlHelp)::GetProcAddress(hHHctrl, "HtmlHelpW");

	//諸設定の読込
	IniFile = new TIniFile(ChangeFileExt(Application->ExeName, ".INI"));
	UnicodeString sct = "General";
	int lst_vno = IniFile->ReadInteger(sct, "Version",0);
	if (lst_vno<VersionNo) {
		IniFile->WriteInteger(sct, "Version", VersionNo);
		IniFile->DeleteKey(sct,	"LastDefExt");	//※V1.65 で廃止
	}

	LastDir    = IniFile->ReadString( sct, "LastDir", 		EmptyStr);
	LastFilter = IniFile->ReadInteger(sct, "LastFilter",	1);
	LastCurDir = IniFile->ReadString( sct, "LastCurDir",	EmptyStr);

	ImgBgCol = (TColor)IniFile->ReadInteger(sct, "ImgBgColor",clBlack);
	ScrollBox1->Color = ImgBgCol;

	sct = "Option";
	HideWait	 = IniFile->ReadInteger(sct, "HideWait",	100);
	CaptureWait  = IniFile->ReadInteger(sct, "CaptureWait",	3000);
	JpegQuality  = IniFile->ReadInteger(sct, "JpegQuality",	100);
	ScaleOpt	 = IniFile->ReadInteger(sct, "ScaleOpt",	0);
	ExtAllIcon	 = IniFile->ReadBool(   sct, "ExtAllIcon",	true);
	CapCurIdx	 = IniFile->ReadInteger(sct, "CapCurIdx",	0);

	sct = "Sound";
	SoundCapImg = get_filename_r(IniFile->ReadString(sct, "CapImage", EmptyStr));
	SoundChgImg = get_filename_r(IniFile->ReadString(sct, "ChgImage", EmptyStr));

	sct = "OnCopy";
	CopyActWndClass  = IniFile->ReadString(sct, "ActWndClass", EmptyStr);
	CopyActWndTitle  = IniFile->ReadString(sct, "ActWndTitle", EmptyStr);
	CopyActWndTitleR = IniFile->ReadString(sct, "ActWndTitleR", EmptyStr);
	CopyCmdLine 	 = IniFile->ReadString(sct, "CmdLine", EmptyStr);
	CopySendKey 	 = IniFile->ReadString(sct, "SendKey", EmptyStr);

	sct = "OnSave";
	SaveCmdLine = IniFile->ReadString(sct, "CmdLine", EmptyStr);

	crCapture  = (TCursor)5;	//捕獲の杖カーソル
	crCapture2 = (TCursor)6;	//捕獲の杖カーソル
	crPaste    = (TCursor)7;	//貼り付けカーソル

	Screen->Cursors[crCapture]	= ::LoadCursor(HInstance, _T("CAPTURE"));
	Screen->Cursors[crCapture2] = ::LoadCursor(HInstance, _T("CAPTURE2"));

	Capturing  = false;
	Selecting  = false;
	SelMoving  = false;
	CsrPasting = false;
	hCurWnd    = NULL;

	for (unsigned int i=0; i<MAX_STD_CSR; i++) {
		TMenuItem *mp = new TMenuItem(MouseCsrItem);
		mp->Caption   = EmptyStr;
		mp->Tag = (int)CsrTable[i];
		if (i==10) mp->Break = mbBarBreak;
		mp->OnClick 	  = CsrPasteItemClick;
		mp->OnDrawItem	  = MouseCsrItemDrawItem;
		mp->OnMeasureItem = MouseCsrItemMeasureItem;
		MouseCsrItem->Add(mp);
	}

	::DragAcceptFiles(Handle, true);
}
//---------------------------------------------------------------------------
void __fastcall TCapStickForm::FormShow(TObject *Sender)
{
	if (!Capturing) {
		//ウィンドウの表示状態を復元
		load_form_pos(this, IniFile, 240, 320);

		UnicodeString sct = "General";
		ClsNamPanel->Visible   = IniFile->ReadBool(   sct, "ShowClsName",	true);
		WndTxtPanel->Visible   = IniFile->ReadBool(   sct, "ShowWndText",	true);

		ListSheet->TabVisible  = IniFile->ReadBool(   sct, "ShowList",		false);
		if (ListSheet->TabVisible) {
			ScrollBox1->Parent = ImageSheet;
		}
		else {
			PageControl1->Visible = false;
			ScrollBox1->Parent	  = this;
		}

		ColValFmt			   = IniFile->ReadInteger(sct, "ColValFmt",		0);
		PixColPanel->Visible   = (ColValFmt>0);
		StatusBar1->Visible    = IniFile->ReadBool(sct, "ShowSttBar",  true);
		MrkImageList->GetBitmap((CapCurIdx==1)? 2 : 1, CapToolImg->Picture->Bitmap);

		//マウスカーソルをキャプチャーツールの位置へ
		TPoint p = CapToolPanel->ClientToScreen(
			Point(CapToolPanel->Width/2, CapToolPanel->Height/2));
		::SetCursorPos(p.x, p.y);
	}

	Timer1->Enabled = true;
}

//---------------------------------------------------------------------------
void __fastcall TCapStickForm::FormClose(TObject *Sender, TCloseAction &Action)
{
	Timer1->Enabled = false;

	//設定を保存
	save_form_pos(this, IniFile);

	UnicodeString sct = "General";
	IniFile->WriteBool(   sct, "ShowClsName",	ClsNamPanel->Visible);
	IniFile->WriteBool(   sct, "ShowWndText",	WndTxtPanel->Visible);
	IniFile->WriteBool(   sct, "ShowList",		ListSheet->TabVisible);
	IniFile->WriteBool(   sct, "ShowSttBar",	StatusBar1->Visible);
	IniFile->WriteString( sct, "LastDir", 		LastDir);
	IniFile->WriteInteger(sct, "LastFilter",	LastFilter);
	IniFile->WriteString( sct, "LastCurDir",	LastCurDir);
	IniFile->WriteInteger(sct, "ImgBgColor",	(int)ImgBgCol);

	IniFile->WriteInteger(sct, "ColValFmt",		ColValFmt);

	sct = "Option";
	IniFile->WriteInteger(sct, "HideWait",		HideWait);
	IniFile->WriteInteger(sct, "CaptureWait",	CaptureWait);
	IniFile->WriteInteger(sct, "JpegQuality",	JpegQuality);
	IniFile->WriteInteger(sct, "ScaleOpt",		ScaleOpt);
	IniFile->WriteBool(   sct, "ExtAllIcon",	ExtAllIcon);
	IniFile->WriteInteger(sct, "CapCurIdx",		CapCurIdx);

	sct = "Sound";
	IniFile->WriteString( sct, "ChgImage",		SoundChgImg);
	IniFile->WriteString( sct, "CapImage",		SoundCapImg);

	sct = "OnCopy";
	IniFile->WriteString( sct, "ActWndClass",	CopyActWndClass);
	IniFile->WriteString( sct, "ActWndTitle",	CopyActWndTitle);
	IniFile->WriteString( sct, "ActWndTitleR",	CopyActWndTitleR);
	IniFile->WriteString( sct, "CmdLine",		CopyCmdLine);
	IniFile->WriteString( sct, "SendKey",		CopySendKey);

	sct = "OnSave";
	IniFile->WriteString( sct, "CmdLine",		SaveCmdLine);
}
//---------------------------------------------------------------------------
void __fastcall TCapStickForm::FormDestroy(TObject *Sender)
{
	for (int i=0; i<UndoList->Count; i++) delete (Graphics::TBitmap *)UndoList->Objects[i];
	delete UndoList;

	delete ImgBuff;
	delete IniFile;

	if (hHHctrl) ::FreeLibrary(hHHctrl);
}

//---------------------------------------------------------------------------
void __fastcall TCapStickForm::ApplicationEvents1Message(tagMSG &Msg, bool &Handled)
{
	if (!Timer1->Enabled) return;

	//ヘルプのキーメッセージ処理
	if (lpfHtmlHelp) {
		Handled = (lpfHtmlHelp(NULL, NULL, HH_PRETRANSLATEMESSAGE, (DWORD)&Msg) != NULL);
	}
}

//---------------------------------------------------------------------
// ファイルのドロップ
//---------------------------------------------------------------------
void __fastcall TCapStickForm::WMDropFiles(TWMDropFiles &message)
{
	UINT filecount = ::DragQueryFile((HDROP)message.Drop, 0xFFFFFFFF, NULL, 0);

	if (filecount>0) {
		SelRect = Rect(0, 0, 0, 0);
		UnicodeString fnam;
		fnam.SetLength(MAX_PATH);
		int len = ::DragQueryFile((HDROP)message.Drop, 0, fnam.c_str(), fnam.Length());
		fnam.SetLength(len);
		bool flag = true;

		if (!WIC_load_image(fnam, ImgBuff)) {
			ScrollBox1->HorzScrollBar->Range = ScrollBox1->ClientWidth;
			ScrollBox1->VertScrollBar->Range = ScrollBox1->ClientHeight;
			Graphics::TBitmap *bp = Image1->Picture->Bitmap;
			bp->SetSize(Image1->ClientWidth, Image1->ClientHeight);
			Image1->Canvas->Brush->Color = ImgBgCol;
			Image1->Canvas->FillRect(Image1->ClientRect);
			//アイコン抽出
			int ixn = (int)::ExtractIcon(HInstance, fnam.c_str(), -1);
			//取得成功(.exe、.dll 、.ico)
			if (ixn>0) {
				if (!ExtAllIcon) ixn = 1;
				int x = 0;
				int y = 0;
				int w, h;
				bp->SetSize(32, 32);
				for (int i=0; i<ixn; i++) {
					std::unique_ptr<Graphics::TIcon> icn(new Graphics::TIcon());
					icn->Handle = ::ExtractIcon(HInstance, fnam.c_str(), i);
					if (icn->Handle) {
						w = icn->Width;
						h = icn->Height;
						if (bp->Width < x+w)  bp->Width  = x + w;
						if (bp->Height < y+h) bp->Height = y + h;
						Image1->Canvas->FillRect(Rect(x, y, x + w, y + h));
						::DrawIcon(Image1->Canvas->Handle, x, y, icn->Handle);
						::DestroyIcon(icn->Handle);
						if ((x + 2*w)>Image1->ClientWidth) {
							x = 0; y += h;
						}
						else {
							x += w;
						}
					}
				}
				if (ixn>1) SelRect = Rect(0, 0, w, h);
				ImgBuff->Assign(bp);
			}
			//その他のファイル
			else {
				SHFILEINFO sif;
				if (::SHGetFileInfo(fnam.c_str(), 0, &sif, sizeof(SHFILEINFO), SHGFI_ICON|SHGFI_LARGEICON) != 0) {
					bp->SetSize(32, 32);
					::DrawIcon(Image1->Canvas->Handle, 0, 0, sif.hIcon);
					ImgBuff->Assign(bp);
				}
				else {
					flag = false;
				}
			}
		}

		if (flag) {
			DrawImgBuff();
			if (!SoundCapImg.IsEmpty()) ::sndPlaySound(SoundCapImg.c_str(), SND_ASYNC);
		}

		SetSelRect();
	}

	::DragFinish((HDROP)message.Drop);
	::SetForegroundWindow(Handle);
}

//---------------------------------------------------------------------------
//アンドゥの追加
//---------------------------------------------------------------------------
void __fastcall TCapStickForm::AddUndo()
{
	Graphics::TBitmap *bp = new Graphics::TBitmap();
	bp->Assign(ImgBuff);
	UndoList->InsertObject(0, EmptyStr, (TObject*)bp);

	while (UndoList->Count>MAX_UNDO_CNT) {
		int idx = UndoList->Count - 1;
		Graphics::TBitmap *bp = (Graphics::TBitmap *)UndoList->Objects[idx];
		delete bp;
		UndoList->Delete(idx);
	}
}

//---------------------------------------------------------------------------
//画像情報等の表示
//---------------------------------------------------------------------------
void __fastcall TCapStickForm::DispInfo(int w, int h)
{
	UnicodeString tmpstr;
	//確定前のサイズ情報
	if (w>0 && h>0) {
		tmpstr.sprintf(_T("画像サイズ: %4d ×%4d "), w, h);
	}
	//選択枠の位置とサイズ
	if (SelShape->Visible && !SelRect.IsEmpty()) {
		tmpstr = SelMoving? "移動中 " : "選択中 ";
		tmpstr.cat_sprintf(_T("(%4d,%4d-%4d,%4d) : (%3d×%3d)"),
			SelRect.Left, SelRect.Top, SelRect.Right, SelRect.Bottom, SelRect.Width(), SelRect.Height());
	}
	//画像情報
	else if (!ImgBuff->Empty) {
		tmpstr.sprintf(_T("画像サイズ: %4d ×%4d "), ImgBuff->Width, ImgBuff->Height);
		switch (ImgBuff->PixelFormat) {
		case  pf1bit: tmpstr.UCAT_TSTR("(モノクロ)"); break;
		case  pf4bit: tmpstr.UCAT_TSTR("(16色)");	  break;
		case  pf8bit: tmpstr.UCAT_TSTR("(256色)");	  break;
		default:	  ;
		}
	}

	//リスト項目
	if (TextListBox->Count>0) tmpstr.cat_sprintf(_T("    リスト項目数=%u"), TextListBox->Count);
	StatusBar1->Panels->Items[1]->Text = tmpstr;
}

//---------------------------------------------------------------------------
//ImgBuffを画面に描画
//---------------------------------------------------------------------------
void __fastcall TCapStickForm::DrawImgBuff()
{
	Image1->Picture->Bitmap->SetSize(ImgBuff->Width, ImgBuff->Height);
	Image1->Canvas->Draw(0, 0, ImgBuff);
	ScrollBox1->HorzScrollBar->Range = ImgBuff->Width;
	ScrollBox1->VertScrollBar->Range = ImgBuff->Height;
	DispInfo();
	Screen->Cursor = crDefault;
}

//---------------------------------------------------------------------------
//選択枠の更新
//---------------------------------------------------------------------------
void __fastcall TCapStickForm::SetSelRect()
{
	TRect rc = SelRect;
	rc.Offset(-ScrollBox1->HorzScrollBar->Position, -ScrollBox1->VertScrollBar->Position);
	SelShape->BoundsRect = rc;
	SelShape->Visible = !SelRect.IsEmpty();
	DispInfo();
}

//---------------------------------------------------------------------------
//指定ウィンドウのキャプチャー
//---------------------------------------------------------------------------
void __fastcall TCapStickForm::CaptWndImg(HWND hw)
{
	if (!hw) return;

	//クラス名およびタイトルを取得して表示
	_TCHAR tmp[1024];
	ClsNamEdit->Text = (::GetClassName(hw, tmp, 255)>0)?   tmp : _T("");
	WndTxtEdit->Text = (::GetWindowText(hw, tmp, 1023)>0)? tmp : _T("");

	//取り込み準備
	Image1->Canvas->Brush->Color = ImgBgCol;
	Image1->Canvas->FillRect(Image1->ClientRect);
	RECT rc = get_window_rect_ex(hCurWnd);

	if (Screen->MonitorCount==1) {
		//1画面の場合、画面外は除外
		if (rc.left<0) rc.left = 0;
		if (rc.top<0)  rc.top  = 0;
		if (rc.right>Screen->Width)   rc.right	= Screen->Width;
		if (rc.bottom>Screen->Height) rc.bottom = Screen->Height;
	}
	int w = rc.right - rc.left;
	int h = rc.bottom - rc.top;
	if (w<=0 || h<=0) return;

	Timer1->Enabled = false;

	ScrollBox1->HorzScrollBar->Range = w;
	ScrollBox1->VertScrollBar->Range = h;
	Image1->Picture->Bitmap->SetSize(w, h);
	Image1->Canvas->Brush->Color = ImgBgCol;
	Image1->Canvas->FillRect(Image1->ClientRect);

	//ウィンドウリージョンを取得
	HRGN hWrgn = ::CreateRectRgn(0, 0, w, h);
	::GetWindowRgn(hw, hWrgn);
	//画面から取り込み
	HDC ImgDc = Image1->Canvas->Handle;
	::SelectClipRgn(ImgDc, hWrgn);
	HDC scr_dc = ::CreateDC(_T("DISPLAY"), NULL, NULL, NULL);
	::BitBlt(ImgDc, 0, 0, w, h, scr_dc, rc.left, rc.top, SRCCOPY|CAPTUREBLT);
	::ReleaseDC(0, scr_dc);
	::SelectClipRgn(ImgDc, NULL);
	::DeleteObject(hWrgn);
	::DeleteDC(scr_dc);

	//リスト/コンボボックスの一覧を取得
	try {
		std::unique_ptr<TStringList> lst(new TStringList());
		if (ListSheet->TabVisible) {
			StatusBar1->Panels->Items[1]->Text = "リスト内容を取得中...";
			StatusBar1->Repaint();
			if (SameText(ClsNamEdit->Text, "ListBox")) {
				int cnt = ::SendMessage(hw, LB_GETCOUNT, 0, 0);
				if (cnt>0) {
					for (int i=0; i<cnt; i++) {
						int n = ::SendMessage(hw, LB_GETTEXTLEN, i, 0);
						if (n==LB_ERR) break;
						std::unique_ptr<_TCHAR[]> sbuf(new _TCHAR[n + 1]);
						int r = ::SendMessage(hw, LB_GETTEXT, i, (WPARAM)sbuf.get());
						if (r==LB_ERR) break;
						if (r<=n) {
							sbuf[r] = '\0';
							lst->Add(sbuf.get());
						}
					}
				}
			}
			else if (SameText(ClsNamEdit->Text, "ComboBox")) {
				int cnt = ::SendMessage(hw, CB_GETCOUNT, 0, 0);
				if (cnt>0) {
					for (int i=0; i<cnt; i++) {
						int n = ::SendMessage(hw, CB_GETLBTEXTLEN, i, 0);
						if (n==LB_ERR) break;
						std::unique_ptr<_TCHAR[]> sbuf(new _TCHAR[n + 1]);
						int r = ::SendMessage(hw, CB_GETLBTEXT, i, (WPARAM)sbuf.get());
						if (r==LB_ERR) break;
						if (r<=n) {
							sbuf[r] = '\0';
							lst->Add(sbuf.get());
						}
					}
				}
			}
		}
		TextListBox->Items->Assign(lst.get());
	}
	catch (...) {
		TextListBox->Clear();
	}

	DispInfo(w, h);

	Timer1->Enabled = true;
}

//---------------------------------------------------------------------------
//タイマー処理　(インターバル 100ms)
//---------------------------------------------------------------------------
void __fastcall TCapStickForm::Timer1Timer(TObject *Sender)
{
	CopyBtn->Enabled = !ImgBuff->Empty;
	SaveBtn->Enabled = !ImgBuff->Empty;

	UnicodeString msg;

	if (Capturing && !WaitTimeBar->Visible) {
		//マウス位置のウィンドウを取得
		POINT p = Mouse->CursorPos;
		msg.sprintf(_T("%d, %d"), p.x, p.y);
		HWND hPosWnd = ::WindowFromPoint(p);
		//キャプチャー
		if (hCurWnd!=hPosWnd) {
			hCurWnd = hPosWnd;
			CaptWndImg(hCurWnd);
			if (!SoundChgImg.IsEmpty()) ::sndPlaySound(SoundChgImg.c_str(), SND_ASYNC);
		}
		//マウス位置のピクセル色を取得
		if (ColValFmt>0) {
			HDC hDc = ::GetDC(0);
			if (hDc) {
				CapToolImg->Picture->Bitmap->SetSize(36, 36);
				TCanvas *cv = CapToolImg->Picture->Bitmap->Canvas;
				COLORREF col[9];
				for (int i=0; i<3; i++) {
					int xp	= i*12;
					int x_i = p.x + i - 1;
					for (int j=0; j<3; j++) {
						int yp	= j*12;
						int y_j = p.y + j - 1;
						col[i*3+j] = ::GetPixel(hDc, x_i, y_j);
						cv->Brush->Color = (TColor)col[i*3 + j];
						TRect rc = Rect(xp, yp, xp + 12, yp + 12);
						cv->FillRect(rc);
					}
				}
				::ReleaseDC(0, hDc);

				COLORREF cref = col[4];
				TColor c = TColor(cref);
				ColPanel->Color = c;

				int h, s, l;
				RgbToHslI(c, &h, &s, &l);
				bool same_col = true;
				for (int i=1; i<9 && same_col; i++) same_col = (col[i]==col[i - 1]);
				cv->Brush->Color = RatioCol((h>=90 && h<270)? clRed : clAqua, (std::min(std::max(100 - l, 25), 75))/100.0);
				if (!same_col) cv->FrameRect(Rect(12, 12, 24, 24));

				int r = GetRValue(cref);
				int g = GetGValue(cref);
				int b = GetBValue(cref);
				UnicodeString colstr;
				switch (ColValFmt) {
				case 1: colstr.sprintf(_T("%3d,%3d,%3d"),	r, g, b); break;
				case 2: colstr.sprintf(_T("0x%08x"),		cref);	  break;
				case 3: colstr.sprintf(_T("#%02x%02x%02x"), r, g, b); break;
				}
				ColValEdit->Text = colstr;
			}
		}
	}

	StatusBar1->Panels->Items[0]->Text = msg;
}

//---------------------------------------------------------------------------
//キャプチャーモードに入る
//---------------------------------------------------------------------------
void __fastcall TCapStickForm::CapToolImgMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
	if (!Capturing && Button==mbLeft) {
		//バッファをいったん破棄
		ImgBuff->Handle = NULL;
		SelRect = Rect(0, 0, 0, 0);
		SetSelRect();

		//マウスカーソル変更
		Screen->Cursor = (CapCurIdx==1)? crCapture2 : crCapture;
		MrkImageList->GetBitmap(0, CapToolImg->Picture->Bitmap);
		CapToolImg->Repaint();
		Capturing = true;
	}
}
//---------------------------------------------------------------------------
//キャプチャーモードから抜ける
//---------------------------------------------------------------------------
void __fastcall TCapStickForm::CapToolImgMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
	if (!Capturing) return;

	//確定前ウェイト
	if (Shift.Contains(ssCtrl)) {
		if (CaptureWait>0) {
			WaitTimeBar->Visible = true;
			MrkImageList->GetBitmap(3, CapToolImg->Picture->Bitmap);
			CapToolImg->Repaint();
			int start_cnt = GetTickCount();
			while (Capturing) {
				int r = CaptureWait - (GetTickCount() - start_cnt);
				if (r<=0) break;
				WaitTimeBar->Position = (int)(WaitTimeBar->Max * r/CaptureWait);
				Application->ProcessMessages();
				Sleep(500);
			}
			WaitTimeBar->Visible = false;
			if (!Capturing) return;
		}
	}

	//マウス位置のウィンドウを取得
	hCurWnd = ::WindowFromPoint(Mouse->CursorPos);
	if (Button==mbLeft) {
		//対象が自分(の一部)でなければ一旦隠して再度キャプチャー
		RECT brc = BoundsRect;
		RECT rc = get_window_rect_ex(hCurWnd);
		RECT rcx;
		::IntersectRect(&rcx, &brc, &rc);
		if (!EqualRect(&rcx, &rc)) {
			Visible = false;
			Sleep(HideWait);
			CaptWndImg(hCurWnd);
			Visible = true;
		}
		ImgBuff->Assign(Image1->Picture->Bitmap);
		SelRect = Rect(0, 0, 0, 0);
		SetSelRect();
		Capturing = false;

		//カーソルをツールバーの位置へ呼び戻す
		Screen->Cursor = crDefault;
		MrkImageList->GetBitmap((CapCurIdx==1)? 2 : 1, CapToolImg->Picture->Bitmap);
		CapToolImg->Repaint();
		Mouse->CursorPos = ToolPanel->ClientToScreen(Point(ToolBar1->Width + 16, ToolPanel->Height/2));

		if (!SoundCapImg.IsEmpty()) ::sndPlaySound(SoundCapImg.c_str(), SND_ASYNC);
	}
}

//---------------------------------------------------------------------------
//カーソルキーによる選択枠の移動、サイズ変更
//---------------------------------------------------------------------------
void __fastcall TCapStickForm::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	switch (Key) {
	case VK_UP: case VK_DOWN: case VK_LEFT: case VK_RIGHT:
		//貼り付けカーソルの移動
		if (CsrPasting) {
			TPoint p = Mouse->CursorPos;
			switch (Key) {
			case VK_UP:		p.y--; break;
			case VK_DOWN:	p.y++; break;
			case VK_LEFT:	p.x--; break;
			case VK_RIGHT:	p.x++; break;
			}
			Mouse->CursorPos = p;
			Key = 0;
		}
		//選択枠
		else if (!Selecting && !SelMoving && !SelRect.IsEmpty()) {
			orgX = orgY = 0;
			//サイズ変更
			if (Shift.Contains(ssCtrl)) {
				switch (Key) {
				case VK_UP:
					SelRect.Bottom--; break;
				case VK_DOWN:
					if (SelRect.Bottom<ImgBuff->Height) SelRect.Bottom++;
					break;
				case VK_LEFT:
					SelRect.Right--;  break;
				case VK_RIGHT:
					if (SelRect.Right<ImgBuff->Width)SelRect.Right++;
					break;
				}
				SetSelRect();
			}
			//移動
			else {
				int dx = SelShape->Width;
				int dy = SelShape->Height;
				if (Shift.Contains(ssShift)) dx = dy = 1;
				SelMoving = true;
				switch (Key) {
				case VK_UP:		SelShapeMouseMove(NULL, Shift,  0,-dy); break;
				case VK_DOWN:	SelShapeMouseMove(NULL, Shift,  0, dy); break;
				case VK_LEFT:	SelShapeMouseMove(NULL, Shift,-dx,  0); break;
				case VK_RIGHT:	SelShapeMouseMove(NULL, Shift, dx,  0); break;
				}
				SelMoving = false;
				if ((SelShape->Top + SelShape->Height) > ScrollBox1->ClientHeight)
					ScrollBox1->VertScrollBar->Position += dy;
				else if (SelShape->Top<0)
					ScrollBox1->VertScrollBar->Position -= dy;
				DispInfo();
			}
			Key = 0;
		}
		break;
	}
}

//---------------------------------------------------------------------------
//コピー
//---------------------------------------------------------------------------
void __fastcall TCapStickForm::CopyBtnClick(TObject *Sender)
{
	//選択枠内
	if (SelShape->Visible && !SelRect.IsEmpty()) {
		std::unique_ptr<Graphics::TBitmap> bmp(new Graphics::TBitmap());
		int w = SelRect.Width();
		int h = SelRect.Height();
		bmp->SetSize(w, h);
		bmp->Canvas->CopyRect(Rect(0, 0, w, h), ImgBuff->Canvas, SelRect);
		Clipboard()->Assign(bmp.get());
	}
	//全体
	else {
		Clipboard()->Assign(ImgBuff);
	}

	if (!is_KeyDown(VK_SHIFT)) {
		HWND hw = get_hwnd_ex(CopyActWndClass, CopyActWndTitle, CopyActWndTitleR);
		if (hw) {
			if (::IsIconic(hw)) ::SendMessage(hw, WM_SYSCOMMAND, SC_RESTORE, 0);
			::SetForegroundWindow(hw);
		}
		else {
			run_app_wait(CopyCmdLine, 30000);	//T.O. = 30秒
		}

		send_key_ex(CopySendKey, 50);
	}
}

//---------------------------------------------------------------------------
//保存
//---------------------------------------------------------------------------
void __fastcall TCapStickForm::SaveBtnClick(TObject *Sender)
{
	bool doOnSave = !is_KeyDown(VK_SHIFT);

	SavePictureDialog1->Title = "画像を保存";
	SavePictureDialog1->Filter = GraphicFilter(__classid(Graphics::TBitmap))
		+ "|JPEGファイル(*.JPG)|*.JPG|PNGファイル(*.PNG)|*.PNG|GIFファイル(*.GIF)|*.GIF|TIFファイル(*.TIF)|*.TIF";
	SavePictureDialog1->FilterIndex = LastFilter;
	SavePictureDialog1->DefaultExt	= (LastFilter==2)? "JPG" :
									  (LastFilter==3)? "PNG" :
									  (LastFilter==3)? "GIF" :
									  (LastFilter==3)? "TIF" : "BMP";
	SavePictureDialog1->InitialDir = LastDir;
	if (SavePictureDialog1->Execute()) {
		std::unique_ptr<Graphics::TBitmap> bmp(new Graphics::TBitmap());
		if (SelShape->Visible && !SelRect.IsEmpty()) {
			int w = SelRect.Right  - SelRect.Left;
			int h = SelRect.Bottom - SelRect.Top;
			bmp->SetSize(w, h);
			bmp->Canvas->CopyRect(Rect(0, 0, w, h), ImgBuff->Canvas, SelRect);
		}
		else {
			bmp->Assign(ImgBuff);
		}

		UnicodeString fnam = SavePictureDialog1->FileName;
		if (!WIC_save_image(fnam, bmp.get(), JpegQuality))
			msgbox_ERR("画像の保存に失敗しました。");

		LastDir    = ExtractFileDir(fnam);
		LastFilter = SavePictureDialog1->FilterIndex;

		if (doOnSave) {
			UnicodeString cmdline = SaveCmdLine;
			int p = fnam.Pos(" "); if (p>0) fnam = "\""+fnam+"\"";
			p = cmdline.Pos("$F");
			if (p>0)
				cmdline = ReplaceStr(cmdline, "$F", fnam);
			else
				cmdline += (" "+fnam);
			run_app_wait(cmdline);
		}
	}
}

//---------------------------------------------------------------------------
//メニュー表示
//---------------------------------------------------------------------------
void __fastcall TCapStickForm::ActionList1Update(TBasicAction *Action, bool &Handled)
{
	ChgSizeItem->Enabled  = !ImgBuff->Empty;
	ColorItem->Enabled	  = !ImgBuff->Empty;
	ImageItem->Enabled	  = !ImgBuff->Empty;
	MouseCsrItem->Enabled = !ImgBuff->Empty;
}
//---------------------------------------------------------------------------
void __fastcall TCapStickForm::ShowMenu(TObject *Sender)
{
	TPoint p;
	if (dynamic_cast<TToolButton *>(Sender) != NULL) {
		p = ToolBar1->ClientToScreen(Point(MenuBtn->Left + 16, 16));
		ShowItem->Visible	= true;
		OptionItem->Visible = true;
		AboutItem->Visible	= true;
	}
	else {
		p = Mouse->CursorPos;
		ShowItem->Visible	= false;
		OptionItem->Visible = false;
		AboutItem->Visible	= false;
	}
	PopupMenu1->OwnerDraw = true;
	PopupMenu1->Popup(p.x, p.y);
}
//---------------------------------------------------------------------------
void __fastcall TCapStickForm::MouseCsrItemClick(TObject *Sender)
{
	PopupMenu1->OwnerDraw = true;
}
//---------------------------------------------------------------------------
void __fastcall TCapStickForm::ShowItemClick(TObject *Sender)
{
	PopupMenu1->OwnerDraw = false;
}
//---------------------------------------------------------------------------
void __fastcall TCapStickForm::AboutItemClick(TObject *Sender)
{
	PopupMenu1->OwnerDraw = true;
}
//---------------------------------------------------------------------------
void __fastcall TCapStickForm::PopMeasureItem(TObject *Sender,
      TCanvas *ACanvas, int &Width, int &Height)
{
	//幅を調整
	if (((TComponent *)Sender)->Tag>1000) {
		int wd = 44+ACanvas->TextWidth(" Version 0.00");
		if (wd>Width) Width = wd;
	}
	else {
		Width += 8;
	}
	//高さを調整
	Height += 2;
}
//---------------------------------------------------------------------------
void __fastcall TCapStickForm::MouseCsrItemDrawItem(TObject *Sender,
    TCanvas *ACanvas, const TRect &ARect, bool Selected)
{
	ACanvas->Brush->Color = Selected ? clHighlight : clMenu;
	ACanvas->FillRect(ARect);
	::DrawIcon(ACanvas->Handle, ARect.Left + 8, ARect.Top + 2, Screen->Cursors[((TComponent *)Sender)->Tag]);
}
//---------------------------------------------------------------------------
void __fastcall TCapStickForm::MouseCsrItemMeasureItem(TObject *Sender,
      TCanvas *ACanvas, int &Width, int &Height)
{
	Height = 36;
	Width  = 40;
}

//---------------------------------------------------------------------------
//バージョン情報を描画
//---------------------------------------------------------------------------
void __fastcall TCapStickForm::AbautItemDrawItem(TObject *Sender,
      TCanvas *ACanvas, TRect &ARect, bool Selected)
{
	TMenuItem *tp =(TMenuItem*)Sender;
	ACanvas->Brush->Color = clBlack;
	ACanvas->FillRect(ARect);
	RECT rc = RECT(ARect);
	TFontStyles fstyle;

	switch (tp->Tag) {
	case 1001:	//上半分
		ACanvas->Draw(ARect.Left + 4, ARect.Bottom - 16, Application->Icon);
		fstyle.Clear(); fstyle << fsBold;
		ACanvas->Font->Style = fstyle;
		ACanvas->Font->Color = clWhite;
		ACanvas->TextOut(ARect.Left + 44, ARect.Top + 2, "CapStick");
		::DrawEdge(ACanvas->Handle, &rc, BDR_SUNKENOUTER, BF_TOP|BF_LEFT|BF_RIGHT);
		break;
	case 1002:	//下半分
		ACanvas->Draw(ARect.Left + 4, ARect.Top - 16, Application->Icon);
		ACanvas->Font->Color = clWhite;
		ACanvas->TextOut(ARect.Left + 48, ARect.Top + 2,
			"Version "+FloatToStrF(VersionNo/100.0, ffFixed, 4, 2));
		::DrawEdge(ACanvas->Handle, &rc, BDR_SUNKENOUTER, BF_BOTTOM|BF_LEFT|BF_RIGHT);
		break;
	}
}

//---------------------------------------------------------------------------
//サイズ変更
//---------------------------------------------------------------------------
void __fastcall TCapStickForm::ResizeActionExecute(TObject *Sender)
{
	if (ImgBuff->Empty) return;

	AddUndo();
	int w = ImgBuff->Width;
	int h = ImgBuff->Height;

	switch (((TAction *)Sender)->Tag) {
	case  25: w/=4;    h/=4;	break;
	case  50: w/=2;    h/=2;	break;
	case  75: w=w*3/4; h=h*3/4; break;
	case 150: w=w*3/2; h=h*3/2; break;
	case 200: w*=2;    h*=2;	break;
	}

	std::unique_ptr<Graphics::TBitmap> bmp(new Graphics::TBitmap());
	bmp->Assign(ImgBuff);
	WIC_resize_image(bmp.get(), ImgBuff, 0.0, w, h, ScaleOpt);
	DrawImgBuff();
	SelRect = Rect(0, 0, 0, 0);
	SetSelRect();
}
//---------------------------------------------------------------------------
void __fastcall TCapStickForm::ResizeActionUpdate(TObject *Sender)
{
	((TAction *)Sender)->Enabled = !ImgBuff->Empty;
}
//---------------------------------------------------------------------------
//簡易減色
//---------------------------------------------------------------------------
void __fastcall TCapStickForm::ReduceActionExecute(TObject *Sender)
{
	if (ImgBuff->Empty) return;

	ToolPanel->Repaint();

	AddUndo();
	ImgBuff->Assign(Image1->Picture->Bitmap);

	switch (((TAction *)Sender)->Tag) {
	case   2:	//モノクロ 2色
		ImgBuff->PixelFormat = pf1bit;
		break;
	case  16:	//16色
		ImgBuff->PixelFormat = pf4bit;
		break;
	case 256:	//256色
		{
			std::unique_ptr<TJPEGImage> jpgimg(new TJPEGImage());
			jpgimg->Assign(Image1->Picture->Bitmap);
			jpgimg->Compress();
			jpgimg->PixelFormat = jf8Bit;
			ImgBuff->Assign(jpgimg.get());
		}
		break;
	case  -2:	//グレースケール
		WIC_grayscale_image(ImgBuff);
	    break;
	}

	DrawImgBuff();
}
//---------------------------------------------------------------------------
void __fastcall TCapStickForm::ReduceActionUpdate(TObject *Sender)
{
	TAction *ap = (TAction *)Sender;
	TPixelFormat pf = ImgBuff->PixelFormat;
	switch (ap->Tag) {
	case   2:	//モノクロ 2色
		ap->Enabled = (pf!=pf1bit);	break;
	case  16:	//16色
		ap->Enabled = (pf!=pf1bit && pf!=pf4bit);	break;
	case 256:	//256色
	case  -2:	//グレースケール
		ap->Enabled = (pf!=pf1bit && pf!=pf4bit && pf!=pf8bit);	break;
	}
}

//---------------------------------------------------------------------------
//セピア
//---------------------------------------------------------------------------
void __fastcall TCapStickForm::SepiaActionExecute(TObject *Sender)
{
	if (ImgBuff->Empty) return;

	ToolPanel->Repaint();
	AddUndo();
	ImgBuff->PixelFormat = pf24bit;
	double V = -0.10;
	double U =  0.05;
	int n0 = 1.4026*V*255;
	int n1 = 0.3444*U*255 + 0.7114*V*255;
	int n2 = 1.7330*U*255;
	int n;
	for (int j = 0; j<ImgBuff->Height; j++) {
		BYTE *p = (BYTE*)ImgBuff->ScanLine[j];
		for (int i=0; i<ImgBuff->Width; i++,p+=3) {
	        int Y = (p[0]*114 + p[1]*587 + p[2]*299)/1000;
			n = Y + n0;	if (n>255) n = 255; else if (n<0) n = 0;
			p[0] = (BYTE)n;	//B
			n = Y - n1; if (n>255) n = 255; else if (n<0) n = 0;
			p[1] = (BYTE)n;	//G
			n = Y + n2; if (n>255) n = 255; else if (n<0) n = 0;
			p[2] = (BYTE)n;	//R
		}
	}

	DrawImgBuff();
}
//---------------------------------------------------------------------------
void __fastcall TCapStickForm::SepiaActionUpdate(TObject *Sender)
{
	TPixelFormat pf = ImgBuff->PixelFormat;
	((TAction *)Sender)->Enabled = (pf!=pf1bit && pf!=pf4bit && pf!=pf8bit);
}

//---------------------------------------------------------------------------
//ネガポジ反転
//---------------------------------------------------------------------------
void __fastcall TCapStickForm::NegColActionExecute(TObject *Sender)
{
	if (ImgBuff->Empty) return;

	ToolPanel->Repaint();
	AddUndo();
	Image1->Canvas->CopyMode = cmNotSrcCopy;
	Image1->Canvas->Draw(0, 0, Image1->Picture->Bitmap);
	Image1->Canvas->CopyMode = cmSrcCopy;
	if (ImgBuff->Palette==0)
		ImgBuff->Assign(Image1->Picture->Bitmap);
	else
		ImgBuff->Canvas->Draw(0, 0, Image1->Picture->Bitmap);
	DispInfo();
}
//---------------------------------------------------------------------------
void __fastcall TCapStickForm::NegColActionUpdate(TObject *Sender)
{
	((TAction *)Sender)->Enabled = !ImgBuff->Empty;
}

//---------------------------------------------------------------------------
//モザイク
//---------------------------------------------------------------------------
void __fastcall TCapStickForm::MosicIActionExecute(TObject *Sender)
{
	if (ImgBuff->Empty) return;
	int msize = ((TAction *)Sender)->Tag;	if (msize<1) return;

	Screen->Cursor = crHourGlass;
	ToolPanel->Repaint();
	AddUndo();

	TRect m_rc;	//対象範囲
	if (SelShape->Visible && !SelRect.IsEmpty())
		m_rc = SelRect;
	else
		m_rc = Rect(0, 0, ImgBuff->Width, ImgBuff->Height);
	int b_w = m_rc.Width()/msize;
	int b_h = m_rc.Height()/msize;

	std::unique_ptr<Graphics::TBitmap> bmp(new Graphics::TBitmap());
	bmp->Assign(Image1->Picture->Bitmap);
	bmp->PixelFormat = pf24bit;

	for (int by = 0; by<=b_h; by++) {
		int yp = by*msize + m_rc.Top;
		for (int bx = 0; bx<=b_w; bx++) {
			int xp = bx * msize + m_rc.Left;
			//ブロック内の平均を求める
			int xR = 0, xG = 0, xB = 0, cnt = 0;
			for (int j=yp; j<yp+msize; j++) {
				if (j>=m_rc.Bottom) break;
				BYTE *p = (BYTE*)bmp->ScanLine[j];
				p += (xp*3);
				for (int i=xp; i<xp+msize; i++,p+=3) {
					if (i>=m_rc.Right) continue;
					xB += p[0];  xG += p[1];  xR += p[2];
					cnt++;
				}
			}
			if (cnt==0) continue;
			xR /= cnt;  xG /= cnt;  xB /= cnt;

			//ブロックを塗り潰す
			for (int j=yp; j<yp+msize; j++) {
				if (j>=m_rc.Bottom)  break;
				BYTE *p = (BYTE*)bmp->ScanLine[j];
				p += (xp*3);
				for (int i=xp; i<xp+msize; i++,p+=3) {
					if (i>=m_rc.Right) continue;
					p[0] = (BYTE)xB;
					p[1] = (BYTE)xG;
					p[2] = (BYTE)xR;
				}
			}
		}
	}

	if (ImgBuff->Palette==0)
		ImgBuff->Assign(bmp.get());
	else
		ImgBuff->Canvas->Draw(0, 0, bmp.get());
	DrawImgBuff();
}
//---------------------------------------------------------------------------
void __fastcall TCapStickForm::MosicIActionUpdate(TObject *Sender)
{
	((TAction *)Sender)->Enabled = !ImgBuff->Empty;
}
//---------------------------------------------------------------------------
//９０°回転
//---------------------------------------------------------------------------
void __fastcall TCapStickForm::RotateActionExecute(TObject *Sender)
{
	if (ImgBuff->Empty) return;

	ToolPanel->Repaint();
	AddUndo();

	int w = ImgBuff->Width;
	int h = ImgBuff->Height;

	std::unique_ptr<Graphics::TBitmap> bmp(new Graphics::TBitmap());
	bmp->Assign(Image1->Picture->Bitmap);
	WIC_rotate_image(bmp.get(), (((TAction *)Sender)->Tag==0)? 1 : 3);
	if (ImgBuff->Palette==0) {
		ImgBuff->Assign(bmp.get());
	}
	else {
		ImgBuff->SetSize(w, h);
		ImgBuff->Canvas->Draw(0, 0, bmp.get());
	}
	DrawImgBuff();
}
//---------------------------------------------------------------------------
void __fastcall TCapStickForm::RotateActionUpdate(TObject *Sender)
{
	((TAction *)Sender)->Enabled = !ImgBuff->Empty;
}
//---------------------------------------------------------------------------
//反転
//---------------------------------------------------------------------------
void __fastcall TCapStickForm::ReverseActionExecute(TObject *Sender)
{
	if (ImgBuff->Empty) return;

	AddUndo();
	WIC_rotate_image(ImgBuff, (((TAction *)Sender)->Tag==0)? 4 : 5);
	Image1->Canvas->Draw(0, 0, ImgBuff);
	DispInfo();
}
//---------------------------------------------------------------------------
void __fastcall TCapStickForm::ReverseActionUpdate(TObject *Sender)
{
	((TAction *)Sender)->Enabled = !ImgBuff->Empty;
}

//---------------------------------------------------------------------------
//マウスカーソル貼り付け
//---------------------------------------------------------------------------
void __fastcall TCapStickForm::CsrPasteItemClick(TObject *Sender)
{
	Screen->Cursor = (TCursor)((TComponent *)Sender)->Tag;

	//マウスカーソルの移動範囲を制限
	TRect rc = ScrollBox1->ClientRect;
	rc.SetLocation(ScrollBox1->ClientToScreen(rc.Location));
	::ClipCursor(&rc);

	SelRect = Rect(0, 0, 0, 0);
	SetSelRect();

	StatusBar1->Panels->Items[1]->Text = "マウスカーソル貼り付け中...";
	CsrPasting = true;
}

//---------------------------------------------------------------------------
void __fastcall TCapStickForm::Image1MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
	if (ImgBuff->Empty) return;
	if (CsrPasting) {
		//マウスカーソル貼り付け
		if (X<=ImgBuff->Width && Y<=ImgBuff->Height && Button==mbLeft) {
			AddUndo();
			//ホットスポット取得
			int hx = 0, hy = 0;
			ICONINFO ii;
			if (::GetIconInfo(Screen->Cursors[Screen->Cursor], &ii)) {
				hx = ii.xHotspot;
				hy = ii.yHotspot;
				if (ii.hbmMask)  ::DeleteObject(ii.hbmMask);
				if (ii.hbmColor) ::DeleteObject(ii.hbmColor);
			}
			//貼り付け
			::DrawIcon(Image1->Canvas->Handle, X - hx, Y - hy, Screen->Cursors[Screen->Cursor]);
			ImgBuff->Assign(Image1->Picture->Bitmap);
			DrawImgBuff();
		}
		::ClipCursor(NULL);
		Screen->Cursor = crDefault;
		CsrPasting = false;
	}
	else {
		if (Button==mbLeft) {
			if (X<=ImgBuff->Width && Y<=ImgBuff->Height) {
				//マウスカーソルの移動範囲を制限
				TRect rc = ScrollBox1->ClientRect;
				rc.Right++; rc.Bottom++;
				rc.SetLocation(ScrollBox1->ClientToScreen(rc.Location));
				::ClipCursor(&rc);

				//選択枠の初期化、選択開始
				SelRect   = Rect(X, Y, X, Y);
				orgX	  = X;
				orgY	  = Y;
				SetSelRect();
				Selecting = true;
			}
		}
		//右クリックメニュー
		else if (Button==mbRight) {
			ShowMenu(ScrollBox1);
		}
	}
}

//---------------------------------------------------------------------------
//枠選択中
//---------------------------------------------------------------------------
void __fastcall TCapStickForm::Image1MouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
	if (Selecting) {
		if (X>ImgBuff->Width)  X = ImgBuff->Width;
		if (Y>ImgBuff->Height) Y = ImgBuff->Height;

		SelRect.Left   = (X < orgX)? X    : orgX;
		SelRect.Right  = (X < orgX)? orgX : X;
		SelRect.Top    = (Y < orgY)? Y    : orgY;
		SelRect.Bottom = (Y < orgY)? orgY : Y;

		SetSelRect();
	}
}
//---------------------------------------------------------------------------
//枠選択終了
//---------------------------------------------------------------------------
void __fastcall TCapStickForm::Image1MouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
	//マウスカーソル移動範囲制限を解除
	::ClipCursor(NULL);

	Selecting = false;
	DispInfo();
}
//---------------------------------------------------------------------------
//選択枠内をクリック
//---------------------------------------------------------------------------
void __fastcall TCapStickForm::SelShapeMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
	//右クリックメニュー
	if (Button==mbRight) {
		ShowMenu(Sender);
	}
	//選択枠移動開始
	else {
		if (ImgBuff->Empty) return;
		SelMoving = true;
		Screen->Cursor = crSizeAll;
		orgX = X;
		orgY = Y;
		DispInfo();
	}
}
//---------------------------------------------------------------------------
//選択枠移動中
//---------------------------------------------------------------------------
void __fastcall TCapStickForm::SelShapeMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
	if (SelMoving) {
		//左上隅移動制限
		SelRect.Offset((X - orgX),(Y - orgY));
		if (SelRect.Left<0) SelRect.Offset(-SelRect.Left, 0);
		if (SelRect.Top<0)  SelRect.Offset(0, -SelRect.Top);
		//右下隅移動制限
		int w = ImgBuff->Width;
		int h = ImgBuff->Height;
		if (SelRect.Right>w)  SelRect.Offset(w - SelRect.Right, 0);
		if (SelRect.Bottom>h) SelRect.Offset(0, h - SelRect.Bottom);

		SetSelRect();
	}
}
//---------------------------------------------------------------------------
//選択枠移動終了
//---------------------------------------------------------------------------
void __fastcall TCapStickForm::SelShapeMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
	SelMoving = false;
	Screen->Cursor = crDefault;
	DispInfo();
}

//---------------------------------------------------------------------------
//トリミング
//---------------------------------------------------------------------------
void __fastcall TCapStickForm::TrimActionExecute(TObject *Sender)
{
	if (ImgBuff->Empty) return;
	if (!SelShape->Visible || SelRect.IsEmpty()) return;

	AddUndo();
	int w = SelRect.Width();
	int h = SelRect.Height();

	std::unique_ptr<Graphics::TBitmap> bmp(new Graphics::TBitmap());
	bmp->Assign(ImgBuff);
	ImgBuff->SetSize(w, h);
	ImgBuff->Canvas->CopyRect(Rect(0, 0, w, h), bmp->Canvas, SelRect);
	SelShape->Visible = false;
	DrawImgBuff();
}
//---------------------------------------------------------------------------
void __fastcall TCapStickForm::TrimActionUpdate(TObject *Sender)
{
	((TAction *)Sender)->Enabled = SelShape->Visible && !SelRect.IsEmpty();
}

//---------------------------------------------------------------------------
//アンドゥ
//---------------------------------------------------------------------------
void __fastcall TCapStickForm::UndoActionExecute(TObject *Sender)
{
	Graphics::TBitmap *bp = (Graphics::TBitmap *)UndoList->Objects[0];
	ImgBuff->Assign(bp);
	delete bp;
	UndoList->Delete(0);

	DrawImgBuff();
	if (!SelRect.IsEmpty()) SelShape->Visible = true;
	DispInfo();
}
//---------------------------------------------------------------------------
void __fastcall TCapStickForm::UndoActionUpdate(TObject *Sender)
{
	((TAction *)Sender)->Enabled = (UndoList->Count>0);
}
//---------------------------------------------------------------------------
//ウィンドウクラス名の表示切り替え
//---------------------------------------------------------------------------
void __fastcall TCapStickForm::ShowClassActionExecute(TObject *Sender)
{
	ClsNamPanel->Visible= !ClsNamPanel->Visible;
}
//---------------------------------------------------------------------------
void __fastcall TCapStickForm::ShowClassActionUpdate(TObject *Sender)
{
	((TAction *)Sender)->Checked = ClsNamPanel->Visible;
}
//---------------------------------------------------------------------------
//ウィンドウテキストの表示切り替え
//---------------------------------------------------------------------------
void __fastcall TCapStickForm::ShowTextActionExecute(TObject *Sender)
{
	WndTxtPanel->Visible = !WndTxtPanel->Visible;
}
//---------------------------------------------------------------------------
void __fastcall TCapStickForm::ShowTextActionUpdate(TObject *Sender)
{
	((TAction *)Sender)->Checked = WndTxtPanel->Visible;
}
//---------------------------------------------------------------------------
//リストタブの表示切り替え
//---------------------------------------------------------------------------
void __fastcall TCapStickForm::ShowListActionExecute(TObject *Sender)
{
	ListSheet->TabVisible = !ListSheet->TabVisible;

	if (ListSheet->TabVisible) {
		ScrollBox1->Parent = ImageSheet;
		PageControl1->Visible = true;
	}
	else {
		PageControl1->Visible = false;
		ScrollBox1->Parent	  = this;
	}
}
//---------------------------------------------------------------------------
void __fastcall TCapStickForm::ShowListActionUpdate(TObject *Sender)
{
	((TAction *)Sender)->Checked = ListSheet->TabVisible;
}
//---------------------------------------------------------------------------
//杖位置のピクセル色取得 表示切り替え
//---------------------------------------------------------------------------
void __fastcall TCapStickForm::CapColItemClick(TObject *Sender)
{
	ColFmt0Item->Checked = (ColValFmt==0);
	ColFmt1Item->Checked = (ColValFmt==1);
	ColFmt2Item->Checked = (ColValFmt==2);
	ColFmt3Item->Checked = (ColValFmt==3);
}
//---------------------------------------------------------------------------
void __fastcall TCapStickForm::ColFmtItemClick(TObject *Sender)
{
	ColValFmt = ((TComponent *)Sender)->Tag;
	PixColPanel->Visible = (ColValFmt>0);
}

//---------------------------------------------------------------------------
//ピクセル色をビットマップでコピー
//---------------------------------------------------------------------------
void __fastcall TCapStickForm::CopyBmpItemClick(TObject *Sender)
{
	int size = ((TComponent *)Sender)->Tag;
	std::unique_ptr<Graphics::TBitmap> bmp(new Graphics::TBitmap());
	bmp->SetSize(size, size);
	TCanvas *cv = bmp->Canvas;
	cv->Brush->Color = ColPanel->Color;
	cv->FillRect(Rect(0, 0, size, size));
	Clipboard()->Assign(bmp.get());
}

//---------------------------------------------------------------------------
//ステータスバーの表示切り替え
//---------------------------------------------------------------------------
void __fastcall TCapStickForm::ShowSttBarActionExecute(TObject *Sender)
{
	StatusBar1->Visible = !StatusBar1->Visible;
}
//---------------------------------------------------------------------------
void __fastcall TCapStickForm::ShowSttBarActionUpdate(TObject *Sender)
{
	((TAction *)Sender)->Checked = StatusBar1->Visible;
}
//---------------------------------------------------------------------------
//背景色指定
//---------------------------------------------------------------------------
void __fastcall TCapStickForm::BgColItemClick(TObject *Sender)
{
	ColorDialog1->Color = ImgBgCol;
	if (ColorDialog1->Execute()) {
		ImgBgCol = ColorDialog1->Color;
		ScrollBox1->Color = ImgBgCol;
		Image1->Canvas->Brush->Color = ImgBgCol;
		Image1->Canvas->FillRect(Image1->ClientRect);
		ImgBuff->Handle = NULL;
	}
}

//---------------------------------------------------------------------------
//ファイルからカーソル貼り付け
//---------------------------------------------------------------------------
void __fastcall TCapStickForm::LoadCsrActionExecute(TObject *Sender)
{
	OpenDialog1->Title		= "カーソルの貼り付け";
	OpenDialog1->Filter 	= "*.cur;*.ani";
	OpenDialog1->DefaultExt = "CUR";
	OpenDialog1->InitialDir = LastCurDir;
	OpenDialog1->FileName	= "*.cur";
	if (OpenDialog1->Execute()) {
		HCURSOR hCur = ::LoadCursorFromFile(OpenDialog1->FileName.c_str());
		if (hCur) {
			LastCurDir = ExtractFileDir(OpenDialog1->FileName);
			Screen->Cursors[crPaste] = hCur;
			Screen->Cursor = crPaste;

			//マウスカーソルの移動範囲を制限
			TRect rc = ScrollBox1->ClientRect;
			rc.SetLocation(ScrollBox1->ClientToScreen(rc.Location));
			::ClipCursor(&rc);

			SelRect = Rect(0, 0, 0, 0);
			SetSelRect();

			StatusBar1->Panels->Items[1]->Text = "マウスカーソル貼り付け中...";
			CsrPasting = true;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TCapStickForm::LoadCsrActionUpdate(TObject *Sender)
{
	((TAction *)Sender)->Enabled = !ImgBuff->Empty;
}

//---------------------------------------------------------------------------
//コピー
//---------------------------------------------------------------------------
void __fastcall TCapStickForm::CopyActionExecute(TObject *Sender)
{
	TWinControl *cp = Screen->ActiveControl;
	if (cp->ClassNameIs("TLabeledEdit")) {
		((TLabeledEdit*)cp)->CopyToClipboard();
	}
	else if (cp->ClassNameIs("TListBox")) {
		TListBox *lp = TextListBox;
		if (lp->SelCount>0) {
			std::unique_ptr<TStringList> cb_buf(new TStringList());
			for (int i=0; i<lp->Count; i++) if (lp->Selected[i]) cb_buf->Add(lp->Items->Strings[i]);
			::SendMessage(lp->Handle, WM_SETREDRAW, 0, 0);
			lp->ClearSelection();
			::SendMessage(lp->Handle, WM_SETREDRAW, 1, 0);
			::InvalidateRect(lp->Handle, NULL, TRUE);
			Clipboard()->AsText = cb_buf->Text;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TCapStickForm::CopyActionUpdate(TObject *Sender)
{
	TAction *ap = (TAction*)Sender;
	TWinControl *cp = Screen->ActiveControl;
	if (cp->ClassNameIs("TLabeledEdit")) {
		ap->Enabled = (((TLabeledEdit*)cp)->SelLength>0);
	}
	else if (cp->ClassNameIs("TListBox")) {
		ap->Enabled = (((TListBox*)cp)->SelCount>0);
	}
	else {
		ap->Enabled = false;
	}
}
//---------------------------------------------------------------------------
//すべて選択
//---------------------------------------------------------------------------
void __fastcall TCapStickForm::SelectAllActionExecute(TObject *Sender)
{
	TWinControl *cp = Screen->ActiveControl;
	if (cp->ClassNameIs("TLabeledEdit")) {
		((TLabeledEdit*)cp)->SelectAll();
	}
	else if (cp->ClassNameIs("TListBox")) {
		::SendMessage(TextListBox->Handle, LB_SETSEL, TRUE, -1);
	}
}
//---------------------------------------------------------------------------
void __fastcall TCapStickForm::SelectAllActionUpdate(TObject *Sender)
{
	TAction *ap = (TAction*)Sender;
	TWinControl *cp = Screen->ActiveControl;
	if (cp->ClassNameIs("TLabeledEdit")) {
		ap->Enabled = !((TLabeledEdit*)cp)->Text.IsEmpty();
	}
	else if (cp->ClassNameIs("TListBox")) {
		ap->Enabled = (((TListBox*)cp)->Count>0);
	}
	else {
		ap->Enabled = false;
	}
}

//---------------------------------------------------------------------------
//オプション設定
//---------------------------------------------------------------------------
void __fastcall TCapStickForm::OptionItemClick(TObject *Sender)
{
	if (OptionDlg->ShowModal()==mrOk) {
		MrkImageList->GetBitmap((CapCurIdx==1)? 2 : 1, CapToolImg->Picture->Bitmap);
		CapToolImg->Repaint();
	}
}

//---------------------------------------------------------------------------
//ヘルプ
//---------------------------------------------------------------------------
void __fastcall TCapStickForm::HelpItemClick(TObject *Sender)
{
	Application->HelpShowTableOfContents();
}
//---------------------------------------------------------------------------
//Webページを開く
//---------------------------------------------------------------------------
void __fastcall TCapStickForm::GoWebItemClick(TObject *Sender)
{
	::ShellExecute(NULL, _T("open"), SUPPORT_URL, NULL, NULL, SW_SHOWNORMAL);
}

//---------------------------------------------------------------------------
//隠しキャンセルボタン
//---------------------------------------------------------------------------
void __fastcall TCapStickForm::CanButtonClick(TObject *Sender)
{
	if (Capturing) {
		Capturing = false;
		WaitTimeBar->Visible = false;
		Screen->Cursor = crDefault;
		MrkImageList->GetBitmap((CapCurIdx==1)? 2 : 1, CapToolImg->Picture->Bitmap);
		CapToolImg->Repaint();
		ClearKeyBuff();
	}
	else {
		Close();
	}
}
//---------------------------------------------------------------------------

