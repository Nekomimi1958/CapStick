//----------------------------------------------------------------------//
//	�ėp���ʊ֐�														//
//																		//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
#include <algorithm>
#include <VersionHelpers.h>
#include "UserFunc.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
// �w��Z�p���[�^�̑O�̕�������擾
// ���Z�p���[�^��������Ȃ��ꍇ�́A����������̂܂ܕԂ�
//---------------------------------------------------------------------------
UnicodeString get_tkn(UnicodeString s, UnicodeString sp)
{
	int p = s.Pos(sp);
	return (p==0)? s : s.SubString(1, p - 1);
}
//---------------------------------------------------------------------------
// �w��Z�p���[�^�̌�̕�������擾
// ���Z�p���[�^��������Ȃ��ꍇ�́AEmptyStr ��Ԃ�
//---------------------------------------------------------------------------
UnicodeString get_tkn_r(UnicodeString s, UnicodeString sp)
{
	int p = s.Pos(sp);
	return (p==0)? EmptyStr : s.SubString(p + sp.Length(), s.Length() - (p - sp.Length() + 1));
}

//---------------------------------------------------------------------------
//���C���t�H�[���̈ʒu�E�T�C�Y�𕜌�
//---------------------------------------------------------------------------
void load_form_pos(TForm *frm, TIniFile *ini_file, int w, int h)
{
	UnicodeString sct = "General";
	if (w>0) frm->Width  = ini_file->ReadInteger(sct, "WinWidth",	w);
	if (h>0) frm->Height = ini_file->ReadInteger(sct, "WinHeight",	h);
	frm->Left = ini_file->ReadInteger(sct, "WinLeft", (Screen->Width - frm->Width)/2);
	frm->Top  = ini_file->ReadInteger(sct, "WinTop",  (Screen->Height - frm->Height)/2);
	if (Screen->MonitorCount==1) {
		//1��ʂ̏ꍇ�A��ʊO�ɏo�Ȃ��悤��
		if (frm->Left<0) frm->Left = 0;
		if (frm->Top<0)  frm->Top  = 0;
		if (frm->BoundsRect.Right>Screen->Width)   frm->Left = Screen->Width - frm->Width;
		if (frm->BoundsRect.Bottom>Screen->Height) frm->Top  = Screen->Height - frm->Height;
	}
	frm->WindowState = (TWindowState)ini_file->ReadInteger(sct, "WinState", (int)wsNormal);
}
//---------------------------------------------------------------------------
//���C���t�H�[���̈ʒu�E�T�C�Y��ۑ�
//---------------------------------------------------------------------------
void save_form_pos(TForm *frm, TIniFile *ini_file)
{
	UnicodeString sct = "General";
	if (frm->WindowState==wsMinimized) frm->WindowState = wsNormal;
	ini_file->WriteInteger(sct, "WinState",	(int)frm->WindowState);
	if (frm->WindowState==wsMaximized) frm->WindowState = wsNormal;
	ini_file->WriteInteger(sct, "WinLeft",	frm->Left);
	ini_file->WriteInteger(sct, "WinTop",	frm->Top);
	ini_file->WriteInteger(sct, "WinWidth",	frm->Width);
	ini_file->WriteInteger(sct, "WinHeight",frm->Height);
}

//---------------------------------------------------------------------------
//Vista�ȍ~�̊g���t���[���ɑΉ������E�B���h�E�����`�̎擾
//---------------------------------------------------------------------------
RECT get_window_rect_ex(HWND hWnd)
{
	RECT rc;
	if (!IsWindowsVistaOrGreater()
		|| ::DwmGetWindowAttribute(hWnd, DWMWA_EXTENDED_FRAME_BOUNDS, &rc, sizeof(rc))!=S_OK)
	{
		::GetWindowRect(hWnd, &rc);
	}
	return rc;
}

//---------------------------------------------------------------------------
//�t�@�C�����Ƀh���C�u�����Ȃ����
//���s�t�@�C���̃f�B���N�g������̑��΃t�@�C�����ɕς���
//�t�@�C�������݂��Ȃ���� EmptyStr ��Ԃ�
//---------------------------------------------------------------------------
UnicodeString get_filename_r(UnicodeString fnam)
{
	if (fnam.IsEmpty()) return EmptyStr;
	if (ExtractFileDrive(fnam).IsEmpty())
		fnam = ExtractFilePath(Application->ExeName) + fnam;
	if (!FileExists(fnam)) fnam = EmptyStr;
	return fnam;
}

