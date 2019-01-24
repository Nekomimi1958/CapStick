//----------------------------------------------------------------------//
// CapStick																//
//																		//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop
#include <tchar.h>

USEFORM("Unit1.cpp", CapStickForm);
USEFORM("Unit2.cpp", OptionDlg);
//---------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
	//ミューテックスにより二重起動を防止
	HANDLE hMutex = ::CreateMutex(NULL, FALSE, _T("CapStick_is_running"));
	if (::GetLastError()==ERROR_ALREADY_EXISTS) {
		//二重起動なら、既存のCapStickをアクティブに
		HWND hFrmWnd = ::FindWindow(_T("TCapStickForm"), NULL);
		if (hFrmWnd) {
			HWND hApp = ::GetWindow(hFrmWnd, GW_OWNER);
			if (::IsIconic(hApp)) ::ShowWindow(hApp, SW_RESTORE);
			::SetForegroundWindow(::GetLastActivePopup(hApp));
		}
		::CloseHandle(hMutex);
		return -1;
	}

	::CoInitialize(NULL);

	try
	{
		Application->Initialize();
		Application->Title = "CapStick";
		Application->CreateForm(__classid(TCapStickForm), &CapStickForm);
		Application->CreateForm(__classid(TOptionDlg), &OptionDlg);
		Application->Run();
	}
	catch (Exception &exception)
	{
		Application->ShowException(&exception);
	}
	catch (...)
	{
		try
		{
			throw Exception("");
		}
		catch (Exception &exception)
		{
			Application->ShowException(&exception);
		}
	}

	::CoUninitialize();
	::CloseHandle(hMutex);
	return 0;
}
//---------------------------------------------------------------------------
