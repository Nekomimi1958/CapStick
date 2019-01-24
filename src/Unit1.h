//----------------------------------------------------------------------//
// CapStick																//
//	メインフォーム														//
//																		//
//----------------------------------------------------------------------//
#ifndef Unit1H
#define Unit1H

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <System.Actions.hpp>
#include <System.ImageList.hpp>
#include <System.IniFiles.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ExtDlgs.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.ToolWin.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.Graphics.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.AppEvnts.hpp>

//---------------------------------------------------------------------------
#define SUPPORT_URL _T("http://nekomimi.la.coocan.jp/")
#define MAX_STD_CSR   20	//標準カーソル数
#define MAX_UNDO_CNT  20	//最大アンドゥ回数

//---------------------------------------------------------------------------
typedef HWND (WINAPI *FUNC_HtmlHelp)(HWND, LPCWSTR, UINT, DWORD);

//---------------------------------------------------------------------------
class TCapStickForm : public TForm
{
__published:	// IDE 管理のコンポーネント
	TAction *CopyAction;
	TAction *GrayAction;
	TAction *LoadCsrAction;
	TAction *MosicI2Action;
	TAction *MosicI4Action;
	TAction *MosicI8Action;
	TAction *MosicI12Action;
	TAction *MosicI16Action;
	TAction *NegColAction;
	TAction *Reduce2Action;
	TAction *Reduce16Action;
	TAction *Reduce256Action;
	TAction *Resize25Action;
	TAction *Resize50Action;
	TAction *Resize75Action;
	TAction *Resize150Action;
	TAction *Resize200Action;
	TAction *ReverseLRAction;
	TAction *ReverseULAction;
	TAction *RotateLAction;
	TAction *RotateRAction;
	TAction *SelectAllAction;
	TAction *SepiaAction;
	TAction *ShowClassAction;
	TAction *ShowListAction;
	TAction *ShowSttBarAction;
	TAction *ShowTextAction;
	TAction *TrimAction;
	TAction *UndoAction;
	TActionList *ActionList1;
	TApplicationEvents *ApplicationEvents1;
	TButton *CanButton;		//隠しキャンセルボタン
	TColorDialog *ColorDialog1;
	TImage *CapToolImg;
	TImage *ColImage;
	TImage *Image1;
	TImageList *BtnImageList;
	TImageList *MrkImageList;
	TLabel *Label3;
	TLabeledEdit *ClsNamEdit;
	TLabeledEdit *ColValEdit;
	TLabeledEdit *WndTxtEdit;
	TListBox *TextListBox;
	TMenuItem *AbautInf1Item;
	TMenuItem *AboutInf2Item;
	TMenuItem *AboutItem;
	TMenuItem *BgColItem;
	TMenuItem *CapColItem;
	TMenuItem *ChgSizeItem;
	TMenuItem *Col2Item;
	TMenuItem *Col16Item;
	TMenuItem *Col256Item;
	TMenuItem *ColFmt0Item;
	TMenuItem *ColFmt1Item;
	TMenuItem *ColFmt2Item;
	TMenuItem *ColFmt3Item;
	TMenuItem *ColorItem;
	TMenuItem *CopyBmp32Item;
	TMenuItem *CopyBmp256Item;
	TMenuItem *CopyItem;
	TMenuItem *GoWebItem;
	TMenuItem *GrayItem;
	TMenuItem *HelpItem;
	TMenuItem *ImageItem;
	TMenuItem *LoadCsrAction1;
	TMenuItem *Mosic2Item;
	TMenuItem *Mosic4Item;
	TMenuItem *Mosic8Item;
	TMenuItem *Mosic12Item;
	TMenuItem *Mosic16Item;
	TMenuItem *MosicItem;
	TMenuItem *MouseCsrItem;
	TMenuItem *NegColItem;
	TMenuItem *OptionItem;
	TMenuItem *Pop_s1;
	TMenuItem *Pop_s2;
	TMenuItem *Pop_s3;
	TMenuItem *Pop_s4;
	TMenuItem *Pop_s11;
	TMenuItem *Pop_s21;
	TMenuItem *Pop_s22;
	TMenuItem *Pos_s12;
	TMenuItem *RevLrItem;
	TMenuItem *RevTbItem;
	TMenuItem *RotL90Item;
	TMenuItem *RotR90Item;
	TMenuItem *SelectAllItem;
	TMenuItem *SepiaItem;
	TMenuItem *ShowClsItem;
	TMenuItem *ShowItem;
	TMenuItem *ShowListItem;
	TMenuItem *ShowSttBarItem;
	TMenuItem *ShowTxtItem;
	TMenuItem *Size25Item;
	TMenuItem *Size50Item;
	TMenuItem *Size75Item;
	TMenuItem *Size150Item;
	TMenuItem *Size200Item;
	TMenuItem *TrimItem;
	TMenuItem *UndoItem;
	TOpenDialog *OpenDialog1;
	TPageControl *PageControl1;
	TPanel *CapToolPanel;
	TPanel *ClsNamPanel;
	TPanel *ColPanel;
	TPanel *PixColPanel;
	TPanel *ToolPanel;
	TPanel *WndTxtPanel;
	TPopupMenu *ColPopupMenu;
	TPopupMenu *EditPopupMenu;
	TPopupMenu *PopupMenu1;
	TProgressBar *WaitTimeBar;
	TSavePictureDialog *SavePictureDialog1;
	TScrollBox *ScrollBox1;
	TShape *SelShape;
	TStatusBar *StatusBar1;
	TTabSheet *ImageSheet;
	TTabSheet *ListSheet;
	TTimer *Timer1;
	TToolBar *ToolBar1;
	TToolButton *CopyBtn;
	TToolButton *MenuBtn;
	TToolButton *SaveBtn;

	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall Timer1Timer(TObject *Sender);
	void __fastcall CopyBtnClick(TObject *Sender);
	void __fastcall SaveBtnClick(TObject *Sender);
	void __fastcall ShowMenu(TObject *Sender);
	void __fastcall CapToolImgMouseDown(TObject *Sender, TMouseButton Button,
		  TShiftState Shift, int X, int Y);
	void __fastcall CapToolImgMouseUp(TObject *Sender, TMouseButton Button,
		  TShiftState Shift, int X, int Y);
	void __fastcall PopMeasureItem(TObject *Sender, TCanvas *ACanvas,
		  int &Width, int &Height);
	void __fastcall AbautItemDrawItem(TObject *Sender, TCanvas *ACanvas,
		  TRect &ARect, bool Selected);
	void __fastcall Image1MouseDown(TObject *Sender, TMouseButton Button,
		  TShiftState Shift, int X, int Y);
	void __fastcall Image1MouseMove(TObject *Sender, TShiftState Shift,
		  int X, int Y);
	void __fastcall Image1MouseUp(TObject *Sender, TMouseButton Button,
		  TShiftState Shift, int X, int Y);
	void __fastcall SelShapeMouseDown(TObject *Sender, TMouseButton Button,
		  TShiftState Shift, int X, int Y);
	void __fastcall SelShapeMouseMove(TObject *Sender, TShiftState Shift,
		  int X, int Y);
	void __fastcall SelShapeMouseUp(TObject *Sender, TMouseButton Button,
		  TShiftState Shift, int X, int Y);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall ColFmtItemClick(TObject *Sender);
	void __fastcall BgColItemClick(TObject *Sender);
	void __fastcall GoWebItemClick(TObject *Sender);
	void __fastcall CanButtonClick(TObject *Sender);
	void __fastcall MouseCsrItemDrawItem(TObject *Sender, TCanvas *ACanvas,
		const TRect &ARect, bool Selected);
	void __fastcall MouseCsrItemMeasureItem(TObject *Sender,
		TCanvas *ACanvas, int &Width, int &Height);
	void __fastcall OptionItemClick(TObject *Sender);
    void __fastcall HelpItemClick(TObject *Sender);
	void __fastcall ShowClassActionExecute(TObject *Sender);
	void __fastcall ShowClassActionUpdate(TObject *Sender);
	void __fastcall ShowTextActionExecute(TObject *Sender);
	void __fastcall ShowTextActionUpdate(TObject *Sender);
	void __fastcall ShowSttBarActionExecute(TObject *Sender);
	void __fastcall ShowSttBarActionUpdate(TObject *Sender);
	void __fastcall UndoActionExecute(TObject *Sender);
	void __fastcall UndoActionUpdate(TObject *Sender);
	void __fastcall SepiaActionExecute(TObject *Sender);
	void __fastcall SepiaActionUpdate(TObject *Sender);
	void __fastcall NegColActionExecute(TObject *Sender);
	void __fastcall NegColActionUpdate(TObject *Sender);
	void __fastcall ReduceActionExecute(TObject *Sender);
	void __fastcall ReduceActionUpdate(TObject *Sender);
	void __fastcall TrimActionExecute(TObject *Sender);
	void __fastcall TrimActionUpdate(TObject *Sender);
	void __fastcall ResizeActionExecute(TObject *Sender);
	void __fastcall ResizeActionUpdate(TObject *Sender);
	void __fastcall MosicIActionExecute(TObject *Sender);
	void __fastcall MosicIActionUpdate(TObject *Sender);
	void __fastcall ReverseActionExecute(TObject *Sender);
	void __fastcall ReverseActionUpdate(TObject *Sender);
	void __fastcall ActionList1Update(TBasicAction *Action, bool &Handled);
	void __fastcall RotateActionExecute(TObject *Sender);
	void __fastcall RotateActionUpdate(TObject *Sender);
	void __fastcall AboutItemClick(TObject *Sender);
	void __fastcall ShowItemClick(TObject *Sender);
	void __fastcall CapColItemClick(TObject *Sender);
	void __fastcall MouseCsrItemClick(TObject *Sender);
	void __fastcall LoadCsrActionUpdate(TObject *Sender);
	void __fastcall LoadCsrActionExecute(TObject *Sender);
	void __fastcall CopyBmpItemClick(TObject *Sender);
	void __fastcall ApplicationEvents1Message(tagMSG &Msg, bool &Handled);
	void __fastcall CopyActionExecute(TObject *Sender);
	void __fastcall CopyActionUpdate(TObject *Sender);
	void __fastcall SelectAllActionExecute(TObject *Sender);
	void __fastcall SelectAllActionUpdate(TObject *Sender);
	void __fastcall ShowListActionExecute(TObject *Sender);
	void __fastcall ShowListActionUpdate(TObject *Sender);

private:	// ユーザー宣言
	int VersionNo;