//---------------------------------------------------------------------------
//�A�v�����N�����ē��͂��L���ɂȂ�܂ő҂�
//---------------------------------------------------------------------------
bool run_app_wait(UnicodeString cmdln, int wait)
{
	STARTUPINFO sinf;
	::ZeroMemory(&sinf, sizeof(sinf) );
	sinf.cb = sizeof(sinf);
	PROCESS_INFORMATION pinf;
	if (::CreateProcess(NULL,cmdln.c_str(), NULL, NULL, FALSE, 0, NULL, NULL, &sinf,&pinf) !=0) {
		//�N�������A�v���̓��͂��L���ɂȂ�܂ő҂�
		::WaitForInputIdle(pinf.hProcess, wait);
		::CloseHandle(pinf.hProcess);
		::CloseHandle(pinf.hThread );
		return true;
	}

	return false;
}

//---------------------------------------------------------------------------
//�Y������E�B���h�E�n���h�����擾
// wcls : �N���X   (���S��v)
// wtit : �^�C�g�� (�O����v)
// wttr : �^�C�g�� (�����v)
//---------------------------------------------------------------------------
HWND __fastcall get_hwnd_ex(UnicodeString wcls, UnicodeString wtit, UnicodeString wttr)
{
	if (wcls.IsEmpty() && wtit.IsEmpty() && wttr.IsEmpty()) return NULL;

	_TCHAR tbuf[1024], cbuf[256];
	::ZeroMemory(tbuf, sizeof(tbuf));
	::ZeroMemory(cbuf, sizeof(cbuf));

	HWND hTemp = ::GetWindow(Application->Handle, GW_HWNDFIRST);
	HWND hRet = NULL;
	while (hTemp) {
		if (::IsWindowVisible(hTemp)) {
			::GetWindowText(hTemp, tbuf, 1023);
			::GetClassName(hTemp,  cbuf,  255);
			do {
				UnicodeString tmp = tbuf;
				if (!wtit.IsEmpty()) {
					//�O����v
					if (tmp.Pos(wtit)!=1) break;
				}
				if (!wttr.IsEmpty()) {
					//�����v
					if (tmp.Pos(wttr) != (tmp.Length()-wttr.Length()+1)) break;
				}
				tmp = cbuf;
				if (!wcls.IsEmpty() && tmp!=wcls) break;
				hRet = hTemp;
            } while (0);
		}
		if (hRet!=NULL) break;
		hTemp = ::GetWindow(hTemp, GW_HWNDNEXT);
	}

	return hRet;
}

//---------------------------------------------------------------------------
//�A�N�e�B�u�ȃE�B���h�E�փL�[�𑗂�
// keystr:  , �ŋ�؂�ꂽ�L�[�̕���
// ALT,CTRL,SHIFT, F1�`F12, A�`Z, 0�`9 ���g�p�\
// ���� / ������ƃL�[�A�b�v�A�Ȃ���΃L�[�_�E��
//---------------------------------------------------------------------------
void send_key_ex(UnicodeString keystr, int wt)
{
	UnicodeString key;
	while (!keystr.IsEmpty()) {
		Sleep(wt);
		int p = keystr.Pos(",");
		if (p>0) {
			key = keystr.SubString(1, p - 1);
			keystr.Delete(1, p);
		}
		else {
			key = keystr;
			keystr = EmptyStr;
		}
		key = key.Trim().UpperCase();
		int flag = 0;
		if (key.SubString(1, 1)=="/") {
			key.Delete(1, 1);
			flag = KEYEVENTF_KEYUP;
		}

		BYTE vk;
		if		(key=="ALT")   vk = VK_MENU;
		else if (key=="CTRL")  vk = VK_CONTROL;
		else if (key=="SHIFT") vk = VK_SHIFT;
		else if (key=="F1")    vk = VK_F1;
		else if (key=="F2")    vk = VK_F2;
		else if (key=="F3")    vk = VK_F3;
		else if (key=="F4")    vk = VK_F4;
		else if (key=="F5")    vk = VK_F5;
		else if (key=="F6")    vk = VK_F6;
		else if (key=="F7")    vk = VK_F7;
		else if (key=="F8")    vk = VK_F8;
		else if (key=="F9")    vk = VK_F9;
		else if (key=="F10")   vk = VK_F10;
		else if (key=="F11")   vk = VK_F11;
		else if (key=="F12")   vk = VK_F12;
		else if (key=="DEL")   vk = VK_DELETE;
		else if (key=="INS")   vk = VK_INSERT;
		else if (key=="BKSP")  vk = VK_BACK; 
		else if (key=="PGUP")  vk = VK_NEXT; 
		else if (key=="PGDN")  vk = VK_PRIOR;
		else if (key=="HOME")  vk = VK_HOME; 
		else if (key=="END")   vk = VK_END;
		else if (key=="LEFT")  vk = VK_LEFT; 
		else if (key=="UP")    vk = VK_UP;
		else if (key=="RIGHT") vk = VK_RIGHT;
		else if (key=="DOWN")  vk = VK_DOWN; 
		else if (key=="ESC")   vk = VK_ESCAPE;
		else if (key=="ENTER") vk = VK_RETURN;
		else if (key=="SPACE") vk = VK_SPACE;
		else if (key=="TAB")   vk = VK_TAB;
		else if (key==".")     vk = VK_OEM_PERIOD;
		else if (key.Pos("0x")==1) {
			int cd = key.ToIntDef(-1);
			if (cd>=0x01 && cd<=0xfe) vk = cd; else vk = 0;
		}
		else {
			vk = (BYTE)key[1];
		}
		if (vk!=0) {
			BYTE sc = (BYTE)::MapVirtualKey(vk, 0);
			::keybd_event(vk, sc, flag, 0);
		}
	}
}

