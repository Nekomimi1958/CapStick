//----------------------------------------------------------------------//
// CapStick																//
//	ÉIÉvÉVÉáÉìê›íË														//
//																		//
//----------------------------------------------------------------------//
#ifndef Unit2H
#define Unit2H

//----------------------------------------------------------------------------
#include <Winapi.Windows.hpp>
#include <System.hpp>
#include <System.SysUtils.hpp>
#include <System.Classes.hpp>
#include <Vcl.Graphics.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.Imaging.pngimage.hpp>

//----------------------------------------------------------------------------
class TOptionDlg : public TForm
{
__published:
	TButton *CancelBtn;
	TButton *OKBtn;
	TButton *RefCapSndBtn;
	TButton *RefChgSndBtn;
	TButton *RefCpyCmdBtn;
	TButton *RefSavCmdBtn;
	TButton *TstCapSndBtn;
	TButton *TstChgSndBtn;
	TCheckBox *AllIconCheckBox;
	TComboBox *ScaleOptComboBox;
	TEdit *CapSndEdit;
	TEdit *ChgSndEdit;
	TEdit *CpyCmdEdit;
	TEdit *SavCmdEdit;
	TGroupBox *CurGroupBox;
	TGroupBox *GroupBox2;
	TGroupBox *GroupBox3;
	TGroupBox *GroupBox4;
	TGroupBox *GroupBox5;
	TGroupBox *SndGroupBox;
	TImage *Image1;
	TImage *Image2;
	TLabel *JpgLabel;
	TLabel *Label3;
	TLabel *Label4;
	TLabel *Label5;
	TLabel *Label6;
	TLabel *Label10;
	TLabeledEdit *ActClassEdit;
	TLabeledEdit *ActTitle2Edit;
	TLabeledEdit *ActTitleEdit;
	TLabeledEdit *CapWaitEdit;
	TLabeledEdit *HideWaitEdit;
	TLabeledEdit *SendKeyEdit;
	TOpenDialog *OpenDialog1;
	TPageControl *PageControl1;
	TRadioButton *CurRadioBtn1;
	TRadioButton *CurRadioBtn2;
	TTabSheet *TabSheet1;
	TTabSheet *TabSheet2;
	TTabSheet *TabSheet3;
	TTabSheet *TabSheet4;
	TTrackBar *JpgTrackBar;

	void __fastcall FormShow(TObject *Sender);
	void __fastcall OKBtnClick(TObject *Sender);
	void __fastcall RefSndBtnClick(TObject *Sender);
	void __fastcall RefCmdBtnClick(TObject *Sender);
	void __fastcall JpgTrackBarChange(TObject *Sender);
	void __fastcall TestSndBtnClick(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);

private:

public:
	virtual __fastcall TOptionDlg(TComponent* AOwner);
};
//----------------------------------------------------------------------------
extern PACKAGE TOptionDlg *OptionDlg;
//----------------------------------------------------------------------------
#endif
