//----------------------------------------------------------------------//
// CapStick																//
//	オプション設定														//
//																		//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop

#include <mmsystem.h>

#include "UserFunc.h"
#include "Unit1.h"
#include "Unit2.h"

//---------------------------------------------------------------------
#pragma resource "*.dfm"
TOptionDlg *OptionDlg;

//---------------------------------------------------------------------
__fastcall TOptionDlg::TOptionDlg(TComponent* AOwner)
	: TForm(AOwner)
{
}
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::FormCreate(TObject *Sender)
{
	str_to_ComboBox(ScaleOptComboBox,
		"ニアレストネイバー|バイリニア|バイキュービック|ファントリサンプリング");
}
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::FormShow(TObject *Sender)
{
	if (CapStickForm->CapCurIdx==1) CurRadioBtn2->Checked = true; else CurRadioBtn1->Checked = true;

	HideWaitEdit->NumbersOnly	= true;
	HideWaitEdit->Text			= IntToStr(CapStickForm->HideWait);
	CapWaitEdit->NumbersOnly	= true;
	CapWaitEdit->Text			= IntToStr(CapStickForm->CaptureWait);
	JpgTrackBar->Position		= CapStickForm->JpegQuality;
	AllIconCheckBox->Checked	= CapStickForm->ExtAllIcon;

	ChgSndEdit->Text			= CapStickForm->SoundChgImg;
	CapSndEdit->Text			= CapStickForm->SoundCapImg;

	ActClassEdit->Text			= CapStickForm->CopyActWndClass;
	ActTitleEdit->Text			= CapStickForm->CopyActWndTitle;
	ActTitle2Edit->Text 		= CapStickForm->CopyActWndTitleR;
	CpyCmdEdit->Text			= CapStickForm->CopyCmdLine;
	SendKeyEdit->Text			= CapStickForm->CopySendKey;
	SavCmdEdit->Text			= CapStickForm->SaveCmdLine;
	ScaleOptComboBox->ItemIndex = CapStickForm->ScaleOpt;
}
//---------------------------------------------------------------------------
//サウンドファイル参照
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::RefSndBtnClick(TObject *Sender)
{
	int tag = ((TComponent *)Sender)->Tag;
	OpenDialog1->Title		= "サウンドファイルの指定";
	OpenDialog1->FileName	= EmptyStr;
	OpenDialog1->Filter 	= "WAVファイル|*.WAV";
	OpenDialog1->DefaultExt = "wav";
	OpenDialog1->InitialDir	= ExtractFileDir((tag==0)? ChgSndEdit->Text : CapSndEdit->Text);
	if (OpenDialog1->InitialDir.IsEmpty())
		OpenDialog1->InitialDir = ExtractFileDir(Application->ExeName);

	if (OpenDialog1->Execute()) {
		UnicodeString fnam  = OpenDialog1->FileName;
		UnicodeString exdir = ExtractFilePath(Application->ExeName);
		if (fnam.UpperCase().Pos(exdir.UpperCase())==1) fnam.Delete(1, exdir.Length());
		switch (tag) {
		case 0: ChgSndEdit->Text = fnam; break;
		case 1: CapSndEdit->Text = fnam; break;
		}
		::sndPlaySound(OpenDialog1->FileName.c_str(), SND_ASYNC);
	}
}
//---------------------------------------------------------------------------
//サウンドのテスト
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::TestSndBtnClick(TObject *Sender)
{
	UnicodeString fnam = (((TComponent *)Sender)->Tag==0)? ChgSndEdit->Text : CapSndEdit->Text;
	::sndPlaySound(fnam.c_str(), SND_ASYNC);
}

//---------------------------------------------------------------------------
//実行プログラム参照
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::RefCmdBtnClick(TObject *Sender)
{
	OpenDialog1->Title		= "実行プログラムの指定";
	OpenDialog1->FileName	= EmptyStr;
	OpenDialog1->Filter 	= "実行ファイル|*.EXE";
	OpenDialog1->DefaultExt = "exe";
	if (OpenDialog1->Execute()) {
		UnicodeString fnam  = OpenDialog1->FileName;
		switch (((TComponent *)Sender)->Tag) {
		case 0: CpyCmdEdit->Text = fnam; break;
		case 1: SavCmdEdit->Text = fnam; break;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::JpgTrackBarChange(TObject *Sender)
{
	JpgLabel->Caption = IntToStr(JpgTrackBar->Position);
}

//---------------------------------------------------------------------------
//設定確定
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::OKBtnClick(TObject *Sender)
{
	CapStickForm->CapCurIdx 	   = CurRadioBtn2->Checked? 1 : 0;
	CapStickForm->HideWait		   = HideWaitEdit->Text.ToIntDef(100);
	CapStickForm->CaptureWait	   = CapWaitEdit->Text.ToIntDef(3000);
	CapStickForm->JpegQuality	   = JpgTrackBar->Position;
	CapStickForm->ExtAllIcon	   = AllIconCheckBox->Checked;
	CapStickForm->SoundChgImg	   = ChgSndEdit->Text;
	CapStickForm->SoundCapImg	   = CapSndEdit->Text;
	CapStickForm->CopyActWndClass  = ActClassEdit->Text;
	CapStickForm->CopyActWndTitle  = ActTitleEdit->Text;
	CapStickForm->CopyActWndTitleR = ActTitle2Edit->Text;
	CapStickForm->CopyCmdLine	   = CpyCmdEdit->Text;
	CapStickForm->CopySendKey	   = SendKeyEdit->Text;
	CapStickForm->SaveCmdLine	   = SavCmdEdit->Text;
	CapStickForm->ScaleOpt		   = ScaleOptComboBox->ItemIndex;
}
//---------------------------------------------------------------------------