//---------------------------------------------------------------------------
//�����񂩂� ComboBox �ɍ��ڂ�ǉ�
// sp: ��؂� default = "|":
//---------------------------------------------------------------------------
void str_to_ComboBox(TComboBox *cp, UnicodeString s, UnicodeString sp)
{
	for (;;) {
		UnicodeString itm = get_tkn(s, sp);
		if (itm.IsEmpty()) break;
		cp->Items->Add(itm);
		s = get_tkn_r(s, sp);
	}
}

//---------------------------------------------------------------------------
//�A�v���P�[�V�����̐��i�o�[�W�����擾
// is_num: ���� true = "nnn"/ false = "n.n.n.0" (default = false)
//---------------------------------------------------------------------------
UnicodeString get_VersionStr(UnicodeString fnam, bool is_num)
{
	UnicodeString verstr;
	unsigned mj, mi, bl;
	if (GetProductVersion(fnam, mj, mi, bl)) {
		verstr.sprintf(is_num? _T("%u%u%u") : _T("%u.%u.%u.0"), mj, mi, bl);
	}
	return verstr;
}
//---------------------------------------------------------------------------
int get_VersionNo()
{
	return get_VersionStr(Application->ExeName, true).ToIntDef(0);
}

//---------------------------------------------------------------------------
//�L�[��������Ă��邩?
//---------------------------------------------------------------------------
bool is_KeyDown(WORD key)
{
	return (HIBYTE(::GetAsyncKeyState(key)) != 0);
}

//---------------------------------------------------------------------------
//�L�[�E�}�E�X�o�b�t�@���N���A
//---------------------------------------------------------------------------
void ClearKeyBuff(
	bool key_only)	//true = �L�[�o�b�t�@�̂݃N���A (default = false)
{
	MSG msg;
	if (key_only) {
		do {
			Sleep(0);
		} while (::PeekMessage(&msg, NULL, WM_KEYFIRST, WM_KEYLAST, PM_REMOVE));
	}
	else {
		do {
			Sleep(50);
		} while (
			::PeekMessage(&msg, NULL, WM_KEYFIRST, WM_KEYLAST, PM_REMOVE) ||
			::PeekMessage(&msg, NULL, WM_MOUSEFIRST, WM_MOUSELAST, PM_REMOVE)
		);
	}
}

//---------------------------------------------------------------------------
//�G���[���b�Z�[�W
//---------------------------------------------------------------------------
void msgbox_ERR(UnicodeString msg, UnicodeString tit)
{
	if (tit.IsEmpty()) tit = "�G���[";	//�f�t�H���g
	Application->MessageBox(msg.c_str(), tit.c_str(), MB_OK|MB_ICONERROR);
}

//---------------------------------------------------------------------------
//�J���[�ɔ{����������
//---------------------------------------------------------------------------
TColor RatioCol(TColor col, float rt)
{
	if (rt>1.0) rt = 1.0; else if (rt<0.0) rt = 0.0;
	int c = ColorToRGB(col);
	int r = GetRValue(c) * rt;
	int g = GetGValue(c) * rt;
	int b = GetBValue(c) * rt;
	return TColor(RGB(r, g, b));
}

//---------------------------------------------------------------------------
// RGB��HSL �ϊ�
//---------------------------------------------------------------------------
void RgbToHslI(TColor col, int *h, int *s, int *l)
{
	int cref = ColorToRGB(col);
	float r = GetRValue(cref)/255.0;
	float g = GetGValue(cref)/255.0;
	float b = GetBValue(cref)/255.0;

	float maxv = std::max(r, std::max(g, b));
	float minv = std::min(r, std::min(g, b));
	float h_f, s_f;
	float l_f = (maxv + minv)/2.0;

	if (maxv==minv) {
		h_f = s_f = 0.0;
	}
	else {
		float d = maxv - minv;
		if		(maxv==r) h_f = (g - b)/d;
		else if (maxv==g) h_f = (b - r)/d + 2.0;
		else			  h_f = (r - g)/d + 4.0;
		h_f *= 60.0;
		if (h_f<0.0) h_f += 360.0;

		s_f = ((l_f<0.5)? d/(maxv + minv) : d/(2.0 - maxv - minv)) * 100.0;
	}

	l_f *= 100.0;

	*h = (int)(h_f + 0.5);
	*s = (int)(s_f + 0.5);
	*l = (int)(l_f + 0.5);
}
//---------------------------------------------------------------------------
