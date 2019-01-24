//----------------------------------------------------------------------//
// CapStick																//
//	�I�v�V�����ݒ�														//
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
		"�j�A���X�g�l�C�o�[|�o�C���j�A|�o�C�L���[�r�b�N|�t�@���g���T���v�����O");
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
//�T�E���h�t�@�C���Q��
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::RefSndBtnClick(TObject *Sender)
{
	int tag = ((TComponent *)Sender)->Tag;
	OpenDialog1->Title		= "�T�E���h�t�@�C���̎w��";
	OpenDialog1->FileName	= EmptyStr;
	OpenDialog1->Filter 	= "WAV�t�@�C��|*.WAV";
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
//�T�E���h�̃e�X�g
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::TestSndBtnClick(TObject *Sender)
{
	UnicodeString fnam = (((TComponent *)Sender)->Tag==0)? ChgSndEdit->Text : CapSndEdit->Text;
	::sndPlaySound(fnam.c_str(), SND_ASYNC);
}

//---------------------------------------------------------------------------
//���s�v���O�����Q��
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::RefCmdBtnClick(TObject *Sender)
{
	OpenDialog1->Title		= "���s�v���O�����̎w��";
	OpenDialog1->FileName	= EmptyStr;
	OpenDialog1->Filter 	= "���s�t�@�C��|*.EXE";
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
//�ݒ�m��
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