	Graphics::TBitmap *ImgBuff;	//取り込みバッファ

	TStringList *UndoList;		//アンドゥリスト

	HWND hCurWnd;				//取り込み対象のウィンドウハンドル

	TCursor crCapture;			//捕獲の杖カーソル
	TCursor crCapture2;

	TCursor crPaste;			//貼り付けカーソル
	bool Capturing;

	TRect SelRect;				//選択枠
	bool Selecting;
	bool SelMoving;
	int  orgX, orgY;

	bool CsrPasting;

	UnicodeString LastDir;
	int LastFilter;
	UnicodeString LastCurDir;

	TColor ImgBgCol;			//取り込み画面の背景色
	int ColValFmt;				//ピクセル色値の表示形式 (0:非表示)

	void __fastcall WMDropFiles(TWMDropFiles &Msg);
	void __fastcall AddUndo();
	void __fastcall DispInfo(int w = 0, int h = 0);
	void __fastcall DrawImgBuff();
	void __fastcall SetSelRect();
	void __fastcall CaptWndImg(HWND hw);
	void __fastcall CsrPasteItemClick(TObject *Sender);

public:		// ユーザー宣言
	TIniFile *IniFile; 		//INIファイル

	int  CapCurIdx;
	int  JpegQuality;
	int  ScaleOpt;
	int  HideWait;
	bool ExtAllIcon;
	int  CaptureWait;

	UnicodeString SoundCapImg;
	UnicodeString SoundChgImg;

	UnicodeString CopyActWndClass;
	UnicodeString CopyActWndTitle;
	UnicodeString CopyActWndTitleR;
	UnicodeString CopyCmdLine;
	UnicodeString CopySendKey;
	UnicodeString SaveCmdLine;

	__fastcall TCapStickForm(TComponent* Owner);

	BEGIN_MESSAGE_MAP
		VCL_MESSAGE_HANDLER(WM_DROPFILES, TWMDropFiles, WMDropFiles)
	END_MESSAGE_MAP(TForm)
};
//---------------------------------------------------------------------------
extern PACKAGE TCapStickForm *CapStickForm;
//---------------------------------------------------------------------------
#endif
