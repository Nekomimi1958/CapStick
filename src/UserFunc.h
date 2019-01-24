//----------------------------------------------------------------------//
//	îƒópã§í ä÷êî														//
//																		//
//----------------------------------------------------------------------//
#ifndef UsrFuncH
#define UsrFuncH

//---------------------------------------------------------------------------
#include <System.StrUtils.hpp>
#include <System.IniFiles.hpp>
#include <Vcl.ComCtrls.hpp>

UnicodeString get_tkn(UnicodeString s, UnicodeString sp);
UnicodeString get_tkn_r(UnicodeString s, UnicodeString sp);

void load_form_pos(TForm *frm, TIniFile *ini_file, int w=0, int h=0);
void save_form_pos(TForm *frm, TIniFile *ini_file);

UnicodeString get_filename_r(UnicodeString fnam);
bool run_app_wait(UnicodeString cmdln, int wait = 10000);

HWND __fastcall get_hwnd_ex(UnicodeString wcls, UnicodeString wtit, UnicodeString wttr);
void send_key_ex(UnicodeString keystr, int wt=0);

void str_to_ComboBox(TComboBox *cp, UnicodeString s, UnicodeString sep = "|");

UnicodeString get_VersionStr(UnicodeString fnam, bool is_num = false);
int get_VersionNo();

bool is_KeyDown(WORD key);
void ClearKeyBuff(bool key_only = false);

void msgbox_ERR( UnicodeString msg, UnicodeString tit = EmptyStr);

TColor RatioCol(TColor col, float rt);
void RgbToHslI(TColor col, int *h, int *s, int *l);

//---------------------------------------------------------------------------
#define UCAT_TSTR(str)	cat_sprintf(_T("%s"), _T(str))

//---------------------------------------------------------------------------
#endif
